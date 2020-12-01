Simple system metrics logger daemon for Linux written in C

v0.1

- uptime
- idle
- load 1 min avg
- load 5 min avg
- load 15 min avg
- process total
- process running
- mem total
- mem free
- mem cached
- mem buffers
- swap total
- swap free


TODO
- exit immediately if the log file is not writeable
- use timer_create instead of sleep for scheduling
- log metrics for diskstats
- add microseconds to log's time field
- update process control signal handling: exit on SIGHUP/CTRL-C


