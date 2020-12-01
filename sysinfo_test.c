#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>

main() {
	// https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
	float f_load = 1.0f / (1 << SI_LOAD_SHIFT);
	struct sysinfo *p;
	p = (struct sysinfo *)calloc(0,sizeof(sysinfo));
	sysinfo(p);
	printf("Uptime: %ld\n", p->uptime);
	printf("Load: %lu %lu %lu\n", p->loads[0], p->loads[1], p->loads[2]);
	printf("Load: %.2f %.2f %.2f\n",
		p->loads[0] * f_load,
		p->loads[1] * f_load,
		p->loads[2] * f_load);
	printf("Number of procs: %d\n", p->procs);
	printf("Memory (free/total/shared/buffer): %lu %lu %lu %lu\n", p->freeram, p->totalram, p->sharedram, p->bufferram);
	printf("Swap (free/total): %lu %lu\n", p->freeswap, p->totalswap);
	printf("Memory unit size: %d bytes\n", p->mem_unit);
	free(p);
	return 0;
}


