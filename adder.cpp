#include "adder.h"

Register Adder::summ(Register &A, Register &B, bool cFlag) {
  bool sumBit, carryBit = cFlag;
  //  Register result(size);
  deque<bool> result;
  for (auto i = size - 1; i >= 0; --i) {

    sumBit = (carryBit ^ A[i] ^ B[i]);
    //    cout << "sumBit = " << sumBit << "\n";
    carryBit = (A[i] && B[i]) || (A[i] && carryBit) || (B[i] && carryBit);
    //    cout << "carryBit = " << carryBit << "\n";
    result.push_front(sumBit);
  }
  carryFlag = carryBit;
  Register reg(result, result.size(), SIGN_MAGNITUDE_REPR);
  //  reg.printShortLogData();
  //  cout << "*\n";
  return reg;
  //  return Register(result, result.size(), SIGN_MAGNITUDE_REPR);
}

Register Adder::summ(Register &A, bool oFlag, bool cFlag) {
  Register tmpReg(1, SIGN_MAGNITUDE_REPR);
  tmpReg.reserve(size);
  tmpReg[size - 1] = oFlag;
  //  Register tmpReg(tmp, 1, SIGN_MAGNITUDE_REPR);
  //  tmpReg.printLogData("tmpReg");
  return summ(A, tmpReg, cFlag);
}
