#include "register.h"

Register::Register(int64_t &num, uint16_t sz, uint16_t repr) {
  size = sz;
  representation = repr;

  MSB = signbit(num);
  if (MSB) {
    num = -num;
  }

  if (representation == BCD_SIGN_MAGNITUDE_REPR ||
      representation == BCD_ONES_COMPLEMENT_REPR) {
    number = convertDecimalToBinaryBCD3String(num);
    //	if (representation == BCD_SIGN_MAGNITUDE_REPR) {
    //    number.push_front(MSB);
    //	  number.push_front(false);
    //	  number.push_front(false);
    //	  number.push_front(false);
    //    }

  } else {

    number = convertDecimalToBinaryString(num);

    if (representation == SIGN_MAGNITUDE_REPR) {
      number.push_front(MSB);
    }
  }
}

Register::Register(string &boolStr, uint16_t sz, uint16_t repr) {
  //  Log(INFO) << "Register initialization...";
  size = sz;
  representation = repr;
  //  numberType = type;
  MSB = /*((boolStr[0] == '1') || */ (boolStr[0] == '-') /*)*/;
  if (MSB) {
    boolStr.erase(0, 1);
  }

  //  {
  //    deque<bool> result;

  //    //    switch (numberType) {

  //    //    case NUMERIC_STRING: {
  //    size_t sz = 0;
  //    //      auto tmpBuf = stoull(boolStr, &sz, 10);
  //    boolStr = convertDecimalToBinaryString(stoll(boolStr, &sz, 10));
  //    //    }

  //    //    case BOOL_STRING: {

  //    for (auto &ch : boolStr) {
  //      result.push_back(ch == '1');
  //    }
  //    while (result.size() < size - 1) {
  //      result.push_front(false);
  //    }
  //    //    } break;
  //    //    }
  //    number = result;
  //  }

  number = convertDecimalToBinaryString(boolStr);

  if (representation == SIGN_MAGNITUDE_REPR) {
    number.push_front(MSB);
  }
  //  if ((!MSB) && (representation == SIGN_MAGNITUDE_REPR)) {
  //    number.push_front(false);

  //  } else if (MSB && (representation == SIGN_MAGNITUDE_REPR)) {
  //    number.push_front(true);
  //    convertToOnesComplementRepresentation();
  //    convertToTwosComplementRepresentation();
  //  }

  //  number = result;
  //  printLogData();
}

// string Register::convertDecimalToBinaryString(long long int n) {
//  string buffer; // символы ответа в обратном порядке
//  // выделим память заранее по максимуму
//  buffer.reserve(size);
//  size_t cnt = 0;
//  do {
//    buffer += char('0' + n % 2); // добавляем в конец
//    n >>= 1;
//    cnt++;
//  } while (n > 0);
//  //  do {
//  //    buffer += '0';
//  //  } while (cnt > size);
//  //  buffer += '0';
//  return string(buffer.crbegin(),
//                buffer.crend()); // разворачиваем результат
//}

deque<bool> Register::convertDecimalToBinaryString(string str) {

  size_t pos{};
  long long int num = stoll(str, &pos, 10); // символы ответа в
  deque<bool> result;
  size_t cnt = 0;
  do {
    result.push_front(num % 2 == 1);
    num >>= 1;
    cnt++;
  } while (num > 0);
  while (result.size() < size - 1) {
    result.push_front(false);
  }
  return result;
}

string Register::convertBinaryToDecimalString(bool isFloat) {
  string buf;
  buf.reserve(number.size());

  if (!isFloat) {
    long long int num;
    for (auto iter = number.begin() + 1; iter != number.end(); ++iter) {
      buf += *iter + '0';
    }
    size_t pos{};
    num = stoll(buf, &pos, 2); // символы ответа в
    buf.clear();
    buf = to_string(num);
  } else {
    double num = 0.0;
    //    for (int const &i : number) {
    //      if (number[i]) {
    //        num += pow(2, -i);
    //      }
    //    }
    for (uint16_t i = 1; i < number.size(); ++i) {
      if (number[i]) {
        num += pow(2, -i);
      }
    }
    buf.clear();
    buf = to_string(num);

    buf.replace(1, 1, ",");
  }

  if (number[0]) {
    buf.reserve(buf.length() + 1);
    buf.insert(0, "-");
  }
  //  deque<bool> result;
  //  size_t cnt = 0;
  //  do {
  //    result.push_front(num % 2 == 1);
  //    num >>= 1;
  //    cnt++;
  //  } while (num > 0);
  //  return result;

  return buf;
}

