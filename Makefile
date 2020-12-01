GCC=/usr/bin/gcc -O2 -Wall
ARMGCC=~/Apps/tools/arm-bcm2708/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc
DIST=dist

dist:
	mkdir -p $(DIST)

clean:
	rm -rf $(DIST)

bin: monitor.c metrics.c common.c
	$(GCC) -o $(DIST)/monitor $^

bin-debug: monitor.c metrics.c common.c
	$(GCC) -g -o $(DIST)/monitor-dbg $^

bin-arm: monitor.c metrics.c common.c
	$(ARMGCC) -o $(DIST)/monitor-arm $^

bin-debug-arm: monitor.c metrics.c common.c
	$(ARMGCC) -g -o $(DIST)/monitor-arm-debug $^

release: clean dist bin bin-arm

all: clean dist bin bin-arm bin-debug bin-debug-arm

diskstats: diskstats.c
	$(GCC) -g -o $(DIST)/diskstats $^
