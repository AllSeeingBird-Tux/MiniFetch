#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <netdb.h>
#include "other.h"



void red () {
  printf("\033[1;31m");
}

void reset () {
  printf("\033[0m");
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

int main()
{
    printf("\n");
    struct ifaddrs *ifaddr, *ifa;
    char host[NI_MAXHOST];
    char hostname[1024];
    int result;
    red();
	printf("------------- MiniFetch -------------");
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
	
    get_gpu_info();
	
	
    get_cpu_info();
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


    
    
    return 0;


}
