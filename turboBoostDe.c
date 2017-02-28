/*
	Linux kernel module for disabling Intel Turbo Boost in CPU.
	Copyright (C) 2013  Blaž Poje

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

// Linux Kernel/LKM headers:
//module.h is needed by all modules and kernel.h is needed for KERN_INFO.
#include <linux/module.h>		// included for all kernel modules
#include <linux/moduleparam.h>
#include <linux/kernel.h>		// included for KERN_INFO
#include <linux/init.h>			// included for __init and __exit macros

static bool parDisable = true;
module_param(parDisable, bool, 0444);
MODULE_PARM_DESC(parDisable, "Disables Intel Turbo Boost if true");

static int __init switchTurboBoost(void)
{
	uint32_t hi,lo;
	hi=0; lo=0xb;

	//Effect can be checked with: sudo rdmsr -p0 -f 38:38 0x1a0
	asm volatile("rdmsr":"=a"(lo),"=d"(hi):"c"(0x1a0));
	printk("rdmsr: hi=%08x lo=%08x\n",hi,lo);

	if (parDisable)
		hi = hi | 0x40;
	else
		hi = hi & 0xBF;

	asm volatile("wrmsr"::"c"(0x1a0),"a"(lo),"d"(hi));
	printk("wrmsr: hi=%08x lo=%08x\n",hi,lo);

	if (parDisable)
		printk(KERN_INFO "Intel Turbo Boost is DISABLED.\n");
	else
		printk(KERN_INFO "Intel Turbo Boost is ENABLED.\n");

	return 0;
}
 
static int __init turboBoostDe_init(void)
{
	printk(KERN_INFO "Intel Turbo Boost disable/enable module loaded.\n");

	switchTurboBoost();

	return 0;    // Non-zero return means that the module couldn't be loaded.
}
 
static void __exit turboBoostDe_cleanup(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
}

module_init(turboBoostDe_init);
module_exit(turboBoostDe_cleanup);

MODULE_AUTHOR("Blaž Poje <blaz.poje@gmail.com>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Disables Intel Turbo Boost by default or if parameter "
	"parDisable is true. If parameter parDisable is false turbo boost is enabled.");
