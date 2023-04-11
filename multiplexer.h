#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H

#include <register.h>

#include <deque>
using namespace std;

class Multiplexer {
  Register number;
  uint16_t size;
  uint16_t adderSize;
  //  uint16_t strobeSignal;

public:
  Multiplexer();
  Multiplexer(Register &num, uint16_t sz,
              uint16_t adderSz /*, uint16_t strbSign*/)
      : number(num), size(sz), adderSize(adderSz) /*, strobeSignal(strbSign)*/ {
//    Log(INFO) << "Multiplexer initialization...";

//    Log(INFO) << "Multiplexer was succefully initialized.";
    //        Log(INFO) << "=======MULTIPLEXER ATTRIBUTES=======\n"
    //                  << setw(18) << " " << setw(20) << left << "VALUE:" <<
    //                  setw(40)
    //                  << printRegister() << "\n"
    //                  << setw(18) << " " << setw(20) << left << "SIZE:" <<
    //                  setw(40)
    //                  << size << "\n"
    //                  << setw(18) << " " << setw(20) << left
    //                  << "REPRESENTATION:" << setw(40)
    //                  << representationNames[representation];
  };
  Register mux(uint16_t strobeSignal);

  void printLogData(string regAlias){
	  Log(INFO) << "Multiplexer " << regAlias << " initialization...";

	  Log(INFO) << "Multiplexer " << regAlias << " was succefully initialized.";
  }


};


#endif // MULTIPLEXER_H
