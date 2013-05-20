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
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xeadbfc22, "i2c_register_driver" },
	{ 0x77edf722, "schedule_delayed_work" },
	{ 0x3bd1b1f6, "msecs_to_jiffies" },
	{ 0x8a5c7a80, "regulator_enable" },
	{ 0xfa2a45e, "__memzero" },
	{ 0xf9a482f9, "msleep" },
	{ 0xfbc74f64, "__copy_from_user" },
	{ 0x67c2fa54, "__copy_to_user" },
	{ 0xe707d823, "__aeabi_uidiv" },
	{ 0x91f4bf26, "misc_deregister" },
	{ 0x1258d9d9, "regulator_disable" },
	{ 0x6baae653, "cancel_delayed_work_sync" },
	{ 0x1579bdaa, "dev_get_drvdata" },
	{ 0xce384330, "input_event" },
	{ 0xfcec0987, "enable_irq" },
	{ 0x6c8d5ae8, "__gpio_get_value" },
	{ 0xcc5005fe, "msleep_interruptible" },
	{ 0xbd82f6b2, "i2c_transfer" },
	{ 0xc5ae0182, "malloc_sizes" },
	{ 0x37a0cba, "kfree" },
	{ 0xd85ac634, "regulator_put" },
	{ 0x175b693d, "input_unregister_device" },
	{ 0xff0197c6, "misc_register" },
	{ 0x9a0e95c0, "input_free_device" },
	{ 0xf20dabd8, "free_irq" },
	{ 0x8eb105bd, "input_register_device" },
	{ 0x3ce4ca6f, "disable_irq" },
	{ 0xd6b8e852, "request_threaded_irq" },
	{ 0xbe6bf35c, "input_set_capability" },
	{ 0x924df216, "input_allocate_device" },
	{ 0xfb0e29f, "init_timer_key" },
	{ 0x2a5687cc, "dev_set_drvdata" },
	{ 0x900362e7, "regulator_get" },
	{ 0x9d669763, "memcpy" },
	{ 0x16a864ec, "kmem_cache_alloc_trace" },
	{ 0xfba4d443, "dev_err" },
	{ 0x27e1a049, "printk" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xd335581f, "nonseekable_open" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("i2c:l3g4200d");
