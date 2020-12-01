#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "metrics.h"

metric__uptime *get_uptime() {
	metric__uptime *value = calloc(1, sizeof(metric__uptime));
	char contents[1024];
	read_file("/proc/uptime", contents, 1024);
	char *token = strtok(contents, " ");
	value->in_seconds = atof(token);
	token = strtok(NULL, " ");
	value->idle_sum_seconds = atof(token);

	return value;
}

metric__loadavg *get_loadavg() {
	metric__loadavg *value = calloc(1, sizeof(metric__loadavg));
	char *t_ptr1, *t_ptr2;
	char contents[1024];
	read_file("/proc/loadavg", contents, 1024);
	char *token = strtok_r(contents, " ", &t_ptr1);
	value->avg_1_min = atof(token);
	token = strtok_r(NULL, " ", &t_ptr1);
	value->avg_5_min = atof(token);
	token = strtok_r(NULL, " ", &t_ptr1);
	value->avg_15_min = atof(token);
	token = strtok_r(NULL, " ", &t_ptr1);
	char *token1 = strtok_r(token, "/", &t_ptr2);
	value->process_running = atoi(token1);
	token1 = strtok_r(NULL, "/", &t_ptr2);
	value->process_total = atoi(token1);
	token = strtok_r(NULL, " ", &t_ptr1);
	value->last_process_id = atoi(token);

	return value;
}

metric__memory *get_meminfo() {
	metric__memory *value = calloc(1, sizeof(metric__memory));
	char *t_ptr1, *t_ptr2;
	char contents[2048];
	read_file("/proc/meminfo", contents, 2048);
	char *token = strtok_r(contents, "\n", &t_ptr1);
	while (token != NULL) {
		char *token1 = strtok_r(token, " ", &t_ptr2);
		if (token1 != NULL) {
			char *prop_value = strtok_r(NULL, " ", &t_ptr2);
			if (strcmp(token1, "MemTotal:") == 0) {
				value->mem_total = atol(prop_value) * 1024;
			} else if (strcmp(token1, "MemFree:") == 0) {
				value->mem_free = atol(prop_value) * 1024;
			} else if (strcmp(token1, "Cached:") == 0) {
				value->mem_cached = atol(prop_value) * 1024;
			} else if (strcmp(token1, "Buffers:") == 0) {
				value->mem_buffers = atol(prop_value) * 1024;
			} else if (strcmp(token1, "SwapTotal:") == 0) {
				value->swap_total = atol(prop_value) * 1024;
			} else if (strcmp(token1, "SwapFree:") == 0) {
				value->swap_free = atol(prop_value) * 1024;
			}
		}

		// get next row
		token = strtok_r(NULL, "\n", &t_ptr1);
	}

	return value;
}
