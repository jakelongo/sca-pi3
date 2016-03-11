
# Output file
MAIN := socketAES

# Compiler
CC := gcc

# SSL Library
SSL_INC := /usr/local/ssl/include
SSL_LIB := /usr/local/ssl/lib

# Linker flags
LD_FLAGS := -lcrypto
LD_FLAGS += -ldl
LD_FLAGS += -lwiringPi
LD_FLAGS += -lpthread


# Gather
CFLAGS := -Wall

# gather all linker and include paths
LIB := -L$(SSL_LIB)
INC := -I$(SSL_INC)

# source directory
SOURCE := src/

# Gather all sources
SRCS := $(wildcard $(SOURCE)*.c)

# Find running architecture
UNAME_M := $(shell uname -m)

ifdef DEBUG
	CFLAGS	 		+= "-D DEBUG"
else
	Q						:=@
endif

all: $(MAIN)

$(MAIN): $(SRCS)
	$(CC) $^ $(CFLAGS) $(LIB) $(INC) $(LD_FLAGS) -o $@

clean:
	$(Q)rm $(MAIN)