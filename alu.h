#ifndef ALU_H
#define ALU_H

#include <iostream>
#include <logger2.h>
#include <register.h>

#include <deque>
using namespace std;

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

public:
  ALU(uint16_t sz) : size(sz){};

  Register summ(Register &A, Register &B, bool cFlag);
};

#endif // ALU_H