string Register::convertBCD3ToDecimalString(uint16_t numSize) {
  string buf = "";
  buf.reserve(number.size());

  long long int num = 0;
  auto iter = number.cend() - 1;
  //  auto cnt = 3;

  uint16_t sz;
  do {
    for (auto cnt = 0; cnt < 4; ++cnt) {
      num += (*iter--) << cnt;
    }
    //    iter += 7;

    buf = to_string(num) + buf;
    num = 0;
  } while (++sz < numSize);

  //  for (auto i = 3; i < size; i += 7) {
  //    num += number[i] * (1 << (i % 4));
  //  }

  //  for (auto iter = number.begin() + 1; iter != number.end(); ++iter) {
  //    buf += *iter + '0';
  //  }
  //  size_t pos{};
  //  num = stoll(buf, &pos, 2); // символы ответа в
  //  buf.clear();
  //  buf = to_string(num);

  if (MSB) {
    buf.reserve(buf.length() + 1);
    buf.insert(0, "-");
  }
  return buf;
}

deque<bool> Register::convertDecimalToBinaryString(int64_t &num) {
  deque<bool> result;
  int64_t tmpNum = num;
  size_t cnt = 0;
  do {
    result.push_front(tmpNum % 2 == 1);
    tmpNum >>= 1;
    cnt++;
  } while (tmpNum > 0);
  while (result.size() < size - 1) {
    result.push_front(false);
  }
  return result;
}

deque<bool> Register::convertDecimalToBinaryBCD3String(int64_t &num) {
  deque<bool> result;
  int64_t tmpNum = num;
  int8_t digit;
  //  auto s = std::to_string(tmpNum);
  //  s.assign(s.rbegin(), s.rend());

  //  for (; ; ) {

  //  }
  do {
    digit = tmpNum % 10;
    do {
      result.push_front(digit % 2 == 1);
      digit >>= 1;
    } while (digit > 0);
    while (result.size() % 4 != 0) {
      result.push_front(false);
    }
    tmpNum /= 10;
  } while (tmpNum > 0);
  //  if (size >> 3 != 0) {
  //    size += 4;
  //  }
  size_t sz = result.size();
  while (sz < size - 1) {
    result.push_front(false);
    result.push_front(false);
    result.push_front(false);
    result.push_front(false);
    sz += 4;
  }

  //  do {
  //    digit = tmpNum % 10;
  //    do {
  //      result.push_back(digit % 2 == 1);
  //      digit >>= 1;
  //    } while (digit > 0);
  //    do {
  //      result.push_back(false);
  //    } while (result.size() % 4 != 0);
  //    tmpNum /= 10;
  //  } while (tmpNum > 0);
  //  if (size >> 3 != 0) {
  //    size += 4;
  //  }
  //  size_t sz = result.size();
  //  while (sz < size - 1) {
  //    result.push_front(true);
  //    result.push_front(true);
  //    result.push_front(false);
  //    result.push_front(false);
  //    sz += 4;
  //  }
  return result;
}

// string Register::convertNegativeDecimalToBinaryString(long long int n) {
//  //  if (n == numeric_limits<short>::min())
//  //    return "-1" + string(numeric_limits<short>::digits, '0');
//  string buffer;
//  buffer.reserve(size + 1); // +1 для минуса
//                            //  bool negative = true;
//                            //  if (negative)
//  n = -n;
//  size_t cnt = 0;
//  do {
//    buffer += char('0' + n % 2);
//    n >>= 1;
//    cnt++;
//  } while (n > 0);
//  do {
//    buffer += '0';
//  } while (cnt > size - 1);
//  //  if (negative)
//  buffer += '1';
//  return std::string(buffer.crbegin(), buffer.crend());
//}
inline void Register::convertToOnesComplementRepresentation() {
  flipNoMSB();
  representation = ONES_COMPLEMENT_REPR;
}

