/* Necessary includes for device drivers */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
//#include <asm/system.h> /* cli(), *_flags */
#include <asm/uaccess.h> /* copy_from/to_user */
#include <linux/device.h> /* class_creatre */
#include <linux/cdev.h> /* cdev_init */

#define DEVICE_NAME "memory"

MODULE_LICENSE("Dual BSD/GPL");

/*Declaration of memory functions */
int  __init memory_init(void);
void __exit memory_exit(void);

int memory_open(struct inode *inode, struct file *filp);
int memory_release(struct inode *inode, struct file *filp);
ssize_t memory_read(struct file *filp, char *buf, size_t count, loff_t *f_ops);
ssize_t memory_write(struct file *filp, const char *buf, size_t count, loff_t *f_ops);

/*Structure that declares
usual file access functions*/

struct file_operations memory_fops={

	.read = memory_read,
	.write = memory_write,
	.open = memory_open,
	.release = memory_release,
};

/*Global varaibles of the driver */
/*Buffer to store data */

char *memory_buffer;
struct class *memory_class = 0;
dev_t dev_num;
struct cdev cdev;

int __init memory_init(void)
{
	int result;
	
	/*Registering device*/
	result=alloc_chrdev_region(&dev_num,0,0,DEVICE_NAME);
	if(result < 0){
		printk(KERN_ALERT "memory:cannot register device\n");
		return result;
	}

	memory_buffer = kmalloc(1,GFP_KERNEL);
	if(!memory_buffer){
		result = -ENOMEM;
		goto fail;
	}

	memset(memory_buffer,0,1);
	
	/*populate sysfs entry*/
	memory_class = class_create(THIS_MODULE,DEVICE_NAME);

	/*connect the file operations with the cdev*/
	cdev_init(&cdev,&memory_fops);

	cdev.owner = THIS_MODULE;

	/*connect the major/minor number to cdev*/

	if(cdev_add(&cdev,dev_num,1)){
		printk(KERN_ALERT "BAd cdev\n");
		return 1;
	}

	/*send uevents to udev,so it'll create the /dev node */
	device_create(memory_class,NULL,dev_num,NULL,"memory%d",1);
	
	printk(KERN_ALERT "Inserting memory module");
	return 0;

	fail:
	memory_exit();
	return result;
	
     }
	
		
void __exit memory_exit(void)
{
	
	/*remove the cdev*/
	cdev_del(&cdev);

	/*Freeing the major number */
	unregister_chrdev_region(dev_num,1);
	
	device_destroy(memory_class,dev_num);

	class_destroy(memory_class);

	/*Freeing buffer memory*/
	if(memory_buffer){
		kfree(memory_buffer);
	}

	printk(KERN_ALERT "Removing memory module\n");
}

int memory_open(struct inode *inode , struct file *filp)
{
	//SUCCESS
	return 0;
}
		
int memory_release(struct inode *inode , struct file *filp)
{
	//SUCCESS
	return 0;
}

ssize_t memory_read(struct file *filp, char *buf, size_t count, loff_t *f_ops)
{
	/*Transfering data to user space*/
	copy_to_user(buf,memory_buffer,1);

	/*Changing reading position as best suits */
	if(*f_ops==0){
		*f_ops+=1;
		return 1;
	}
	else{
		return 0;
	}
}

ssize_t memory_write(struct file *filp, const char *buf, size_t count, loff_t *f_ops)
{

	const char *tmp;
	
	tmp = buf + count -1;
	copy_from_user(memory_buffer,tmp,1);
	return 1;

}

/*declaration of the init and exit functions*/

module_init(memory_init);
module_exit(memory_exit);

