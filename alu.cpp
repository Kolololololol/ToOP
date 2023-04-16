#include "alu.h"

// ALU::ALU()
//{

//}

Register ALU::adc(Register &A, Register &B) {
  bool carryBit = flags.CF;
  bool sumBit;
  //  Register result(size);
  deque<bool> result;
  for (auto i = size - 1; i >= 0; --i) {

    sumBit = (carryBit ^ A[i] ^ B[i]);

    carryBit = (A[i] && B[i]) || (A[i] && carryBit) || (B[i] && carryBit);

    result.push_front(sumBit);
  }
  flags.CF = carryBit;
  Register reg(result, result.size(), SIGN_MAGNITUDE_REPR);

  flags.AF = 0;
  return reg;
  //  return Register(result, result.size(), SIGN_MAGNITUDE_REPR);
};

Register ALU::add(Register &A, Register &B /*, uint16_t times*/) {
  uint16_t times = A.getSize() / size;
  if (times == 1) {
    Register reg = adc(A, B);

    flags.SF = reg.getMSB();
    return reg;
  }
  uint16_t szReg = A.getSize();
  Multiplexer mulA(A, szReg, size);
  mulA.printLogData(stringify(mulA));
  //  Log(INFO) << "Multiplexer B info";
  Multiplexer mulB(B, szReg, size);
  mulB.printLogData(stringify(mulB));

  Demultiplexer demux(szReg, size);
  Register regC(szReg, A.getRepresentation());

  //    bool flag = false;

  size_t strobeSignal = 0;
  for (size_t i = 0; i < times; ++i) {
    strobeSignal = times - i - 1;

    Register tmpA = mulA.mux(strobeSignal);

    Register tmpB = mulB.mux(strobeSignal);

    //      Adder adder(tmpA, tmpB, sizeAdder);
    Register sum = adc(tmpA, tmpB);

#ifdef NO_ADDER_LOG
    tmpA.printShortLogData("regA" + to_string(i + 1));
    tmpB.printShortLogData("regB" + to_string(i + 1));

    sum.printShortLogData("temSum" + to_string(i + 1));
#endif

    demux.demux(regC, sum, strobeSignal);
    //      flag = adder.getCarryFlag();
  }
  flags.OF = flags.CF;
  flags.SF = regC.getMSB();

  return regC;
};

extern inline Register ALU::sbb(Register &A, Register &B){

};

Register ALU::shl(Register &A) {
  bool tmpMSB = A[0];
  flags.CF = tmpMSB;
  //  flags.OF = tmpMSB ^ A[0];
  A.removeFront();
  A.add();
  return A;
};

Register ALU::shl(Register &A, uint16_t pos) {
  for (auto i = 0; i < pos; ++i) {
    A = shr(A);
  }
  return A;
};

Register ALU::shr(Register &A) {
  flags.CF = A[A.getSize() - 1];
  //  flags.OF = A[0];
  A.remove();
  A.addFront();
  return A;
};

Register ALU::shr(Register &A, uint16_t pos) {
  for (auto i = 0; i < pos; ++i) {
    A = shl(A);
  }
  return A;
};
;

Register ALU::mul2(Register &A, Register &B) {

  bool sign = A.getMSB() ^ B.getMSB();
  uint16_t regSize = A.getSize();
  A.setSign(false);
  B.setSign(false);
  Register regCHigh(regSize, SIGN_MAGNITUDE_REPR);
  regCHigh.reserve(regSize);
  Register regCLow(regSize, SIGN_MAGNITUDE_REPR);
  regCLow.reserve(regSize);
  Register AHigh(regSize, SIGN_MAGNITUDE_REPR);
  AHigh.reserve(regSize);
//  tmpB.reserve(regSize);
#define NO_ADDER_LOG
  for (auto i = B.getSize() - 1; i > 0; --i) {
    if (B[i]) {
      regCLow = add(A, regCLow);
      regCHigh = add(AHigh, regCHigh);
    }
    shl(A);
    bool bit = flags.CF;
    shl(AHigh);
    AHigh[regSize - 1] = bit;
  }
#undef NO_ADDER_LOG
  regCHigh.setSign(sign);
  regCHigh.setMSB();
  Register result(regSize * 2, regCHigh.getRepresentation());
  Demultiplexer demux(regSize * 2, size);
  uint16_t times = regSize / size;
  size_t strobeSignal = times - 1;

  //  strobeSignal = times - 1;
  demux.demux(result, regCLow, strobeSignal);
  strobeSignal--;
  demux.demux(result, regCHigh, strobeSignal);
  //  for (size_t i = 0; i < times; ++i) {
  //    strobeSignal = times - i - 1;
  //    demux.demux(result, regCHigh, strobeSignal);
  //  }
  //  for (size_t i = 0; i < times; ++i) {
  //	strobeSignal = times - i - 1;
  //	demux.demux(result, regCLow, strobeSignal);
  //  }

  return result;
}

// Register ALU::sar(Register &A, Register &B){};

// Register ALU::sal(Register &A, Register &B){};

// Register ALU::scr(Register &A, Register &B){};

// Register ALU::scl(Register &A, Register &B){};
