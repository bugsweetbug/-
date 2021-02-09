#include <linux/module.h>
#include <linux/module.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/gpio.h>
#define BUFFER_MAX  (10)
#define OK          (0)
#define ERROR       (-1)

struct cdev *gDev;
struct file_operations *gFile;
dev_t devNum; //设备号
unsigned int subDevNum = 1;
int reg_major = 232;
int reg_minor = 0;
char *buffer;
int flag = 0;

int testOpen(struct inode *p, struct file *f)
{
    printk(KERN_EMERG "testOpen\r\n");
    return 0;
}

ssize_t testWrite(struct file *f, const char __user *u, size_t s, loff_t *l)
{
    //从u指向的buf中拿到数据，然后操作寄存器，写到硬件里去
    printk(KERN_EMERG "testWrite\r\n");
    return 0;
}

ssize_t testRead(struct file *f, char __user *u, size_t s, loff_t *l)
{
    //操作寄存器，从硬件里读取数据，然后放到u指向的buf里
    printk(KERN_EMERG "testRead\r\n");
    return 0;
}

int charDrvInit(void)
{
    devNum = MKDEV(reg_major, reg_minor);
    printk(KERN_EMERG "devnum is %d\r\n", devNum);

    if(OK == register_chrdev_region(devNum, subDevNum, "testchar"))
    {
        printk(KERN_EMERG"register_chrdev_region OK\r\n");
    }
    else
    {
        printk(KERN_EMERG"register_chrdev_region error\r\n");
        return ERROR;
    }
    /*
    if(OK == alloc_chrdev_region(&devNum, subDevNum, subDevNum, "test"))
    {
        printk(KERN_EMERG"alloc_chrdev_region OK\r\n");
    }
    else
    {
        printk(KERN_EMERG"alloc_chrdev_region error\r\n");
        return ERROR;
    }
    */
    printk(KERN_EMERG"devNum is %d\r\n", devNum);

    gDev    = kzalloc(sizeof(struct cdev), GFP_KERNEL);//kmalloc,kzalloc会清0
    gFile   = kzalloc(sizeof(struct file_operations), GFP_KERNEL);//GFP_KERNEL表示申请的空间用于内核

    gFile->open     = testOpen;
    gFile->read     = testRead;
    gFile->write    = testWrite;
    gFile->owner    = THIS_MODULE;
    //gDev->owner     = THIS_MODULE;

    cdev_init(gDev, gFile);
    cdev_add(gDev, devNum, 3);
    return 0;
}

void __exit charDrvExit(void)
{
    cdev_del(gDev);
    unregister_chrdev_region(devNum, subDevNum);
    return;
}

module_init(charDrvInit);   //insmod ***.ko时加载入口函数，module_init就是声明charDrvInit为入口函数
module_exit(charDrvExit);   //rmmod ***.ko卸载函数
MODULE_LICENSE("GPL");      //声明支持GPL协议