#include <QCommandLineParser>
#include <QCoreApplication>
#include <QRegularExpression>

#include <adder.h>
#include <demultiplexer.h>
#include <logger2.h>
#include <multiplexer.h>
#include <register.h>

#include <QDebug>
#include <cmath>
#include <ctime>
#include <iostream>
#include <limits>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#define ERROR_RETURN                                                           \
  {                                                                            \
    Log(FATAL) << "Program finished with exit code 1";                         \
    return 1;                                                                  \
  }

#define OK_RETURN                                                              \
  {                                                                            \
    Log(INFO) << "Program finished successfully.";                             \
    Log(INFO) << "Program finished with exit code 0";                          \
    return 0;                                                                  \
  }

#define stringify(name) #name

using namespace std;
int16_t sizeReg = 0, sizeAdder = 0;
char operSign;
string str1, str2;
int64_t num1, num2;
string getUserInputAsUInt32(string input, int64_t min, int64_t max);

bool numberValidation(string input, int64_t min, int64_t max, int64_t *num);
int addingSignMagnitude(/*string str1, string str2*/ int64_t num1,
                        int64_t num2);
int addingOnesComlement(/*string str1, string str2*/ int64_t num1,
                        int64_t num2);

loglevel_e loglevel = DEBUG;
int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  QCommandLineParser parser;
  parser.setApplicationDescription("Helper for ToOP\n\u00A9Kolololololol 2023");

  QCoreApplication::setApplicationVersion(QString("1.0.0"));
  //  parser.addPositionalArgument(
  //      "source", QCoreApplication::translate("main", "Source file to
  //      copy."));
  //  parser.addPositionalArgument(
  //      "destination",
  //      QCoreApplication::translate("main", "Destination directory."));

  QCommandLineOption optAdderSize(
      QStringList() << "a"
                    << "adder-size",
      QCoreApplication::translate("main", "Set the size of adder."),
      QCoreApplication::translate("main", "size"));
  parser.addOption(optAdderSize);

  //  parser.addHelpOption();
  const QCommandLineOption optHelp = parser.addHelpOption();

  QString ocNums = QCoreApplication::translate("main", "numbers");
  QCommandLineOption optAddingOnesComplementRepr(
      QStringList() << "n"
                    << "add-ones-compl",
      QCoreApplication::translate(
          "main", "One`s complement addition and substraction."),
      ocNums);
  parser.addOption(optAddingOnesComplementRepr);

  QCommandLineOption optOperation(
      QStringList() << "o"
                    << "operation",
      QCoreApplication::translate("main", "Set an operation on two numbers."),
      QCoreApplication::translate("main", "oper"));
  parser.addOption(optOperation);

  QCommandLineOption optRegisterSize(
      QStringList() << "r"
                    << "register-size",
      QCoreApplication::translate("main", "Set the size of register."),
      QCoreApplication::translate("main", "size"));
  parser.addOption(optRegisterSize);

  QString smNums = QCoreApplication::translate("main", "numbers");
  QCommandLineOption optAddingSignMagnitudeRepr(
      QStringList() << "s"
                    << "add-sign-magn",
      QCoreApplication::translate("main",
                                  "Sign magnitude addition and substraction."),
      smNums);
  parser.addOption(optAddingSignMagnitudeRepr);

  //  parser.addVersionOption();
  const QCommandLineOption optVersion = parser.addVersionOption();

  //  parser.process(a);
  //  const QStringList args = parser.positionalArguments();
  if (!parser.parse(QCoreApplication::arguments())) {
    Log(ERROR) << parser.errorText().toStdString();
    Log(INFO) << "Try '" << argv[0]
              << " -h' to get help on command line parameters.";
    ERROR_RETURN
  }

  //  parser.process(a);

  //  const QStringList args = parser.positionalArguments();

  if (parser.isSet(optHelp)) {
    parser.showHelp();
  }
  if (parser.isSet(optVersion)) {
    parser.showVersion();
  }

  if (parser.isSet(optRegisterSize)) {
    Log(INFO) << "Register size initialization...";
    QString val = parser.value(optRegisterSize);
    if (val.isNull() || val == "") {
      Log(ERROR) << "Register size is not set.";

      ERROR_RETURN
    }
    sizeReg = val.toUInt();

    if (sizeReg == 0) {
      Log(ERROR) << "Invalid param at" << val.toStdString()
                 << ": argument should contain "
                    "a number as size of registers";

      ERROR_RETURN
    } else if (sizeReg > 0) {
      Log(INFO) << "Register size was succefully initialized with value "
                << sizeReg;
    }
  }

  if (parser.isSet(optAdderSize)) {

    Log(INFO) << "Adder size initialization...";
    QString val = parser.value(optAdderSize);
    if (val.isNull() || val == "") {
      Log(ERROR) << "Adder size is not set.";

      ERROR_RETURN
    }

    sizeAdder = val.toUInt();

    if (sizeAdder == 0) {
      Log(ERROR) << "Invalid param at" << val.toStdString()
                 << ": argument should contain "
                    "a number as size of registers";

      ERROR_RETURN
    } else if (sizeAdder > 0) {
      Log(INFO) << "Register size was succefully initialized with value "
                << sizeAdder;
    }
  }

  if (parser.isSet(optAddingSignMagnitudeRepr)) {
    QString tmp = parser.value(optAddingSignMagnitudeRepr);
    auto nums =
        tmp.split(QRegularExpression("[,\\s]+"), QString::SkipEmptyParts);
    //    QRegularExpression reA("/(?<=^|)-?\\d+(?=|$)/gx");
    if (nums.count() != 2) {
      Log(ERROR) << "Invalid number of params at " << tmp.toStdString()
                 << ": argument should contain "
                    "2 numbers, separated by comma";

      ERROR_RETURN
    }

    //    if (!parser.isSet(optRegisterSize)) {

    if (!parser.isSet(optOperation)) {
      Log(ERROR) << "Operation type is not set.";

      ERROR_RETURN
    }

    str1 = nums.at(0).toStdString();
    //    str1 = getUserInputAsUInt32()
    //    cout << str1 << "\n";
    str2 = nums.at(1).toStdString();
    {
      auto sz = (1 << (sizeReg - 1));
      if (numberValidation(str1, -sz, (sz - 1), &num1) &&
          numberValidation(str2, -sz, (sz - 1), &num2)) {
        //          cout << num2;
        //          cout << num1;
        if (abs(num1) < abs(num2)) {
          Log(ERROR) << "Invalid param at " << tmp.toStdString() << ": " << num1
                     << " should be greater than " << num2
                     << " in absolute value.";

          ERROR_RETURN
        }
      }
    }
    int status = addingSignMagnitude(num1, num2);
    //      int tmp = ad

    if (status == 1) {
      ERROR_RETURN
    }
    OK_RETURN

  } /*else {
    Log(ERROR) << "Invalid number of params at"
               << parser.value(optAddingSignMagnitudeRepr).toStdString()
               << ": operation argument should contain "
                  "2 numbers, separated by comma";

    ERROR_RETURN
  }*/

  if (parser.isSet(optAddingOnesComplementRepr)) {
    QString tmp = parser.value(optAddingOnesComplementRepr);
    auto nums =
        tmp.split(QRegularExpression("[,\\s]+"), QString::SkipEmptyParts);
    //    QRegularExpression reA("/(?<=^|)-?\\d+(?=|$)/gx");
    cout << tmp.toStdString();
    if (nums.count() != 2) {
      Log(ERROR) << "Invalid number of params at " << tmp.toStdString()
                 << ": argument should contain "
                    "2 numbers, separated by comma";

      ERROR_RETURN
    }

    //    if (!parser.isSet(optRegisterSize)) {

    if (!parser.isSet(optOperation)) {
      Log(ERROR) << "Operation type is not set.";

      ERROR_RETURN
    }

    str1 = nums.at(0).toStdString();
    //    str1 = getUserInputAsUInt32()
    //    cout << str1 << "\n";
    str2 = nums.at(1).toStdString();
    {
      auto sz = (1 << (sizeReg - 1));
      if (numberValidation(str1, -sz, (sz - 1), &num1) &&
          numberValidation(str2, -sz, (sz - 1), &num2)) {
        //          cout << num2;
        //          cout << num1;
        if (abs(num1) < abs(num2)) {
          Log(ERROR) << "Invalid param at " << tmp.toStdString() << ": " << num1
                     << " should be greater than " << num2
                     << " in absolute value.";

          ERROR_RETURN
        }
      }
    }
    //      cout << num1 << endl;
    //      cout << num2 << endl;
    //      int tmp = addingSignMagnitude(str1, str2);
    int status = addingOnesComlement(num1, num2);
    //      int tmp = ad

    if (status == 1) {
      ERROR_RETURN
    }
    OK_RETURN
  }

  if (parser.isSet(optOperation)) {
    operSign = static_cast<char>(parser.value(optOperation).at(0).toLatin1());
    switch (operSign) {
    case '+': {

    } break;
    case '-': {

      num2 = -num2;

    } break;
    case '*': {
    } break;
    case '/': {
    } break;
    default: {
      Log(ERROR) << "Invalid param at" << operSign
                 << ": operation argument should contain only "
                    "+ or -";

      ERROR_RETURN
    }
    }
  }

  //  if (parser.isSet(optOperation)) {
  //    operSign =
  //    static_cast<char>(parser.value(optOperation).at(0).toLatin1()); switch
  //    (operSign) { case '+': {
  //      //      cout << num1 << endl;
  //      //      cout << num2 << endl;
  //      //      int tmp = addingSignMagnitude(str1, str2);
  //      int tmp = addingSignMagnitude(num1, num2);
  //      //      int tmp = ad

  //      if (tmp == 1) {
  //        ERROR_RETURN
  //      }
  //      OK_RETURN
  //    } break;
  //    case '-': {
  //      //      if (str2[0] == '-') {
  //      //        str2.erase(0, 1);
  //      //      } else {
  //      //        str2.reserve(str2.length() + 1);
  //      //        str2.insert(0, "-");
  //      //      }
  //      num2 = -num2;
  //      //      cout << str2 << "\n";
  //      //      str2[0] = '-';
  //      //      int tmp = addingSignMagnitude(str1, str2);
  //      int tmp = addingSignMagnitude(num1, num2);

  //      if (tmp == 1) {
  //        ERROR_RETURN
  //      }
  //      OK_RETURN
  //    } break;
  //    case '*': {
  //    } break;
  //    case '/': {
  //    } break;
  //    default: {
  //      Log(ERROR) << "Invalid param at" << operSign
  //                 << ": operation argument should contain only "
  //                    "+ or -";

  //      ERROR_RETURN
  //    }
  //    }
  //  }

  return a.exec();
}

