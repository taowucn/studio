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
#include <linux/syscalls.h>
#include <linux/mman.h>

#include "cma_mem_ioctl.h"

#define DEVICE_NAME "cma_mem"

#define MEM_DEBUG 1

enum cma_status{
	UNKNOW_STATUS = 0,
	HAVE_ALLOCED = 1,
	HAVE_MMAPED =2,
};

struct cmamem_dev {
	unsigned int count;
	struct miscdevice dev;
	struct mutex cmamem_lock;
};

struct current_status{
	int status;
	int id_count;
	dma_addr_t phy_base;
};

struct cmamem_block {
	char name[10];
	char is_use_buffer;
	char is_free;
	int id;
	unsigned long offset;
	unsigned long len;
	unsigned long phy_base;
	unsigned long mem_base;
	void *kernel_base;
	struct list_head memqueue_list;
};

static struct current_status cmamem_status;
static struct cmamem_dev cmamem_dev;
static struct cmamem_block *cmamem_block_head;
static int mem_block_count = 0;

static void dump_mem(struct cmamem_block *mem_blk)
{
	printk("%s:CMA name:%s\n", __func__, mem_blk->name);
	printk("%s:CMA id:%d\n", __func__, mem_blk->id);
	printk("%s:Is usebuf:%d\n", __func__, mem_blk->is_use_buffer);
	printk("%s:PHY Base:0x%08lx\n", __func__, mem_blk->phy_base);
	printk("%s:KER Base:%p\n", __func__, mem_blk->kernel_base);
	printk("%s:USR Base:0x%08lx\n", __func__, mem_blk->mem_base);
}

static long cmamem_alloc(struct file *file, unsigned long arg)
{
	struct cmamem_block *mem_blk = NULL;
	struct mem_block usr_cma;
	unsigned long size = 0;
	int ret = 0;

	ret = copy_from_user(&usr_cma, (void __user *)arg, sizeof(struct mem_block));
	if (ret) {
		printk(KERN_ERR "Alloc: copy_from_user error:%d\n", ret);
		return -1;
	}

	if (usr_cma.name[0] == '\0') {
		printk(KERN_ERR "Alloc: no set mem name\n");
		return -1;
	}
	if (usr_cma.len == 0) {
		printk(KERN_ERR "Alloc: the len is NULL\n");
		return -1;
	}

	size = PAGE_ALIGN(usr_cma.len);
	printk(KERN_INFO "Alloc: org_size: %lu, page_align_size: %lu, is_use_buffer: %d\n",
			 usr_cma.len, size, usr_cma.is_use_buffer);
	if (size == 0) {
		printk(KERN_ERR "Alloc: size is zero\n");
		return -ENOMEM;
	}
	usr_cma.len = size;

	if (usr_cma.is_use_buffer) {
		usr_cma.kernel_base = dma_alloc_writecombine(NULL, size,
			(dma_addr_t *)(&(usr_cma.phy_base)), GFP_KERNEL);
	} else {
		usr_cma.kernel_base = dma_alloc_coherent(NULL, size,
			(dma_addr_t *)(&(usr_cma.phy_base)), GFP_KERNEL);
	}
	if ((usr_cma.phy_base == 0) || (usr_cma.kernel_base == NULL)) {
		printk(KERN_ERR "Alloc: dma alloc err\n");
		return -ENOMEM;
	}

	usr_cma.id = ++mem_block_count;
	cmamem_status.phy_base = 	usr_cma.phy_base;
	cmamem_status.id_count = usr_cma.id;
	cmamem_status.status = HAVE_ALLOCED;

	usr_cma.mem_base = vm_mmap(file, 0, size, PROT_READ | PROT_WRITE, MAP_SHARED, 0);
	if (usr_cma.mem_base < 0) {
		printk(KERN_ERR "Alloc: vm_mmap err\n");
		usr_cma.id = --mem_block_count;
		return -ENOMEM;
	}
	printk(KERN_INFO "Alloc: virt_addr: 0x%lx\n", usr_cma.mem_base);
	//mem_block_count ++;

	if (copy_to_user((void __user *)arg, (void *)(&usr_cma), sizeof(struct mem_block))) {
		return -EFAULT;
	}
	/* setup the memory block */
	mem_blk = (struct cmamem_block *)kmalloc(sizeof(struct cmamem_block), GFP_KERNEL);
	if (mem_blk == NULL) {
		printk(KERN_ERR "Alloc: kmalloc err\n");
		mem_block_count --;
		return -1;
	}

	if (usr_cma.name[0] != '\0') {
		memcpy(mem_blk->name, usr_cma.name, 10);
	}
	mem_blk->id = usr_cma.id;
	mem_blk->is_free = 0;
	mem_blk->is_use_buffer	= usr_cma.is_use_buffer;
	mem_blk->mem_base = usr_cma.mem_base;
	mem_blk->kernel_base = usr_cma.kernel_base;
	mem_blk->phy_base = usr_cma.phy_base;
	mem_blk->len = usr_cma.len;

#ifdef MEM_DEBUG
	dump_mem(mem_blk);
#endif

#ifdef CMA_TEST
	int i = 0;
	for (i = 0; i < 10; i++) {
		((char *)(usr_cma.kernel_base))[i] = (usr_cma.id * i);
	}
#endif
	/* add to memory block queue */
	list_add_tail(&mem_blk->memqueue_list, &cmamem_block_head->memqueue_list);

	return 0;
}
static int cmamem_free(struct file *file, unsigned long arg)
{
	struct cmamem_block *mem_blk = NULL;
	struct mem_block usr_cma;
	int ret = 0;

	ret = copy_from_user(&usr_cma, (void __user *)arg, sizeof(struct mem_block));
	if (ret) {
		printk(KERN_ERR "Free: copy_from_user err: %d\n", ret);
		return -1;
	}

	list_for_each_entry(mem_blk, &cmamem_block_head->memqueue_list, memqueue_list) {
		if (mem_blk) {
			if (strcmp(usr_cma.name, mem_blk->name) == 0) {
				if (mem_blk->is_free == 0) {
					printk(KERN_INFO "Free: mem id: %d, name: %s\n",
						usr_cma.id, usr_cma.name);
					vm_munmap(mem_blk->mem_base, mem_blk->len);
					if (mem_blk->is_use_buffer) {
						dma_free_coherent(NULL,	mem_blk->len,
							mem_blk->kernel_base, mem_blk->phy_base);
					} else {
						dma_free_writecombine(NULL, mem_blk->len,
							mem_blk->kernel_base, mem_blk->phy_base);
					}
					mem_blk->is_free = 1;
					list_del(&mem_blk->memqueue_list);
					break;
				}
			}
		}
	}

	return 0;
}

