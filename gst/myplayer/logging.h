#ifndef LOGGING_H
#define LOGGING_

static size_t buff_size;
static char *msg_buff;

#define LOG_(format, level, ...)				\
  buff_size = snprintf(NULL, 0, format, ##__VA_ARGS__);		\
  msg_buff = malloc(buff_size + 1);				\
  snprintf(msg_buff, buff_size + 1, format, ##__VA_ARGS__);	\
  g_##level("%s:%d(%s) : %s", __func__, __LINE__,		\
	 __FILE__, msg_buff);					\
  free(msg_buff);						\

#define LOG_DEBUG(format, ...)			\
  LOG_(format, debug, ##__VA_ARGS__)

#define LOG_INFO(format, ...)			\
  LOG_(format, info, ##__VA_ARGS__)

#define LOG_ERROR(format, ...)			\
  LOG_(format, error, ##__VA_ARGS__)

#define LOG_CRITICAL(format, ...)		\
  LOG_(format, critical, ##__VA_ARGS__)

#define LOG_WARNING(format, ...)		\
  LOG_(format, warning, ##__VA_ARGS__)

#define LOG_MESSAGE(format, ...)		\
  LOG_(format, message, ##__VA_ARGS__)

#define DEBUG(x) g_debug(x)

extern void init_logging();
extern void dirty_log(const char *msg);
extern void log_info(const char *format, ...);

#endif
