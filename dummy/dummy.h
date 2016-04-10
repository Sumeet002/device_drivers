
#include <linux/device.h>
#include <linux/mutex.h>
#include <linux/notifier.h>


enum dummy_update_reason {
	DUMMY_UPDATE_SYSFS,
};

enum dummy_type {
	DUMMY_RAW = 1,
	DUMMY_PLATFORM,
	DUMMY_FIRMWARE,
	DUMMY_TYPE_MAX,
};

struct dummy_device;

struct dummy_ops {
	unsigned int options;

	/* Notify the dummy driver some property has changed */
	int (*update_status)(struct dummy_device *);
	
	/* Return the current string */
	char *(*get_hello_name)(struct dummy_device *);
	
};


/* This structure defines all the properties of a dummy */
struct dummy_properties {
	
	/* Current User requested string name */
	char *hello_name;
	
	/* Dummy type */
	enum dummy_type type;

	/* Flags used to signal drivers of state changes */
	/* Upper 4 bits are reserved for driver internal use */
	unsigned int state;

};

struct dummy_device {
	/* Dummy properties */
	struct dummy_properties props;

	/* Serialise access to update_status method */
	struct mutex update_lock;

	/* This protects the 'ops' field. If 'ops' is NULL, the driver that
	   registered this device has been unloaded, and if class_get_devdata()
	   points to something in the body of that driver, it is also invalid. */
	struct mutex ops_lock;
	const struct dummy_ops *ops;

	struct device dev;
};

static inline void dummy_update_status(struct dummy_device *dm)
{
	mutex_lock(&dm->update_lock);
	if (dm->ops && dm->ops->update_status)
		dm->ops->update_status(dm);
	mutex_unlock(&dm->update_lock);
}


void dummy_device_unregister(struct dummy_device *dm);
static ssize_t dummy_string_show(struct device *dev,struct device_attribute *attr, char *buf);
static ssize_t dummy_string_store(struct device *dev,struct device_attribute *attr, const char *buf, size_t count);
static ssize_t dummy_show_type(struct device *dev,struct device_attribute *attr, char *buf);
static ssize_t dummy_show_result(struct device *dev,struct device_attribute *attr, char *buf);

#define to_dummy_device(obj) container_of(obj, struct dummy_device, dev)

static inline void * dummy_get_data(struct dummy_device *dummy_dev)
{
	return dev_get_drvdata(&dummy_dev->dev);
}

struct generic_dummy_info {
	const char *name;
};
