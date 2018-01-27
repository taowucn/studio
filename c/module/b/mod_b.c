
// Module B (mod_b.c)
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>

static struct task_struct *tsk;

static int func2(void)
{
//       extern int func1(void);
//       func1();
       printk("In Func: %s...\n",__func__);

	dump_stack();

	printk("In Func: %s done\n",__func__);
       return 0;
}

static int thread_function(void *data)
{
	int time_count = 0;

	do {
		printk(KERN_INFO "thread_function: %d times", ++time_count);
		schedule_timeout(1000);
//		msleep(1000);
	}while(!kthread_should_stop() && time_count<=30);

	return time_count;
}

static int __init hello_init(void)
{
       printk("Module 2,Init!\n");
       func2();
	tsk = kthread_run(thread_function, NULL, "mythread%d", 1);
	if (IS_ERR(tsk)) {
		printk("create kthread failed\n");
	} else {
		printk("create kthread ok\n");
	}
       return 0;
}

static void __exit hello_exit(void)
{
       printk("Module 2,Exit!\n");
	if (!IS_ERR(tsk)) {
		int ret = kthread_stop(tsk);
		printk("thread function has run %d\n", ret);
	}
}

module_init(hello_init);
module_exit(hello_exit);

