#include "alu.h"

#define NIBBLE 4
// ALU::ALU()
//{

//}

Register ALU::adc(Register &A, Register &B) {
  //    initFLAGS();
  bool carryBit = flags.CF;
  bool sumBit;

  //  Register result(size);
  deque<bool> result;
  for (auto i = size - 1; i >= 0; --i) {
    if (blockAF && (i % 4 == 3)) {
      carryBit = false;
    }

    sumBit = (carryBit ^ A[i] ^ B[i]);

    carryBit = (A[i] && B[i]) || (A[i] && carryBit) || (B[i] && carryBit);

    result.push_front(sumBit);
    if (allowAF && i == 4 && !blockAF) {
      prevAF = carryBit;
    }
  }
  if (blockAF) {
    Register reg(result, result.size(), BCD_SIGN_MAGNITUDE_REPR);
    return reg;
  } else {
    flags.CF = carryBit;
    if (allowAF) {
      flags.AF = carryBit;
      Register reg(result, result.size(), BCD_SIGN_MAGNITUDE_REPR);
      return reg;
    } else {
      Register reg(result, result.size(), SIGN_MAGNITUDE_REPR);
      return reg;
    }
  }

  //  return Register(result, result.size(), SIGN_MAGNITUDE_REPR);

  //      bool carryBit = flags.CF;
  //  bool sumBit;
  //  deque<bool> result;
  //  for (auto i = size - 1; i >= 0; --i) {
  //      sumBit = (flags.CF ^ A[i] ^ B[i]);

  //      flags.AF = (A[i] && B[i]) || (A[i] && flags.AF) || (B[i] && flags.AF);
  //    if (allowAF) {
  //      if (i % 4 == 3) {
  //        if (forbidAF) {
  //          flags.AF = false;
  //        } else {
  //          flags.AF = flags.CF;
  //        }  //  bool sumBit;

  //  //  Register result(size);
  //  deque<bool> result;
  //  for (auto i = size - 1; i >= 0; --i) {

  //    sumBit = (carryBit ^ A[i] ^ B[i]);

  //    carryBit = (A[i] && B[i]) || (A[i] && carryBit) || (B[i] && carryBit);

  //    result.push_front(sumBit);
  //  }
  //  flags.CF = carryBit;
  //  Register reg(result, result.size(), SIGN_MAGNITUDE_REPR);

  //  flags.AF = 0;
  //  return reg;
  //  //  return Register(result, result.size(), SIGN_MAGNITUDE_REPR);

  //    bool carryBit = flags.CF;

  //        flags.CF = false;
  //        sumBit = (flags.AF ^ A[i] ^ B[i]);

  //        flags.AF = (A[i] && B[i]) || (A[i] && flags.AF) || (B[i] &&
  //        flags.AF);
  //      }
  //    } else {
  //      sumBit = (flags.CF ^ A[i] ^ B[i]);

  //      flags.CF = (A[i] && B[i]) || (A[i] && flags.CF) || (B[i] && flags.CF);
  //    }

  //    result.push_front(sumBit);

  //    //      flags.CF = carryBit;
  //  }
  //  //    flags.CF = carryBit;
  //  Register reg(result, result.size(), SIGN_MAGNITUDE_REPR);

  //  flags.AF = 0;
  //  return reg;
};

