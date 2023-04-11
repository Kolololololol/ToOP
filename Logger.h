#ifndef LOGGER_H
#define LOGGER_H

#define LOG(level)                                                             \
  if (level > Log::ReportingLevel()) {                                         \
  } else                                                                       \
    Log().get(level)
#define stringify(name) #name

#include <iostream>
#include <sstream>
using namespace std;

enum TLogLevel { INFO = 0, WARN, ERROR, FATAL, DEBUG };

class Log {
public:
  Log();
  ~Log();
  ostringstream &get(TLogLevel level = INFO);

public:
  static TLogLevel &ReportingLevel(){};

protected:
  ostringstream os;

private:
  Log(const Log &);
  Log &operator=(const Log &);
  const char *TLogLevelNames[5] = {stringify(INFO), stringify(WARN),
                                   stringify(ERROR), stringify(FATAL),
                                   stringify(DEBUG)};

private:
  TLogLevel messageLevel;
  string nowTime();
};
// std::ostringstream& Log::Get(TLogLevel level)
//{
//   os << "- " << nowTime();
//   os << " " << ToString(level) << ": ";
////   os << std::string(level > logDEBUG ? 0 : level - logDEBUG, '\t');
//   messageLevel = level;
//   return os;
//}
// TLogLevel &Log::ReportingLevel(){};
#endif // LOGGER_H
