#include "demultiplexer.h"

void Demultiplexer::demux(Register &result, Register &num, uint16_t strbSign) {
  ;
  //  Register result(adderSize);
  uint16_t tmp = strbSign * adderSize;
  //  cout << tmp;
  //  for (auto i = tmp; i < tmp + adderSize; ++i) {
  //    result.add(num[i - tmp]);
  //  }

  for (auto i = tmp + adderSize - 1; i >= tmp; --i) {
    result.addFront(num[i - tmp]);
  }
}
