#include <stdlib.h>
#include <stdio.h>

enum log_level{
	LOG_ERROR = 0,
	LOG_WARN,
	LOG_INFO,
	LOG_DEBUG,
	LOG_VERBOSE,

	LOG_LEVEL_NUM,
	LOG_LEVEL_FIRST = LOG_ERROR,
	LOG_LEVEL_LAST =  LOG_VERBOSE,
};

static enum log_level G_current_level = LOG_INFO;

#define log_print(_level_, _str_, _arg_...) do {\
	if (_level_ <= G_current_level ) {\
		printf(_str_, ##_arg_);	\
	}							\
}while(0)

#define loge(_str_, _arg_...) do {		\
	log_print(LOG_ERROR, _str_, ##_arg_);\
}while(0)

#define logw(_str_, _arg_...) do {		\
	log_print(LOG_WARN, _str_, ##_arg_);\
}while(0)

#define logi(_str_, _arg_...) do {		\
	log_print(LOG_INFO, _str_, ##_arg_); \
}while(0)

#define logd(_str_, _arg_...) do {		\
	log_print(LOG_DEBUG, _str_, ##_arg_);\
}while(0)

#define logv(_str_, _arg_...) do {		\
	log_print(LOG_VERBOSE, _str_, ##_arg_);\
}while(0)

void set_log_level(int level)
{
	G_current_level = level;
	printf("Set Log Level: %d\n", G_current_level);
}

int get_log_level(void)
{
	printf("Get Log Level: %d\n", G_current_level);
	return G_current_level;
}


int main (int argc, char ** argv)
{
	int i = 128;
	char s[] = "abc";

	set_log_level(LOG_VERBOSE);
	get_log_level();

	loge("i = %d, c = %s\n", i, s);
	logw("i = %d, c = %s\n", i, s);
	logi("i = %d, c = %s\n", i, s);
	logd("i = %d, c = %s\n", i, s);
	logv("i = %d, c = %s\n", i, s);

	loge("ERROR\n");

	printf("Hello world\n");

	return 0;
}

