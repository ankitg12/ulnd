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
	{ 0x7c7e1083, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x31aa331f, __VMLINUX_SYMBOL_STR(unregister_netdev) },
	{ 0x7d73a919, __VMLINUX_SYMBOL_STR(free_netdev) },
	{ 0x58efec82, __VMLINUX_SYMBOL_STR(register_netdev) },
	{ 0x8230bef6, __VMLINUX_SYMBOL_STR(alloc_netdev_mqs) },
	{ 0xba98f931, __VMLINUX_SYMBOL_STR(ether_setup) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "3A90367E73458397074C91C");
