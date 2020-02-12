//////////////////////////////////////////////////////////////////////
//                             North Carolina State University
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
//     Skeleton of a Pseudo Device
//
////////////////////////////////////////////////////////////////////////


#ifndef _UAPI_MYMODULE_H
#define _UAPI_MYMODULE_H

#include <linux/types.h>


struct mymodule_cmd {
    int value;
};

#define MYMODULE_IOCTL_GET  _IOWR('N', 0x49, struct mymodule_cmd)
#define MYMODULE_IOCTL_SET  _IOWR('N', 0x48, struct mymodule_cmd)

#endif
