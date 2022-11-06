# hello
Next, Module ```hello``` will be used to create a proc file named ```hello```, which will send us 'Hello World' from Kernel space

## Pre-requisites
- [Finish workspace setup](../../../#setting-up-workspace)
- [Understand workflow](../../../#our-workflow)
- Download the folder ```hello``` to local workspace

## Steps

- **Navigate to Project directory**
```
cd hello
```

- **compile Module**
```
sudo make
```

-  **Look for ```hello``` in loaded Modules**
```
sudo lsmod | grep hello
```
> You will get no results

- **Look for file ```hello``` in porc fs**
```
cat /proc/hello
```
> you will see : ```cat: proc/hello: No such file or directory```

- **Insert Module**
```
sudo insmod hello.ko
```

- **Again check for simple in module list**
```
sudo lsmod | grep hello
```
> Now you will see the ```hello``` module listed
> ![image](https://user-images.githubusercontent.com/75921254/200165604-88bb069c-2e5f-4c68-94e8-2984d91394a1.png)

- **Check the proc file again**
```
cat /proc/hello
```
![image](https://user-images.githubusercontent.com/75921254/200165790-4d758cb7-84c1-4b23-a772-de51a2d06d77.png)

> Now we can see ```Hello World``` ! </br>

- **Remove the Module and verify it's gone**
```
sudo rmmod hello
sudo lsmod | grep hello
```
> No output means module is removed

- **Check for proc file again**
```
cat /proc/hello
```
![image](https://user-images.githubusercontent.com/75921254/200165896-e81b327a-cc2c-4214-9155-b84122e31fd8.png)

## Understanding the Code
- **Necessary Headers**
```
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
```

- **Define macros**
```
#define BUFFER_SIZE 128
#define PROC_NAME "hello"
```

- **Declare function to be called upon proc file read**
```
static ssize_t proc_read(struct file *file, char __user *usr_buf,size_t count, loff_t *pos);
```
> ```proc_read``` will be called whenver we read the created proc file

- **Set options for creating new proc file**
```
static const struct proc_ops hello_ops= {
.proc_read = proc_read
};
```
> ```proc_ops``` is struct which has options to be used while creating a proc file entry, here we only need to read proc file, hence, we only set the 
> ```proc_read``` member, which is a function pointer.

- **This function is called when module is loaded**
```
static int proc_init(void)
{
/* creates the /proc/hello entry */
proc_create(PROC_NAME, 0666, NULL, &hello_ops);
return 0;
}
```
> ```proc_create``` function creates a new proc file entry with name as ```PROC_NAME```.
>  It returns a type called ```proc_dir_entry``` which is a struct representing a proc file. </br>
> Permissions of read and write are given by passing ```0666``` . [Permissions : Numeric Notation](https://en.wikipedia.org/wiki/File-system_permissions#Numeric_notation) </br>
> parent is set to ```NULL``` hence our file be directly inside /proc </br>
> lastly ```&hello_ops``` refers to options we defined above to be used while creating the proc file. </br>

- **This function is called when module is removed**
```
{
static void proc_exit(void)
{
/* removes the /proc/hello entry */
remove_proc_entry(PROC_NAME, NULL);
}
}
```
> We remove the prof file using it's name ```PROC_NAME``` and passing parent as NULL, since it was created at top level.

- **This function is called each time ```/proc/hello``` is read**
```
static ssize_t proc_read(struct file *file, char __user *usr_buf,size_t count, loff_t *pos)
{
int rv = 0;
char buffer[BUFFER_SIZE];
static int completed = 0;
if (completed) {
completed = 0;
return 0;
}
completed = 1;
rv = sprintf(buffer,"%s\n" ,"Hello World");
/* copies kernel space buffer to user space usr buf */
copy_to_user(usr_buf, buffer, rv);
return rv;
}
```
> This function will be called each time we read a proc file. So function will be called from user space and function lives in kernel space </br>
> ```file``` is a file pointer, which refers to ```proc file```, we are reading. </br>
> ```usr_buf``` is the buffer in user space which will be written into. </br>
> ```count``` refers to the max bytes of data that can written to ```usr_buf```. </br>
> ```pos``` refers to positional offset while writting to ```usr_buf```. </br>

**Note** : in our case when we write ```cat /proc/hello```, so the ```cat``` command calls the ```proc_read``` function with ```file``` pointer to our ```hello``` proc file, ```usr_buf``` as our ```terminal```, and a significant ```count``` and generally zero ```pos```. </br>

**But how would the ```proc_read``` function caller, here ```cat command``` know that no more data is to be received?** </br>
> ```proc_read``` returns the number of bytes, which is equal to number of chars written, so ```proc_read``` is called repeteadly until it returns a ```0``` value,
> which means no more data is to be written, whereas a return value of ```-1``` indicates failure! </br>

> So we initialize ```buffer``` and ```rv``` in Kernel space, referring to Kernel buffer and read-value which is returned by ```proc_read```. </br>
> initially ```completed``` is made 0, to specify that data is still left to be written. </br>
> finally we count ```rv``` using sprintf, and copy the Kernel buffer : ```buffer``` to user buffer : ```usr_buf```, and return rv. </br>
> In next call to ```proc_read```, completed is 1 and hence function returns! </br>



- **Macros for registering module entry and exit points**
```
module_init(simple_init);
module_exit(simple_exit);
```
- **Defining additional module info**
```
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello Module");
MODULE_AUTHOR("SGG");
```

> **Note** : We declare custom functions and variable as static, this is to limit their scope to current module,otherwise they would be accessible in whole of Kernel and pollute global namespace!

**Next up : [jiffies](./jiffies) Module** 
