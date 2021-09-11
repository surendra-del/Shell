->make command in the terminal creates the a.out executable file for running the shell
-> main.c:
  . Shell implementation
  . Signal handler 

-> headers.h:
  . All the headers required are inserted here it self

-> functions.h
  . All the global variables and functions are ideclared here

-> functions.c:
  . string tokeninizig function
  . cd , pinfo, echo  implementations
  . setenv,unsetenv,jobs,kjob has been implemneted
  .fg and bg , overkill, quit

-> ls.c:
  . whole ls implementation has been done here

-> history.c:
  . History recording and history command implementation has been coded here

->History command implementation is acheived through creating a txt file where the executable file is running, it is modified every time when a command     given to  the  terminal.

-> If & is given with builtin commands like(ls &, cd &, pid &,...), they have been assumed as foreground ....

->ctrl-z,ctrl-c has been handled by creating handlers

->last working directory is achieved through storing cwd after     implentation of command.
