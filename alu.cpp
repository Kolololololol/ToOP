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
  initFLAGS();
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

    demux.demux(regC, sum, strobeSignal);
    //    if (noAdderLog) {
    if (!noAdderLog) {
      Log(INFO) << "Multiplexer mulA succefully return part no " << (i + 1)
                << " of register regA.";
      tmpA.printShortLogData("regA" + to_string(i + 1));
      Log(INFO) << "Multiplexer mulB succefully return part no " << (i + 1)
                << " of register regB.";
      tmpB.printShortLogData("regB" + to_string(i + 1));
      Log(INFO) << "The result of adding: ";
      sum.printShortLogData("temSum" + to_string(i + 1));
      //    }
      Log(INFO) << "Demultiplexer demux succefully add part no " << (i + 1)
                << " to register regC.";
    }

    //      flag = adder.getCarryFlag();
  }
  //  regC.setSign(A.getMSB());
  regC.setMSB();
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
      //      regTmp = add(regBTCR, regTmp);
      Log(INFO) << "regA is above 0";
      Log(INFO) << "Subtraction from register A the value of register B";
      regTmp = sub(regTmp, B);
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
