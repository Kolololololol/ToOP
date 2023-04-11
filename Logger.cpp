#include <Logger.h>

Log::~Log() {
  //  if (messageLevel >= Log::ReportingLevel()) {
  os << std::endl;
  fprintf(stderr, "%s", os.str().c_str());
  fflush(stderr);
  //  }
}
ostringstream &Log::get(TLogLevel level) {
  os << "- " << nowTime();
  os << " " << TLogLevelNames[level] << ": ";
  //  os << string(level > logDEBUG ? 0 : level - logDEBUG, '\t');
  messageLevel = level;
  return os;
}
string Log::nowTime() {
  time_t rawtime;
  struct tm *timeinfo;
  char buffer[80];

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  //  strftime(buffer, 80, "[%a,%d-%m-%Y==%T]", timeinfo);
  strftime(buffer, 80, "[%T]", timeinfo);
  return string(buffer);
}
