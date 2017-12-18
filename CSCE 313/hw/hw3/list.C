// list.C
// Carsten Hood
// CSCE-313 HW 3 Problem 4
// UIN: 922009787

/*
 (Small Programming Assignment) Write a program to list and count the entries in a directory and all its subdirectories. A process can either list entries or count them, but not both. In order to do both you need to create at least two processes: the first process lists the entries of the directory and sends them to the second process, which in turn counts the number of entries received from the first process. The processes communicate using unnamed pipes. Your program should have one argument, namely the name of the directory you want to list and count (e.g. /foo/bar/tree). Make sure that all processes are terminated once your program terminates.
*/

// compile: $ g++ -std=c++11 list.C -o list
// run:     $ ./list /home/ugrads/c/chood/csce_313

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

// for processes and pipes
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <cstring>

// for navigating file system
#include <sys/types.h>
#include <dirent.h>
#include <string>

using namespace std;

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

#define READ 0  // via stackoverflow.com
#define WRITE 1

string path;
int fd[2];

/*--------------------------------------------------------------------------*/
/* FUNCTIONS */
/*--------------------------------------------------------------------------*/

// recursively lists sub-entities and updates counter process
void list_files(const char * filepath, int depth) {
    struct dirent * entry;
    DIR * directory;
    directory = opendir(filepath);
    if (!directory) return;
    
    while (entry = readdir(directory)) {
        
        // establish current entity name
        size_t name_size = sizeof(entry->d_name);
        char name[name_size];
        strncpy(name, entry->d_name, name_size);
        name[name_size-1] = '\0';
        
        // check type of entity (directory, file, etc)
        if (strcmp(name, ".") == 0) {
            continue;
        } else if (strcmp(name, "..") == 0) {
            continue;
        } else if (entry->d_type == DT_DIR) {   // is directory
            char new_path[1024];
            int path_length = snprintf(new_path, sizeof(new_path)-1,
                                       "%s/%s", filepath, name);
            new_path[path_length] = 0;
            printf("%*s[ %s ]\n", depth*2, "", name);
            write(fd[WRITE], "1", 1);       // write addition to pipe
            list_files(new_path, depth+1);  // recursive call on subdirs
        } else {                            // is file
            printf("%*s( %s )\n", depth*2, "", name);
            write(fd[WRITE], "1", 1);       // write addition to pipe
        }
    }
    closedir(directory);
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
    char update[1];
    pipe(fd);
    
    if (fork() == 0) {                          // child writer
        close(fd[READ]);                        // reader unneeded
        list_files(argv[1], 0);                 // start recursion
        write(fd[WRITE], "!", strlen("!")+1);   // '!' ends loop
        close(fd[WRITE]);
    } else {                                    // parent reader
        close(fd[WRITE]);                       // writer unneeded
        int counter = 0;
        
        // read updates from pipe until terminator '!' is received
        while (true) {
            read(fd[READ], update, 1);
            string update_str(update);
            if (update_str .find("!") != std::string::npos) break;
            ++counter;
        }
        close(fd[READ]);
        printf("Total entity count: %d\n", counter);
    }
}
// end list.C
