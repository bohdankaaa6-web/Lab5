// SPDX-License-Identifier: Dual BSD/GPL
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include "hello1.h" // Includes prototype of print_hello()

MODULE_AUTHOR("Bohdana Zhuravel <zhuravel.bohdana_io42@edu.kpi.ua>");
MODULE_DESCRIPTION("Hello2 module - Parameter and dependency test");
MODULE_LICENSE("Dual BSD/GPL");

/* Module parameter declaration */
static uint print_count = 1;
module_param(print_count, uint, 0444);
MODULE_PARM_DESC(print_count, "Number of times to print 'Hello, world!'");

static int __init hello2_init(void)
{
    /* Call the function exported by hello1.ko */
    return print_hello(print_count);
}

static void __exit hello2_exit(void)
{
    pr_info("Module hello2 unloaded.\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
