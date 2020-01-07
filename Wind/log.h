#ifndef LOG_H
#define LOG_H


#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_WARN 2
#define LOG_ERROR 3
#define LOG_FATAL 4
#define LOG_ALL 5
#define LOG_OFF 6
#define LOG_TRACE 7


void c_log(int level, char *msg);
void c_logf(int level, char *format, ...);

#endif
