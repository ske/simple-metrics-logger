#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <sys/time.h>

void read_file(const char *filename, char *buffer, int len) {
	FILE *fp = fopen(filename, "r");
	char row[1024];
	strcpy(buffer, "");
	while (fgets(row, 1024, fp)!=NULL) {
		strcat(buffer, row);
		strcat(buffer, "\n");
	}
	fclose(fp);
}

void write_file(const char *row, const char *filename) {
	FILE* fp = fopen(filename, "a");
	if (fp!=NULL) {
		fputs(row, fp);
		fputs("\n", fp);
		fclose(fp);
	} else {
		syslog(LOG_USER | LOG_ERR, "Cannot open file %s for writing.", filename);
	}
}

int get_time(char *timestr) {
	struct timeval time_now;
	struct tm time_detail;
	gettimeofday(&time_now, NULL);
	localtime_r(&time_now.tv_sec, &time_detail);

	sprintf(timestr, "%04d-%02d-%02d %02d:%02d:%02d",
		time_detail.tm_year+1900,
		time_detail.tm_mon+1,
		time_detail.tm_mday,
		time_detail.tm_hour,
		time_detail.tm_min,
		time_detail.tm_sec);

	return time_now.tv_sec;
	// TODO fixme return time with millisec as remainder part of the fractional number
	// return (float)time_now.tv_sec + (float)((float)time_now.tv_usec /  1000000.0f);
}
