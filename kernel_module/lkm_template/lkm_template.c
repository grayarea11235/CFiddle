// Template for a Linux kernel module

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sachith Muhandiram");
MODULE_DESCRIPTION("Simple first Linux module");
MODULE_VERSION("1.0.0");

static int __init initHelloWorld(void)
{
  printk(KERN_INFO "Hello, this is my first kernel module \n");

  return 0;
}

static void __exit exitHelloWorld(void)
{
  printk(KERN_INFO "Exit Hello world module\n");
}

module_init(initHelloWorld);
module_exit(exitHelloWorld);