int addingSignMagnitude(/*string str1, string str2*/ int64_t num1,
                        int64_t num2) {

  //  Log(INFO) << "Register A info";
  Register regA(num1, sizeReg, SIGN_MAGNITUDE_REPR);
  regA.setNumberRepresentation(TWOS_COMPLEMENT_REPR);
  regA.printLogData(stringify(regA));

  //  Log(INFO) << "Register B info";
  Register regB(num2, sizeReg, SIGN_MAGNITUDE_REPR);
  regB.setNumberRepresentation(TWOS_COMPLEMENT_REPR);
  regB.printLogData(stringify(regB));
  //  int loopCnt = ;
  //  Multiplexer mul;
  //  Log(INFO) << "Multiplexer A info";
  Multiplexer mulA(regA, sizeReg, sizeAdder);
  mulA.printLogData(stringify(mulA));
  //  Log(INFO) << "Multiplexer B info";
  Multiplexer mulB(regB, sizeReg, sizeAdder);
  mulB.printLogData(stringify(mulB));

  //  regA<regB
  //  uint16_t repr =
  //      (regA < regB) ? regB.getRepresentation() : regA.getRepresentation();

  Adder adder(sizeAdder);
  Demultiplexer demux(sizeReg, sizeAdder);
  Register regC(sizeReg, regA.getRepresentation());

  {
    bool flag = false;
    size_t tmp = sizeReg / sizeAdder;
    size_t strobeSignal = 0;
    for (size_t i = 0; i < tmp; ++i) {
      strobeSignal = tmp - i - 1;

      Register tmpA = mulA.mux(strobeSignal);
      tmpA.printShortLogData("regA" + to_string(i + 1));

      Register tmpB = mulB.mux(strobeSignal);
      tmpB.printShortLogData("regB" + to_string(i + 1));

      //      Adder adder(tmpA, tmpB, sizeAdder);
      Register sum = adder.summ(tmpA, tmpB, flag);
      sum.printShortLogData("temSum" + to_string(i + 1));
      demux.demux(regC, sum, strobeSignal);
      flag = adder.getCarryFlag();
    }
  }
  //  Log(INFO) << "Register C info";
  regC.setMSB();
  //  cout << regC.getRepresentation();
  //  regC.printLogData("regC");
  regC.setNumberRepresentation(-1);
  regC.printLogData(string("regC"));
  Log(INFO) << "Result of operation is: "
            << regC.convertBinaryToDecimalString();
  //  Register regC = demux.getDemuxResult();
}

