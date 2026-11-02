#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define MAX_PATH_LEN 4096
#define SECONDS_PER_DAY 86400L //Leap years/seconds are igored

// a structure that holds the filter settings parsed from the command line
typedef struct {
    int use_older; long older_days;  // -o option
    int use_young; long younger_days;  // -y option
    int use_larger; long long larger; // -l option
    int use_smaller; long long smaller; // -s option
    char dir[MAX_PATH_LEN+1]; //target directory  

}options;