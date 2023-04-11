#include "register.h"

Register::Register(int64_t &num, uint16_t sz, uint16_t repr) {
  size = sz;
  representation = repr;

  MSB = signbit(num);
  if (MSB) {
    num = -num;
  }

  number = convertDecimalToBinaryString(num);

  if (representation == SIGN_MAGNITUDE_REPR) {
    number.push_front(MSB);
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

string Register::convertBinaryToDecimalString() {
  string buf;
  buf.reserve(number.size());
  for (auto iter = number.begin() + 1; iter != number.end(); ++iter) {
    buf += *iter + '0';
  }

  size_t pos{};
  long long int num = stoll(buf, &pos, 2); // символы ответа в
  buf.clear();
  buf = to_string(num);
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
  if (MSB) {
    switch (representation) {
    case SIGN_MAGNITUDE_REPR: {
      switch (type) {
      case ONES_COMPLEMENT_REPR: {
        convertToOnesComplementRepresentation();
        //        cout << representation;
        representation = type;
      } break;

      case TWOS_COMPLEMENT_REPR: {
        convertToTwosComplementRepresentation();
        representation = type;
      } break;
      default:
        Log(ERROR) << "Wrong number representation!";
      }
      break;
    }
    case ONES_COMPLEMENT_REPR: {
      convertToOnesComplementRepresentation();
      representation = SIGN_MAGNITUDE_REPR;
    } break;

    case TWOS_COMPLEMENT_REPR: {
      convertToTwosComplementRepresentation();
      representation = SIGN_MAGNITUDE_REPR;
    } break;
    default:
      Log(ERROR) << "Wrong number representation!";
    }
  }
}

bool operator<(const Register &A, const Register &B) {
  return lexicographical_compare(A.getNumber().begin() + 1, A.getNumber().end(),
                                 B.getNumber().begin() + 1,
                                 B.getNumber().end());
}
bool operator>(const Register &A, const Register &B) { return B < A; }
