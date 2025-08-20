# Define the name of the executable
NAME = minishell
EXEC = $(NAME)

# Define compiler and flags
CC = cc -O3
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -Iinclude -Ilibft
LDFLAGS = -Llib -lft

# Define the source and object directories
SRCDIR = src
OBJDIR = obj
VPATH = $(SRCDIR)

# List the subdirectories containing your source files.
# The `ls -d` command ensures `find` doesn't include the parent directory in the list.
# The output is `src/lex src/parse ...`
SRCDIRS = $(shell find $(SRCDIR) -mindepth 1 -type d)

# Use wildcard and patsubst to build the lists of files.
SRCS = $(wildcard $(addsuffix /*.c,$(SRCDIRS)))
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
OBJDIRS = $(sort $(dir $(OBJS)))
HEAD = include/$(NAME).h

# The main rule to build the executable
all: $(EXEC)

$(EXEC): $(OBJS) lib/libft.a
	$(CC) $(OBJS) $(LDFLAGS) -o $(EXEC)

# The static pattern rule to build each object file
# This is a robust and standard way to do multi-directory compilation.
$(OBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIRS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Rule to build the libft library
lib/libft.a:
	make -C libft

# Rule to create object directories if they don't exist
$(OBJDIRS):
	mkdir -p $@

# A clean rule to remove generated files
clean:
	$(RM) $(EXEC)
	$(RM) -r $(OBJDIRS)
	$(MAKE) -C libft clean

head: $(SRCS)
	util/update_proto.sh $(HEAD) $(SRCS)

test0:
	$(CC) $(CPPFLAGS) $(LDFLAGS) $(OBJS) test/test0.c -o $@

.PHONY: all clean
