SRCS := $(wildcard *.c)
BINS := $(SRCS:%.c=%)

all: $(BINS)

$(BINS): $(SRCS)
  $(CC) $(CFLAGS) -o $@ $<