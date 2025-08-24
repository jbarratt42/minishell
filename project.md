awesome—let’s turn your Minishell into a **production-grade** project with a crystal-clear roadmap, “done” criteria, edge-case traps, and implementation blueprints. I’ll reference the official spec where it matters.&#x20;

---

# 🚀 Minishell: Production-Grade Plan, Deliverables, and To-Dos

## 0) Non-negotiables from the spec

- Mandatory capabilities: prompt, history, PATH resolution, quotes, env expansion (incl. `$?`), redirections (`<`, `>`, `>>`, `<<`), pipes, signals (Ctrl-C/D/\\), required built-ins, **at most one global** for the signal number.&#x20;
- Makefile rules + flags `-Wall -Wextra -Werror`, no unnecessary relinking.&#x20;
- `readline` may leak internally; **your** code must be leak-free.&#x20;
- Bonus only after perfect mandatory: `&&`, `||`, `()` precedence, `*` wildcard.&#x20;

---

## 1) Architecture & Directory Layout (deliverable)

```
minishell/
├─ Makefile
├─ include/
│  ├─ minishell.h
│  ├─ ast.h
│  ├─ lexer.h
│  ├─ parser.h
│  ├─ exec.h
│  ├─ env.h
│  ├─ builtin.h
│  ├─ redir.h
│  ├─ signals.h
│  ├─ heredoc.h
│  └─ utils.h
├─ src/
│  ├─ main.c
│  ├─ repl/         (readline loop, prompt, history)
│  ├─ lexer/        (tokenization, quote handling)
│  ├─ parser/       (AST builder, syntax errors)
│  ├─ expander/     (env + `$?`, quote removal)
│  ├─ exec/         (PATH search, execve, pipelines, fd mgmt)
│  ├─ redir/        (>, >>, <, <<)
│  ├─ builtins/     (echo, cd, pwd, export, unset, env, exit)
│  ├─ env/          (env list, export formatting)
│  ├─ signals/      (setups, single global int)
│  ├─ errors/       (uniform errors + errno bridges)
│  └─ utils/        (string, lists, safe alloc)
├─ tests/           (unit + integration scripts)
└─ libft/           (if used)
```

**Design principles**

- **Single source of truth** for environment: intrusive linked list of `KEY=VALUE` nodes + hash (optional) for O(1) lookup; always serializable to `char **envp` for `execve`.
- **AST execution** instead of “token stream gymnastics”: cleaner redirection/pipe handling and enables bonus (`&&`, `||`, `()`).
- **Zero fd leaks**: central “fd registry” for created descriptors; close on scope exit.
- **Single global `volatile sig_atomic_t g_sig`:** _only_ store the received signal number.&#x20;

**Definition of Done (DoD) for this section**

- `make all/clean/fclean/re` work; compiles with `-Wall -Wextra -Werror`.&#x20;
- Running `valgrind` on trivial commands shows **no leaks** from your code.
- `README.md` explains build, run, features, and test usage.

---

## 2) REPL & Prompt (deliverable)

**Responsibilities**

- Detect interactive vs. non-interactive (`isatty`), show prompt, maintain history, exit on EOF (Ctrl-D).&#x20;

**Implementation To-Dos**

- [ ] `repl_run()`:

  - loop: `line = readline(prompt())`
  - if `line == NULL` (EOF): `printf("exit\n")`, clean shutdown. (Ctrl-D → exit)&#x20;
  - ignore empty/whitespace-only lines (no history).
  - `add_history(line)` for non-empty.

- [ ] `prompt()`:

  - Use `getcwd()`; on failure, fall back to `?` or cached value.

- [ ] Non-interactive:

  - Read from stdin until EOF, process lines without prompt/history.

**Edge cases to watch**

- `readline` internal leak is acceptable; wrap your allocations, free `line`.&#x20;
- Ctrl-C behavior: **new prompt on new line** (don’t exit). Ctrl-\ does nothing.&#x20;

**DoD**

- Ctrl-C prints newline + prompt; Ctrl-D exits; Ctrl-\ ignored.
- History works while not recording heredoc text (see heredoc section).&#x20;

---

## 3) Lexing (tokens) (deliverable)

**Grammar (mandatory subset)**

- Words, quotes (`'` and `"`), operators: `<`, `>`, `>>`, `<<`, `|`.
- No backslash or semicolon semantics. Reject unclosed quotes.&#x20;

**To-Dos**

- [ ] Finite state lexer producing tokens: `WORD`, `SQUOTE`, `DQUOTE`, `PIPE`, `REDIR_IN`, `REDIR_OUT`, `REDIR_APP`, `HEREDOC`.
- [ ] Quote collection:

  - `'...’` preserves literal content; no expansion inside.&#x20;
  - `"..."` allows `$` expansions only; other metachars are literal.&#x20;

