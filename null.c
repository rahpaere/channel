#include <channel.h>

#include <syslog.h>

static void null_send(struct channel *channel, void *message)
{
	(void)channel;
	(void)message;
	syslog(LOG_DEBUG, "ignored message on null channel");
}

static struct channel null_channel = { null_send };

struct channel *null_setup(void)
{
	syslog(LOG_INFO, "loaded null module");
	return &null_channel;
}
