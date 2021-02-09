#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <mach/ gpio.h>
#include <mach/regs-gpio.h>
#include <linux/slab.h>
#include <plat/gpio-cfg.h>
#include <linux/gpio.h>
#define DEVICE_NAME "leds"
#define BUFFER_MAX  (10)
#define OK          (0)
#define ERROR       (-1)
#define LED_NUM ARRAY_SIZE(led_gpios)
static int led_gpios[] = {
    S5PV210_GPJ2(0),
    S5PV210_GPJ2(1),
    S5PV210_GPJ2(2),
    S5PV210_GPJ2(3)
};


struct cdev *gDev;
struct file_operations *gFile;
dev_t devNum; //设备号
unsigned int subDevNum = 1;
int reg_major = 232;
int reg_minor = 0;
char *buffer;
int flag = 0;
char kbuf[4] = {0};

int ledsOpen(struct inode *p, struct file *f)
{
    printk(KERN_EMERG "ledsOpen\r\n");
    return 0;
}

ssize_t ledsWrite(struct file *f, const char __user *u, size_t s, loff_t *l)
{
    int n = 0;
    int len = 4;
    int i;
    printk(KERN_EMERG "ledsWrite\r\n");

    n = copy_from_user(kbuf, u, len);
    if(n != 0)
        return -ENOMEM;
    for (i = 0; i < LED_NUM; i++)
        gpio_set_value(led_gpios[i], !kbuf[i]);
    return len;
}

ssize_t ledsRead(struct file *f, char __user *u, size_t s, loff_t *l)
{
    //操作寄存器，从硬件里读取数据，然后放到u指向的buf里
    printk(KERN_EMERG "ledsRead\r\n");
    return 0;
}

static long ledsIoctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    printk(KERN_EMERG "ledsIoctl\r\n");
    switch(cmd)
    {
        case 0:
        case 1:
            if(arg > LED_NUM){
                return -EINVAL;
            }
            gpio_set_value(led_gpios[arg], !cmd);
			printk(DEVICE_NAME":%d %d\n", arg, cmd);
            break;
        default:
            return -EINVAL;
    }
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

    gDev    = kzalloc(sizeof(struct cdev), GFP_KERNEL);
    gFile   = kzalloc(sizeof(struct file_operations), GFP_KERNEL);

    gFile->open     = ledsOpen;
    gFile->read     = ledsRead;
    gFile->write    = ledsWrite;
    gFile->unlocked_ioctl = ledsIoctl;
    gFile->owner    = THIS_MODULE;
    //gDev->owner     = THIS_MODULE;

    cdev_init(gDev, gFile);
    cdev_add(gDev, devNum, 3);
    return 0;
}

void __exit charDrvExit(void)
{
    int i;
    for (i = 0; i < LED_NUM; i++)
    {
        gpio_free(led_gpios[i]);
    }
        cdev_del(gDev);
    unregister_chrdev_region(devNum, subDevNum);
    return;
}

module_init(charDrvInit);   //insmod ***.ko时加载入口函数，module_init就是声明charDrvInit为入口函数
module_exit(charDrvExit);   //rmmod ***.ko卸载函数
MODULE_LICENSE("GPL");      //声明支持GPL协议