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

// ****function prototypes********
static void usage(const char *prog); 
static int parse_args(int argc, char *argv[], Options *opt);
static const char *type_string(mode_t mode);
static void perm_string (mode_t mode, char out[10]);
static void format_time(time_t t, char *buf, size_t len);
static int passes_filters(const struct stat *st, const Options *opt, time_t now);
static int list_directory(const Options *opt);