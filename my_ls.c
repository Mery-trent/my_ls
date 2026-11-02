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
    int use_younger; long younger_days;  // -y option
    int use_larger; long long larger; // -l option
    int use_smaller; long long smaller; // -s option
    char dir[MAX_PATH_LEN+1]; //target directory  

}options;

// ****function prototypes********
static void usage(const char *prog); 
static int parse_args(int argc, char *argv[], options *opt);
static const char *type_string(mode_t mode);
static void perm_string (mode_t mode, char out[10]);
static void format_time(time_t t, char *buf, size_t len);
static int passes_filters(const struct stat *st, const options *opt, time_t now);
static int list_directory(const options *opt);

// start of main 
int main (int argc, char *argv[]){

    options opt;
    // if an error happend when reading the arguments from the command line
    if(parse_args(argc,argv, &opt)!=0){
        usage(argv[0]);
       return EXIT_FAILURE; 
    }
    return list_directory(&opt)== 0? EXIT_SUCCESS: EXIT_FAILURE;
}

// print a short usage msg in case of errors
static void usage(const char *prog)
{
    fprintf(stderr, "Usage: %s [-o days] [-y days] [-l bytes] [-s bytes] [directory]\n", prog);
}

/*
 * Parse argv into an Options struct.
 * Flags come in (flag, value) pairs; if argc is even, the final
 * argument is the directory. Returns 0 on success, -1 on error.
 */
static int parse_args(int argc, char *argv[], options *opt)
{
    int i, last_flag_arg;

    //zeroing the opt fields, to ensure a clean start
    memset(opt, 0, sizeof(*opt));
    strcpy(opt-> dir, "./");  //default, current directory

    // case of even number of args, directory is provided
    if (argc % 2 ==0 ){

        //check wether the path provided is accepted in terms of length
        if(strlen(argv[argc-1])>MAX_PATH_LEN){
            fprintf(stderr, "Error: path is too long (max %d characters)\n", MAX_PATH_LEN);
        return -1;
        }
        
        // in case of eccepting the path, copy it to the struc opt
        strcpy(opt->dir, argv[argc-1]);
        last_flag_arg = argc - 2; // flags end before the the path


    } else{
        // case of no directory provided
        last_flag_arg= argc -1; 
    }
    
    // walk through the flag, value pairs 
    for(i= 1; i< last_flag_arg; i+=2){
        const char *flag = argv[i];
        const char *val = argv[i + 1];
        if(strcmp(flag, "-o")==0){
            opt -> use_older = 1; opt -> older_days = atol(val);
        } else if(strcmp(flag, "-y") == 0){
            opt -> use_younger = 1; opt -> younger_days = atol(val);

        } else if(strcmp(flag, "-l") == 0){
            opt -> use_larger = 1; opt->larger = atoll(val);
        
        }else if (strcmp(flag, "-s") == 0){
            opt -> use_smaller = 1; opt-> smaller = atoll(val);

        }else{
            fprintf(stderr, "Error: unknown option '%s'\n", flag);
            return -1;
        }
    }
    return 0;
}
// three-letter file type indecator
static const char *type_string (mode_t mode)
{
    if(S_ISREG(mode)) return "REG";
    else if (S_ISDIR(mode)) return "DIR";
    else if (S_ISLNK(mode)) return "LNK";
    else if (S_ISCHR(mode)) return "CHR";
    else if (S_ISBLK(mode)) return "BLK";
    else if (S_ISFIFO(mode)) return "FIF";
    else if (IS_SOCK(mode)) return "SOC";
    return "???";

}

// build a 9 character permission string such as rwxr--r--
void perm_string(mode_t mode, char out[10])
{
// declare an array that holds the actual permissions 
mode_t bits[9]= {
    S_IRUSR, S_IWUSR, S_IXUSR, 
    S_IRGRP, S_IWGRP, S_IXGRP,
    S_IROTH, S_IWOTH, S_IXOTH
};
// declare another array that contain the charachters that would be displayed late
char chars[3]= {'r', 'w', 'x'}; 
}