int addingOnesComlement(/*string str1, string str2*/ int64_t num1,
                        int64_t num2) {

  Register regA(num1, sizeReg, SIGN_MAGNITUDE_REPR);
  regA.setNumberRepresentation(ONES_COMPLEMENT_REPR);
  regA.printLogData(stringify(regA));

  Register regB(num2, sizeReg, SIGN_MAGNITUDE_REPR);
  regB.setNumberRepresentation(ONES_COMPLEMENT_REPR);
  regB.printLogData(stringify(regB));

  Multiplexer mulA(regA, sizeReg, sizeAdder);
  mulA.printLogData(stringify(mulA));

  Multiplexer mulB(regB, sizeReg, sizeAdder);
  mulB.printLogData(stringify(mulB));

  Adder adder(sizeAdder);
  Demultiplexer demux(sizeReg, sizeAdder);
  Register regC(sizeReg, regA.getRepresentation());

  {
    bool cFlag = false, oFlag = false;
    size_t tmp = sizeReg / sizeAdder;
    size_t strobeSignal = 0;
    for (size_t i = 0; i < tmp; ++i) {
      strobeSignal = tmp - i - 1;

      Register tmpA = mulA.mux(strobeSignal);
      tmpA.printShortLogData("regA" + to_string(i + 1));

      Register tmpB = mulB.mux(strobeSignal);
      tmpB.printShortLogData("regB" + to_string(i + 1));

      //      Adder adder(tmpA, tmpB, sizeAdder);
      Register sum = adder.summ(tmpA, tmpB, cFlag);
      sum.printShortLogData("temSum" + to_string(i + 1));
      demux.demux(regC, sum, strobeSignal);
      cFlag = adder.getCarryFlag();
    }

    if (cFlag) {
      Log(INFO) << "A sign overflow has occurred...";
      Log(INFO) << "The result is corrected by adding 1 to the least "
                   "significant digit.";
      Multiplexer mulC(regC, sizeReg, sizeAdder);
      mulB.printLogData(stringify(mulC));
      regC = Register(sizeReg, regA.getRepresentation());
      strobeSignal = 0;
      oFlag = cFlag;
      cFlag = false;
      for (size_t i = 0; i < tmp; ++i) {
        strobeSignal = tmp - i - 1;

        Register tmpC = mulC.mux(strobeSignal);
        tmpC.printShortLogData("regC" + to_string(i + 1));

        Register sum = adder.summ(tmpC, oFlag, cFlag);
        oFlag = false;
        sum.printShortLogData("temSum" + to_string(i + 1));
        demux.demux(regC, sum, strobeSignal);
        //        regC.printShortLogData("regC" + to_string(i + 1));
        cFlag = adder.getCarryFlag();
      }
    }
  }

  //  Log(INFO) << "Register C info";
  regC.setMSB();
  //  cout << regC.getRepresentation();
  //  regC.printLogData("regC");
  regC.setNumberRepresentation(-1);
  regC.printLogData(string("regC"));
  Log(INFO) << "Result of operation is: "
            << regC.convertBinaryToDecimalString();
  //  Register regC = demux.getDemuxResult();
}

