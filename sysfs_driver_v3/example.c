#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/slab.h>
  
static int foo;
static ssize_t foo_show(struct class *class, struct class_attribute *attr,char *buf){
          return sprintf(buf, "%d\n", foo);
}
  
static ssize_t foo_store(struct class *class, struct class_attribute *attr,const char *buf, size_t count){
          sscanf(buf, "%du", &foo);
          return count;
}
  
static struct class_attribute example_class_attrs[] = {
	__ATTR(foo, 0666, foo_show, foo_store),
	__ATTR_NULL,
};

static struct class example_class = {
	.name =		"example",
	.owner =	THIS_MODULE,

	.class_attrs =	example_class_attrs,
};

static int __init example_sysfs_init(void){

	int status;	
	status = class_register(&example_class);
	if (status < 0)
		return status;
	
}

static int __exit example_exit(void){

	class_destroy(example_class);

}

module_init(example_sysfs_init);
module_exit(example_exit);
MODULE_LICENSE("GPL");	

