#include <stdio.h>
#include <string.h>

// For open, ...
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// For close, ...
#include <unistd.h>
// For various protocol headers and types
#include <linux/if_ether.h> // struct ethhdr, Ethernet protocol definitions
#include <linux/ip.h> // struct iphdr
#include <linux/udp.h> // struct udphdr, UDP definitions
#include <linux/tcp.h> // struct tcphdr, TCP definitions
#include <arpa/inet.h> // IP protocol definitions, hton...

#define MTU 1500

void parse_pkt(const unsigned char *pkt, int len)
{
	unsigned int parsed_hdr_size;
	struct ethhdr *eh;
	struct iphdr *ih;
	struct udphdr *uh;
	struct tcphdr *th;
	unsigned char *addr;
	int i;

	parsed_hdr_size = 0;
	eh = (struct ethhdr *)(pkt + parsed_hdr_size);
	if (len < (parsed_hdr_size + sizeof(struct ethhdr)))
	{
		printf("Incomplete Packet. Aborting ... \n");
		return;
	}
	/* TODO 1: Print the Ethernet Hdr Fields */
	// ideas from https://stackoverflow.com/a/5712681
	printf(
        "Destination MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
        eh->h_dest[0], eh->h_dest[1], eh->h_dest[2],
        eh->h_dest[3], eh->h_dest[4], eh->h_dest[5]
    );

	printf(
        "Sender MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
        eh->h_source[0], eh->h_source[1], eh->h_source[2],
        eh->h_source[3], eh->h_source[4], eh->h_source[5]
    );
	if (ntohs(eh->h_proto) != ETH_P_IP)
	{
		printf("Non-IP Hdr follows. Skipping ...\n");
		return;
	}
	parsed_hdr_size += sizeof(struct ethhdr);
	ih = (struct iphdr *)(pkt + parsed_hdr_size);
	if (len < (parsed_hdr_size + sizeof(struct iphdr)))
	{
		printf("Incomplete Packet. Aborting ... \n");
		return;
	}
	/*	
			version:4;
	__u8	tos;
	__be16	tot_len;
	__be16	id;
	__be16	frag_off;
	__u8	ttl;
	__u8	protocol;
	__sum16	check;
	__be32	saddr;
	__be32	daddr; */
	/* TODO 2: Print the IP Hdr Fields */
	printf("ID: 0x%04hX\n",ntohs(ih->id));
	printf("Fragment Offset: %hu \n",ntohs(ih->frag_off));
	printf ("IP TTL = %d\n", ih->ttl);
    printf ("IP check sum = 0x%02hhX\n", ntohs(ih->check));
	printf("Protocol : 0x%02hhX \n",ih->protocol);
    printf ("src IP = %d.%d.%d.%d\n", ih->saddr & 0x000000FF, (ih->saddr & 0x0000FF00) >> 8, (ih->saddr & 0x00FF0000) >> 16,(ih->saddr & 0xFF000000) >> 24);
	printf ("dst IP = %d.%d.%d.%d\n", ih->daddr & 0x000000FF, (ih->daddr & 0x0000FF00) >> 8, (ih->daddr & 0x00FF0000) >> 16,(ih->daddr & 0xFF000000) >> 24);
	parsed_hdr_size += sizeof(struct iphdr);
	if (ih->protocol == IPPROTO_UDP)
	{
		uh = (struct udphdr *)(pkt + parsed_hdr_size);
		if (len < (parsed_hdr_size + sizeof(struct udphdr)))
		{
			printf("Incomplete Packet. Aborting ... \n");
			return;
		}
		/* TODO 3: Print the UDP Hdr Fields 
		struct udphdr {
			__be16	source;
			__be16	dest;
			__be16	len;
			__sum16	check;
		};
		*/

		printf("UDP Src port: %d\n",ntohs(uh->source));
    	printf("UDP Dst port: %d\n",ntohs(uh->dest));
		printf("Hdr + Data Len: %hu, \n",ntohs(uh->len));
		printf("Chksum: 0x%04X\n",ntohs(uh->check));
		parsed_hdr_size += sizeof(struct udphdr);
	}
	else if (ih->protocol == IPPROTO_TCP)
	{
		th = (struct tcphdr *)(pkt + parsed_hdr_size);
		if (len < (parsed_hdr_size + sizeof(struct tcphdr)))
		{
			printf("Incomplete Packet. Aborting ... \n");
			return;
		}
		/* TODO 4: Print the TCP Hdr Fields */
		printf("TCP Src port: %d\n",ntohs(th->source));
    	printf("TCP Dst port: %d\n",ntohs(th->dest));
		printf("TCP window: %hu\n",ntohs(th->window));
		printf("TCP checksum: 0x%04hX\n", ntohs(th->check));
		printf("Seq: %u",ntohl(th->seq));
		printf("Ack no.: %u",ntohl(th->ack_seq));
		parsed_hdr_size += sizeof(struct tcphdr);
	}
	else
	{
		printf("Non-UDP/TCP Hdr follows. Skipping ...\n");
		return;
	}
	printf("Payload (%d bytes):", len - parsed_hdr_size);
	for (i = parsed_hdr_size; i < len; i++)
	{
		printf(" %02hhX", pkt[i]);
	}
	printf("\n");
}
int main(int argc, char *argv[])
{
	char *fn;
	int fd;
	unsigned char pkt[MTU];
	int len = MTU;

	if (argc != 2)
	{
		printf("Usage : %s <pkt_file_name>\n", argv[0]);
		return 1;
	}
	fn = argv[1];
	if ((fd = open(fn, O_RDONLY)) == -1)
	{
		perror("open");
		return 2;
	}
	if ((len = read(fd, pkt, MTU)) == -1)
	{
		perror("read");
		close(fd);
		return 3;
	}
	parse_pkt(pkt, len);
	close(fd);

	return 0;
}
