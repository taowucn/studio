#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init hello_init(void)
{
	printk("%s(%d) HelloWorld!\n", __FUNCTION__, __LINE__);

	return 0;
}

static void __exit hello_exit(void)
{
	printk("%s(%d) Goodbye!\n", __FUNCTION__, __LINE__);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tao Wu <wtomybo@gmail.com>");
MODULE_DESCRIPTION("Hello Driver");
MODULE_VERSION("1.0");
