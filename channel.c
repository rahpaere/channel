#include <channel.h>

#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <dlfcn.h>

struct module {
	struct module *next;
	char *name;
	void *handle;
	struct channel *channel;
};

static struct module *modules;

struct channel *channel_lookup(const char *name)
{
	struct module *module;
	struct channel *(*setup)(void);
	void *handle;
	char str[256];

	for (module = modules; module; module = module->next)
		if (!strcmp(module->name, name))
			return module->channel;

	snprintf(str, sizeof(str), "%s.so", name);
	handle = dlopen(str, RTLD_LAZY);
	if (!handle) {
		syslog(LOG_WARNING, "unable to load module %s", name);
		return NULL;
	}

	snprintf(str, sizeof(str), "%s_setup", name);
	setup = dlsym(handle, str);
	if (!setup) {
		syslog(LOG_WARNING, "module %s has no setup function", name);
		dlclose(handle);
		return NULL;
	}

	module = malloc(sizeof(*module));
	if (!module) {
		syslog(LOG_WARNING, "%s", strerror(errno));
		dlclose(handle);
		return NULL;
	}

	module->next = modules;
	module->name = strdup(name);
	module->handle = handle;
	module->channel = setup();
	modules = module;
	return module->channel;
}

static void unload_modules(void)
{
	struct module *next;

	while (modules) {
		dlclose(modules->handle);
		next = modules->next;
		free(modules);
		modules = next;
	}
}

int main(int argc, char **argv)
{
	int i;

	for (i = 1; i < argc; i++)
		channel_lookup(argv[i]);

	unload_modules();
	return EXIT_SUCCESS;
}
