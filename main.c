#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sysinfo.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <netdb.h>
#include "other.h"
#include "gpugrab.h"
#include <vulkan/vulkan.h>



void red () {
  printf("\033[0;32m");
}

void reset () {
  printf("\033[0m");
}


extern char *distro_name;

int main() {

    printf("\n");
    struct ifaddrs *ifaddr, *ifa;
	struct sysinfo info;
    char host[NI_MAXHOST];
    char hostname[1024];
    int result;

    red();
	printf("------------------------------------ MiniFetch -----------------------------");
	printf("\n");


    printf(" |       _____   .__         .__ ___________          __           .__      \n"
           " |      /     \\  |__|  ____  |__|\\_   _____/  ____  _/  |_   ____  |  |__   \n"
           " |     /  \\ /  \\ |  | /    \\ |  | |    __)  _/ __ \\ \\   __\\_/ ___\\ |  |  \\  \n"
           " |    /    Y    \\|  ||   |  \\|  | |     \\   \\  ___/  |  |  \\  \\___ |   Y  \\ \n"
           " |    \\____|__  /|__||___|  /|__| \\___  /    \\___  > |__|   \\___  >|___|  / \n"
           " |            \\/          \\/          \\/         \\/             \\/      \\/ \n");

    reset();
	printf("\n");
	
	result = gethostname(hostname, sizeof(hostname));
    if (result == 0) {
        printf("Hostname: %s\n", hostname);
    }   else {
        perror("gethostname");
    }
	
    #if defined(__linux__)
        printf("OS: Linux\n");
		
	#endif
    get_linux_distribution();
	
	get_cpu_info();
	
	int gpu_count = 0;
    char *gpu_models[10];  // Assuming a maximum of 10 GPUs

    get_gpu_model_names(gpu_models, &gpu_count);

    for (int i = 0; i < gpu_count; i++) {
        printf("GPU %d: %s\n", i + 1, gpu_models[i]);
        free(gpu_models[i]);  // Free allocated memory
    }
	
	 if (sysinfo(&info) != 0) {
        perror("sysinfo");
        return EXIT_FAILURE;
    }
	
   
    unsigned long ramamt = info.totalram / 1024 / 1024;
    float ramtot;

    if (ramamt >= 1024) {
        ramtot = ramamt / 1024.0;
        printf("Total RAM: %.2f GB\n", ramtot);
    } else {
        ramtot = ramamt; 
        printf("Total RAM: %lu MB\n", ramtot);
    }
	

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return EXIT_FAILURE;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        if (ifa->ifa_addr->sa_family == AF_INET) {
            int result = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

            if (result != 0) {
                printf("getnameinfo() failed: %s", gai_strerror(result));
                continue;
            }

            printf("Interface (%s): %s", ifa->ifa_name, host);
            printf("\n");

        }
    
    }

    


    freeifaddrs(ifaddr);
	
	printf("\n");
	
	return 0;

}
