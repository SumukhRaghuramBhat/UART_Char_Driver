#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm/io.h>

#define MAJOR (60)
#define UARTCLASS "uart_class"
#define MYDEVICE "my_device"

enum {
	UART_FR_RXFE = 0x10,
	UART_FR_TXFF = 0x20,
	UART0_ADDR = 0x101F1000,
};

static u16 uart_data;
void *uartDrRegion = NULL;
void *uartFrRegion = NULL;

ssize_t uart_read(struct file *filp, char *buf,
		size_t count, loff_t *f_pos)
{
	ssize_t retval = 0;

	if(count == 0) {
		return 0;
	}
	printk("\nIn %s\n", __func__);

	while(ioread16((uartFrRegion)) & UART_FR_RXFE);
	uart_data = (ioread16(uartDrRegion));
	if(copy_to_user((char __user *)buf, &uart_data, 1)) {
		return -EFAULT;
	}
	if(*f_pos == 0) {
		*f_pos += 1;
		retval = 1;
	} else {
		retval = 0;
	}

	return retval;
	/*return simple_read_from_buffer((char __user *)buf,
	  count, f_pos, &uart_data, 1);*/
}

ssize_t uart_write( struct file *filp, const char *buf,
		size_t count, loff_t *f_pos)
{
	int i;
	int counter = 0;
	printk("In %s\n", __func__);
	for(i = 0; i < count; i++) {
		if(copy_from_user(&uart_data, (char __user *)buf+i, 1)) {
			return -ENOMEM;
		}

		while(ioread16(uartFrRegion) & UART_FR_TXFF);
		iowrite16(uart_data, uartDrRegion);
		counter++;
	}
	return counter;
}

struct file_operations f_ops = {
	.read   =  uart_read,
	.write  =  uart_write,
};

int init_mod(void)
{
	int result;

	result = register_chrdev(MAJOR, MYDEVICE, &f_ops);
	if (result < 0) {
		printk(KERN_ERR "mem_dev: cannot obtain MAJOR number %d\n", MAJOR);
		return result;
	}

	if((uartDrRegion = ioremap(UART0_ADDR, 4095/*0xF*/)) == NULL) {
		printk(KERN_ERR "Error in DR ioremap.\n");
		unregister_chrdev(MAJOR, MYDEVICE);
		return -1;
	}
	uartFrRegion = uartDrRegion + 0x18;
	/*if((uartFrRegion = ioremap(UART0_ADDR+0x18, 0xF)) == NULL)
	  {
	  printk(KERN_ERR "Error in FR ioremap.\n");
	  unregister_chrdev(MAJOR, MYDEVICE);
	  iounmap(uartDrRegion);
	  return -1;
	  }*/

	printk(KERN_INFO "Inserting memory module\n");
	return 0;
}

void exit_mod(void)
{
	printk(KERN_INFO "Removing memory module\n");
	unregister_chrdev(MAJOR, MYDEVICE);
	iounmap(uartDrRegion);
}

module_init(init_mod);
module_exit(exit_mod);

MODULE_AUTHOR("Avadhut Naik");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Embedded Operating Systems: Lab-6");
