#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include "dummy.h"
#include <linux/notifier.h>
#include <linux/ctype.h>
#include <linux/err.h>
#include <linux/fb.h>
#include <linux/slab.h>

#define DEVICE_NAME "dummy"
MODULE_LICENSE("Dual BSD/GPL");

static const char const *dummy_types[] = {
	[DUMMY_RAW] = "raw",
	[DUMMY_PLATFORM] = "platform",
	[DUMMY_FIRMWARE] = "firmware",
};

static void dummy_generate_event(struct dummy_device *dm,
				     enum dummy_update_reason reason)
{
	char *envp[2];

	switch (reason) {
	case DUMMY_UPDATE_SYSFS:
		envp[0] = "SOURCE=sysfs";
		break;
	default:
		envp[0] = "SOURCE=unknown";
		break;
	}
	envp[1] = NULL;

	kobject_uevent_env(&dm->dev.kobj, KOBJ_CHANGE, envp);
	
	sysfs_notify(&dm->dev.kobj, NULL, "result");
}

static struct device_attribute dummy_device_attributes[]={

	__ATTR(hello_name, 0644, dummy_string_show, dummy_string_store),
	__ATTR(result, 0444, dummy_show_result, NULL),
	__ATTR(type, 0444, dummy_show_type, NULL),
	__ATTR_NULL,
};


static ssize_t dummy_string_show(struct device *dev,struct device_attribute *attr, char *buf)
{
	struct dummy_device *dm = to_dummy_device(dev);

	return sprintf(buf, "%s\n", dm->props.hello_name);
}


static ssize_t dummy_string_store(struct device *dev,struct device_attribute *attr, const char *buf, size_t count)
{
	
	struct dummy_device *dm = to_dummy_device(dev);
	char *hello_name;	

	mutex_lock(&dm->ops_lock);

	dm->props.hello_name = hello_name;

	dummy_update_status(dm);
	
	mutex_unlock(&dm->ops_lock);

	dummy_generate_event(dm, DUMMY_UPDATE_SYSFS);

	return (0);
}


static ssize_t dummy_show_type(struct device *dev,struct device_attribute *attr, char *buf)
{
	struct dummy_device *dm = to_dummy_device(dev);

	return sprintf(buf, "%s\n", dummy_types[dm->props.type]);
}


static ssize_t dummy_show_result(struct device *dev,struct device_attribute *attr, char *buf)
{
	int rc = -ENXIO;
	
	struct dummy_device *dm = to_dummy_device(dev);

	mutex_lock(&dm->ops_lock);
	if (dm->ops && dm->ops->get_hello_name)
		rc = sprintf(buf, "%s\n", dm->ops->get_hello_name(dm));
	mutex_unlock(&dm->ops_lock);

	return rc;
}

static struct class *dummy_class;

static void dummy_device_release(struct device *dev)
{
	struct dummy_device *dm = to_dummy_device(dev);
	kfree(dm);
}




struct dummy_device *dummy_device_register(const char *name,
	struct device *parent, void *devdata, const struct dummy_ops *ops,
	const struct dummy_properties *props)
{
	struct dummy_device *new_dm;
	int rc;

	pr_debug("dummy_device_register: name=%s\n", name);

	new_dm = kzalloc(sizeof(struct dummy_device), GFP_KERNEL);
	if (!new_dm)
		return ERR_PTR(-ENOMEM);

	mutex_init(&new_dm->update_lock);
	mutex_init(&new_dm->ops_lock);

	new_dm->dev.class = dummy_class;
	new_dm->dev.parent = parent;
	new_dm->dev.release = dummy_device_release;
	dev_set_name(&new_dm->dev, name);
	dev_set_drvdata(&new_dm->dev, devdata);


	if (props)
		memcpy(&new_dm->props, props, sizeof(struct dummy_properties));
	
	new_dm->props.type = DUMMY_RAW;
	

	rc = device_register(&new_dm->dev);
	if (rc) {
		kfree(new_dm);
		return ERR_PTR(rc);
	}

	
	new_dm->ops = ops;

	return new_dm;
}

EXPORT_SYMBOL(dummy_device_register);



void dummy_device_unregister(struct dummy_device *dm)
{
	if (!dm)
		return;


	mutex_lock(&dm->ops_lock);
	dm->ops = NULL;
	mutex_unlock(&dm->ops_lock);

	device_unregister(&dm->dev);
}


EXPORT_SYMBOL(dummy_device_unregister);

static void __exit dummy_class_exit(void)
{
	class_destroy(dummy_class);
}

static int __init dummy_class_init(void)
{
	dummy_class = class_create(THIS_MODULE, DEVICE_NAME);
	if (IS_ERR(dummy_class)) {
		printk(KERN_WARNING "Unable to create dummy class; errno = %ld\n",
				PTR_ERR(dummy_class));
		return PTR_ERR(dummy_class);
	}
	
	dummy_class->dev_attrs = dummy_device_attributes;
	return 0;

}

module_init(dummy_class_init);
module_exit(dummy_class_exit);


