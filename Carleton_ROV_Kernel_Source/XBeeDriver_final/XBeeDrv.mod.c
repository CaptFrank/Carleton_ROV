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
	{ 0x83df6f39, "struct_module" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x349cba85, "strchr" },
	{ 0xd0d8621b, "strlen" },
	{ 0xeb1de92, "malloc_sizes" },
	{ 0x48d1d0c2, "filp_close" },
	{ 0x1d26aa98, "sprintf" },
	{ 0x3c9bdf0c, "vfs_read" },
	{ 0x1b7d4074, "printk" },
	{ 0xb6ed1e53, "strncpy" },
	{ 0x2da418b5, "copy_to_user" },
	{ 0x6c2e3320, "strncmp" },
	{ 0x529c3dd6, "kmem_cache_alloc" },
	{ 0x758c3582, "register_chrdev" },
	{ 0x37a0cba, "kfree" },
	{ 0x9ef749e2, "unregister_chrdev" },
	{ 0xf2a644fb, "copy_from_user" },
	{ 0x2b3a5c21, "vfs_write" },
	{ 0xe914e41e, "strcpy" },
	{ 0x22261a55, "filp_open" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

