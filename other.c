#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "other.h"

#define BUFFER_SIZE 256

void get_gpu_info() {
    FILE *fp;
    char buffer[BUFFER_SIZE];
    char *gpu_info = NULL;

    fp = popen("lspci | grep -i vga", "r");
    if (fp == NULL) {
        perror("Failed to run lspci");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        size_t length = strlen(buffer);
        gpu_info = (char *)malloc(length + 1);
        if (gpu_info != NULL) {
            strncpy(gpu_info, buffer, length);
            gpu_info[length] = '\0';
            break;
        }
    }

    pclose(fp);

    if (gpu_info != NULL) {
        printf("GPU: %s", gpu_info);
        free(gpu_info);
    } else {
        printf("Failed to detect GPU Information\n");
    }
}

#define BUFFER_SIZE 256
void get_linux_distribution() {
    FILE *fp;
    char buffer[BUFFER_SIZE];
    char *distro_name = NULL;

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
        free(distro_name);
    } else {
        printf("Failed to detect Linux distribution\n");
    }
}