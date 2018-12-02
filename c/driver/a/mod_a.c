
// Module A (mod_a.c)
#include <linux/list.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
static int func1(void)
{
       printk("In Func: %s...\n",__func__);
       return 0;
}

// Export symbol func1
EXPORT_SYMBOL(func1);


#define MAX_NAME_LEN 32
#define MAX_ID_LEN 10


typedef struct stud
{
    struct list_head list;
    char name[MAX_NAME_LEN];
    char stu_number[MAX_ID_LEN];
}num_n_stu;

int test_mem(void)
{
   	int *p_mem = NULL;
	unsigned long phy_addr = 0;

	p_mem = kzalloc(1024, GFP_KERNEL);
	if (!p_mem) {
		printk("kzalloc failed\n");
		return -1;
	}
	*p_mem = 1234;

	phy_addr = virt_to_phys(p_mem);

	printk("int size=%ld, unsigned long=%ld, int *=%ld\n", sizeof(int), sizeof(unsigned long), sizeof(p_mem));
	printk("p_mem=%p, 0x%lX,  data=%d\n", p_mem, (unsigned long)p_mem, *p_mem);
	printk("phy=0x%lX, \n", phy_addr);
	printk("data=%d \n",  *(int *)phys_to_virt(phy_addr));

	if (p_mem) {
		kfree(p_mem);
		p_mem = NULL;
	}
	return 0;
}

static int __init hello_init(void)
{
       printk("Module 1,Init!\n");

    struct list_head head;
    num_n_stu stu_1;
    num_n_stu stu_2;
    num_n_stu *entry;

    struct list_head *p;
    INIT_LIST_HEAD(&head);

    strcpy(stu_1.name,"lisi");
    strcpy(stu_1.stu_number,"10000000");

    strcpy(stu_2.name,"zhangsan");
    strcpy(stu_2.stu_number,"10000001");

    list_add(&stu_1.list,&head);
    list_add(&stu_2.list,&head);

//    list_del(&stu_2.list);

    list_for_each(p,&head)
    {

        entry=list_entry(p,struct stud,list);

        printk("name: %s\n",entry->name);

        printk("stu_number: %s\n",entry->stu_number);

    }

    list_del(&stu_1.list);

    test_mem();

    return 0;
}

static void __exit hello_exit(void)
{
       printk("Module 1,Exit!\n");
}

module_init(hello_init);
module_exit(hello_exit);

