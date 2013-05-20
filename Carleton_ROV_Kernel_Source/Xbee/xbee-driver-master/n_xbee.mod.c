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
	{ 0x86b4f7b4, "register_netdev" },
	{ 0xdb69b58b, "alloc_netdev_mqs" },
	{ 0x51bf89f5, "tty_register_ldisc" },
	{ 0x3cc48689, "free_netdev" },
	{ 0xb2854412, "unregister_netdev" },
	{ 0x268f4ed2, "netif_rx" },
	{ 0xa784859d, "skb_push" },
	{ 0xe9752e9a, "skb_put" },
	{ 0x6128b5fc, "__printk_ratelimit" },
	{ 0xba759793, "dev_alloc_skb" },
	{ 0x7d11c268, "jiffies" },
	{ 0x92461556, "consume_skb" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x5f754e5a, "memset" },
	{ 0x9d669763, "memcpy" },
	{ 0xc5ae0182, "malloc_sizes" },
	{ 0x4141f80, "__tracepoint_module_get" },
	{ 0x16a864ec, "kmem_cache_alloc_trace" },
	{ 0x37a0cba, "kfree" },
	{ 0x61238fc7, "module_put" },
	{ 0x27e1a049, "printk" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xe74d8a38, "__netif_schedule" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

