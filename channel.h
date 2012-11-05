#ifndef CHANNEL_H
#define CHANNEL_H

struct channel {
	void (*send)(struct channel *channel, void *message);
};

struct channel *channel_lookup(const char *name);

#endif
