//Jay Shah
//COEN 146L, Lab 5
//Date: 2/16/20
//Description: Build a stop and wait reliable protocol on top of UDP to provide a reliable transport service




#define PBADCKSUM 5 // P(bad checksum) = 1 / PBADCKSUM

typedef struct {
    int seq_ack;
    int len;
    int cksum;
} Header;

typedef struct {
    Header header;
    char data[10];
} Packet;

int getChecksum(Packet packet);

void logPacket(Packet packet);
