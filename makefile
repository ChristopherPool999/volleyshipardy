# Compiler
# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic -std=c++11

# Linker flags
LDFLAGS = -lncurses

# Directory for object files
OBJDIR = obj

# Common source files
COMMON_SRCS = volleyball.cc battleship.cc

# Common object files
COMMON_OBJS = $(patsubst %.cc,$(OBJDIR)/%.o,$(COMMON_SRCS))

# Header files
HEADERS = volleyball.h battleship.h

# Client source file
CLIENT_SRC = client.cc

# Client object file
CLIENT_OBJ = $(OBJDIR)/client.o

# Server source file
SERVER_SRC = server.cc

# Server object file
SERVER_OBJ = $(OBJDIR)/server.o

# Output executables
CLIENT_TARGET = client
SERVER_TARGET = server

# Default target
all: $(CLIENT_TARGET) $(SERVER_TARGET)

# Rule for building the client executable
$(CLIENT_TARGET): $(COMMON_OBJS) $(CLIENT_OBJ)
	$(CC) $(CFLAGS) $(COMMON_OBJS) $(CLIENT_OBJ) -o $@ $(LDFLAGS)

# Rule for building the server executable
$(SERVER_TARGET): $(COMMON_OBJS) $(SERVER_OBJ)
	$(CC) $(CFLAGS) $(COMMON_OBJS) $(SERVER_OBJ) -o $@ $(LDFLAGS)

# Rule for building common object files
$(OBJDIR)/%.o: %.cc $(HEADERS)
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(OBJDIR) $(CLIENT_TARGET) $(SERVER_TARGET)
