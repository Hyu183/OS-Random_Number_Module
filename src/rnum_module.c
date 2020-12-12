#include <linux/module.h>
//#include <linux/version.h>
//#include <linux/kernel.h>
#include <linux/types.h>
//#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>  
#include <linux/random.h>

#define  DEVICE_NAME "rand_numchar"    
#define  CLASS_NAME  "rand_num" 


static dev_t first; // Global variable for the first device number
static struct cdev c_dev; // Global variable for the character device structure
static struct class *dev_class; // Global variable for the device class

static int dev_open(struct inode *i, struct file *f)
{
    printk(KERN_INFO "RNM: open()\n");
    return 0;
}

static int dev_release(struct inode *i, struct file *f)
{
    printk(KERN_INFO "RNM: close()\n");
    return 0;
}

 static ssize_t dev_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    int result;

    printk(KERN_INFO "RNM: read()\n");
    get_random_bytes(&result,sizeof(result));
    //printk(KERN_INFO "RNM: Generated number: %d\n", result);
    if(copy_to_user(buf,&result,sizeof(result))){
        return -EFAULT;
    }

    return 0;
}


 static struct file_operations pugs_fops =
{
 .owner = THIS_MODULE,
 .open = dev_open,
 .release = dev_release,
 .read = dev_read,
};

static int __init rnm_init(void) /* Constructor */
{
    printk(KERN_INFO "RNM: Initializing\n");
    if (alloc_chrdev_region(&first, 0, 1, DEVICE_NAME) < 0)
    {
        printk(KERN_ALERT "RNM: failed to register <major, minor>\n");
        return -1;
    }
    printk(KERN_INFO  "<Major, Minor>: <%d, %d>\n", MAJOR(first), MINOR(first));

    if ((dev_class = class_create(THIS_MODULE, CLASS_NAME)) == NULL)
    {
        printk(KERN_ALERT "RNM: Failed to create device class\n");
        unregister_chrdev_region(first, 1);
        return -1;
    }
    printk(KERN_INFO "RNM: Create device class successfully\n");

    if (device_create(dev_class, NULL, first, NULL, DEVICE_NAME) == NULL)
    {
        printk(KERN_ALERT "RNM: Failed to create the device \n");
        class_destroy(dev_class);
        unregister_chrdev_region(first, 1);
        return -1;
    }
    printk(KERN_INFO "RNM: Create device successfully\n");

    cdev_init(&c_dev, &pugs_fops);

    if (cdev_add(&c_dev, first, 1) == -1)
    {
        printk(KERN_ALERT "RNM: Failed to add char device to the system\n");
        device_destroy(dev_class, first);
        class_destroy(dev_class);
        unregister_chrdev_region(first, 1);
        return -1;
    }
    return 0;
}
static void __exit rnm_exit(void) /* Destructor */
{
    cdev_del(&c_dev);
    device_destroy(dev_class, first);
    class_destroy(dev_class);
    unregister_chrdev_region(first, 1);
    printk(KERN_INFO "RNM: Our rnm unregistered");
}
module_init(rnm_init);
module_exit(rnm_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("@18120168 @18120183");
MODULE_DESCRIPTION("Create random number");
MODULE_VERSION("0.1");   