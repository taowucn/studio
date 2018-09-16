#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/pci.h>
#include <linux/gpio.h>

#define DEVICE_NAME "mymap"

int debug_level = 0;
module_param(debug_level, int, 0644);
MODULE_PARM_DESC(debug_level,
	"driver debug level, [0, 2]");

#include "mmap_print.h"

static unsigned char *kvirt = NULL;

static void my_vm_open(struct vm_area_struct *vma)
{
	prt_debug("pid: %u, virt: 0x%lx\n", current->pid, vma->vm_start);
	return;
}

static void my_vm_close(struct vm_area_struct *vma)
{
	prt_debug( "pid: %u, virt: 0x%lx\n", current->pid, vma->vm_start);
	return;
}

static const struct vm_operations_struct my_vm_ops = {
	.open = my_vm_open,
	.close = my_vm_close,
};

static int my_map(struct file *filp, struct vm_area_struct *vma)
{
	unsigned long start = (unsigned long)vma->vm_start;
	unsigned long size = (unsigned long)(vma->vm_end - vma->vm_start);
	unsigned long page = 0;

	page = virt_to_phys(kvirt);
	if (remap_pfn_range(vma, start, page >> PAGE_SHIFT, size, PAGE_SHARED)) {
		return -EFAULT;
	} else {
		prt_info("mmap: remap_pfn_range success\n");
		vma->vm_ops = &my_vm_ops;
		vma->vm_private_data = kvirt;
		my_vm_open(vma);
	}

	return 0;
}

static int my_open(struct inode *inode, struct file *file)
{
	prt_debug("pid: %u\n",  current->pid);
	return 0;
}

static int my_close(struct inode *inode, struct file *file)
{
	prt_debug("pid: %u\n", current->pid);
	return 0;
}

static struct file_operations dev_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_close,
	.mmap = my_map,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &dev_fops,
};

static ssize_t hwrng_attr_current_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	char *p = NULL;
	int i = 0;

	p = (char *)kvirt;
	for (i = 0; i < 8; i++) {
		printk(KERN_DEBUG "%d, ", *p++);
	}
	printk(KERN_DEBUG "\n");

	return 0;
}

static DEVICE_ATTR(rng_current, S_IRUGO, hwrng_attr_current_show, NULL);

static int __init dev_init(void)
{
	int ret = 0;

	do {
		ret = misc_register(&misc);
		if (ret < 0) {
			printk(KERN_ERR "misc register err\n");
			break;
		}

		kvirt = (unsigned char *)kmalloc(PAGE_SIZE, GFP_KERNEL);
		if (kvirt == NULL) {
			prt_err("kmalloc err\n");
			ret = -1;
			break;
		}
		memset(kvirt, 12, 8);
		//SetPageReserved(virt_to_page(kvirt));
		ret = device_create_file(misc.this_device, &dev_attr_rng_current);
		if (ret < 0) {
			prt_err("misc register err\n");
			break;
		}
	} while (0);

	prt_debug("\n");

	return ret;
}

static void __exit dev_exit(void)
{
	misc_deregister(&misc);
	//ClearPageReserved(virt_to_page(kvirt));

	if (kvirt != NULL) {
		kfree(kvirt);
		kvirt = NULL;
	}
	prt_debug("\n");
}

module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TaoWu");
