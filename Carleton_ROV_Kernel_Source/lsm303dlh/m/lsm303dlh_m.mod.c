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
	{ 0xf2c658e5, "module_layout" },
	{ 0xcf7de6a6, "i2c_del_driver" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xeadbfc22, "i2c_register_driver" },
	{ 0x64d515f3, "sysfs_remove_group" },
	{ 0x996bdb64, "_kstrtoul" },
	{ 0xcb3b5371, "i2c_smbus_write_byte_data" },
	{ 0xc5ae0182, "malloc_sizes" },
	{ 0x37a0cba, "kfree" },
	{ 0xd85ac634, "regulator_put" },
	{ 0x1258d9d9, "regulator_disable" },
	{ 0x6637df1e, "sysfs_create_group" },
	{ 0xdc798d37, "__mutex_init" },
	{ 0x8a5c7a80, "regulator_enable" },
	{ 0x900362e7, "regulator_get" },
	{ 0x178bda9, "dev_set_name" },
	{ 0x9d669763, "memcpy" },
	{ 0x2a5687cc, "dev_set_drvdata" },
	{ 0x16a864ec, "kmem_cache_alloc_trace" },
	{ 0x62b72b0d, "mutex_unlock" },
	{ 0xfba4d443, "dev_err" },
	{ 0x4c32c4d4, "i2c_smbus_read_i2c_block_data" },
	{ 0xe16b893b, "mutex_lock" },
	{ 0xa04aa921, "_dev_info" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x91715312, "sprintf" },
	{ 0x1579bdaa, "dev_get_drvdata" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

