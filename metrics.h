#ifndef METRICS_H
#define METRICS_H

typedef struct {
	float in_seconds;
	float idle_sum_seconds;
} metric__uptime;

typedef struct {
	unsigned long mem_total;
	unsigned long mem_free;
	unsigned long mem_cached;
	unsigned long mem_buffers;
	unsigned long swap_total;
	unsigned long swap_free;
} metric__memory;

typedef struct {
	float avg_1_min;
	float avg_5_min;
	float avg_15_min;
	unsigned int process_running;
	unsigned int process_total;
	unsigned int last_process_id;
} metric__loadavg;

metric__uptime *get_uptime();
metric__loadavg *get_loadavg();
metric__memory *get_meminfo();

#endif
