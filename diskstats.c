#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/statvfs.h>
#include <sys/types.h>

#define SKIP_LOOP 1

typedef struct device_mount_info_list device_mount_info_list;

typedef struct {
	unsigned int major;
	unsigned int minor;
	char mount_point[250];
	char device[250];
	char fs_type[64];
} device_mount_info;

struct device_mount_info_list {
	device_mount_info *dev;
	device_mount_info_list *next;
};

void free_devices(device_mount_info_list *d_list) {
	while (d_list->next!=NULL) {
		free(d_list->dev);
		device_mount_info_list *act = d_list;
		d_list = d_list->next;
		free(act);
	}
}

void print_devices(device_mount_info_list *d_list) {
	while (d_list->next!=NULL) {
		printf("%s %s %s\n", d_list->dev->mount_point, d_list->dev->device, d_list->dev->fs_type);
		struct statvfs fsstat;
		statvfs(d_list->dev->mount_point, &fsstat);
		printf("block-size: %lu, blocks-free: %lu\n", fsstat.f_bsize, fsstat.f_bavail);
		d_list = d_list->next;
	}
}

int main() {
	printf("Diskstats v0.1\n");
	const char *mountinfo = "/proc/self/mountinfo";
	FILE *fp = fopen(mountinfo, "r");
	if (fp==NULL) {
		perror("open");
		exit(1);
	}
	char *row = NULL;
	size_t buff_size = 0;
	device_mount_info_list *devices = calloc(1, sizeof(device_mount_info_list));
	device_mount_info_list *d_list_item = devices;

	while (getline(&row, &buff_size, fp)>=0) {
		device_mount_info *d = calloc(1, sizeof(device_mount_info));
		// 30 1 8:6 / / rw,relatime shared:1 - ext4 /dev/sda6 rw,errors=remount-ro
		sscanf(row, "%*u %*u %u:%u %*s %s %*s %*s - %s %s %*s", &d->major, &d->minor, d->mount_point, d->fs_type, d->device);
		bool add = false;
		if (strncmp(d->device, "/dev", 4)==0) {
			if (SKIP_LOOP) {
				if (strncmp(d->device, "/dev/loop", 9)!=0) add = true;
			} else add = true;
		}
		if (add) {
			d_list_item->dev = d;
			d_list_item->next = calloc(1, sizeof(device_mount_info));
			d_list_item = d_list_item->next;
		} else {
			free(d);
		}
	}
	free(d_list_item->next);
	d_list_item->next=NULL;
	free(row);
	fclose(fp);

	print_devices(devices);
	free_devices(devices);
}
