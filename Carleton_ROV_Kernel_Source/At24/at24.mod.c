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
	{ 0xc3fe87c8, "param_ops_uint" },
	{ 0xcf7de6a6, "i2c_del_driver" },
	{ 0xeadbfc22, "i2c_register_driver" },
	{ 0x27e1a049, "printk" },
	{ 0xba2e8f8d, "i2c_smbus_read_byte_data" },
	{ 0x640a1617, "i2c_smbus_read_word_data" },
	{ 0x4c32c4d4, "i2c_smbus_read_i2c_block_data" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x7d11c268, "jiffies" },
	{ 0x62b72b0d, "mutex_unlock" },
	{ 0xf9a482f9, "msleep" },
	{ 0xbd82f6b2, "i2c_transfer" },
	{ 0x28f21f0a, "i2c_smbus_write_i2c_block_data" },
	{ 0x3bd1b1f6, "msecs_to_jiffies" },
	{ 0x9d669763, "memcpy" },
	{ 0xe16b893b, "mutex_lock" },
	{ 0x57975a44, "dev_notice" },
	{ 0xa04aa921, "_dev_info" },
	{ 0x2a5687cc, "dev_set_drvdata" },
	{ 0x6984a952, "sysfs_create_bin_file" },
	{ 0x3bc6065c, "i2c_new_dummy" },
	{ 0xdc798d37, "__mutex_init" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0xfba4d443, "dev_err" },
	{ 0x2110f53, "dev_warn" },
	{ 0x5985bf03, "of_get_property" },
	{ 0xe707d823, "__aeabi_uidiv" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x37a0cba, "kfree" },
	{ 0x56a9e9bb, "i2c_unregister_device" },
	{ 0x38b4ccd1, "sysfs_remove_bin_file" },
	{ 0x1579bdaa, "dev_get_drvdata" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("i2c:24c00");
MODULE_ALIAS("i2c:24c01");
MODULE_ALIAS("i2c:24c02");
MODULE_ALIAS("i2c:spd");
MODULE_ALIAS("i2c:24c04");
MODULE_ALIAS("i2c:24c08");
MODULE_ALIAS("i2c:24c16");
MODULE_ALIAS("i2c:24c32");
MODULE_ALIAS("i2c:24c64");
MODULE_ALIAS("i2c:24c128");
MODULE_ALIAS("i2c:24c256");
MODULE_ALIAS("i2c:24c512");
MODULE_ALIAS("i2c:24c1024");
MODULE_ALIAS("i2c:at24");