static int cmamem_freeall(void)
{
	struct cmamem_block *mem_blk = NULL;

	printk(KERN_INFO "Free_all: delete all cma mem\n");
	list_for_each_entry(mem_blk, &cmamem_block_head->memqueue_list, memqueue_list) {
		if (mem_blk && (mem_blk->id > 0) && (mem_blk->is_free == 0)) {
			printk(KERN_INFO "Free_all: mem id: %d, name: %s\n", mem_blk->id, mem_blk->name);
			if (mem_blk->is_use_buffer) {
				dma_free_coherent(NULL, mem_blk->len,
					mem_blk->kernel_base, mem_blk->phy_base);
			} else {
				dma_free_writecombine(NULL, mem_blk->len,
					mem_blk->kernel_base, mem_blk->phy_base);
			}
			mem_blk->is_free = 1;
		}
	}

	return 0;
}

static long cmamem_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret = 0;

	switch (cmd) {
	case CMEM_ALLOCATE:
		printk(KERN_INFO "CMEM_ALLOCATE\n");
		mutex_lock(&cmamem_dev.cmamem_lock);
		ret = cmamem_alloc(file, arg);
		mutex_unlock(&cmamem_dev.cmamem_lock);
		break;
	case CMEM_UNMAP:
		printk(KERN_INFO "CMEM_UNMAP\n");
		mutex_lock(&cmamem_dev.cmamem_lock);
		ret = cmamem_free(file, arg);
		mutex_unlock(&cmamem_dev.cmamem_lock);
		break;
	default:
		printk(KERN_ERR "cma mem not support command\n");
		ret = -1;
		break;
	}

	return ret;
}

static int cmamem_mmap(struct file *filp, struct vm_area_struct *vma)
{
	unsigned long start = vma->vm_start;
	unsigned long size = vma->vm_end - vma->vm_start;
	unsigned long offset = vma->vm_pgoff << PAGE_SHIFT;
	unsigned long page = 0, pos = 0;

	if (cmamem_status.status != HAVE_ALLOCED) {
		printk(KERN_ERR "%s, you should allocted memory firstly\n", __func__);
		return -EINVAL;
	}

	pos = cmamem_status.phy_base + offset;
	page = pos >> PAGE_SHIFT ;
	printk(KERN_INFO "Mmap: vmstart: 0x%08lx, vmsize: 0x%lx, offset: 0x%lx, page: 0x%lx\n",
		start, size, offset, page);

	if (remap_pfn_range(vma, start, page, size, PAGE_SHARED)) {
		return -EAGAIN;
	} else {
		printk(KERN_INFO "Mmap: remap_pfn_range success\n");
	}
	//vma->vm_flags &= ~VM_IO;
	//vma->vm_flags |=  (VM_DONTEXPAND | VM_DONTDUMP);
	cmamem_status.status = HAVE_MMAPED;

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
	cmamem_dev.count = 0;
	cmamem_dev.dev.name = DEVICE_NAME;
	cmamem_dev.dev.minor = MISC_DYNAMIC_MINOR;
	cmamem_dev.dev.fops = &dev_fops;

	cmamem_block_head =
		(struct cmamem_block *)kmalloc(sizeof(struct cmamem_block), GFP_KERNEL);
	if (cmamem_block_head == NULL) {
		printk(KERN_ERR "kmalloc err\n");
		return -1;
	}
	cmamem_block_head->id = -1;
	mem_block_count = 0;
	INIT_LIST_HEAD(&cmamem_block_head->memqueue_list);
	cmamem_status.status = UNKNOW_STATUS;
	cmamem_status.id_count = -1;
	cmamem_status.phy_base = 0;

	rval = misc_register(&cmamem_dev.dev);

	return rval;
}

static void __exit cmamem_exit(void)
{
	cmamem_freeall();
	misc_deregister(&cmamem_dev.dev);
	printk(KERN_ERR"%s\n", __func__);
}

module_init(cmamem_init);
module_exit(cmamem_exit);
MODULE_LICENSE("GPL");