string getUserInputAsUInt32(string input, int64_t min, int64_t max) {
  //    assert(min < max);
  //  cout << input << "\n";
  string result = "";
  //  uint32_t number = 0u;
  while (true) {

    //    ;
    //    getline(cin, input);
    size_t length = input.length();
    if (length == 0) {
      Log(ERROR) << "Input data is empty!";
      //      cout << input;
      return "";
    }
    if (input[0] == '-') {
      length--;
    }
    const int64_t digitsCount =
        count_if(input.begin(), input.end(),
                 [](unsigned char ch) { return isdigit(ch); });
    //    cout << input << "\n";
    if (static_cast<size_t>(digitsCount) == length) {
      const int64_t /*temp*/ number = stoll(input);
      /*      if (temp <= numeric_limits<int64_t>::max()) {
              number = static_cast<int64_t>(temp)*/
      ;
      if (number >= min && number <= max) {
        result = input;
        //        cout << input << "\n";
        break;
      } else {
        Log(ERROR) << "Input number does not meet data size requirements!";
        return "";
      }
      //      }
    } else {
      Log(ERROR) << "Input data is NaN!";
      return "";
    }
  }
  return result;
}

bool numberValidation(string input, int64_t min, int64_t max, int64_t *num) {
  //    assert(min < max);
  //  cout << input << "\n";
  //  string result = "";
  //  int64_t result = ;
  //  uint32_t number = 0u;
  while (true) {

    //    ;
    //    getline(cin, input);
    size_t length = input.length();
    if (length == 0) {
      Log(ERROR) << "Input data is empty!";
      //      cout << input;
      return false;
    }
    if (input[0] == '-') {
      length--;
    }

    const int64_t digitsCount =
        count_if(input.begin(), input.end(),
                 [](unsigned char ch) { return isdigit(ch); });
    //    cout << input << "\n";
    if (static_cast<size_t>(digitsCount) == length) {
      const int64_t /*temp*/ number = stoll(input);
      //      cout << number;
      /*      if (temp <= numeric_limits<int64_t>::max()) {
              number = static_cast<int64_t>(temp)*/
      ;
      if (number >= min && number <= max) {
        *num = number;
        //        cout << num;
        return true;
        //        cout << input << "\n";
        //        break;
      } else {
        Log(ERROR) << "Input number does not meet data size requirements!";
        return false;
      }
      //      }
    } else {
      Log(ERROR) << "Input data is NaN!";
      return false;
    }
  }
  //  return number;
}
