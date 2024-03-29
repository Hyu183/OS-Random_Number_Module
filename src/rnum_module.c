#include <linux/module.h>
#include <linux/types.h>
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
	//Tao so ngau nhien
    get_random_bytes(&result,sizeof(result));
    
	//Tra so ngau nhien ve cho user
    if(copy_to_user(buf,&result,sizeof(result))){
        printk(KERN_ALERT "RNM: Failed to return generated number to user\n");
        return -EFAULT;
    }
    printk(KERN_INFO "RNM: Return generated number to user successfully\n");
    return 0;
}

//Cac thao tac muon thuc hien
 static struct file_operations pugs_fops =
{
 .owner = THIS_MODULE,
 .open = dev_open,
 .release = dev_release,
 .read = dev_read,
};

//Ham duoc thuc thi nay sau khi lap module vao kernel
static int __init rnm_init(void)
{ 
    printk(KERN_INFO "RNM: Initializing\n");
	//Dang ky so hieu <major, minor> cho file thiet bi
    if (alloc_chrdev_region(&first, 0, 1, DEVICE_NAME) < 0)
    {
        printk(KERN_ALERT "RNM: failed to register <major, minor>\n");
        return -1;
    }
    printk(KERN_INFO  "<Major, Minor>: <%d, %d>\n", MAJOR(first), MINOR(first));

	/*Tao file thiet bi tu dong*/

	//Tao lop thiet bi
    if ((dev_class = class_create(THIS_MODULE, CLASS_NAME)) == NULL)
    {
        printk(KERN_ALERT "RNM: Failed to create device class\n");
        unregister_chrdev_region(first, 1);
        return -1;
    }
    printk(KERN_INFO "RNM: Create device class successfully\n");

	//Tao thong tin <major, minor>  cho thiet bi
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

// Ham duoc thuc thi ngay truoc khi thao module ra khoi kernel
static void __exit rnm_exit(void) 
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
MODULE_VERSION("1.0");   