Register ALU::add(Register &A, Register &B /*, uint16_t times*/) {
  initFLAGS();
  string correctingRegStr = "";
  deque<bool> correctingReg;
  correctingRegStr.reserve(size);

  uint16_t times = A.getSize() / size;
  if (times == 1) {
    Register reg = adc(A, B);

    flags.SF = reg.getMSB();
    return reg;
  }
  uint16_t szReg = A.getSize();
  Multiplexer mulA(A, szReg, size);

  //  Log(INFO) << "Multiplexer B info";
  Multiplexer mulB(B, szReg, size);

  Demultiplexer demux(szReg, size);
  Register regC(szReg, A.getRepresentation());
  //  if (noAdderLog) {
  if (!noAdderLog) {
    mulA.printLogData("regA");
    mulB.printLogData("regB");
    demux.printLogData("demuxer");
  }
  //    bool flag = false;

  size_t strobeSignal = 0;
  for (size_t i = 0; i < times; ++i) {
    strobeSignal = times - i - 1;

    Register tmpA = mulA.mux(strobeSignal);

    Register tmpB = mulB.mux(strobeSignal);

    //      Adder adder(tmpA, tmpB, sizeAdder);
    Register sum = adc(tmpA, tmpB);

    if (!noAdderLog) {
      Log(INFO) << "Multiplexer mulA succefully return part no " << (i + 1)
                << " of register regA.";
      tmpA.printShortLogData("regA" + to_string(i + 1));
      Log(INFO) << "Multiplexer mulB succefully return part no " << (i + 1)
                << " of register regB.";
      tmpB.printShortLogData("regB" + to_string(i + 1));
      Log(INFO) << "The result of adding: ";
      sum.printShortLogData("temSum" + to_string(i + 1));
    }

    //    корректировка строки

    if (allowAF) {
      correctingReg.clear();
      correctingReg.push_front(true);
      correctingReg.push_front(prevAF);
      correctingReg.push_front(!prevAF);
      correctingReg.push_front(!prevAF);

      correctingReg.push_front(true);
      correctingReg.push_front(flags.AF);
      correctingReg.push_front(!flags.AF);
      correctingReg.push_front(!flags.AF);
      //      correctingRegStr = prevAF ? "0011" : "1101" + correctingRegStr;
      //      correctingRegStr = flags.AF ? "0011" : "1101" + correctingRegStr;
      blockAF = true;
      Register corrReg(correctingReg, correctingReg.size(),
                       BCD_SIGN_MAGNITUDE_REPR);
      sum = adc(sum, corrReg);
      blockAF = false;
      Log(INFO) << "The result of adding after correction: ";
      sum.printShortLogData("corrTemSum" + to_string(i + 1));
    }
    demux.demux(regC, sum, strobeSignal);

    //    if (noAdderLog) {
    if (!noAdderLog) {
      Log(INFO) << "Demultiplexer demux succefully add part no " << (i + 1)
                << " to register regC.";
    }

    //      flag = adder.getCarryFlag();
  }
  //  regC.setSign(A.getMSB());
  flags.OF = flags.CF;
  if (!blockAF && !allowAF) {
    regC.setMSB();

  } else {
    regC.setMSB(A.getMSB() ^ B.getMSB() ^ flags.AF);
  }
  //  if (regC.getMSB() && regC.getRepresentation() == BCD_SIGN_MAGNITUDE_REPR)
  //  {
  //	  regC.setRepresentation(BCD_ONES_COMPLEMENT_REPR);
  //  }
  flags.SF = regC.getMSB();
  //  regC.setNumberRepresentation();
  return regC;
};

Register ALU::addBcd(Register &A, Register &B /*, uint16_t times*/) {
  allowAF = true;
  Register regC = add(A, B);
  allowAF = false;

  //  uint16_t correctingStringSize = size;

  //  if (flags.AF) {
  //      correctingStringSize+=4;
  //  }
  //  //  выполняем корректировку тетрад
  //  string correctingRegStr = "";
  //  //  deque<bool> tmpRegC = regC.getNumber();
  //  for (auto i = 0; i < correctingStringSize; i += 4) {
  //    //      auto j = (i+1)%4;
  //    if (!regC[i] || (!regC[i + 1] && !regC[i + 2])) {
  //      correctingRegStr += "1101";
  //    } else if (regC[i] && (regC[i + 1] || regC[i + 2])) {
  //      correctingRegStr += "0011";
  //    }
  //  }
  //  forbidAF = true;
  //  Register correctingReg(correctingRegStr, correctingRegStr.length(),
  //                         BCD_SIGN_MAGNITUDE_REPR);
  //  Register result = add(regC, correctingReg);
  //  forbidAF = false;
  return regC;
};

