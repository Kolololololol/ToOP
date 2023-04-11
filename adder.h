#ifndef ADDER_H
#define ADDER_H

#include <iostream>
#include <logger2.h>
#include <register.h>

#include <deque>
using namespace std;

// loglevel_e loglevel = DEBUG;
class Adder {
  //  Register firstNum;
  //  Register secondNum;
  uint16_t size;
  bool carryFlag;

public:
  Adder(/*Register &A, Register &B, */ uint16_t sz /*, bool cFlag*/)
      : /*firstNum(A), secondNum(B), */ size(sz) /*, carryFlag(cFlag)*/ {
            //    Log(INFO) << "Adder initialization...";
        };

  Register summ(Register &A, Register &B, bool cFlag);
  Register summ(Register &A, bool oFlag, bool cFlag);
  bool getCarryFlag() { return carryFlag; };
};

#endif // ADDER_H
