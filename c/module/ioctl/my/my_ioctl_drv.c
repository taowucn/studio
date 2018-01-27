#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/io.h>
#include <linux/slab.h>

#include <asm/uaccess.h>

#include "my_cmd.h"

#ifndef my_info
#define my_info(str, arg...) do {\
	printk(KERN_INFO"[my] "str, ##arg); \
}while(0);
#endif

static int fmt = 0;
module_param(fmt, int, S_IRUGO);

typedef struct my_drv_s {
	u32 major;
	u32 minor;
	int value;
	char buf[MAX_BUFF_SIZE];
	u32 cur_size;
	char *mem_start;
	u32 mem_size;
	dev_t devno;
	struct cdev my_cdev;
} my_drv_t;

my_drv_t mydrv;

static int my_open(struct inode *node, struct file *filp)
{
	my_drv_t *dev;
	dev = container_of(node->i_cdev, my_drv_t, my_cdev);
	filp->private_data = dev;

	my_info("%s(%d)\n", __FUNCTION__, __LINE__);
	return 0;
}

static int my_close(struct inode *node, struct file *filp)
{
	my_info("%s(%d)\n", __FUNCTION__, __LINE__);
	return 0;
}

ssize_t my_read(struct file *filp, char __user *buf, size_t count, loff_t *offset)
{
	int ret = 0;
	my_drv_t *dev = filp->private_data;

	if (*offset > MAX_BUFF_SIZE) {
		return count ? -ENXIO: 0;
	}
	if ((*offset + count ) > MAX_BUFF_SIZE) {
		count = MAX_BUFF_SIZE - *offset;
	}
	my_info("u_buf[%s], k_buf[%s], offset[%d],  count[%u]\n",
		buf, dev->buf, *offset, count);

	if (copy_to_user(buf, dev->buf + *offset, count)) {
		ret = - EFAULT;
	} else{
		ret = count;
		mydrv.cur_size -= count;
		*offset += count;
		my_info("read to user buf[%s]\n", buf);
	}

	my_info("%s(%d)\n", __FUNCTION__, __LINE__);

	return ret;
}

ssize_t my_write(struct file *filp, const char __user *buf, size_t count, loff_t *offset)
{
	int ret = 0;
	my_drv_t *dev = filp->private_data;

	if (copy_from_user(dev->buf + *offset, buf, count)) {
		ret = - EFAULT;
	} else{
		ret = count;
		mydrv.cur_size += count;
		*offset += count;
		my_info("wirte to kernel buf[%s], cur_size[%u], offset[%d]\n",
			mydrv.buf, mydrv.cur_size, *offset);
	}

	my_info("%s(%d)\n", __FUNCTION__, __LINE__);
	return ret;
}

loff_t my_llseek (struct file *filp, loff_t offset, int whence)
{
	loff_t new_pos;
	loff_t old_pos = filp->f_pos;

	switch(whence){
	case SEEK_SET:
		new_pos = offset;
		break;
	case SEEK_CUR:
		new_pos = old_pos + offset;
		break;
	case SEEK_END:
		new_pos = MAX_BUFF_SIZE;
		break;
	default:
		return -EINVAL;
		break;
	}
	if ((new_pos < 0) || (new_pos > MAX_BUFF_SIZE)) {
		my_info("invalid pos\n");
		return -EINVAL;
	}
	filp->f_pos = new_pos;

	my_info("%s(%d)\n", __FUNCTION__, __LINE__);
	return new_pos;
}

static int a_ioctl(my_drv_t *dev, unsigned int cmd, u32 __user * arg)
{
	int ret = 0;

	my_info("%s(%d)\n", __FUNCTION__, __LINE__);

	switch (cmd){
	case MY_IO_SET:
		ret = copy_from_user(arg, dev->value, sizeof(u32));
		my_info("%s(%d), my_buf[%s], arg[%d]\n",
			__FUNCTION__,__LINE__, dev->buf, arg);
		break;
	case MY_IO_GET:
		ret = copy_to_user(arg, dev->value, sizeof(u32));
		my_info("%s(%d), my_buf[%s], arg[%d]\n",
			__FUNCTION__, __LINE__, dev->buf, arg);
		break;
	default:
		break;
	}

	return ret;
}

