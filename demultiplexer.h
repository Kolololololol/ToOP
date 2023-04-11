#ifndef DEMULTIPLEXER_H
#define DEMULTIPLEXER_H

#include <register.h>

#include <deque>
using namespace std;

class Demultiplexer {
  //  Register /*number, */ resultNumber;
  uint16_t size;
  uint16_t adderSize;
  //  uint16_t strobeSignal;

public:
  Demultiplexer(uint16_t sz, uint16_t adderSz) {
    size = sz;
    adderSize = adderSz;
    //    deque<bool> tmp;
    //    this->resultNumber = Register(tmp, size, SIGN_MAGNITUDE_REPR);
  }
  void demux(Register &result, Register &num, uint16_t strbSign);
  //  Register getDemuxResult() { return resultNumber; }
};

#endif // DEMULTIPLEXER_H
