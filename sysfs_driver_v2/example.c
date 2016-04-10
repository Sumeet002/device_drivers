#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>



  static int foo;
  static int baz;
  static int bar;


  static ssize_t foo_show(struct kobject *kobj, struct attribute *attr,
                          char *buf)
  {
          return sprintf(buf, "%d\n", foo);
  }
  
  static ssize_t foo_store(struct kobject *kobj, struct attribute *attr,
                           const char *buf, size_t count)
  {
          sscanf(buf, "%du", &foo);
          return count;
  }

static struct attribute foo_attribute =
          __ATTR(foo, 0666, foo_show, foo_store);


static struct attribute * myattr[] = {
    &foo_attribute.attr,
    NULL,
};

static struct sysfs_ops myops = {
    .show = foo_show,
    .store = foo_store,
};

static struct kobj_type type = {
    .sysfs_ops = &myops,
    .default_attrs = myattr,
};

struct kobject *mykobj;

static int __init hello_module_init(void)
{
    int err = -1;

    mykobj = kzalloc(sizeof(*mykobj), GFP_KERNEL);

    if (mykobj) {
        kobject_init(mykobj, &type);

        if (kobject_add(mykobj, NULL, "%s", "sysfs_sample")) {
             err = -1;
             printk("Sysfs creation failed\n");
             kobject_put(mykobj);
             mykobj = NULL;1
        }
        err = 0;
    }
    return err;
}

static void __exit hello_module_exit(void)
{
    if (mykobj) {
        kobject_put(mykobj);
        kfree(mykobj);
    }
}

module_init(hello_module_init);
module_exit(hello_module_exit);
MODULE_LICENSE("GPL");
