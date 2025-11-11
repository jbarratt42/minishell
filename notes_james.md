# Notes from James' review

lines that end in a pipe cause a segmentation fault:
```
$ ./minishell
minishell$ echo |
syntax error
Segmentation fault (core dumped)
```
except in valgrind:
```
$ valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp -s ./minishell
==5921== Memcheck, a memory error detector
==5921== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==5921== Using Valgrind-3.19.0 and LibVEX; rerun with -h for copyright info
==5921== Command: ./minishell
==5921== 
minishell$ echo |
syntax error
==5921== Invalid read of size 8
==5921==    at 0x10A539: free_tokens (free.c:19)
==5921==    by 0x10934A: free_for_input (main.c:56)
==5921==    by 0x10934A: process_input (main.c:91)
==5921==    by 0x10934A: main (main.c:104)
==5921==  Address 0x4b1a200 is 32 bytes inside a block of size 40 free'd
==5921==    at 0x484417B: free (vg_replace_malloc.c:872)
==5921==    by 0x10A72F: free_tokens (free.c:22)
==5921==    by 0x10A72F: free_tokens (free.c:20)
==5921==    by 0x10A72F: free_tokens (free.c:20)
==5921==    by 0x10A72F: free_node (free.c:30)
==5921==    by 0x10A0C7: parse (parse.c:89)
==5921==    by 0x10EE80: parse_and_execute (main.c:36)
==5921==    by 0x109327: process_input (main.c:90)
==5921==    by 0x109327: main (main.c:104)
==5921==  Block was alloc'd at
==5921==    at 0x48417B4: malloc (vg_replace_malloc.c:381)
==5921==    by 0x109F7C: token_new (util.c:82)
==5921==    by 0x109BFA: lex (lex.c:109)
==5921==    by 0x10EE68: parse_and_execute (main.c:30)
==5921==    by 0x109327: process_input (main.c:90)
==5921==    by 0x109327: main (main.c:104)
==5921== 
...
```
but only the first time:
```
minishell$ echo |
syntax error
minishell$ 
```
---

ctrl-c in a blocking command like cat produces a double prompt
```
minishell$ minishell$ echo $?
130
minishell$ cat
hello
hello
^C
minishell$ minishell$ 
```
someone told me this could be avoided by setting a flag while execve is running and not catching the signals during that time.
something about a default child signal handler so that signals passed on from processes run with execve don't print weird things.
---
when we pipe to a command that doesn't read from the pipe, this happens:
```
$ valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell
==5850== Memcheck, a memory error detector
==5850== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==5850== Using Valgrind-3.19.0 and LibVEX; rerun with -h for copyright info
==5850== Command: ./minishell
==5850== 
minishell$ cat echo | echo

==5853== 
==5853== HEAP SUMMARY:
==5853==     in use at exit: 236,187 bytes in 299 blocks
==5853==   total heap usage: 550 allocs, 251 frees, 256,756 bytes allocated
==5853== 
==5853== 2 bytes in 1 blocks are still reachable in loss record 3 of 95
==5853==    at 0x48417B4: malloc (vg_replace_malloc.c:381)
==5853==    by 0x10F02A: ft_strdup (in /home/joshua/42/minishell/minishell)
==5853==    by 0x109F9D: token_new (util.c:88)
==5853==    by 0x109AAB: lex_operator (lex.c:57)
==5853==    by 0x109AAB: lex_tokens (lex.c:81)
==5853==    by 0x109AAB: lex (lex.c:106)
==5853==    by 0x10EE68: parse_and_execute (main.c:30)
==5853==    by 0x109327: process_input (main.c:90)
==5853==    by 0x109327: main (main.c:104)
==5853== 
...
```
---
nits:
- the global variable should be declared `volatile [or?] atomic`?
- env should not accept arguments
- exit should not accept an argument greater than `INT_MAX`
