#include <stdio.h>
#include <stdarg.h>
#include <syslog.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "common.h"
#include "metrics.h"

#define PRG_NAME "system-metrics-logger v0.1"
#define SLEEP_INTERVAL_SEC 5

int signal_received = 0;
const char *logfile = "/var/log/monitor/system-metrics.log";

void process_metrics();

void sighandler(int sig, siginfo_t *siginfo, void *ctx) {
	// char msg[1024];
	// sprintf(msg, "Received signal: %d", sig);
	// write(STDERR_FILENO, msg, sizeof(msg)-1);
	signal_received = sig;
}

void init() {
	// Assign signal handlers
	struct sigaction act;
	memset(&act, '\0', sizeof(act));
	act.sa_sigaction = &sighandler;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGHUP, &act, NULL); // < 0 --> error -- lost terminal
	sigaction(SIGINT, &act, NULL); // < 0 --> error -- eg. CTRL-c
	sigaction(SIGCONT, &act, NULL); // < 0 --> error
	sigaction(SIGTERM, &act, NULL); // < 0 --> error -- termination request
	sigaction(SIGSTOP, &act, NULL); // stop
}

void do_loop() {
	// TODO ez detektalni kell, de nem szabad megallitani
	// sigaction(SIGTSTP, &act, NULL); // stop from terminal CTRL-z

	/*
	if (sigaction(SIGTERM, &act, NULL) < 0) {
		perror("sigaction");
		syslog(LOG_USER | LOG_ERR, "%s exiting, due to sigaction error.", PRG_NAME);
		return 1;
	}
	*/
	int i = 1;
	int do_exit = 0;
	while (1) {
		process_metrics();
		if (i>0) syslog(LOG_USER | LOG_NOTICE, "boop");
			else syslog(LOG_USER | LOG_NOTICE, "bip");
		i = -i;
		if (signal_received!=0) {
			syslog(LOG_USER | LOG_INFO, "signal received: %d", signal_received);
			switch (signal_received) {
				case SIGINT:
					// kilepes
				case SIGTSTP:
					// nem kellene kilepni, hanem csak pauzalni
					syslog(LOG_USER | LOG_NOTICE, "interrupt/stop request");
				break;
				case SIGCONT:
					syslog(LOG_USER | LOG_NOTICE, "continue request");
				break;
				default:
					do_exit = 1;
				break;
			}
			if (do_exit) break;
			signal_received = 0;
		}
		sleep(SLEEP_INTERVAL_SEC);
	}
}

int start() {
	syslog(LOG_USER | LOG_INFO, "%s started", PRG_NAME);
	init();
	do_loop();
	syslog(LOG_USER | LOG_INFO, "%s exiting", PRG_NAME);
	return 0;
}

void process_metrics() {
	metric__uptime *uptime = get_uptime();
	metric__loadavg *loadavg = get_loadavg();

	char time_str[64];
	int time_in_millis = get_time(time_str);
	char logrow[1024];
	sprintf(logrow, "%d %s %.2f %.2f %.2f %.2f %.2f %d",
		time_in_millis, time_str,
		uptime->in_seconds, uptime->idle_sum_seconds,
		loadavg->avg_1_min, loadavg->avg_5_min, loadavg->avg_15_min, loadavg->process_total
	);

	free(uptime);
	free(loadavg);
	write_file(logrow, logfile);
}

int main(int argc, char *argv[]) {
	start();
}
