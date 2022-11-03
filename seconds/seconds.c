#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>
#include <asm/param.h>
#define BUFFER_SIZE 128
#define PROC_NAME "seconds"
unsigned long jiffies_init;
ssize_t proc_read(struct file *file, char __user *usr_buf,size_t count, loff_t *pos);
static const struct proc_ops hello_ops= {
.proc_read = proc_read
};
/* This function is called when the module is loaded. */
int proc_init(void)
{
jiffies_init = jiffies;
/* creates the /proc/hello entry */
proc_create(PROC_NAME, 0666, NULL, &hello_ops);
return 0;
}
/* This function is called when the module is removed. */
void proc_exit(void)
{
/* removes the /proc/hello entry */
remove_proc_entry(PROC_NAME, NULL);
}

/* This function is called each time /proc/hello is read */
ssize_t proc_read(struct file *file, char __user *usr_buf,size_t count, loff_t *pos)
{
int rv = 0;
char buffer[BUFFER_SIZE];
static int completed = 0;
if (completed) {
completed = 0;
return 0;
}
completed = 1;
rv = sprintf(buffer,"Module living for %lu seconds\n" ,(jiffies-jiffies_init)/HZ);
/* copies kernel space buffer to user space usr buf */
copy_to_user(usr_buf, buffer, rv);
return rv;
}
module_init(proc_init);
module_exit(proc_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SECONDS MODULE");
MODULE_AUTHOR("SGG");
