#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "other.h"
#include <dirent.h> 

#define BUFFER_SIZE 256

char *distro_name = NULL;

typedef struct {
    const char* vendor_id;
    const char* device_id;
    const char* model_name;
} GPUInfo;


GPUInfo gpu_database[] = {
    {"0x1002", "0x73ff", "AMD Radeon RX 6600 XT"},
    {"0x10DE", "0x1F02", "NVIDIA GeForce RTX 3080"},
    {"0x8086", "0x9BC5", "Intel UHD Graphics 630"},
    {"0x1002", "0x73BF", "AMD Radeon RX 6700 XT"},
    {"0x1002", "0x731F", "AMD Radeon RX 5700 XT"},
    {"0x1002", "0x7310", "AMD Radeon RX 5700"},
    {"0x1002", "0x67DF", "AMD Radeon RX 580"},
    {"0x1002", "0x67EF", "AMD Radeon RX 570"},
    {"0x1002", "0x687F", "AMD Radeon RX Vega 64"},
    {"0x1002", "0x687F", "AMD Radeon RX Vega 56"},
    {"0x1002", "0x67B1", "AMD Radeon RX 460"},
    {"0x1002", "0x67DF", "AMD Radeon RX 480"},

};

const char* lookup_gpu_model(const char* vendor_id, const char* device_id) {
    for (int i = 0; i < sizeof(gpu_database)/sizeof(GPUInfo); i++) {
        if (strcmp(gpu_database[i].vendor_id, vendor_id) == 0 &&
            strcmp(gpu_database[i].device_id, device_id) == 0) {
            return gpu_database[i].model_name;
        }
    }
    return "Unknown GPU Model";
}

void get_gpu_info() {
    FILE *fp;
    char vendor[128], device[128];


    fp = fopen("/sys/class/drm/card1/device/vendor", "r");
    if (fp == NULL) {
        perror("Failed to open /sys/class/drm/card1/device/vendor");
        return;
    }
    fgets(vendor, sizeof(vendor), fp);
    fclose(fp);


    fp = fopen("/sys/class/drm/card1/device/device", "r");
    if (fp == NULL) {
        perror("Failed to open /sys/class/drm/card1/device/device");
        return;
    }
    fgets(device, sizeof(device), fp);
    fclose(fp);

    vendor[strcspn(vendor, "\n")] = 0;
    device[strcspn(device, "\n")] = 0;

    const char* model_name = lookup_gpu_model(vendor, device);


    printf("GPU Model: %s\n", model_name);
}


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
