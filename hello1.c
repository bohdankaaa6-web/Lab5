// SPDX-License-Identifier: Dual BSD/GPL
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>
#include "hello1.h"

MODULE_AUTHOR("Bohdana Zhuravel <zhuravel.bohdana_io42@edu.kpi.ua>");
MODULE_DESCRIPTION("Hello1 module - List operations and export symbol");
MODULE_LICENSE("Dual BSD/GPL");

struct hello_time_node {
	struct list_head list_node;
	ktime_t time_start;
	ktime_t time_end;
};

static LIST_HEAD(hello_time_list);

int print_hello(uint count)
{
	uint i;

	if (count == 0 || (count >= 5 && count <= 10)) {
		pr_warn("Warning: print_count is %u (0 or 5-10)\n", count);
	} else if (count > 10) {
		pr_err("Error: print_count %u > 10. Module won't load.\n", count);
		return -EINVAL;
	}

	for (i = 0; i < count; i++) {
		struct hello_time_node *node;

		node = kmalloc(sizeof(*node), GFP_KERNEL);
		if (!node) {
			struct hello_time_node *cursor, *tmp;

			pr_err("Memory allocation failed\n");

			list_for_each_entry_safe(cursor, tmp, &hello_time_list, list_node) {
				list_del(&cursor->list_node);
				kfree(cursor);
			}
			return -ENOMEM;
		}

		node->time_start = ktime_get();
		pr_info("Hello, world!\n");
		node->time_end = ktime_get();

		list_add_tail(&node->list_node, &hello_time_list);
	}

	return 0;
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	return 0;
}

static void __exit hello1_exit(void)
{
	struct hello_time_node *cursor, *tmp;

	list_for_each_entry_safe(cursor, tmp, &hello_time_list, list_node) {
		ktime_t duration = ktime_sub(cursor->time_end, cursor->time_start);

		pr_info("Printing took: %lld ns\n", ktime_to_ns(duration));

		list_del(&cursor->list_node);
		kfree(cursor);
	}

	pr_info("Module hello1 unloaded.\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
