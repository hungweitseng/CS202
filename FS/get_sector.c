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
#define FALLOC_FL_NO_HIDE_STALE  0x4

// Following is not open for public use
struct sector {
    unsigned long long slba;
    unsigned long long count;
};

#define FE_COUNT	8000
#define FE_FLAG_LAST	(1 <<  0)
#define FE_FLAG_UNKNOWN	(1 <<  1)
#define FE_FLAG_UNALLOC	(1 <<  2)
#define FE_FLAG_NOALIGN	(1 <<  8)

#define EXTENT_UNKNOWN (FE_FLAG_UNKNOWN | FE_FLAG_UNALLOC | FE_FLAG_NOALIGN)

struct fe_s {
	__u64 logical;
	__u64 physical;
	__u64 length;
	__u64 reserved64[2];
	__u32 flags;
	__u32 reserved32[3];
};

struct fm_s {
	__u64 start;
	__u64 length;
	__u32 flags;
	__u32 mapped_extents;
	__u32 extent_count;
	__u32 reserved;
};

struct fs_s {
	struct fm_s fm;
	struct fe_s fe[FE_COUNT];
};

#define FIEMAP	_IOWR('f', 11, struct fm_s)
#define SECTOR_OFFSET 9

static int get_sector_list(int fd, struct stat *st, struct sector **slist_p, size_t offset, size_t length)
{
    int blk_size = st->st_blksize >> SECTOR_OFFSET;
    size_t processed_size = 0;
    if(length == 0)
    {
        length = st->st_size;
        length-=offset;
    }
    unsigned long num_blocks = (st->st_size + st->st_blksize - 1) / st->st_blksize;
    int i, err;
    int list_count = 1;
    struct sector *slist;
    *slist_p = slist = (struct sector *)malloc((st->st_blocks / (st->st_blksize >> SECTOR_OFFSET)*sizeof(struct sector)));
#ifdef DEBUG
    struct timeval time_start, time_end;
            gettimeofday(&time_start, NULL);    
#endif
    struct fs_s fs;
    //Seems we can't transfer more than 65536 LBAs (2^16) at once so
    // in that case we split it into multiple transfers
//    int chunk_offset = 16;
//    int chunk_offset = 3;
//    int chunk_offset = (int)log2(NVMED_CHUNK);
    memset(&fs, 0, sizeof(fs));
    fs.fm.length = length;
    fs.fm.flags  = FIEMAP_FLAG_SYNC;
    fs.fm.start = offset;
    fs.fm.extent_count = FE_COUNT;
    #ifdef DEBUG
    fprintf(stderr, "offset: 0x%x\n", offset);
    #endif
    __u64 chunk_num, length_of_the_extent, copied_length_of_the_extent;
    if (!(err = ioctl(fd, FIEMAP, &fs)))
    {
            #ifdef DEBUG
            fprintf(stderr, "FIEMAP %d\n",fs.fm.mapped_extents);
            #endif
            for (int j = 0; j < fs.fm.mapped_extents; j++) 
            {
                copied_length_of_the_extent = 0;
                length_of_the_extent = fs.fe[j].length;
                slist->slba = fs.fe[j].physical >> SECTOR_OFFSET;
//                #ifdef DEBUG
//                    fprintf(stderr, "extent: offset=%x - %x len=%llu flags=0x%x\n", fs.fe[j].logical,
//						fs.fe[j].logical+fs.fe[j].length, fs.fe[j].length, fs.fe[j].flags);
//                #endif
                    if(list_count == 1) // The first sector, starting offset is within the extend.
                    {
                        slist->slba = (fs.fe[j].physical + (offset - fs.fe[j].logical)) >> SECTOR_OFFSET;
                    }
                slist->count = 1;
                #ifdef DEBUG
                fprintf(stderr, "FIEMAP start: 0x%x end: 0x%x extent_length %llu (0x%x) -- extent: %d\n", slist->slba, slist->slba+(length_of_the_extent>>SECTOR_OFFSET) , length_of_the_extent,length_of_the_extent, list_count);
                #endif
                list_count++;
                slist++;
        }
    }
    else
    {

//        fprintf(stderr, "FIEMAP(%d) doesn't work, try FIBMAP\n");
        for (i = 0; i < num_blocks; i++) {
            unsigned long blknum = i;

            if (ioctl(fd, FIBMAP, &blknum) < 0)
                return -1;

            //Seems we can't transfer more than 65536 LBAs at once so
            // in that case we split it into multiple transfers
            if (i != 0 && blknum * blk_size == slist->slba + slist->count &&
                slist->count + blk_size <= 65536) {
                slist->count += blk_size;
                continue;
            }
            

            if (i != 0) {
                if(offset > 0)
                {
                    if((offset >> SECTOR_OFFSET) < slist->count) // starting address falls within this block
                    {
                        slist->slba += (offset >> SECTOR_OFFSET);
                        slist->count -= (offset >> SECTOR_OFFSET);
                        offset = 0;
                    }
                    else
                    {
                        offset -= (slist->count << SECTOR_OFFSET);
                        slist->slba = blknum * blk_size;
                        slist->count = blk_size;
                        continue;
                    }
                }
                processed_size += (slist->count << SECTOR_OFFSET);
                if(processed_size >= length)
                {
                    slist->count -=((processed_size-length) >> SECTOR_OFFSET);
                fprintf(stderr, "FIBMAP start: %p length: %llu copied: %llu total length: %llu\n", slist->slba, slist->count << SECTOR_OFFSET, processed_size, length);
                    fprintf(stderr, "FIBMAP start: %p length: %llu copied: %llu total length: %llu\n", slist->slba, slist->count << SECTOR_OFFSET, processed_size, length);
                    list_count++;
                    return list_count;
                }

                #ifdef DEBUG
                fprintf(stderr, "FIBMAP start: %p length: %llu copied: %llu total length: %llu\n", slist->slba, slist->count << SECTOR_OFFSET, processed_size, length);
                #endif
                slist++;
                list_count++;
            }

            slist->slba = blknum * blk_size;
            slist->count = blk_size;
        }
        if(processed_size < length)
        {
            if(processed_size + (slist->count << SECTOR_OFFSET) >= length)
            slist->count -=((processed_size + (slist->count << SECTOR_OFFSET) - length) >> SECTOR_OFFSET);
          fprintf(stderr, "FIBMAP start: %p length: %llu copied: %llu total length: %llu\n", slist->slba, slist->count << SECTOR_OFFSET, processed_size, length);
          list_count++;
            return list_count;
        }
    }
    #ifdef DEBUG
    gettimeofday(&time_end, NULL);
    printf("HGProfile: FIEMAP %ld\n",((time_end.tv_sec * 1000000 + time_end.tv_usec) - (time_start.tv_sec * 1000000 + time_start.tv_usec)));
    gettimeofday(&time_start, NULL);    
    #endif
    return list_count;
}

