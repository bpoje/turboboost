Linux kernel module for disabling Intel Turbo Boost in CPU. It was created to reduce laptop overheating while using CUDA for longer periods of time (solving Travelling salesman problem on GPU for university thesis). Other methods didn't work on my laptop and BIOS had no option to disable turbo boost.

Created and tested on Ubuntu 12.04 LTS (3.13.0-110-generic). Also works on newer systems.

Use:
```
make
sudo insmod turboBoostDe.ko
dmesg | tail
```

Output:
```
[...] Intel Turbo Boost disable/enable module loaded.
[...] rdmsr: hi=00000040 lo=00850089
[...] wrmsr: hi=00000040 lo=00850089
[...] Intel Turbo Boost is DISABLED.
```
