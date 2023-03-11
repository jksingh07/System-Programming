// JASKARAN SINGH - 110090236 - ASP ASSIGNMENT 1
// @author jasakaransingh

// IMPORT LIBRARIES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ftw.h>
#include <libgen.h>

// DEFINING MACROS
#define _XOPEN_SOURCE 500
#define Max 8192

extern char* strdup(const char*);

// DECLARING SOME GLOBAL VARIABLES
char *source_path;
char *destination_path;
int is_flag_mv;
int n_ext;
char **extensions;


// THIS FUNCTION CREATES THE DIRECTORY OR FILE, BASED UPON SOURCE AND DESTINATION PATH
void generate_file_dir(const char *source, const char *destination) {
  struct stat source_stat;
  stat(source, &source_stat);

  if (S_ISREG(source_stat.st_mode)) {
    FILE *source_file_ptr = fopen(source, "rb");
    FILE *destination_file_ptr = fopen(destination, "wb");

    char buff[4096];
    size_t n_read_bytes;

    while ((n_read_bytes = fread(buff, 1, sizeof(buff), source_file_ptr)) > 0) {
      fwrite(buff, 1, n_read_bytes, destination_file_ptr);
    }

    fclose(source_file_ptr);
    fclose(destination_file_ptr);
  } else if (S_ISDIR(source_stat.st_mode)) {
    mkdir(destination, 0777); // CREATE NEW DIRECTORY 
  }
}

// FUNCTION TO CREATE NESTED FOLDERS
void recursively_make_dir(const char *path) {
  char *path_copy = strdup(path);
  for (char *ptr = path_copy + 1; *ptr; ptr++) {
    if (*ptr == '/') {
      *ptr = '\0';
      mkdir(path_copy, 0777);
      *ptr = '/';
    }
  }
  free(path_copy);
  mkdir(path, 0777);
}

// FUNCTION TO CREATE BOTH FILE AND DIRECTORY RECURSILVELY , IT USES ABOVE FUNCTIONS 
void generate_file_dir_recursive(const char *source, const char *destination) {
  char *path_copy = strdup(destination);
  for (char *ptr = path_copy + 1; *ptr; ptr++) {
    if (*ptr == '/') {
      *ptr = '\0';
      recursively_make_dir(path_copy);
      *ptr = '/';
    }
  }
  free(path_copy);

  generate_file_dir(source, destination);
}

// THIS FUNCTION IS USED TO COPY OR MOVE FILE OR DIRECTORY FROM SOURCE TO DESTINATION
int action_move_copy(const char *file_path, const struct stat *stat_b, int flag, struct FTW *buffer_ftw) {
  
    //flag for creating the file 0 -FALSE, 1-TRUE
    int create_flag = 0;
    if (n_ext == 0) {
        create_flag = 1;
    } else if (S_ISREG(stat_b->st_mode)) {
        
        int i;
        for (i=0;i<n_ext;i++) {
            
            int extension_length = strlen(extensions[i]);
            int path_length = strlen(file_path);

           
            
            //checking extension exist or not
            if (strcmp(file_path + path_length - extension_length, extensions[i]) == 0 && *(file_path+path_length-extension_length-1) == '.') {
                create_flag = 1;
            }
        }
        
    } else {
      //CREATE DIRECTORY so set FlAG to 1
        create_flag = 1;
    }

    if (create_flag) {
      
        char *temp = strdup(file_path);
        int source_length = strlen(source_path);
        temp = temp + source_length;
        
        char path[strlen(temp)+strlen(destination_path)];
        strcpy(path, "");
        strcat(path, destination_path);
        strcat(path, temp);
        
        
        
        generate_file_dir_recursive(file_path, path);
        //Delete only if the move option is chosen.
        if (is_flag_mv) {
            int result = remove(file_path);
        }
    }
    else{
      if (is_flag_mv) {
            int result = remove(file_path);
        }
    }

    return 0;
}

// CHECK IF THE PATH PROVIDED IS DIRECTORY OR NOT
int exist_dir(const char* file_name)
{
    struct stat path;

    stat(file_name, &path);

    return S_ISDIR(path.st_mode) ? 1 : 0;
}

// FUNCTION TO RETRIEVE THE EXTENSION OF THE FILE
char* fetch_extension (char* path) {
    char *file_name = strrchr(path, '/');

    char *symbol_dot = strrchr(file_name, '.');
    char *symbol_slash = strrchr(file_name, '/');
    if (symbol_dot == NULL || strlen(symbol_dot) > strlen(symbol_slash)) {
        return "";
    } else {
        return symbol_dot;
    }
}


// MAIN 
int main (int arguments, char* arguments_val[]) {
  
    char cwd[1024];
    char path[1024];

    // IF COMMAND IS NOT CORRECT THEN EXIT 
    if (arguments < 4) {
        
        exit(1);
    }
    
    // IF COMMAND CONTAINS MORE ARGUMENTS THEN REQUIRED THEN EXIT
    if (arguments > 10) {
      
        exit(1);
    }
 

    source_path = strdup(arguments_val[1]);  // SOURCE PATH
    destination_path = strdup(arguments_val[2]); // DESTINATION PATH

    if (source_path[strlen(source_path) - 1] == '/') {

        *(source_path + strlen(source_path) - 1) = '\0';
    }

    if (!exist_dir(source_path)) {
      
      exit(1);
    }

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
    perror("getcwd");
    return 1;
  }
  
   
  strcpy(path, source_path);

  if (path[0] == '.' && path[1] == '/') {
    char output[1024];
    sprintf(output, "%s/%s", cwd, path + 2);
    source_path = output;
  }
  strcpy(path, destination_path);

  if (path[0] == '.' && path[1] == '/') {
    char output[1024];
    sprintf(output, "%s/%s", cwd, path + 2);
    destination_path = output;
  }

    char * folder_path;

    char new_destination_path[1024];
    

    folder_path = basename(source_path);
    strcpy(new_destination_path, destination_path);
    strcat(new_destination_path, "/");
    strcat(new_destination_path, folder_path); 
    destination_path = new_destination_path;

    
    char *opts = arguments_val[3];
    int dest_len = strlen(destination_path);

    if (strcmp(opts, "-cp") == 0) {
        is_flag_mv = 0;
    } else if (strcmp(opts, "-mv") == 0) {
        is_flag_mv = 1;
    } else {
       
        exit(1);
    }

    n_ext = arguments - 4;

    char* home_path = getenv("HOME");
    
    if (strncmp(source_path, home_path, (size_t) strlen(home_path)) != 0 ||
    strncmp(destination_path, home_path, (size_t) strlen(home_path)) != 0)
    {
      exit(0);
    }
    

    int i;

    extensions = (arguments_val + 4);
    return nftw(source_path, action_move_copy, 64, FTW_DEPTH | FTW_PHYS);
}
