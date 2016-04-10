#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x4c46c04, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xbb323702, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x9b1126bd, __VMLINUX_SYMBOL_STR(dev_set_drvdata) },
	{ 0xf087137d, __VMLINUX_SYMBOL_STR(__dynamic_pr_debug) },
	{ 0x2c3615b1, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0x2e433490, __VMLINUX_SYMBOL_STR(device_register) },
	{ 0x72728632, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x976a9df5, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0x507efb1b, __VMLINUX_SYMBOL_STR(kobject_uevent_env) },
	{ 0x3af0232e, __VMLINUX_SYMBOL_STR(sysfs_notify) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0x7149fc2a, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xa8eda5f0, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x2d428ede, __VMLINUX_SYMBOL_STR(device_unregister) },
	{ 0xe0a5c66c, __VMLINUX_SYMBOL_STR(dev_set_name) },
	{ 0xac517418, __VMLINUX_SYMBOL_STR(__class_create) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "4EF5296E9F37AF21C9CBF20");