- [ ] Track positions (line, col) for precise error messages.
- [ ] Reject: unclosed quotes → recover by printing `syntax error: unclosed quote`.

**Edge cases**

- Operators glued to words (`echo>f`) → tokenize as `WORD`, `>`, `WORD`.
- Multiple spaces/tabs; treat as separators outside quotes.
- Here-document delimiter token must be a raw **WORD** (unexpanded) per bash-like behavior for delimiter matching.

**DoD**

- Token stream round-trips typical inputs; robust against long lines (up to, say, 64 KiB).

---

## 4) Parsing → AST (deliverable)

**AST nodes**

- `NODE_PIPELINE`: left | right | right | …
- `NODE_COMMAND`: {argv\[], redirs\[], is_builtin, builtin_id}
- `NODE_REDIR`: {type, target} for `<`, `>`, `>>`
- `NODE_HEREDOC`: {delimiter, quoted_delim_flag}

**To-Dos**

- [ ] `parse_pipeline()` builds left-associative chain on `|`.
- [ ] `parse_command()` collects argv and redirs in arrival order.
- [ ] Validate syntax: operator without operand, pipe at start/end, etc.
- [ ] For bonus (deferred): `parse_list()` handles `&&`, `||`, and `()`, precedence.

**DoD**

- Clear parse errors with caret pointing to token position.
- AST pretty-printer for debugging.

---

## 5) Expansion & Quote Removal (deliverable)

**Rules**

- Env expansion in unquoted and **double-quoted** strings; not in single-quoted.&#x20;
- `$?` → last foreground pipeline exit status.&#x20;
- After expansion, do **field splitting** behavior minimalism: keep as a single field unless you deliberately implement POSIX splitting; for Minishell generally keep simple (no glob by default—wildcards are **bonus**).&#x20;

**To-Dos**

- [ ] Expand `$VAR`, `${VAR}` (optional), `$?`.
- [ ] Implement quote removal after expansion.
- [ ] Implement “empty becomes empty string” semantics (e.g., `echo "$UNSET"` prints empty).

**Edge cases**

- `"$PATH"` with colons → **don’t** split into multiple argv unless you choose to emulate bash word splitting (not required).
- Unset variables → expand to empty.

**DoD**

- Snapshot `$?` updated **after** each pipeline finishes.

---

## 6) Environment Store (deliverable)

**Data structure**

```c
typedef struct s_env {
  char *key;    // malloc'd
  char *value;  // malloc'd (may be "")
  struct s_env *next;
} t_env;
```

- Build from `envp` at startup; maintain `PWD`, `OLDPWD` updates for `cd`.

**To-Dos**

- [ ] `env_get(key)`, `env_set(key, value, overwrite)`, `env_unset(key)`
- [ ] Serializer to `char **` for `execve`
- [ ] Export ordering (ASCII) when printing `export` with no args (if you mimic bash’s display format, fine; spec only requires “no options,” so minimal compliant output is acceptable).&#x20;

**Edge cases**

- `export NAME` with no `=` marks variable as exported with empty value.
- Reject invalid identifiers in `export`/`unset` with appropriate status.

**DoD**

- Zero leaks on env rebuilds; `execve` env array freed after use in child.

---

## 7) Built-ins (deliverable)

**Run location rule**

- Built-ins that **affect parent state** (`cd`, `export`, `unset`, `exit`) must run **in the parent** when not part of a pipeline; if they appear inside a pipeline, they run in a forked child (effects won’t persist—this matches bash expectations during pipelines).&#x20;

**To-Dos & Behaviors**

- [ ] `echo [-n] args...` → join with spaces, newline unless `-n` (accept multiple `-n`s).
- [ ] `cd [path]` → default to `$HOME` if no arg; update `PWD` and `OLDPWD`; handle relative/absolute paths; print errors with `perror` style.
- [ ] `pwd` → print current dir from `getcwd` (fallback to env if needed).
- [ ] `export` (no options):

  - `export KEY=VAL` set or overwrite.
  - `export KEY` create empty if missing.
  - Return non-zero for invalid identifier.

- [ ] `unset KEY` → remove; invalid identifiers are errors.
- [ ] `env` (no options/args) → print current environment in `KEY=VALUE` lines.&#x20;
- [ ] `exit` → with optional numeric status (handle overflow, non-numeric → error & 255).

**DoD**

- Exact exit statuses mirror bash for the tested cases.
- Proper error messages to `stderr`.

