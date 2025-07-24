# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g \
         -Iinclude \
         -Isrc/ethernet \
         -Isrc/ip \
         -Isrc/tap \
         -Isrc/arp \
         -Isrc/net \
         -Isrc/icmp

# Source files
SRCS = main.c \
       src/ethernet/ethernet.c \
       src/ip/ipv4.c \
       src/tap/tap_interface.c \
       src/arp/arp.c \
       src/net/net.c \
       src/icmp/icmp.c

# Object files
OBJS = $(SRCS:.c=.o)

# Output binary
TARGET = start_test

# Default rule
all: $(TARGET)

# Link object files into the final binary
$(TARGET): $(OBJS)
	$(CC) -o $@ $^

# Compile .c files to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)
