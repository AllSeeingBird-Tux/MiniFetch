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

int main()
{
    struct ifaddrs *ifaddr, *ifa;
    char host[NI_MAXHOST];
    char hostname[1024];
    int result;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return EXIT_FAILURE;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        if (ifa->ifa_addr->sa_family == AF_INET) {
            int result = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                                     host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (result != 0) {
                printf("getnameinfo() failed: %s\n", gai_strerror(result));
                continue;
            }

            printf("Interface: %s\tAddress: %s\n", ifa->ifa_name, host);

        }
    
    }

    freeifaddrs(ifaddr);

    result = gethostname(hostname, sizeof(hostname));
    if (result == 0) {
        printf("Hostname: %s\n", hostname);
    }   else {
        perror("gethostname");
    }
    
    return 0;


}