Register ALU::prn3(Register &A) {
  deque<bool> adj;
  for (int i = 0; i < A.getSize() / 4; i++) {
    adj.push_front(true);
    adj.push_front(true);
    adj.push_front(false);
    adj.push_front(false);
  }

  Register adjust(adj, A.getSize(), BCD_SIGN_MAGNITUDE_REPR);
  noAdderLog = true;
  blockAF = true;
  Register result = add(A, adjust);
  blockAF = false;
  noAdderLog = false;
  return result;
}

Register ALU::prn(Register &A) {
  deque<bool> adj;
  for (int i = 0; i < A.getSize() / 4; i++) {
    adj.push_front(true);
    adj.push_front(false);
    adj.push_front(true);
    adj.push_front(true);
  }

  Register adjust(adj, A.getSize(), BCD_SIGN_MAGNITUDE_REPR);
  noAdderLog = true;
  blockAF = true;
  Register result = add(A, adjust);
  blockAF = false;
  noAdderLog = false;
  return result;
}

extern inline Register ALU::sbb(Register &A, Register &B){

};

Register ALU::shl(Register &A) {
  bool tmpMSB = A[0];
  flags.CF = tmpMSB;
  //  flags.OF = tmpMSB ^ A[0];
  A.removeFront();
  A.add();
  A.setMSB();
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
  Log(INFO) << "Multiplier register is being expanded...";
  Register regAHigh(regSize, SIGN_MAGNITUDE_REPR);
  regAHigh.reserve(regSize);
  Log(INFO) << "Register regAHigh initialization...";
  Log(INFO) << "Register regAHigh was succefully initialized with zero value.";

  Log(INFO) << "Product register initialization...";
  Log(INFO) << "Product register is being expanded...";
  Register regCHigh(regSize, SIGN_MAGNITUDE_REPR);
  regCHigh.reserve(regSize);
  Log(INFO) << "Register regCHigh initialization...";
  Log(INFO) << "Register regCHigh was succefully initialized with zero value.";

  //  Log(INFO) << "Multiplexer " << mulAlias << " was succefully initialized.";
  Register regCLow(regSize, SIGN_MAGNITUDE_REPR);
  regCLow.reserve(regSize);
  Log(INFO) << "Register regCLow initialization...";
  Log(INFO) << "Register regCLow was succefully initialized with zero value.";

  //  tmpB.reserve(regSize);

  noAdderLog = true;
  for (auto i = B.getSize() - 1; i > 0; --i) {
    if (B[i]) {
      regCLow = add(A, regCLow);
      //      Log(INFO) << "The operation of adding the lower part of the
      //      register "
      //                   "regA and regC was performed.";
      //      regCLow.printShortLogData("regCLow");
      regCHigh = add(regAHigh, regCHigh);
      //      Log(INFO) << "The operation of adding the upper part of the
      //      register "
      //                   "regA and regC was performed.";
      //      regCLow.printShortLogData("regCHigh");

      Log(INFO) << "RegC\t" << regCHigh.printRegister() << ' '
                << regCLow.printRegister();
    }
    A = shl(A);
    //    Log(INFO) << "A logical shift operation to the left was performed for
    //    he "
    //                 "lower part of the register regA (regALow).";

    Log(INFO) << "A logical shift operation to the left was performed for he "
                 "register regA.";
    //    A.printShortLogData("regALow");
    bool bit = flags.CF;
    regAHigh = shl(regAHigh);

    //    Log(INFO) << "A logical shift operation to the left was performed for
    //    he "
    //                 "upper part of the register regA (regAHigh).";
    //    regAHigh.printShortLogData("regAHigh");
    regAHigh[regSize - 1] = bit;
    //    Log(INFO) << "The carry flag was saved to the least significant bit.";
    //    regAHigh.printShortLogData("regAHigh");
    Log(INFO) << "RegA\t" << regAHigh.printRegister() << ' '
              << A.printRegister();
  }
  noAdderLog = false;

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

Register ALU::div2(Register &A, Register &B) {

  bool sign = A.getMSB() ^ B.getMSB();
  uint16_t regSize = A.getSize();
  //  A.setSign(false);
  B.setSign(false);
  B.setMSB();
  A.setSign(false);
  A.setMSB();

  Log(INFO) << "Product register initialization...";
  Log(INFO) << "Product register was succefully initialized with zero value.";
  Register regC(regSize, SIGN_MAGNITUDE_REPR);
  regC.reserve(regSize);

  Register regTmp(regSize, SIGN_MAGNITUDE_REPR);
  regTmp.reserve(regSize);
  Register regBTCR(B);
  regBTCR = neg(regBTCR);
  regBTCR.setNumberRepresentation(TWOS_COMPLEMENT_REPR);

  noAdderLog = true;
  auto cnt = 0;
  //  regTmp = add(regBTCR, A);
  regTmp = sub(A, B);
  if (!regTmp.getMSB()) {
    Register emptyReg;
    return emptyReg;
  }
  //  regTmp.setMSB();
  Log(INFO) << "regA\t" << regTmp.printRegister();
  Log(INFO) << "RegC\t" << regC.printRegister();
  regC[regSize - 1] = !regTmp.getMSB();
  while (cnt < regSize - 1) {
    Log(INFO) << "Left shift regA.";
    regTmp = shl(regTmp);
    Log(INFO) << "regA\t" << regTmp.printRegister();
    //    bool bit = flags.CF;
    //    regAHigh = shl(regAHigh);

    //    regAHigh[regSize - 1] = bit;
    if (regTmp.getMSB()) {
      Log(INFO) << "regA is below 0";
      Log(INFO) << "Сorrection of the result by adding the value of register B";
      regTmp = add(B, regTmp);
    } else {

      Log(INFO) << "regA is above 0";
      Log(INFO) << "Subtraction from register A the value of register B";
      regTmp = add(regBTCR, regTmp);
      //      regTmp = sub(regTmp, B);
    }
    regTmp.setMSB();
    Log(INFO) << "regA\t" << regTmp.printRegister();
    regC = shl(regC);
    Log(INFO) << /*"A logical shift operation to the left was performed for the
                 " "register */
        "Left shift regC.";
    Log(INFO) << "Adding a new digit to the end of regC";
    regC[regSize - 1] = !regTmp.getMSB();
    //    Log(INFO) << "regA\t" << regTmp.printRegister();
    Log(INFO) << "RegC\t" << regC.printRegister();
    ++cnt;
  }
  //    for (auto i = B.getSize() - 1; i > 0; --i) {
  //      if (B[i]) {
  //        regCLow = add(A, regCLow);

  //        regCHigh = add(regAHigh, regCHigh);

  //        Log(INFO) << "RegC\t" << regCHigh.printRegister() << ' '
  //                  << regCLow.printRegister();
  //      }
  //      A = shl(A);

  //  Log(INFO) << "A logical shift operation to the left was performed for he "
  //               "register regA.";

  //  bool bit = flags.CF;
  //  regAHigh = shl(regAHigh);

  //  regAHigh[regSize - 1] = bit;

  //  Log(INFO) << "RegA\t" << regAHigh.printRegister() << ' ' <<
  //  A.printRegister();

  //  noAdderLog = false;

  //  regCHigh.setSign(sign);
  //  regCHigh.setMSB();
  //  Register result(regSize * 2, regCHigh.getRepresentation());
  //  Demultiplexer demux(regSize * 2, size);
  //  uint16_t times = regSize / size;
  //  size_t strobeSignal = times - 1;

  //  demux.demux(result, regCLow, strobeSignal);
  //  strobeSignal--;
  //  demux.demux(result, regCHigh, strobeSignal);
  regC.setSign(sign);
  regC.setMSB();

  return regC;
}

// Register ALU::sar(Register &A, Register &B){};

// Register ALU::sal(Register &A, Register &B){};

// Register ALU::scr(Register &A, Register &B){};

// Register ALU::scl(Register &A, Register &B){};
