## 17) Milestones & Checklists

### M1 — Bootstrap & CI

- [x] Makefile rules & fast rebuilds.&#x20;
- [ ] REPL (read, exec, prompt, loop) baseline with prompt + history.
  - [-] History
- [ ] Logging & error module.

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
