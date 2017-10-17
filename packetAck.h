#ifndef _PACKETACK_
#define _PACKETACK_

#include <stdio.h>

class PacketAck {
  private:
    char ack; //1 byte (0x6)
    int nextSeqNumb; //4 byte
    unsigned char advWindowSize; //sisa recv buffer
    char checksum; //1 byte

  public:
    PacketAck (int nextSeqNumb, unsigned char advWindowSize) {
      ack = 0x6;
      this->nextSeqNumb = nextSeqNumb;
      this->advWindowSize = advWindowSize;
      checksum = checkSumAck();
    }

    int getNextSeqNumb() {
      return nextSeqNumb;
    }

    unsigned char getAdvWinsowSize() {
      return advWindowSize;
    }

    char getChecksum() {
      return checksum;
    }

    //checksum
    char checkSumAck() {
      char seq_1 = (nextSeqNumb & 0xFF000000) >> 24; // ngambil 0xXX------ dari nextSeqNum
      char seq_2 = (nextSeqNumb & 0xFF0000) >> 16;   // ngambil 0x--XX---- dari nextSeqNum
      char seq_3 = (nextSeqNumb & 0xFF00) >> 8;      // ngambil 0x----XX-- dari nextSeqNum
      char seq_4 = (nextSeqNumb & 0xFF);             // ngambil 0x------XX dari nextSeqNum
      return ((ack + seq_1 + seq_2 + seq_3 + seq_4 + advWindowSize) % 256);
    }
};

#endif