---

## 8) Redirection (deliverable)

**Rules**

- `<` read; `>` truncate write; `>>` append; `<<` heredoc (no history updates).&#x20;

**To-Dos**

- [ ] Build redir list per command; open files **before** `execve`.
- [ ] Detect ambiguous redirect (missing target).
- [ ] Set up `dup2()` into stdin/stdout in child; close unused fds.
- [ ] Error path: if `open()` fails, print error and **skip exec** of that command (set exit status).

**DoD**

- Combined redirs work: `cmd >a 2>&1` isn’t required unless you implement `2>`; stick to stdin/stdout per spec.

---

## 9) Heredoc (deliverable)

**Rules**

- `<< DELIM` → read lines until a line **exactly** equals delimiter; **do not add to history**.&#x20;
- If delimiter is quoted (e.g., `<< 'EOF'`), **disable expansions** in the heredoc body (bash-like convention; matches typical evaluators).
- Implement heredoc by:

  - Writing content to a pipe (preferred) or a secure temp file via `mkstemp`.
  - Set read end as stdin for that command only.

**Signals**

- Ctrl-C during heredoc should cancel the heredoc and set status (like bash).

**DoD**

- No leftover temp files; no blocking on SIGINT; no history update.

---

## 10) Pipelines (deliverable)

**To-Dos**

- [ ] N-stage pipelines with `pipe()` per edge; fork per command.
- [ ] Parent closes all fds; waits for **last** command’s status → becomes `$?`.&#x20;
- [ ] In child, set pgid if you choose; at minimum, ignore SIGQUIT like bash requirement (Ctrl-\ “does nothing” interactively).&#x20;

**Edge cases**

- Leading/trailing pipes → syntax error.
- Pipeline containing built-ins: run in child, no env effects persist.

**DoD**

- Stress test: chain of 10 pipes works without fd leaks or zombies.

---

## 11) Signal Handling (deliverable)

**Rules**

- One global variable to store the signal number. **No global access to main data.**&#x20;
- Interactive mode:

  - Ctrl-C → newline + prompt
  - Ctrl-D → exit
  - Ctrl-\ → no action (ignore)&#x20;

**To-Dos**

- [ ] Use `sigaction` for SIGINT, SIGQUIT; restore default in children **before** exec so programs receive signals normally.
- [ ] Make handlers **async-signal-safe** (write to a pipe or set `g_sig` only).
- [ ] In REPL, check `g_sig` and refresh prompt.

**DoD**

- Foreground children receive SIGINT; shell doesn’t die on Ctrl-C.

---

## 12) PATH Resolution & `execve` (deliverable)

**To-Dos**

- [ ] If command contains `/`, try `execve` directly.
- [ ] Else resolve against `PATH` from env; try each directory with `access(X_OK)`.
- [ ] Distinguish “not found” vs. “permission denied” vs. “is a directory”.

**DoD**

- Matches bash exit codes for common cases (`127` not found, `126` cannot execute).

---

## 13) Error Handling & UX (deliverable)

**Principles**

- All errors go to `stderr` with informative context: `minishell: cmd: message`.
- Preserve `errno` when needed; return meaningful exit statuses.

**To-Dos**

- [ ] Central `ms_error(fmt, ...)` helper.
- [ ] Uniform messages for open/dup2/fork/execve errors.
- [ ] Syntax errors: `minishell: syntax error near unexpected token '…'`.

**DoD**

- No silent failures; negative paths covered in tests.

---

## 14) Memory, FDs, and Resource Safety (deliverable)

**To-Dos**

- [ ] `xmalloc`/`xstrdup` wrappers exit on OOM or handle gracefully.
- [ ] AST & token arenas with deterministic free on each loop iteration.
- [ ] FD registry: record and close all created fds; helper `close_quiet(int)`.

**DoD**

- `valgrind --leak-check=full` shows 0 definitely/indirect leaks (excluding `readline`).&#x20;

---

## 15) Testing Strategy (deliverable)

**Unit tests (where practical)**

- Lexer: quote cases, operators, edge whitespaces.
- Parser: errors (`|`, `>`, `>>` at ends), complex pipelines.
- Expander: `$VAR`, `$?`, quotes.
- Env: set/unset/export ordering & invalid ids.

**Integration scripts**

- Golden tests with expected stdout/stderr/status:

  - Basics: `echo`, `pwd`, `cd`, `env`, `export`, `unset`, `exit`.&#x20;
  - Exec: PATH/relative/absolute, not found, permission.
  - Redirs: `<`, `>`, `>>`, combos.
  - Heredoc: quoted/unquoted delimiters; Ctrl-C behavior.
  - Pipes: multi-stage; mix with redirs/builtins.
  - Signals: Ctrl-C in REPL and during pipeline.
  - `$?` correctness after each foreground pipeline.&#x20;

