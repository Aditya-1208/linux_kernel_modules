# jiffies
Jiffies is a global variable defined in Linux header linux/jiffies.h,
it represents the number of ticks of system clock from system boot till the time it's value is read. We'll display it's value in a proc file

## Pre-requisites
- [Finish workspace setup](../../../#setting-up-workspace)
- [Understand workflow](../../../#our-workflow)
- Download the folder ```jiffies``` to local workspace

## Steps
Almost similar to hello module, just we output current value of jiffies.

- **Navigate to Project directory**
```
cd jiffies
```

- **compile Module**
```
sudo make
```

-  **Look for ```jiffies``` in loaded Modules**
```
sudo lsmod | grep jiffies
```
> You will get no results

- **Look for file ```jiffies``` in proc fs**
```
cat /proc/jiffies
```
> you will see : ```cat: proc/jiffies: No such file or directory```

- **Insert Module**
```
sudo insmod jiffies.ko
```

- **Again check for jiffies in module list**
```
sudo lsmod | grep jiffies
```
> Now you will see the ```jiffies``` module listed
> ![image](https://user-images.githubusercontent.com/75921254/200175162-09050062-0770-4dc8-8cb4-77344a30ef18.png)

- **Check the proc file again**
```
cat /proc/jiffies
```
![image](https://user-images.githubusercontent.com/75921254/200175201-26155ef1-91ab-4d08-9d12-62f2b2837565.png)

> Now we can see ```Current Jiffies value = 4299321332``` ! </br>

- **Remove the Module and verify it's gone**
```
sudo rmmod jiffies
sudo lsmod | grep jiffies
```
> No output means module is removed

- **Check for proc file again**
```
cat /proc/jiffies
```
![image](https://user-images.githubusercontent.com/75921254/200175281-39525985-9b00-4639-87ab-f233f77ead90.png)


## Understanding the Code
- **Additional Header(s)**
```
#include <linux/jiffies.h>
```

- **Inside proc_read modified ```sprintf```**
```
rv = sprintf(buffer,"Current Jiffies value = %lu\n" ,jiffies);
```

> **Note** : We declare custom functions and variable as static, this is to limit their scope to current module,otherwise they would be accessible in whole of Kernel and pollute global namespace!

**Next up : [seconds](/seconds) Module** 
