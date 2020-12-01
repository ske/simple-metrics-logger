#ifndef COMMON_H
#define COMMON_H

void read_file(const char *filename, char *buffer, int len);
void write_file(const char *row, const char *filename);
int get_time(char *timestr);

#endif
