#ifndef xlogger_message_h
#define xlogger_message_h

#include <list.h>
#include <string.h>
#define XLOGER_APPEND_BUFFER_CAP 512

typedef enum logger_level {
	LOG_DEBUG = 0,
	LOG_INFO = 1,
	LOG_WARNING = 2,
	LOG_ERROR = 3,
	LOG_FATAL = 4
} logger_level;

struct xlogger_message {
	char cmd[128];
	void *ud;
};

struct xlogger_append_buffer {
	struct list_head node;
	char data[XLOGER_APPEND_BUFFER_CAP];
	int size;					           // 缓冲区已使用字节数
};

static inline void
xlogger_append_buffer_init(struct xlogger_append_buffer *self) {
	INIT_LIST_HEAD(&self->node);
	memset(self->data, 0, XLOGER_APPEND_BUFFER_CAP);
	self->size = 0;
}

#endif // !xlogger_message_h
