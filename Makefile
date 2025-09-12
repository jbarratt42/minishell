# Define the name of the executable
NAME = minishell
EXEC = $(NAME)

# Define compiler and flags
CC = cc
ifndef DEBUG
CC += -O3
endif
CFLAGS = -Wall -Wextra -Werror

# Check if DEBUG is defined (make DEBUG=1 or simply make debug)
ifdef DEBUG
CFLAGS += -g -DDEBUG=1
endif

# Set flags based on OS
ifeq ($(shell uname),Darwin)
	CPPFLAGS = -Iinclude -Ilibft -I/opt/homebrew/opt/readline/include
	LDFLAGS  = -Llibft -lft -L/opt/homebrew/opt/readline/lib -lreadline
else
	CPPFLAGS = -Iinclude -Ilibft 
	LDFLAGS  = -Llibft -lft -lreadline
endif


# Define the source and object directories
SRCDIR = src
OBJDIR = obj
VPATH = $(SRCDIR)

# List the subdirectories containing your source files.
# The `ls -d` command ensures `find` doesn't include the parent directory in the list.
# The output is `src/lex src/parse ...`
SRCDIRS = $(shell find $(SRCDIR) -mindepth 1 -type d)

# Use wildcard and patsubst to build the lists of files.
SRCS = $(wildcard $(addsuffix /*.c,$(SRCDIRS))) $(SRCDIR)/main.c
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
OBJDIRS = $(sort $(dir $(OBJS)))
HEAD = include/$(NAME).h

# The main rule to build the executable
all: $(EXEC)

$(EXEC): $(OBJS) libft/libft.a
	mkdir -p .$(NAME)
	$(CC) $(OBJS) $(LDFLAGS) -o $(EXEC)

# The static pattern rule to build each object file
# This is a robust and standard way to do multi-directory compilation.
$(OBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIRS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Rule to build the libft library
libft/libft.a:
	make -C libft

# Rule to create object directories if they don't exist
$(OBJDIRS):
	mkdir -p $@

# A clean rule to remove generated files
clean:
	$(RM) $(EXEC)
	$(RM) -rf .$(NAME)
	$(RM) -r $(OBJDIRS)
	$(MAKE) -C libft clean

fclean: clean
	$(RM) $(NAME)

# Add debug target that forces rebuild
debug:
	$(MAKE) fclean
	$(MAKE) all DEBUG=1

head: $(SRCS)
	util/update_proto.sh $(HEAD) $(SRCS)

# Rebuild the project
re: fclean all

test0:
	$(CC) $(CPPFLAGS) $(LDFLAGS) $(OBJS) test/test0.c -o $@

.PHONY: all clean debug