static int find_dev(dev_t dev)
{
    static struct dev_cache {
        dev_t dev;
        int fd;
    } cache[] = {[16] = {.dev=-1}};

    for (struct dev_cache *c = cache; c->dev != -1; c++)
        if (c->dev == dev)
            return c->fd;

    const char *dir = "/dev";
    DIR *dp = opendir(dir);
    if (!dp)
        return -errno;

    struct dirent *entry;
    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_type != DT_UNKNOWN  && entry->d_type != DT_BLK)
            continue;

        struct stat st;
        if (fstatat(dirfd(dp), entry->d_name, &st, 0))
            continue;

        if (!S_ISBLK(st.st_mode))
            continue;

        if (st.st_rdev != dev)
            continue;


        int ret = openat(dirfd(dp), entry->d_name, O_RDONLY);

        for (struct dev_cache *c = cache; c->dev != -1; c++) {
            if (c->dev == 0) {
                c->dev = dev;
                c->fd = ret;
            }
        }

        closedir(dp);
        return ret;
    }

    errno = ENOENT;
    closedir(dp);
    return -1;
}

int dev_find(dev_t dev)
{
    int devfd = find_dev(dev);
    if (devfd < 0)
        return -EPERM;

    return devfd;
}

int main(int argc, char **argv)
{
    int ret, fd;
    struct stat st;
    // Hung-Wei: add a current pointer to the destination GPU memory address
    void *current;
    // Hung-Wei: maintain the original writing length
#ifdef DEBUG
    struct timeval time_start, time_end;
#endif
    fd = open(argv[1], O_RDONLY);

    if (fstat(fd, &st))
        return -1;
        
//    fprintf(stderr, "We're here\n");
/*
    int devfd = dev_find(st.st_dev);
    if (devfd < 0) {
        return -1;
    }
*/
//    struct nvme_dev_sector slist[st.st_blocks / (st.st_blksize / 512)];
    struct sector *slist;
    int sector_count = get_sector_list(fd, &st, &slist, 0, st.st_size);
    return 0;
}