static int b_ioctl(my_drv_t *dev, unsigned int cmd, u32 __user * arg)
{
	int ret = 0;

	my_info("%s(%d)\n", __FUNCTION__, __LINE__);

	switch (cmd){
	case MY_IO_INFO:
		my_info("mem_start: %s\n", mydrv.mem_start);
		break;
	default:
		break;
	}

	return ret;
}

static long my_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	my_drv_t *dev = filp->private_data;

	my_info("%s(%d)\n", __FUNCTION__, __LINE__);

	switch (_IOC_TYPE(cmd)) {
	case MY_MAGIC_A:
		ret = a_ioctl(dev, cmd,  (void __user *)arg);
		break;
	case MY_MAGIC_B:
		ret = b_ioctl(dev, cmd,  (void __user *)arg);
		break;
	default:
		printk("unknow ioc type\n");
		ret = -ENOIOCTLCMD;
		break;
	}

	my_info("%s(%d)\n", __FUNCTION__, __LINE__);

	return ret;
}

static int my_mmap(struct file *filp, struct vm_area_struct *vma)
{
	int ret = 0;
	my_drv_t *dev = filp->private_data;

	//vma->vm_flags |= VM_IO;
	//vma->vm_flags |= VM_RESERVED;
	ret = remap_pfn_range(vma, vma->vm_start, virt_to_phys(dev->mem_start)>>PAGE_SHIFT,
		MAX_BUFF_SIZE, vma->vm_page_prot);
	if (ret) {
		ret = -EAGAIN;
	}
	my_info("virt[0x%lx], phy[0x%x]", dev->mem_start, virt_to_phys(dev->mem_start));
	my_info("%s(%d)\n", __FUNCTION__, __LINE__);
	return ret;
}

struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_close,
	.write = my_write,
	.read = my_read,
	.llseek = my_llseek,
	.unlocked_ioctl = my_ioctl,
	.mmap = my_mmap,
};

static int __init my_init(void)
{
	int ret = 0;
	mydrv.major = 250;
	mydrv.minor = 0;
	mydrv.cur_size = 0;
	memset(mydrv.buf, 0, sizeof(mydrv.buf));

	if (mydrv.major) {
		mydrv.devno = MKDEV(mydrv.major, mydrv.minor);
		ret = register_chrdev_region(mydrv.devno, 1, "mydrv");
	} else {
		ret = alloc_chrdev_region(&mydrv.devno, mydrv.minor, 1, "mydrv");
		mydrv.major = MAJOR(mydrv.devno);
		mydrv.minor = MINOR(mydrv.devno);
	}

	if (ret < 0) {
		my_info("register failed\n");
	}
	my_info("MAJOR[%d], MINOR[%d]\n", mydrv.major, mydrv.minor);

	cdev_init(&mydrv.my_cdev, &my_fops);
	mydrv.my_cdev.owner = THIS_MODULE;
	ret = cdev_add(&mydrv.my_cdev, mydrv.devno, 1);
	if (ret < 0) {
		my_info("add failed\n");
	}

	mydrv.mem_start = kmalloc(MAX_BUFF_SIZE, GFP_KERNEL);
	if (!mydrv.mem_start) {
		ret = -ENOMEM;
		my_info("kmalloc failed\n");
	}
	memset(mydrv.mem_start, 0, MAX_BUFF_SIZE);

	my_info("%s(%d)\n", __FUNCTION__, __LINE__);

	return ret;
}

static void __exit my_exit(void)
{
	if (mydrv.mem_start) {
		kfree(mydrv.mem_start);
	}

	cdev_del(&mydrv.my_cdev);
	unregister_chrdev_region(mydrv.devno, 1);

	my_info("%s(%d)\n", __FUNCTION__, __LINE__);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tao Wu <wtomybo@gmail.com>");
MODULE_DESCRIPTION("Test ioctl");
MODULE_VERSION("v0.1");

