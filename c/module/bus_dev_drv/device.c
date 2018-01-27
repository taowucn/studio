#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/string.h>

extern struct device my_bus;
extern struct bus_type my_bus_type;

//static int my_device_release()
//{
//	return 0;
//}

struct device my_dev = {
	.bus = &my_bus_type,
	.parent = &my_bus,
//	.release = my_device_release,
};

static ssize_t mydev_show(struct device *dev, char *buf)
{
	return sprintf(buf, "%s\n", "This is my device!");
}
static DEVICE_ATTR(dev, S_IRUGO, mydev_show, NULL);

static int __init my_device_init(void)
{
	int ret;
	strncpy(my_dev.init_name, "my_dev", 128);

	ret = device_register(&my_dev);
	if (ret)
		printk("device register!\n");

	ret = device_create_file(&my_dev, &dev_attr_dev);
	if (ret)
		printk("create_device_file failed!\n");

	return ret;
}

static void __exit my_device_exit(void)
{
	device_unregister(&my_dev);
}

module_init(my_device_init);
module_exit(my_device_exit);
MODULE_AUTHOR("TaoWu");
MODULE_LICENSE("GPL");

