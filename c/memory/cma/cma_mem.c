#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/mm.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/debugfs.h>
#include <linux/mempolicy.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/cacheflush.h>
#include <linux/dma-mapping.h>
#include <linux/export.h>

#include "cma_mem_ioctl.h"

#define DEVICE_NAME "cma_mem"

enum cma_status{
	UNKNOW_STATUS = 0,
	HAVE_ALLOCED = 1,
	HAVE_MMAPED = 2,
};

struct cmamem_dev {
	unsigned int count;
	struct miscdevice dev;
	struct mutex cmamem_lock;
	struct list_head info_list;
};

struct current_status{
	int status;
	int id_count;
	dma_addr_t phy_base;
};

static struct current_status cmamem_status;
static struct cmamem_dev cmamem_dev;
static struct cmamem_info cma_info[32];

static long cmamem_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	dma_addr_t map_dma;
	unsigned long size = 0;
	u8 *p = NULL;
	int ret = 0;

	switch (cmd) {
	case CMEM_ALLOCATE:
		printk(KERN_INFO "CMEM_ALLOCATE\n");
		cmamem_status.id_count = cmamem_dev.count++;
		cma_info[cmamem_status.id_count].id = cmamem_status.id_count;
		ret = copy_from_user(&cma_info[cmamem_status.id_count], (void __user *)arg,
			sizeof(struct cmamem_info));
		if (ret) {
			printk(KERN_ERR "CMEM_ALLOCATE:copy_from_user error: %d\n", ret);
			ret = -EFAULT;
			goto err;
		}

		size = cma_info[cmamem_status.id_count].len;
		size = PAGE_ALIGN(size);
		printk(KERN_INFO "CMEM_ALLOCATE: [%d], org_size: %lu, page_align_size: %lu\n",
			cmamem_status.id_count, cma_info[cmamem_status.id_count].len, size);
		if (size == 0) {
			printk(KERN_ERR "CMEM_ALLOCATE: size is zero\n");
			ret = -ENOMEM;
			goto err;
		}

		p = dma_alloc_writecombine(NULL, size, &map_dma, GFP_KERNEL);
		if (p == NULL) {
			printk(KERN_ERR "CMEM_ALLOCATE: dma alloc err: %p\n", p);
			ret = -ENOMEM;
			goto err;
		}
		cma_info[cmamem_status.id_count].mem_base = (unsigned long)p;
		printk(KERN_INFO "CMEM_ALLOCATE: phy_addr: 0x%llx, size: %lu, virt: %p, virt_base: 0x%lx\n",
			map_dma, size, p, cma_info[cmamem_status.id_count].mem_base);

		cma_info[cmamem_status.id_count].phy_base = map_dma;
		cmamem_status.phy_base = map_dma;

		mutex_lock(&cmamem_dev.cmamem_lock);
		cmamem_status.status = HAVE_ALLOCED;
		mutex_unlock(&cmamem_dev.cmamem_lock);
		break;
	default:
		printk(KERN_ERR "cma mem not support command\n");
		break;
	}
err:
	return ret;
}

static int cmamem_mmap(struct file *filp, struct vm_area_struct *vma)
{
	unsigned long start = vma->vm_start;
	unsigned long size = vma->vm_end - vma->vm_start;
	unsigned long offset = vma->vm_pgoff << PAGE_SHIFT;
	unsigned long page = 0, pos = 0;

	//dump_stack();
	if (cmamem_status.status != HAVE_ALLOCED) {
		printk(KERN_ERR "%s, you should allocted memory firstly\n", __func__);
		return -EINVAL;
	}

	pos = cmamem_status.phy_base + offset;
	page = pos >> PAGE_SHIFT;

	printk(KERN_INFO "Mmap: vmstart: 0x%08lx, vmsize: 0x%lx, offset: 0x%lx, page: 0x%lx\n",
		start, size, offset, page);
	if (remap_pfn_range(vma, start, page, size, PAGE_SHARED)) {
		return -EAGAIN;
	} else {
		printk(KERN_INFO "Mmap: remap_pfn_range success\n");
	}
	//vma->vm_flags &= ~VM_IO;
	//vma->vm_flags |= (VM_DONTEXPAND | VM_DONTDUMP);

	return 0;
}

static struct file_operations dev_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = cmamem_ioctl,
	.mmap = cmamem_mmap,
};

static int __init cmamem_init(void)
{
	int rval = 0;

	printk(KERN_INFO "%s\n", __func__);
	mutex_init(&cmamem_dev.cmamem_lock);
	INIT_LIST_HEAD(&cmamem_dev.info_list);
	cmamem_dev.count = 0;
	cmamem_dev.dev.name = DEVICE_NAME;
	cmamem_dev.dev.minor = MISC_DYNAMIC_MINOR;
	cmamem_dev.dev.fops = &dev_fops;

	cmamem_status.status = UNKNOW_STATUS;
	cmamem_status.id_count = -1;
	cmamem_status.phy_base = 0;

	rval = misc_register(&cmamem_dev.dev);
	return rval;
}

static void __exit cmamem_exit(void)
{
	misc_deregister(&cmamem_dev.dev);
	printk(KERN_INFO "%s\n", __func__);
}

module_init(cmamem_init);
module_exit(cmamem_exit);
MODULE_LICENSE("GPL");
