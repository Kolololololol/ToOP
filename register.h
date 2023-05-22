#ifndef REGISTER_H
#define REGISTER_H

#define SIGN_MAGNITUDE_REPR 0x00
#define ONES_COMPLEMENT_REPR 0x01
#define TWOS_COMPLEMENT_REPR 0x02
#define BCD_SIGN_MAGNITUDE_REPR 0x03
#define BCD_ONES_COMPLEMENT_REPR 0x04

#define stringify(name) #name

#include <algorithm>
#include <bitset>
#include <cmath>
#include <deque>
#include <iomanip>
#include <logger2.h>
#include <stdint.h>
#include <string>
using namespace std;

// loglevel_e loglevel = DEBUG;
class Register {
  deque<bool> number;
  uint16_t size;
  uint16_t representation;
  uint16_t numberType;
  bool MSB;
  string convertDecimalToBinaryString(long long int n);
  string convertNegativeDecimalToBinaryString(long long int n);
  deque<bool> convertDecimalToBinaryString(string str);
  deque<bool> convertDecimalToBinaryString(int64_t &num);
  deque<bool> convertDecimalToBinaryBCD3String(int64_t &num);

  void convertToOnesComplementRepresentation();
  void convertToTwosComplementRepresentation();

  const char *representationNames[5] = {
      "sign magnitude", "one`s complement", "two`s complement",
      "binary coded decimal", "binary coded decimal one`s complement"};

public:
  Register(){};
  Register(uint16_t sz, uint16_t repr)
      : size(sz), representation(repr){
                      //    Log(INFO) << "Register initialization...";
                  };
  Register(uint16_t sz) : size(sz) {}
  Register(deque<bool> &num, uint16_t sz, uint16_t repr)
      : number(num), size(sz),
        representation(repr){
            //    Log(INFO) << "Register initialization...";
            //    printLogData();
        };
  Register(string &boolStr, uint16_t sz, uint16_t repr);
  Register(int64_t &num, uint16_t sz, uint16_t repr);
  Register(const Register &reg) {
    number = reg.getNumber();
    size = reg.getSize();
    representation = reg.getRepresentation();
    MSB = reg.getMSB();
  }
  //  uint16_t getRepresentation() { return representation; }
  void setNumberRepresentation(int type);
  void setNumberRepresentation();
  void setRepresentation(int type);
  string convertBinaryToDecimalString(bool isFloat = false);
  string convertBCD3ToDecimalString(uint16_t numSize);
  void printLogData(string regAlias) {
    Log(INFO) << "Register " << regAlias << " initialization...";
    Log(INFO) << "Register " << regAlias << " was succefully initialized.";
    Log(INFO) << "==========REGISTER ATTRIBUTES==========\n"
              << setw(21) << " " << setw(20) << left << "VALUE:" << setw(40)
              << printRegister() << "\n"
              << setw(21) << " " << setw(20) << left << "SIZE:" << setw(40)
              << size << "\n"
              << setw(21) << " " << setw(20) << left
              << "REPRESENTATION:" << setw(40)
              << representationNames[representation];
  };

  void printShortLogData(string regAlias) {
    Log(INFO) << "Register " << regAlias << " value: " << printRegister();
  };

  void reset() {
    for (auto iter = number.begin(); iter != number.end(); ++iter) {
      number[*iter] = false;
    }
  }
  void reset(size_t position) { number[position] = false; }

  void set() {
    for (auto iter = number.begin(); iter != number.end(); ++iter) {
      number[*iter] = true;
    }
  }
  void set(size_t position) { number[position] = true; }

  void flip() {
    for (auto iter = number.begin(); iter != number.end(); ++iter) {
      *iter = !(*iter);
    }
  }
  void flipNoMSB() {
    for (auto iter = number.begin() + 1; iter != number.end(); ++iter) {
      *iter = !(*iter);
    }
  }
  void flip(size_t position) { number[position] = !number[position]; }
  void add(bool bit = false) { number.push_back(bit); }
  void addFront(bool bit = false) { number.push_front(bit); }
  void remove() { number.pop_back(); }
  void removeFront() { number.pop_front(); }
  void reserve(int sz) {
    for (auto i = 0; i < sz; ++i) {
      number.push_back(false);
    }
  }

  string printRegister() {

    string str;
    for (auto iter = number.begin(); iter != number.end(); ++iter) {

      str += *iter + '0';
    }
    return str;
  }

  bool &operator[](size_t ind) { return number[ind]; }
  const bool &operator[](size_t ind) const { return number[ind]; }
  friend bool operator<(const Register &A, const Register &B);
  friend bool operator>(const Register &A, const Register &B);
  //  bool operator>(const Register &reg) {

  //    //    return reg<this;
  //  }

  deque<bool> getNumber() const { return number; };

  uint16_t getRepresentation() const { return representation; };

  int16_t getSize() const { return size; };
  void setMSB() {
    MSB = number[0];
    //    cout << MSB;
  }
  void setMSB(bool sign) {
    MSB = sign;
    //    cout << MSB;
  }

  void setSign(bool sign) {
    number[0] = sign;
    //    cout << MSB;
  }

  bool getMSB() const { return MSB; }

  bool isEmpty() { return number.empty(); }
};

#endif // REGISTER_H
