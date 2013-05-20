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
	{ 0xa8c16cf3, "module_layout" },
	{ 0x94cf417b, "i2c_del_driver" },
	{ 0xe213b160, "i2c_register_driver" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0x28318305, "snprintf" },
	{ 0x9297e8ae, "mutex_unlock" },
	{ 0xa8f330aa, "i2c_smbus_read_byte_data" },
	{ 0x7d11c268, "jiffies" },
	{ 0x5ae4fa58, "mutex_lock" },
	{ 0x8ea914af, "hwmon_device_register" },
	{ 0x1928bb46, "sysfs_create_group" },
	{ 0x3e18b1c5, "__mutex_init" },
	{ 0xc5ac94d0, "dev_set_drvdata" },
	{ 0xd4590aa1, "devm_kzalloc" },
	{ 0x47d5fe66, "sysfs_remove_group" },
	{ 0xbd0c0ca7, "hwmon_device_unregister" },
	{ 0x70c55916, "dev_get_drvdata" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("i2c:ltc2990");

MODULE_INFO(srcversion, "41151220B082FB254032297");
