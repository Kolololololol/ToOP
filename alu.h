#ifndef ALU_H
#define ALU_H

#include <demultiplexer.h>
#include <iostream>
#include <logger2.h>
#include <multiplexer.h>
#include <register.h>

#include <deque>
using namespace std;
// extern bool noAdderLog;
// noAdderLog = true
;
class ALU {

  struct FLAGS {
    unsigned int CF : 1;
    unsigned int PF : 1;
    unsigned int AF : 1;
    unsigned int ZF : 1;
    unsigned int SF : 1;
    unsigned int OF : 1;
  };

  uint16_t size;
  FLAGS flags;
  bool noAdderLog = false;
  bool allowAF = false;
  bool blockAF = false;
  bool prevAF = false;

public:
  ALU(uint16_t sz) : size(sz) { initFLAGS(); };

  //  arithmetic operatopn implementation
  //  Register add(Register &A, Register &B);
  Register adc(Register &A, Register &B);
  Register add(Register &A, Register &B /*, uint16_t times*/);
  Register addBcd(Register &A, Register &B);
  Register prn3(Register &A);
  Register prn(Register &A);
  Register sub(Register &A, Register &B);
  Register sbb(Register &A, Register &B);
  Register inc(Register &A);
  Register inc(Register &A, uint16_t times);
  Register dec(Register &A);
  Register dec(Register &A, uint16_t times);
  Register neg(Register &A);
  Register sar(Register &A);
  Register sar(Register &A, uint16_t pos);
  Register sal(Register &A);
  Register sal(Register &A, uint16_t pos);

  Register mul2(Register &A, Register &B);
  Register div2(Register &A, Register &B);
  //  Register scr(Register &A, Register &B);
  //  Register scl(Register &A, Register &B);

  //  logic operatopn implementation
  Register shr(Register &A);
  Register shr(Register &A, uint16_t pos);
  Register shl(Register &A);
  Register shl(Register &A, uint16_t pos);

  bool getCF() { return flags.CF; }
  bool getOF() { return flags.OF; }

  //  Register or(Register &A, Register &B);
  //  Register and(Register &A, Register &B);
  //  Register xor(Register &A, Register &B);

private:
  void initFLAGS() {

    flags.CF = 0;
    flags.PF = 0;
    flags.AF = 0;
    flags.ZF = 0;
    flags.SF = 0;
    flags.OF = 0;
  }
};

// extern inline Register ALU::add(Register &A, Register &B) {
//  flags.CF = 0;
//  return adc(A, B);
//};

extern inline Register ALU::sub(Register &A, Register &B) {
  Register notB = B;
  notB = neg(notB);
  //    B = neg(B);
  notB.setNumberRepresentation(TWOS_COMPLEMENT_REPR);
  return add(A, notB);
};

extern inline Register ALU::inc(Register &A) {
  initFLAGS();
  if (A.getRepresentation() == BCD_SIGN_MAGNITUDE_REPR ||
      A.getRepresentation() == BCD_ONES_COMPLEMENT_REPR) {
    deque<bool> additionOne;
    additionOne.push_front(false);
    additionOne.push_front(false);
    additionOne.push_front(true);
    additionOne.push_front(false);
    for (auto i = 0; i < (A.getSize()) / 4 - 1; ++i) {
      additionOne.push_front(true);
      additionOne.push_front(true);
      additionOne.push_front(false);
      additionOne.push_front(false);
    }
    Register tmpReg(additionOne, A.getSize(), BCD_SIGN_MAGNITUDE_REPR);

    allowAF = true;
    Register result = add(A, tmpReg);
    allowAF = false;
    return result;
  } else {
    Register tmpReg(A.getSize(), SIGN_MAGNITUDE_REPR);
    tmpReg.reserve(A.getSize());
    tmpReg[A.getSize() - 1] = true;
    return add(A, tmpReg);
  }
};

extern inline Register ALU::neg(Register &A) {
  uint16_t repr = A.getRepresentation();
  A.setNumberRepresentation(-1);
  A.flip(0);
  A.setMSB();
  if (!(repr == A.getRepresentation())) {
    A.setNumberRepresentation(repr);
  }
  return A;
};

#endif // ALU_H
