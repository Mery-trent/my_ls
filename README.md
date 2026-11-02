
## my_ls.c ##
-  COIS3380H Assignment 3: Directories, Files and Timestamps

A simplified "ls -l" that can filter files by modification age and size.
## Usage:
     ./my_ls [-o days] [-y days] [-l bytes] [-s bytes] [directory_path]
     -o N   OLDER THAN   : only files modified MORE than N days ago
    -y N   YOUNGER THAN : only files modified LESS than N days ago
    -l N   LARGER THAN  : only files with size  >  N bytes
    -s N   SMALLER THAN : only files with size  <  N bytes
    If no directory is given, the current directory (./) is used.
 
## Argument-count:
    Every flag takes one value, so flags consume arguments in pairs.
    argc is odd  -> no directory on the command line
    argc is even -> the last argument is the directory
 
  Compile:  gcc -Wall -Wextra -o my_lab3 my_lab3.c
