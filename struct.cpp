#include <iostream>
using namespace std;

struct segment {
  char soh; //1 byte (0x1)
  int seqNumb; //4 byte
  char stx; //1 byte (0x2)
  char data; //1 byte
  char etx; //1 byte (0x3)
  char checksum; //1 byte
};

struct packetAck {
  char ack; //1 byte (0x6)
  int nextSeqNumb; //4 byte
  unsigned char advWindowSize; //sisa recv buffer
  char checksum; //1 byte
};

struct buffer {
  char data[256]; //256 byte of data
  int *ptr; //pointer
};

//inisiasi segment
segment initSegment(int seqNumb, char data, char checksum) {
  segment seg;
  seg.soh = 0x1;
  seg.seqNumb = seqNumb;
  seg.stx = 0x2;
  seg.data = data;
  seg.etx = 0x3;
  seg.checksum = checksum;

  return seg;
}

//inisiasi packetAck
packetAck initPacketAck (int nextSeqNumb, unsigned char advWindowSize, char checksum) {
  packetAck pAck;
  pAck.ack = 0x6;
  pAck.nextSeqNumb = nextSeqNumb;
  pAck.advWindowSize = advWindowSize;
  pAck.checksum = checksum;

  return pAck;
}

// int main () {
//   segment segmentData = initSegment(1, 'a', 'a');
//   packetAck pAck = initPacketAck(2, 0xff, 'a');
  
//   return 0;
// }
