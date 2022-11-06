# Linux Kernel Modules
This will be a walkthrough of steps for making custom linux kernel modules, interacting with kernel space and understanding how proc file system works in linux.
We will be completing the programming-project of Chapter-2 of [Operating System Concepts : 10th-edition](https://www.wiley.com/en-us/Operating+System+Concepts%2C+10th+Edition-p-9781119320913)

The code in this repository belongs to the book [Operating System Concepts : 10th-edition](https://www.wiley.com/en-us/Operating+System+Concepts%2C+10th+Edition-p-9781119320913). Though there are minor modifications pertaining to breaking changes in newer kernel versions! </br>
See complete [References](#referencess) below.

# Table Of Contents
- [Introduction](#introduction)
  - [Linux Kernel](#linux-kernel)
  - [Kernel Modules](#kernel-modules)
  - [Proc File System](#proc-file-system)
  - [Compiling Kernel Modules](#compiling-kernel-modules)
  - [Makefile](#makefile)
    - [Introduction](#introduction-1)
    - [Usage in our project](#usage-in-our-project)
- [Setting up workspace](#setting-up-workspace)
  - [Installing Linux](#installing-linux)
  - [Installing necessary package(s) and header(s)](#installing-necessary-packages-and-headers)
- [Our Workflow](#our-workflow)
  - [Compliling Module](#compiling-module)
  - [Testing Module](#testing-module)
- [Running The Project Module(s)](#running-the-project-modules)
  - [First Linux Module](#first-linux-module)
  - [Hello from proc file-system](#hello-from-proc-file-system)
  - [Jiffies](#jiffies)
  - [Lifespan of a Module](#lifespan-of-a-module)
- [What's next](#whats-next)
- [References(s)](#referencess)

# Introduction

## Linux Kernel
Linux operating system follows a monolithic architecture, so it's Kernel is called as monolothic, which means most of the operating system functions from Kernel space, this increases the speed of execution, but makes the Kernel bulky and takes away modularity. So, in modern Kernels, this problem is addressed by linux kernel modules.
![image](https://user-images.githubusercontent.com/75921254/200150901-3d4549e0-853d-482c-b6c0-47241eac5dee.png)
> Image Source : [eBPF - Rethinking the Linux Kernel
](https://www.infoq.com/presentations/facebook-google-bpf-linux-kernel/)

## Kernel Modules
Linux Kernel Modules are object files which can be laoded/linked at Runtime, thus extending the functionality of Kernel when needed, and similarly can be unloaded/unlniked when no longer needed.</br>
Usually Linux Kernel Modules are written in C programming language 
![image](https://user-images.githubusercontent.com/75921254/200151022-3142170a-948b-4fd1-819a-a9ed8684e24f.png)
> Image Source : [How to load or unload a Linux kernel module
](https://opensource.com/article/18/5/how-load-or-unload-linux-kernel-module) 

## Proc File System
In Linux, Proc file system contains runtime information of system and acts as a source of communcation between user and kernel spaces. It is a virtual or pseudo file system
which means the files listed in this aren't really stored permanently, this file system is created and updated at runtime, while it gets destroyed as system is turned off.
![image](https://user-images.githubusercontent.com/75921254/200152086-7ad49e43-f43d-4fa5-8c1b-772144445384.png)

> Image Source : [Slide 38 : Lecture 5 proc file system](https://slideplayer.com/slide/12696991/)


## Compiling Kernel Modules
Compiling a Linux Kernel Module Differs from compiling a normal C-program, since Kernel Modules live in and interact with Kernel space, so the headers used are different and also they must be compiled with same options as Kernel to that make them compatible. Now all this is achieved through use of Makefile and make command!

## Makefile
### Introduction
In general a Makefile is like a guide for compiling a program. When dealing with a simple program with limited files, there's not much inter-dependency and we can manually compile them.</br> But in case of larger projects with many files and inter-dependencies it become neccasary to use a Makefile as :
- It defines the order in which files must be compiled, so as to resolve referencing conflicts!
- In case of any changes in files, it only recompiles required portions thus saving precious time and compuational power.

### Usage in our project
For a module name ```moduleName```, our Makefile will be of form:
```
obj-m += moduleName.o

all:

	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:

	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
```

To compile the Module
```
sudo make
```
> Here ```make```, will automatically identify ```Makefile``` and run it. </br>
> ```moduleName.ko``` will be the Kernel module, we are concerned with.

To destroy the built module
```
sudo make clean
```

#### Understanding the Makefile
```
obj-m += moduleName.o
```
> ```obj-m``` specifies, we need a module as output, while ```moduleName.o``` refers to the source object file, in our case we have moduleName.c source file,
so object file will automatically be generated from it!
```
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
```
> ```all``` is the default ```target``` for a Makefile, which means running command ```make``` alone, will trigger ```all```. </br>
> **Note** : We are recursively calling make again inside Makefile. </br>
> -C option changes the directory of Makfile to ```/lib/modules/$(shell uname -r)/build```, which contains the Makefile used to build the Kernel, thus effectively using it's options. </br>
>Whereas ```M``` is a variable, which causes that makefile to move back into your module source directory before trying to build the modules target. </br>
> And finally ```modules``` is the target we call, which means we are trying to make a module. </br>

```
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
```
> The only difference here is we are using target clean, so ```sudo make clean``` will invoke clean from Makefile of Kernel and clean modules in ```M``` directory.

# Setting up workspace

## Installing Linux
### In this project tutorial, we will be using :
  - [VM Virtual Box](https://www.virtualbox.org/) to install linux, which works like a virtual machine for a given operating system.
  - [Ubuntu](https://ubuntu.com/) distrbution of Linux as Operating system.</br>
> **Note** : Althugh other distributions **may** work with instructions given in this tutorial, it's advisable to adhere strictly to specifications!

### Step(s):
- Download Virtual Box for your Operating system from [here](https://www.virtualbox.org/wiki/Downloads).
- Download Ubuntu 22.04-LTS from [here](https://ubuntu.com/download/desktop/thank-you?version=22.04.1&architecture=amd64).
- Follow this [tutorial](https://www.freecodecamp.org/news/how-to-install-ubuntu-with-oracle-virtualbox/#setup-for-ubuntu) to make a new Ubuntu machine using VMbox.

## Installing necessary package(s) and header(s)

1. **Boot up your Ubuntu system in VMbox and open a terminal**</br>
2. **Install ```build-essential``` & ```kmod``` packages** </br> 
```
sudo apt-get install build-essential kmod
```
> ```kmod``` package provides commands ```lsmod```,```insmod``` and ```rmmod``` which we'll use to list,insert and remove kernel modules respectively.</br>
> ```build-essential``` package provides us with ```gcc compiler``` which we'll use to compile C-files, and ```make``` command to build kernel modules.

3. **Install ```kernel-headers``` for your Kernel version**
```
sudo apt-get update
sudo apt-get install linux-headers-$(uname -r)
```
> ```apt-get update``` fetches latest avaliable versions of packages available to compare against current ones, more info [here](https://linuxhint.com/linux-apt-get-update-command/). </br>
> ```uname -r``` refers to ```unix-name release``` which essentially is the currently installed kernel. </br>
> ```linux-headers-$(uname -r)``` refers to headers corresponding to current kernel version, modules already available in kernel were compiled using these headers and hence, these modules no longer need the headers, but now to compile our own modules, we need these headers! </br>

# Our Workflow
Each of our module will live in a separate directory named ```moduleName```, where ```moduleName``` also is the name of the module we are dealing with, and at starting point before compilation , we would have two files inside ```moduleName``` directory : </br>
- ```moduleName.c``` => the source code of module.
- ```Makefile``` => the Makefile for current module. </br>

> For example : we have a module named ```simple```, so we would have a  directory named ```simple```, and inside it we will have ```simple.c``` and ```Makefile```.

## Compiling Module
1. **navigate to ```modulename``` directory**
``` 
cd /moduleName 
```

2. **To compile the Module**
```
sudo make
```

3. **To destroy the Module**
```
sudo make clean
```

## Testing Module
General instructions for Modules: </br>

- **List All Modules loaded into Kernel**
```
sudo lsmod
```
> same info can be obtained from proc fs
```
cat /proc/modules
```
- **Filtering a specific module named ```moduleName``` from those loaded in Kernel**
```
sudo lsmod | grep moduleName.
```
> [Working with grep](https://www.digitalocean.com/community/tutorials/grep-command-in-linux-unix)

- **Remove a Module named ```moduleName``` loaded into Kernel**
```
sudo rmmod moduleName
```

- **Reading Messages from Kernel Buffer**
```
sudo dmesg
```
> Kernel Modules live in Kernel space and have separate buffer, namely ```kernel ring buffer``` where they write their output.

- **Clearing Kernel Buffer**
```
sudo dmesg -c
```
> Being limited in size, Kernel Buffer may get filled with custom Module outputs, so it's a good idea to clean it up.

- **Reading proc file named ```moduleFile```**
```
cat /proc/moduleFile
```
> this will be useful when we'll create files in proc file system and read them from user space.

# Running The Project Module(s)

## First Linux Module
Make your first Kernel Module named ```simple```, which will essentially just log some messsages when it's loaded and is removed.
We'll also explore some of Kernel headers and utilize them in the module.
> Head over to [simple](/simple)

## Hello from proc file-system
Next, Module ```hello``` will be used to create a proc file named ```hello```, which will send us 'Hello World' from Kernel space
> Head over to [hello](/hello)

## Jiffies
```Jiffies``` is a global variable defined in Linux header ```linux/jiffies.h```, it represents the number of ticks of system clock from system boot till the time it's value is read.
We'll display it's value in a proc file
> Head over to [jiffies](/jiffies)

## Lifespan of a Module
Dynamically calculate the time duration passed since a module is loaded, and display the value in seconds again in proc file named ```seconds```.
Along with jiffies, we'll utilize another constant variable named ```HZ```, which represents the tick frequency, to calculate time elapsed since module was loaded.
> Head over to [seconds](/seconds)

# What's Next
We just touched upon the surface of what Linux Kernel modules actually are, there's so much more to learn!
> Dive Deeper into Kernel and Kernel Modules with [The Linux Kernel Module Programming Guide](https://github.com/sysprog21/lkmpg)

# References(s)
- This Project statement,steps and source codes:
  - [Operating System Concepts, 10th edition](https://www.wiley.com/en-us/Operating+System+Concepts%2C+10th+Edition-p-9781119320913)
- The Linux Operating System source code:
  - [Linux Repository](https://github.com/torvalds/linux)
- Understanding Linux Kernel and Makefile : 
  - [The Linux Kernel](https://linux-kernel-labs.github.io/refs/heads/master/labs/kernel_modules.html)
  - [Decoding Makefile used in our project](https://stackoverflow.com/a/28276326/16729995)
  - [Targets in Makefile](https://stackoverflow.com/a/11288756/16729995)
- Linux Terminal Commands:
  - [Why we run apt-get update](https://linuxhint.com/linux-apt-get-update-command/)
