#ifndef _SEGMENT_
#define _SEGMENT_

#include <stdio.h>

class Segment {
  private:
    char soh; //1 byte (0x1)
    int seqNumb; //4 byte
    char stx; //1 byte (0x2)
    char data; //1 byte
    char etx; //1 byte (0x3)
    char checksum; //1 byte

  public:
    Segment (int seqNumb, char data) {
      soh = 0x1;
      this->seqNumb = seqNumb;
      stx = 0x2;
      this->data = data;
      etx = 0x3;
      checksum = checkSumSeg();
    }

    int getSeqNumb() {
      return seqNumb;
    }

    char getData() {
      return data;
    }

    char getChecksum() {
      return checksum;
    }

    //checksum
    char checkSumSeg() {
      char seq_1 = (seqNumb & 0xFF000000) >> 24; // ngambil 0xXX------ dari seqNum
      char seq_2 = (seqNumb & 0xFF0000) >> 16;   // ngambil 0x--XX---- dari seqNum
      char seq_3 = (seqNumb & 0xFF00) >> 8;      // ngambil 0x----XX-- dari seqNum
      char seq_4 = (seqNumb & 0xFF);             // ngambil 0x------XX dari seqNum
      return ((soh + stx + etx + seq_1 + seq_2 + seq_3 + seq_4 + data) % 256);
    }

    char* toBuffer() {
      static char buffer[9];
      
      buffer[0] = soh;
      buffer[1] = (seqNumb & 0xFF000000) >> 24; // ngambil 0xXX------ dari seqNum
      buffer[2] = (seqNumb & 0xFF0000) >> 16;   // ngambil 0x--XX---- dari seqNum
      buffer[3] = (seqNumb & 0xFF00) >> 8;      // ngambil 0x----XX-- dari seqNum
      buffer[4] = (seqNumb & 0xFF);             // ngambil 0x------XX dari seqNum
      buffer[5] = stx;
      buffer[6] = data;
      buffer[7] = etx;
      buffer[9] = checksum;

      return buffer;
    }

    Segment(unsigned char* buffer) {
      int seqNmbr = ((buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4]);
      int data = buffer[6];
      soh = 0x1;
      this->seqNumb = seqNmbr;
      stx = 0x2;
      this->data = data;
      etx = 0x3;
      checksum = checkSumSeg();
    }

};

#endif
