# Simple
Let's make our first Kernel Module named simple, which will essentially just log some messsages when it's loaded and is removed.
We'll also explore some of Kernel headers and utilize them in the module.

## Pre-requisites
- [Finish workspace setup](../../#setting-up-workspace)
- [Understand workflow](../../#our-workflow)
- Download the folder ```simple``` to local workspace

## Steps

- **Navigate to Project directory**
```
cd simple
```

- **compile Module**
```
sudo make
```

-  **Look for ```simple``` in loaded Modules**
```
sudo lsmod | grep simple
```
> You will get no results

- **Insert Module**
```
sudo insmod simple.ko
```

- **Again check for simple in module list**
```
sudo lsmod | grep simple
```
> Now you will see the ```simple``` module listed
> ![image](https://user-images.githubusercontent.com/75921254/200163554-07648585-8834-482c-85cc-6828b7f55edc.png)

- **Check the ouput in Kernel Buffer**
```
sudo dmesg
```
![image](https://user-images.githubusercontent.com/75921254/200163925-7c53887f-ae23-4441-8779-91a202b32d0a.png)

> You can find it with a bit of searching. </br>
> **Note** : You can safely ignore the message ```module verification failed``` and ```tainting Kernel```, this arises because this module is custom made and not signed.

- **Remove the Module and verify it's gone**
```
sudo rmmod simple
sudo lsmod | grep simple
```
> No output means module is removed

- **Check for message again in Kernel Buffer**
```
sudo dmesg
```
![image](https://user-images.githubusercontent.com/75921254/200163870-3ae9bca6-06f1-4850-a172-907e3e1272ce.png)

- **Clean Kernel Buffer**
```
sudo dmesg -c
```

## Understanding the Code
- **Necessary Headers**
```
#include <linux/init.h>
#include <linux/module.h>
#include <linux/hash.h>
#include <linux/gcd.h>
```

- **This function is called when the module is loaded**
```
static int simple_init(void)
{
printk(KERN_INFO "%s\n", "Loading Kernel Module");
printk(KERN_INFO "golden ratio is %llu\n",GOLDEN_RATIO_PRIME);
printk(KERN_INFO "gcd(3300,24) = %lu\n", gcd(3300,24));
return 0;
}
```
> printk is kernel equivalent of printf, and KERN_INFO is a ```level``` of Kernel log which represents informational message, there are 8 levels of logs depending on severity. </br>
> We utilize the constant ```GOLDEN_RATIO_PRIME``` and function ```gcd``` for outputting some useful stuff. </br>

- **This function is called when the module is removed**
```
static void simple_exit(void)
{
printk(KERN_INFO "%s\n", "Removing Kernel Module");
}
```

- **Macros for registering module entry and exit points**
```
module_init(simple_init);
module_exit(simple_exit);
```
- **Defining additional module info**
```
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
```

> **Note** : We declare custom functions and variable as static, this is to limit their scope to current module,otherwise they would be accessible in whole of Kernel and pollute global namespace!

**Next up : [hello](./hello) Module** 
