# 21sh
Relatively stable shell.


# Installation

```
git clone https://github.com/ag95v2/21sh
cd ./21sh
make
```
# Usage

Use it like your favourite shell. 
Should be intuitive.

# Features:

- Partial implemenation of GNU readline using termcaps (Unstable on some terminal emulators). 
- Environmental variables 
- Builtins (env, setenv, unsetenv, echo, exit, cd)
- Simple signal magenent
- Pipes
- Redirections <<, >>, <, >, >&, <&
- History management (no search)
- Expansions ~ $ (simple)
- Almost POSIX-compliant (subset of) grammar
- Works stable in interactive / non-interactive mode
- Automatic tests
- Implementation of huge part of libc (for educational purposes)
