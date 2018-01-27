#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/string.h>

extern struct bus_type my_bus_type;

static int my_probe(struct device *dev)
{
	printk("Driver found device!\n");
	return 0;
}

static int my_remove(struct device *dev)
{
	printk("Driver unpluged!\n");
	return 0;
}

struct device_driver my_driver = {
	.name = "my_dev",
	.bus = &my_bus_type,
	.probe = my_probe,
	.remove = my_remove,
};

static ssize_t my_driver_show(struct device_driver *driver, char *buf)
{
	return sprintf(buf, "%s\n", "This is my driver!");
};
static DRIVER_ATTR(drv, S_IRUGO, my_driver_show, NULL);

static int __init my_driver_init(void)
{
	int ret;

	ret = driver_register(&my_driver);
	if (ret)
		printk("driver_register failed!\n");

	ret = driver_create_file(&my_driver, &driver_attr_drv);
	if (ret)
		printk("create_driver_file failed!\n");

	return ret;
}

static void __exit my_driver_exit(void)
{
	driver_unregister(&my_driver);
}

module_init(my_driver_init);
module_exit(my_driver_exit);
MODULE_AUTHOR("TaoWu");
MODULE_LICENSE("GPL");

