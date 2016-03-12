SRCS := $(wildcard *.c)
BINS := $(SRCS:%.c=%)

all: $(BINS)

aarch32: aarch32.c
	$(CC) $(CFLAGS) -o $@ $<

aarch64: aarch64.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm aarch32
	rm aarch64
