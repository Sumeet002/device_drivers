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
	{ 0x7d3420dc, __VMLINUX_SYMBOL_STR(single_release) },
	{ 0x4a9cd35b, __VMLINUX_SYMBOL_STR(seq_read) },
	{ 0x5449ff9, __VMLINUX_SYMBOL_STR(seq_lseek) },
	{ 0x48f9a0c5, __VMLINUX_SYMBOL_STR(remove_proc_entry) },
	{ 0xed798b1, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0x5f2ea640, __VMLINUX_SYMBOL_STR(seq_printf) },
	{ 0x8ffa2322, __VMLINUX_SYMBOL_STR(single_open) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "15E0A3D7C56AB0154E5A911");
