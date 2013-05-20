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
	{ 0xeadbfc22, "i2c_register_driver" },
	{ 0x44e69afe, "spi_register_driver" },
	{ 0x993e5324, "driver_unregister" },
	{ 0x62b72b0d, "mutex_unlock" },
	{ 0xb1a2def9, "gpiochip_is_requested" },
	{ 0x4abdb61c, "seq_printf" },
	{ 0xe16b893b, "mutex_lock" },
	{ 0x8a0117ac, "spi_write_then_read" },
	{ 0xba2e8f8d, "i2c_smbus_read_byte_data" },
	{ 0xcb3b5371, "i2c_smbus_write_byte_data" },
	{ 0x640a1617, "i2c_smbus_read_word_data" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xa0780f5f, "i2c_smbus_write_word_data" },
	{ 0xc5ae0182, "malloc_sizes" },
	{ 0x16a864ec, "kmem_cache_alloc_trace" },
	{ 0x2a5687cc, "dev_set_drvdata" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x3f02ca87, "spi_get_device_id" },
	{ 0x63f8869c, "gpiochip_add" },
	{ 0xdc798d37, "__mutex_init" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x37a0cba, "kfree" },
	{ 0xfba4d443, "dev_err" },
	{ 0xf907a894, "gpiochip_remove" },
	{ 0x1579bdaa, "dev_get_drvdata" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("i2c:mcp23008");
MODULE_ALIAS("i2c:mcp23017");
MODULE_ALIAS("spi:mcp23s08");
MODULE_ALIAS("spi:mcp23s17");
