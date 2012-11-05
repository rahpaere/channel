#include <channel.h>

#include <syslog.h>
#include <stdlib.h>

struct channel *null_test_setup(void)
{
	struct channel *channel;

	channel = channel_lookup("null");
	channel->send(channel, NULL);

	syslog(LOG_INFO, "loaded null_test module");
	return NULL;
}
