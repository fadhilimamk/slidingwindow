#ifndef _BUFFER_
#define _BUFFER_

#include <stdio.h>

#define null 0x0

class Buffer {
  private:
    char *data;
    char *seqNumber;
    int size;

  public:
    Buffer() {
      size = 256;
      data = new char[size];
      seqNumber = new char[size];
      for (int i = 0; i < size; i++) {
        data[i] = null;
        seqNumber[i] = null;
      }
    }

    Buffer(int size) {
      this->size = size;
      data = new char[size];
      seqNumber = new char[size];
      for (int i = 0; i < size; i++) {
        data[i] = null;
        seqNumber[i] = null;
      }
    }

    Buffer(int size, int windowSize1, int windowSize2) {
      this->size = size;
      data = new char[size];
      seqNumber = new char[size];
      int max = windowSize1 + windowSize2 + 1;
      for (int i = 0; i < size; i++) {
        data[i] = null;
        seqNumber[i] = i % max;
      }
    }

    ~Buffer() {
      delete [] data;
      delete [] seqNumber;
    }

    int getSize() {
      return size;
    }

    char* getData() {
      return data;
    }

    char* getSeqNumber() {
      return seqNumber;
    }

    char getDataFromIndex(int idx) {
      return data[idx];
    }

    bool isBufferEmpty() {
      int i = 0;
      bool empty = true;
      while (i < size && empty) {
        if (data[i] != null) {
          empty = false;
        }
        i++;
      }
      return empty;
    }

    bool isBufferFull() {
      int i = 0;
      bool full = true;
      while (i < size && full) {
        if (data[i] == null) {
          full = false;
        }
        i++;
      }
      return full;
    }

    int getIndexSeqNumb(char seqNumber, int from, int to) {
      int i = from;
      bool found = false;
      while (i <= to && !found) {
        if (seqNumber == this->seqNumber[i]) {
          found = true;
        }
        else {
          i++;
        }
      }
      return i;
    }

    void clearBuffer() {
      for (int i = 0; i < size; i++) {
        data[i] = null;
        seqNumber[i] = null;
      }
    }

    //update seqNumber

};

#endif
