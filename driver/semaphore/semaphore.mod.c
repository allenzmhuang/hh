#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
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
	{ 0x2faaac00, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x691aa8af, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x3dabe7aa, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x8ab8e058, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x587ddf3d, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x63b1a160, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0x8eb0d694, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0xbc6492db, __VMLINUX_SYMBOL_STR(cdev_alloc) },
	{ 0x8ce8c859, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x6dc0c9dc, __VMLINUX_SYMBOL_STR(down_interruptible) },
	{ 0x50eedeb8, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x78e739aa, __VMLINUX_SYMBOL_STR(up) },
	{ 0xb4390f9a, __VMLINUX_SYMBOL_STR(mcount) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "D526763C83773337DFD7250");
