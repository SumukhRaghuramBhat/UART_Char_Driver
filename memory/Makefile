# Cross compilation Makefile for ARM
KERN_SRC=/home/hrishi/eos_labs/linux-3.16.49/
obj-m = memory.o
 
all: 
	make -C $(KERN_SRC) ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-  M=$(PWD) modules

clean:
	make -C $(KERN_SRC) ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-  M=$(PWD) clean
	rm exec_mem_test

app:
	arm-linux-gnueabi-gcc -Wall --static -O2 -marm -march=armv5 mem_test.c -o exec_mem_test