**Quality gates**

- Static checks: compile with `-fsanitize=address,undefined` (locally, not for final submission if your school forbids extra flags in final Makefile).
- `valgrind` integration target: `make valgrind TEST="..."`.

**DoD**

- CI-like script in `tests/run.sh` returning non-zero on any failure.

---

## 16) Bonus Plan (after mandatory is perfect)

**Short-circuit lists**

- Extend grammar: `list := pipeline ( (&& || ||) pipeline )*`
- Execution: evaluate left; if `&&` then run right only on success; if `||` run right only on failure; parentheses create sub-trees for precedence.&#x20;

**Wildcard (`*`) in CWD**

- Implement simple globbing: transform `*` → `readdir()` + `fnmatch`-like match; sort results lexicographically; if no matches, typically keep `*` literal or empty—choose bash-like behavior (usually keeps literal if no matches, but check your evaluator).

---

## 17) Milestones & Checklists

### M1 — Bootstrap & CI

- [ ] Makefile rules & fast rebuilds.&#x20;
- [ ] Logging & error module.
- [ ] REPL baseline with prompt + history.

### M2 — Lexer + Parser

- [ ] All token types; quote correctness.
- [ ] AST builder with diagnostics.

### M3 — Expander + Env

- [ ] `$VAR` / `$?`; quote removal.
- [ ] Env store + serializer.

### M4 — Exec Core

- [ ] PATH resolution; external exec.
- [ ] Built-ins in parent/child correctly.

### M5 — Redirs + Pipes + Heredoc

- [ ] All redirs; heredoc pipe/temp; no history writes.&#x20;
- [ ] Pipelines N-stage; `$?` from last.&#x20;

### M6 — Signals & Polishing

- [ ] `g_sig` and handlers; interactive behavior.&#x20;
- [ ] Errors, help text, edge-case pass.

### M7 — Test & Harden

- [ ] Valgrind clean; sanitizer clean.
- [ ] 100+ integration assertions.

### M8 — Bonus

- [ ] `&&`, `||`, `()`, `*`.

---

## 18) Nasty Edge-Cases (watchlist)

- Empty inputs: `""`, `''`, and `""''`.
- `echo -n -n -n hi` → no newline.
- `cd` with:

  - no args (HOME), invalid HOME, permission denied, `cd ..` on root.

- `export`:

  - `export =oops`, `export 1abc=foo` (invalid id): non-zero exit.

- Pipelines with built-ins: `export A=1 | cat` (var doesn’t persist).
- Redirs clobbering: `cat < in < otherin` → last wins for stdin.
- Heredoc:

  - Quoted delimiter vs. unquoted; expansion toggled.
  - Interrupt during heredoc should cancel just that heredoc.

- Signals during child exec: restore default handlers before `execve`.
- PATH unset or empty: only direct/relative paths should work.
- File/dir ambiguity: command is a directory → `126`.

---

## 19) Coding Standards & Conventions

- No dynamic buffers without bounds; always check returns of `malloc/open/dup2/fork`.
- Every `fork()` path handles errors: if fork fails → print error, set status, continue shell.
- Every FD created is either `dup2`’d or closed before `execve`.
- Children: close parent-only fds, restore signals, set correct stdio, then `execve`.
- Parents: close used pipe ends immediately; wait on children (avoid zombies).
- Do not keep pointers to freed tokens/strings after expansion.

---

## 20) Acceptance Demo Script (deliverable)

Create `tests/demo.sh` that runs:

```
# Builtins
echo ok
echo -n "no newline"
pwd
export FOO=bar && env | grep ^FOO=
unset FOO && env | grep ^FOO= || true
cd / && pwd
exit 7
```

…plus files for redirs/pipes/heredocs and asserts expected outputs & statuses.

---

## 21) What I’ll expect to see at the end (final deliverables)

- ✅ Fully working `minishell` binary meeting **all** mandatory spec items.&#x20;
- ✅ Clean `valgrind` (excluding `readline` internals).&#x20;
- ✅ Robust tests (`tests/run.sh`) with green output.
- ✅ Clear `README.md` explaining edge behaviors and any deliberate bash deviations.
- ✅ Optional: Bonus features implemented correctly.&#x20;

---

if you want, I can also generate:

- a **ready-to-paste Makefile** (strict, fast, colored output), and
- skeleton C headers + stub implementations for each module
  so you can start coding immediately without scaffolding work.
