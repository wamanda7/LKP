#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>
#include <linux/spinlock.h>
#include <asm/switch_to.h>

/* Jumper probe for context switch */

/*match arguments with context switch routine's*/
//static struct rq *my_jcontext_switch(	struct rq *rq, 
//						struct task_struct *prev, 
//						struct task_struct *next)
void jswitch_to(struct task_struct prev, struct task_struct next, struct task_struct last)
{
	/* May need to change this depending on arguments */
	pr_info("jprobe: prev task: %d, next task: %d\n", prev.pid , next.pid);

	jprobe_return();
}

static struct jprobe my_jprobe = {
	.entry			= (kprobe_opcode_t *)jswitch_to,
	.kp = {
		.symbol_name = "switch_to",
	}
};

//static int __init my_jp_init(void)
int init_module(void)
{
	int ret; 
	/*my_jprobe.kp.addr = (kprobe_opcode_t *)kallsyms_lookup_name("context_switch");
	if(!my_jprobe.kp.addr)
	{
		printk(KERN_INFO "Couldn't find %s to plant jprobe", "context_switch");
		return -1; 
	}*/

	ret = register_jprobe(&my_jprobe);
	if (ret < 0) {
		printk(KERN_INFO "register_jprobe failed, returned %d\n", ret);
		return -1; 
	}
	printk(KERN_INFO "Planted jprobe at %p, handler addr %p\n", my_jprobe.kp.addr, my_jprobe.entry); 

	return 0; 
}

//static void __exit my_jp_exit(void)
void cleanup_module(void)
{
	unregister_jprobe(&my_jprobe); 
	printk(KERN_INFO "jprobe at %p unregistered\n", my_jprobe.kp.addr);

}



MODULE_LICENSE("GPL");
MODULE_AUTHOR("Amanda Woolson <wamanda7@vt.edu>");
MODULE_DESCRIPTION("JProbe Created for context_switch() function");
