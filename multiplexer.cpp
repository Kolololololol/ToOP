#include "multiplexer.h"

Register Multiplexer::mux(uint16_t strobeSignal) {
  //  Register result(adderSize);

  deque<bool> result;
  uint16_t tmp = strobeSignal * adderSize;
  for (auto i = tmp; i < tmp + adderSize; ++i) {
    //    result.add(number[i]);
    result.push_back(number[i]);
  }
  Log(INFO) << "Multiplexer succefully return part no "
            << (size / adderSize - strobeSignal) << " of register.";
  Register reg(result, result.size(), number.getRepresentation());
  //  reg.printShortLogData("reg"+to_string(strobeSignal+1));
  return reg;
}
