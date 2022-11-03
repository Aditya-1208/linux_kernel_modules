#include <linux/init.h>
#include <asm/param.h>
#include <linux/jiffies.h>
#include <linux/gcd.h>
#include <linux/hash.h>
#include <linux/module.h>

long unsigned int jiffies_init;

/* This function is called when the module is loaded. */
int simple_init(void)
{
printk(KERN_INFO "%s\n", "Loading Kernel Module");
printk(KERN_INFO "golden ratio is %llu\n",GOLDEN_RATIO_PRIME);
printk(KERN_INFO "gcd(3300,24) = %lu\n", gcd(3300,24));
printk(KERN_INFO "Clock rate = %i\n",HZ);
printk(KERN_INFO "jiffies at load = %lu\n", jiffies);
jiffies_init = jiffies;
return 0;
}
/* This function is called when the module is removed. */
void simple_exit(void)
{
printk(KERN_INFO "jiffies at remove = %lu\n",jiffies);
printk(KERN_INFO "modules lived for around %lu seconds\n",(jiffies-jiffies_init)/HZ);
printk(KERN_INFO "%s\n", "Removing Kernel Module");
}
/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
