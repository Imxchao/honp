#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <pcap.h>

void error_die(char *err)
{
    perror(err);
    exit(EXIT_FAILURE);
}

void handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes);
void print_mac_addr(char *name, char *mac);

int main()
{
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevsp;

    if(pcap_findalldevs(&alldevsp, errbuf)) {
        error_die("pcap_findalldevs() failed.");
    }

    pcap_if_t *devsp = alldevsp;
    do {
        printf("%s\n", devsp->name);
        if (devsp->description) {
            printf("\t%s\n", devsp->description);
        }
    }while((devsp = devsp->next));

    printf("Listening on %s\n", alldevsp->name); /* listen on first interface */

    pcap_t *cap_handle = pcap_open_live(alldevsp->name, 2048, 1, 5000, errbuf);
    
    if (cap_handle == NULL) {
        perror("pcap_open_live() failed().");
    }

    pcap_loop(cap_handle, -1, handler, NULL);

    pcap_freealldevs(alldevsp);
    return 0;
}


void handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes)
{
    printf("\n\n------------------------\n");
    printf("Packet Length: %d\n", h->len);

    /*for (int i = 0; i < h->len; i++) {
        if (i % 16 == 0) printf("\n");
        printf("%02x:", *(bytes + i));
    }
    printf("\b \n\n");*/

    struct ether_header *eth = (struct ether_header *)bytes;

    uint16_t eth_type = ntohs(eth->ether_type);

    if (eth_type == ETHERTYPE_IP) {
        printf("It's a IP packet\n");
        struct iphdr *ip = (struct iphdr*)(bytes + 14);

        if (ip->protocol == IPPROTO_TCP) {
            printf("Layer Protocal is TCP\n");
            struct tcphdr *tcp = (struct tcphdr*)(bytes + 14 + 20);
            struct servent *srv = getservbyport(tcp->th_dport, "tcp");
            if (!srv)
                srv = getservbyport(tcp->th_sport, "tcp");
            printf("Service Name: %s\n", srv ? srv->s_name : "Unkown Service");
            return;
        }

        if (ip->protocol == IPPROTO_UDP) {
            printf("Layer Protocal is UDP\n");
            struct udphdr* udp = (struct udphdr*)(bytes + 14 + 20);
            struct servent *srv = getservbyport(udp->uh_dport, "udp");
            if (!srv) {
                srv = getservbyport(udp->uh_sport, "udp");
            }

            printf("Service Name: %s\n", srv ? srv->s_name : "Unkown Service");
            return;
        }
        return;
    }

    switch(eth_type) {
    case ETHERTYPE_PUP: printf("PUP\n"); break;
    case ETHERTYPE_SPRITE: printf("SPRITE\n"); break;
    case ETHERTYPE_ARP: printf("ARP\n"); break;
    case ETHERTYPE_REVARP: printf("REVARP\n"); break;
    case ETHERTYPE_AT: printf("AT\n"); break;
    case ETHERTYPE_AARP: printf("AARP\n"); break;
    case ETHERTYPE_VLAN: printf("VLAN\n"); break;
    case ETHERTYPE_IPX: printf("IPX\n"); break;
    case ETHERTYPE_IPV6: printf("IPV6\n"); break;
    case ETHERTYPE_LOOPBACK: printf("LOOPBACK\n"); break;
    }
    print_mac_addr("dst", eth->ether_dhost);
    print_mac_addr("src", eth->ether_shost);
}

void print_mac_addr(char *name, char *mac)
{
    int len = ETHER_ADDR_LEN;
    printf("%s ", name);
    do {
        printf("%02x:", (*mac++) & 0xFF);
    } while(--len);

    printf("\b \n");
}
