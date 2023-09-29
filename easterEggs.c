/** 
 * file:  easterEggs.c 
 * author:  Michael Mathews 
 * course: CSI 3336 
 * assignment: Project 7 
 * due date:  4/3/2023 
 * 
 * date modified: 4/3/2023 
 *      - file created 
 * 
 * recursively searches through all files in current and subdirectories
 * for a given file name. returns the count of each "color" in any found file
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>

enum color {red, green, blue, yellow, orange};

/** 
* preconditions: files are not searched through and count is not updated
* postconditions: count of certain color is updated
* parameters: filename, directory, num, count
* return: none 
*/ 

void searchFiles(const char* filename, char* directory, int num, int* count) {
    DIR *dir;
    char path[2048];
    struct dirent *cur;
    int val;
    FILE *fp;
    if ((dir = opendir(directory)) != NULL) {
        while ((cur = readdir(dir)) != NULL) {
            if (cur->d_type == DT_DIR) {
                if (strcmp(cur->d_name, ".") == 0 || strcmp(cur->d_name, "..") == 0) {
                    continue;
                }
                //add to file path                        
                sprintf(path, "%s/%s", directory, cur->d_name);
                searchFiles(filename, path, num, count);
            } else if (strcmp(cur->d_name, filename) == 0) {
                fp = fopen(cur->d_name, "rb");
                if (fp) {
                    while (fread(&val, 4, 1, fp) != 0) {
                        if (val == num) {
                            (*count)++;
                        }
                    }
                    fclose(fp);
                } else { printf("Error: cannot open file"); }
            }
        }
        closedir(dir);
    }
}

int main(int argc, char** argv){
    int i, val, result;
    pid_t pid;

    if(argc != 2){
        printf("Error: no file entered");
        return 1;
    }
    
    char *filename = argv[1];
    int count[5];

    for(i = 0; i < 5; i++){
        pid = fork();
        //child
        if(pid == 0){
            val = 0;
            searchFiles(filename, ".",  i, &val);
            return val;
        }
        //parent
        else {
            wait(&result);
            count[i] = WEXITSTATUS(result);
        }
    }

    printf("%-10s%d\n", "red:", count[0]);
    printf("%-10s%d\n", "green:", count[1]);
    printf("%-10s%d\n", "blue:", count[2]);
    printf("%-10s%d\n", "yellow:", count[3]);
    printf("%-10s%d\n", "orange:", count[4]);

    return 0;
}
