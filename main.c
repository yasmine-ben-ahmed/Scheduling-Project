#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <unistd.h>

#include "main.h"

// Function for dynamic menu selection based on directory content
int dynamic_menu_selection(char *dirName, int choice, int *nbf, char *selectedFileName) {
    DIR *d;
    struct dirent *dir;
    d = opendir(dirName);
    
    int nbFiles;
    nbFiles = 0;
    char *ptr;
    int dot = '.';

    if (d) {
        if (choice == 0) {
            printf("\n************************************* \n");
            printf("\n OUR Scheduling Programs \n");
            printf("\n************************************* \n");
        }
        
        while ((dir = readdir(d)) != NULL) {
            ptr = strchr(dir->d_name, dot);
            if (ptr == NULL) {
                nbFiles++;
                
                if (nbFiles == choice && nbFiles > 0) {
                    strcpy(selectedFileName, dir->d_name);
                    break;
                }
                
                if (choice == 0) {
                    printf("-%d: %s\n", nbFiles, dir->d_name);
                }
            }
        }
        
        if (choice == 0) {
            *nbf = nbFiles;
            selectedFileName[0] = '\0';
        } else {
            *nbf = -1;
        }
        
        printf("\n");
        closedir(d);
    } else {
        printf("\n Unable to open : %s ", dirName);
        return -1;
    }
    
    return (0);
}

int main(int argc, char **argv) {
    // Random input
    input();
    
    int n, i, choice, result, nbf;
    char selectedFileName[256];
    char path[256];
    char default_algo[10];
    int res;
    int ver;
    
    ver = 0;
    
    strcpy(path, "./executable/");
    strcpy(default_algo, "fifo");
    
    result = dynamic_menu_selection(path, 0, &nbf, selectedFileName);
    printf("Type the number of the algorithm you want to use \n");
    printf("Press any character to use the default algorithm = FIFO \n");
    
    if (result == 0) {
        do {
            printf("\n-");
            res = scanf("%d", &choice);
            if (res == 1) {
                if (choice > 0 && choice <= nbf) {
                    ver = 1;
                    result = dynamic_menu_selection(path, choice, &nbf, selectedFileName);
                    strcat(path, selectedFileName);
                } else {
                    printf("You must enter a valid number\n");
                }
            } else {
                ver = 1;
                printf("\nThe FIFO algorithm will be used by default \n");
                strcat(path, default_algo);
            }
        } while (!(ver == 1));  // ver=0;
        
        strcat(path, " ");
        strcat(path, argv[1]);
        int status = system(path);
    } else {
        printf("Error occurred while listing the content of the directory \n");
        return -1;
    }
}

