#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/wait.h>
#include <linux/semaphore.h>
#include <linux/sched.h>

//#include <semaphore.h>

MODULE_LICENSE("GPL");

#define MAJOR_NUM 254
static ssize_t globalvar_read(struct file *, char *, size_t, loff_t*);
static ssize_t globalvar_write(struct file *, const char *, size_t, loff_t*);

struct file_operations globalvar_fops =
{
	read: globalvar_read,
	write: globalvar_write,
};

static int global_var = 0;
static struct semaphore sem;
static wait_queue_head_t outq;
static int flag = 0;

static int __init globalvar_init(void)
{
	int ret;

	ret = register_chrdev(MAJOR_NUM, "globalvar", &globalvar_fops);
	if (ret) {
		 printk("globalvar register failure");
	} else {
		printk("globalvar register success");
		//init_MUTEX(&sem);
		sema_init(&sem, 1);
		init_waitqueue_head(&outq);
	}

	return ret;
}
static void __exit globalvar_exit(void)
{
//	int ret;

	unregister_chrdev(MAJOR_NUM, "globalvar");
	/*
	if (ret) {
		printk("globalvar unregister failure");
	} else {
		printk("globalvar unregister success");
	}
	*/
}

static ssize_t globalvar_read(struct file *filp, char *buf, size_t len, loff_t *off)
{
	if (wait_event_interruptible(outq, flag != 0)) {
		return - ERESTARTSYS;
	}
	if (down_interruptible(&sem)) {
		return - ERESTARTSYS;
	}
	flag = 0;
	if (copy_to_user(buf, &global_var, sizeof(int))) {
		up(&sem);
		return - EFAULT;
	}
	up(&sem);

	return sizeof(int);
}

static ssize_t globalvar_write(struct file *filp, const char *buf, size_t len,loff_t *off)
{
	if (down_interruptible(&sem)) {
		return - ERESTARTSYS;
	}

	if (copy_from_user(&global_var, buf, sizeof(int))) {
		up(&sem);
		return - EFAULT;
	}

	up(&sem);
	flag = 1;
	wake_up_interruptible(&outq);

	return sizeof(int);
}

module_init(globalvar_init);
module_exit(globalvar_exit);
