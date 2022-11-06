#include <linux/init.h>
#include <linux/module.h>
#include <linux/hash.h>
#include <linux/gcd.h>

/* This function is called when the module is loaded. */
static int simple_init(void)
{
printk(KERN_INFO "%s\n", "Loading Kernel Module");
printk(KERN_INFO "golden ratio is %llu\n",GOLDEN_RATIO_PRIME);
printk(KERN_INFO "gcd(3300,24) = %lu\n", gcd(3300,24));
return 0;
}
/* This function is called when the module is removed. */
static void simple_exit(void)
{
printk(KERN_INFO "%s\n", "Removing Kernel Module");
}
/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