inline void Register::convertToTwosComplementRepresentation() {
  //  auto i = size - 1;
  //  while (number[i]) {
  //    i--;
  //  }
  //  for (auto k = i; k >= 0; k--) {
  //    flip(k);
  //  }
  flipNoMSB();
  bool carryFlag = 1, summ = 0;
  for (auto iter = number.rbegin(); iter != number.rend(); ++iter) {
    //      *iter = !(*iter);
    summ = carryFlag ^ *iter;
    carryFlag &= *iter;
    *iter = summ;
  }
  representation = TWOS_COMPLEMENT_REPR;
}
void Register::setNumberRepresentation(int type) {
  //  if ((representation == ONES_COMPLEMENT_REPR) && MSB) {
  //    convertToOnesComplementRepresentation();
  //    representation = SIGN_MAGNITUDE_REPR;
  //  } else if ((representation == TWOS_COMPLEMENT_REPR) && MSB) {
  //    convertToTwosComplementRepresentation();
  //    representation = SIGN_MAGNITUDE_REPR;
  //  } else if ((representation == SIGN_MAGNITUDE_REPR) && MSB) {
  //    if (type == ONES_COMPLEMENT_REPR) {
  //      convertToOnesComplementRepresentation();
  //      representation = type;
  //    } else if (type == TWOS_COMPLEMENT_REPR) {
  //      convertToTwosComplementRepresentation();
  //      representation = type;
  //    } else {
  //      Log(ERROR) << "";
  //    }
  //  } else {
  //  }
  //  if (MSB) {
  //    switch (representation) {
  //    case SIGN_MAGNITUDE_REPR: {
  //      switch (type) {
  //      case ONES_COMPLEMENT_REPR: {
  //        convertToOnesComplementRepresentation();
  //        //        cout << representation;
  //        representation = type;
  //      } break;

  //      case TWOS_COMPLEMENT_REPR: {
  //        convertToTwosComplementRepresentation();
  //        representation = type;
  //      } break;
  //      default:
  //        Log(ERROR) << "Wrong number representation!";
  //      }
  //      break;
  //    }
  //    case ONES_COMPLEMENT_REPR: {
  //      convertToOnesComplementRepresentation();
  //      representation = SIGN_MAGNITUDE_REPR;
  //    } break;

  //    case TWOS_COMPLEMENT_REPR: {
  //      convertToTwosComplementRepresentation();
  //      representation = SIGN_MAGNITUDE_REPR;
  //    } break;
  //    default:
  //      Log(ERROR) << "Wrong number representation!";
  //    }
  //  }

  //  if (MSB && representation == SIGN_MAGNITUDE_REPR) {
  //    switch (type) {
  //    case ONES_COMPLEMENT_REPR: {
  //      convertToOnesComplementRepresentation();
  //      //        cout << representation;
  //      representation = type;
  //    } break;

  //    case TWOS_COMPLEMENT_REPR: {
  //      convertToTwosComplementRepresentation();
  //      representation = type;
  //    } break;
  //    default:
  //      Log(ERROR) << "Wrong number representation!";
  //    }
  //  }

  if (MSB) {
    if (representation == SIGN_MAGNITUDE_REPR) {
      switch (type) {
      case ONES_COMPLEMENT_REPR: {
        convertToOnesComplementRepresentation();
        //        cout << representation;
        //        representation = type;
      } break;

      case TWOS_COMPLEMENT_REPR: {
        convertToTwosComplementRepresentation();
        //        representation = type;
      } break;
      default:
        Log(ERROR) << "Wrong number representation!";
      }
    } else if (representation == BCD_SIGN_MAGNITUDE_REPR &&
               type == BCD_ONES_COMPLEMENT_REPR) {
      flip();
      representation = type;
    } else {
      Log(ERROR) << "Wrong number representation!";
    }
  }
}

// переводим в ПК
void Register::setNumberRepresentation() {
  if (MSB) {
    switch (representation) {
    case ONES_COMPLEMENT_REPR: {
      convertToOnesComplementRepresentation();
      representation = SIGN_MAGNITUDE_REPR;
    } break;

    case TWOS_COMPLEMENT_REPR: {
      convertToTwosComplementRepresentation();
      representation = SIGN_MAGNITUDE_REPR;
    } break;
    case BCD_ONES_COMPLEMENT_REPR: {
      flip();
      representation = BCD_SIGN_MAGNITUDE_REPR;
    } break;
    default:
      Log(ERROR) << "Wrong number representation!";
    }
  }
}

void Register::setRepresentation(int type) { representation = type; }

bool operator<(const Register &A, const Register &B) {
  return lexicographical_compare(A.getNumber().begin() + 1, A.getNumber().end(),
                                 B.getNumber().begin() + 1,
                                 B.getNumber().end());
}
bool operator>(const Register &A, const Register &B) { return B < A; }
