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
    
    {"0x10DE", "0x1B06", "NVIDIA GeForce GTX 1080 Ti"},
    {"0x10DE", "0x1B80", "NVIDIA GeForce GTX 1080"},
    {"0x10DE", "0x1B82", "NVIDIA GeForce GTX 1070 Ti"},
    {"0x10DE", "0x1B81", "NVIDIA GeForce GTX 1070"},
    {"0x10DE", "0x1C03", "NVIDIA GeForce GTX 1060"},
    {"0x10DE", "0x1C82", "NVIDIA GeForce GTX 1050 Ti"},
    {"0x10DE", "0x1C81", "NVIDIA GeForce GTX 1050"},

    
    {"0x10DE", "0x1E07", "NVIDIA GeForce RTX 2080 Ti"},
    {"0x10DE", "0x1E87", "NVIDIA GeForce RTX 2080"},
    {"0x10DE", "0x1E84", "NVIDIA GeForce RTX 2070 Super"},
    {"0x10DE", "0x1E82", "NVIDIA GeForce RTX 2070"},
    {"0x10DE", "0x1F08", "NVIDIA GeForce RTX 2060"},
    {"0x10DE", "0x2182", "NVIDIA GeForce GTX 1660 Ti"},
    {"0x10DE", "0x21C4", "NVIDIA GeForce GTX 1660 Super"},
    {"0x10DE", "0x1F83", "NVIDIA GeForce GTX 1650 Super"},


    {"0x10DE", "0x2204", "NVIDIA GeForce RTX 3090"},
    {"0x10DE", "0x2206", "NVIDIA GeForce RTX 3080"},
    {"0x10DE", "0x2482", "NVIDIA GeForce RTX 3070 Ti"},
    {"0x10DE", "0x2484", "NVIDIA GeForce RTX 3070"},
    {"0x10DE", "0x2486", "NVIDIA GeForce RTX 3060 Ti"},
    {"0x10DE", "0x2503", "NVIDIA GeForce RTX 3060"},

  
    {"0x1002", "0x67DF", "AMD Radeon RX 580"},
    {"0x1002", "0x67DF", "AMD Radeon RX 570"},
    {"0x1002", "0x67FF", "AMD Radeon RX 560"},
    {"0x1002", "0x67FF", "AMD Radeon RX 550"},
    {"0x1002", "0x67DF", "AMD Radeon RX 480"},
    {"0x1002", "0x67DF", "AMD Radeon RX 470"},


    {"0x1002", "0x687F", "AMD Radeon RX Vega 64"},
    {"0x1002", "0x687F", "AMD Radeon RX Vega 56"},
    {"0x1002", "0x66AF", "AMD Radeon VII"},

 
    {"0x1002", "0x731F", "AMD Radeon RX 5700 XT"},
    {"0x1002", "0x7310", "AMD Radeon RX 5700"},
    {"0x1002", "0x731F", "AMD Radeon RX 5600 XT"},
    {"0x1002", "0x7340", "AMD Radeon RX 5500 XT"},
    {"0x1002", "0x73BF", "AMD Radeon RX 6800 XT"},
    {"0x1002", "0x73FF", "AMD Radeon RX 6800"},
    {"0x1002", "0x73FF", "AMD Radeon RX 6700 XT"},
    {"0x1002", "0x73ff", "AMD Radeon RX 6600 XT"},



    {"0x8086", "0x1912", "Intel HD Graphics 530"},
    {"0x8086", "0x591B", "Intel HD Graphics 630"},
    {"0x8086", "0x3E92", "Intel UHD Graphics 620"},
    {"0x8086", "0x3E9B", "Intel UHD Graphics 630 (Desktop)"},
    {"0x8086", "0x3EA0", "Intel Iris Plus Graphics 640"},
    {"0x8086", "0x3EA1", "Intel Iris Plus Graphics 655"},

 
    {"0x8086", "0x4905", "Intel Iris Xe MAX"},
    {"0x8086", "0x56A5", "Intel Arc A380"},
    {"0x8086", "0x56B1", "Intel Arc A750"},
    {"0x8086", "0x56A0", "Intel Arc A770"},
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
    const char* paths[] = {
        "/sys/class/drm/card0/device/vendor",
        "/sys/class/drm/card1/device/vendor"
    };
    const char* device_paths[] = {
        "/sys/class/drm/card0/device/device",
        "/sys/class/drm/card1/device/device"
    };

    int i;
    for (i = 0; i < 2; i++) {
        fp = fopen(paths[i], "r");
        if (fp != NULL) {
            fgets(vendor, sizeof(vendor), fp);
            fclose(fp);

            fp = fopen(device_paths[i], "r");
            if (fp != NULL) {
                fgets(device, sizeof(device), fp);
                fclose(fp);
                break;  
            }
        }
    }

    if (i == 2) {
        perror("Failed to open GPU device files");
        return;
    }


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
