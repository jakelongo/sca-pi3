SRCS := $(wildcard *.c)
BINS := $(SRCS:%.c=%)

all: $(BINS)

aarch32: $(SRCS)
	$(CC) $(CFLAGS) -o $@ $<

aarch64: $(SRCS)
	$(CC) $(CFLAGS) -o $@ $<
