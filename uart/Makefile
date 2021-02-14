# Cross compilation Makefile for ARM
KERN_SRC=/home/hrishi/eos_labs/linux-3.16.49/
obj-m = uart_char_driver.o
 
all: app
	make -C $(KERN_SRC) ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-  M=$(PWD) modules

clean:
	make -C $(KERN_SRC) ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-  M=$(PWD) clean
	rm exec_uart_test

app:
	arm-linux-gnueabi-gcc -Wall --static -O2 -marm -march=armv5 uart_test.c -o exec_uart_test

