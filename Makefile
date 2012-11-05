targets = channel null.so null_test.so

.PHONY: all clean

all: $(targets)

clean:
	rm -f $(targets)

channel: channel.c channel.h
	gcc -Wall -Wextra -g -I. -Wl,-rpath,. -Wl,-export-dynamic -o $@ $< -ldl

%.so: %.c channel.h
	gcc -Wall -Wextra -g -I. -Wl,-rpath,. -Wl,-export-dynamic -shared -fPIC -o $@ $<
