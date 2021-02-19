////////////////////////////////////////////////////////////////////////
//
//   Author: Hung-Wei Tseng
//
//   Date:   Nov 02 2016
//
//   Description:
//	Program to dump block addresses assoicated with a file
//
////////////////////////////////////////////////////////////////////////
#include <stdio.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <sys/time.h>

#include <math.h>

#include <string.h>
#include <stdlib.h>

#include <linux/fiemap.h>
#include <linux/nvme.h>
	struct nvme_user_io io = {
		.opcode		= opcode,
		.flags		= 0,
		.control	= control,
		.nblocks	= nblocks,
		.rsvd		= 0,
		.metadata	= (__u64)(uintptr_t) metadata,
		.addr		= (__u64)(uintptr_t) data,
		.slba		= slba,
		.dsmgmt		= dsmgmt,
		.reftag		= reftag,
		.appmask	= apptag,
		.apptag		= appmask,
	};

int main(int argc, char **argv)
{
	static const char *perrstr;
	int err, fd;
	char *data;
	struct nvme_user_io io;
	int max_packet_size = 256;
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <device>\n", argv[0]);
		return 1;
	}
	if(argc > 2)
	 max_packet_size = atoi(argv[2]);
	data = (char *)malloc(max_packet_size*512*sizeof(char));
	perrstr = argv[1];
	fd = open(argv[1], O_RDWR);
	if (fd < 0)
		goto perror;

	io.opcode = nvme_cmd_write;
	io.flags = 0;
	io.control = 0;
	io.metadata = (unsigned long)0;
	io.addr = (unsigned long)data;
	io.slba = 0;
	io.nblocks = 7;
	io.dsmgmt = 0;
	io.reftag = 0;
	io.apptag = 0;
	io.appmask = 0;

	io.opcode = nvme_cmd_read;
	io.nblocks = max_packet_size - 1;
	err = ioctl(fd, NVME_IOCTL_SUBMIT_IO, &io);
	if (err < 0)
		goto perror;
	if (err)
		fprintf(stderr, "nvme read status:%x\n", err);
        free(data);
	return 0;

 perror:
        free(data);
	perror(perrstr);
	return 1;
}
