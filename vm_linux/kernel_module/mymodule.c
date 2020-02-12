//////////////////////////////////////////////////////////////////////
//                             University of California, Riverside
//
//
//
//                             Copyright 2016
//
////////////////////////////////////////////////////////////////////////
//
// This program is free software; you can redistribute it and/or modify it
// under the terms and conditions of the GNU General Public License,
// version 2, as published by the Free Software Foundation.
//
// This program is distributed in the hope it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
//
////////////////////////////////////////////////////////////////////////
//
//   Author:  Hung-Wei Tseng
//
//   Description:
//     Skeleton of KeyValue Pseudo Device
//
////////////////////////////////////////////////////////////////////////

#include "mymodule.h"

#include <asm/processor.h>
#include <asm/segment.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/poll.h>
#include <linux/random.h>

int myvalue;

static long mymodule_return_myvalue(struct mymodule_cmd __user *user_cmd)
{
    return myvalue;
}
static long mymodule_set_myvalue(struct mymodule_cmd __user *user_cmd)
{
    struct mymodule_cmd cmd;
    if (copy_from_user(&cmd, user_cmd, sizeof(cmd)))
        return -1;
    myvalue = cmd.value;
    return myvalue;
}
     
unsigned int mymodule_poll(struct file *filp, struct poll_table_struct *wait)
{
    unsigned int mask = 0;
    printk("mymodule_poll called. Process queued\n");
    return mask;
}

static long mymodule_ioctl(struct file *filp, unsigned int cmd,
                                unsigned long arg)
{
    switch (cmd) {
    case MYMODULE_IOCTL_GET:
        return mymodule_return_myvalue((void __user *) arg);
    case MYMODULE_IOCTL_SET:
        return mymodule_set_myvalue((void __user *) arg);
    default:
        return -ENOTTY;
    }
}

static const struct file_operations mymodule_fops = {
    .owner                = THIS_MODULE,
    .unlocked_ioctl       = mymodule_ioctl,
//    .mmap                 = mymodule_mmap,
//    .poll		  = mymodule_poll,
};

static struct miscdevice mymodule_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "mymodule",
    .fops = &mymodule_fops,
};

static int __init mymodule_init(void)
{
    int ret = 1;
    get_random_bytes ( &myvalue, sizeof (myvalue) );
    if ((ret = misc_register(&mymodule_dev)))
        printk(KERN_ERR "Unable to register \"mymodule\" misc device\n");
    return ret;
}

static void __exit mymodule_exit(void)
{
    misc_deregister(&mymodule_dev);
}

MODULE_AUTHOR("Hung-Wei Tseng <htseng3@ncsu.edu>");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
module_init(mymodule_init);
module_exit(mymodule_exit);
