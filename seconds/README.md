# Lifespan of a module
Dynamically calculate the time duration passed since a module is loaded, and display the value in seconds again in proc file named seconds.
Along with jiffies, we'll utilize another constant variable named HZ, which represents the tick frequency, to calculate time elapsed since module was loaded.

## Pre-requisites
- [Finish workspace setup](../../../#setting-up-workspace)
- [Understand workflow](../../../#our-workflow)
- Download the folder ```seconds``` to local workspace

## Steps
Almost similar to hello jiffies, just we calculate and output time elapsed since module loaded.

- **Navigate to Project directory**
```
cd seconds
```

- **compile Module**
```
sudo make
```

-  **Look for ```seconds``` in loaded Modules**
```
sudo lsmod | grep seconds
```
> You will get no results

- **Look for file ```seconds``` in proc fs**
```
cat /proc/seconds
```
> you will see : ```cat: proc/seconds: No such file or directory```

- **Insert Module**
```
sudo insmod seconds.ko
```

- **Again check for seconds in module list**
```
sudo lsmod | grep seconds
```
> Now you will see the ```seconds``` module listed </br>
> ![image](https://user-images.githubusercontent.com/75921254/200176059-dd1f139d-cfe9-4a22-bf25-c7a21cb6bd39.png)

- **Check the proc file again**
```
cat /proc/seconds
```
![image](https://user-images.githubusercontent.com/75921254/200176012-0e4e6342-f128-4778-a4fe-90756f36204e.png)

> Now we can see ```Module living for 14 seconds``` ! </br>
> each time we read ```seconds``` proc file, we will see a new value, which is dynamically calculated when read! </br>
- **Remove the Module and verify it's gone**
```
sudo rmmod seconds
sudo lsmod | grep seconds
```
> No output means module is removed

- **Check for proc file again**
```
cat /proc/seconds
```
![image](https://user-images.githubusercontent.com/75921254/200176097-22b44397-807b-4f21-965d-154a0ff3e30d.png)


## Understanding the Code
- **Additional Header(s)**
```
#include <asm/param.h>
```

- **Initialize a variable to store ```jiffies``` at module load**
```
static unsigned long jiffies_init;
```

- **Store jiffies into jiffies_init at module load inside ```proc_init```**
```
jiffies_init = jiffies;
```

- **Inside proc_read modified ```sprintf```**
```
rv = sprintf(buffer,"Module living for %lu seconds\n" ,(jiffies-jiffies_init)/HZ);
```
We calculate effective duration since module was loaded using current ```jiffies``` and ```jiffies_init```, together with ```HZ``` contant.


> **Note** : We declare custom functions and variable as static, this is to limit their scope to current module,otherwise they would be accessible in whole of Kernel and pollute global namespace!

**Next up : [What's next](/../../#whats-next)** 
