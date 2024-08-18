#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "other.h"
#include <dirent.h> 

#define BUFFER_SIZE 256

char *distro_name = NULL;



void get_linux_distribution() {
    FILE *fp;
    char buffer[BUFFER_SIZE];

    fp = fopen("/etc/os-release", "r");
    if (fp == NULL) {
        perror("Failed to open /etc/os-release");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strncmp(buffer, "PRETTY_NAME", 11) == 0) {
            char *start = strchr(buffer, '"');
            if (start != NULL) {
                char *end = strchr(start + 1, '"');
                if (end != NULL) {
                    size_t length = end - start - 1;
                    distro_name = (char *)malloc(length + 1);
                    if (distro_name != NULL) {
                        strncpy(distro_name, start + 1, length);
                        distro_name[length] = '\0';
                    }
                }
            }
            break;
        }
    }

    fclose(fp);

    if (distro_name != NULL) {
        printf("Distro: %s\n", distro_name);
    } else {
        printf("Failed to detect Linux distribution\n");
    }
}

void get_cpu_info() {
        FILE* cpuinfo = fopen("/proc/cpuinfo", "rb");
        char* line = NULL;
        size_t size = 0;
    
    if (cpuinfo == NULL) {
        perror("Failed to open /proc/cpuinfo");
        return;
    }



    while (getline(&line, &size, cpuinfo) != -1) {
        if (strstr(line, "model name") != NULL) {
            char* model_name = strchr(line, ':');
            if (model_name) {
                model_name++; 
                while (*model_name == ' ' || *model_name == '\t') model_name++; 
                printf("CPU Model: %s", model_name);
            }
            break;
        }
    }
    free(line);
    fclose(cpuinfo);
}

/***
 *       _____   .__         .__ ___________          __           .__      
 *      /     \  |__|  ____  |__|\_   _____/  ____  _/  |_   ____  |  |__   
 *     /  \ /  \ |  | /    \ |  | |    __)  _/ __ \ \   __\_/ ___\ |  |  \  
 *    /    Y    \|  ||   |  \|  | |     \   \  ___/  |  |  \  \___ |   Y  \ 
 *    \____|__  /|__||___|  /|__| \___  /    \___  > |__|   \___  >|___|  / 
 *            \/          \/          \/         \/             \/      \/  
 */
