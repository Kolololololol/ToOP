#include "logger2.h"

string Logger2::nowTime() {
  time_t rawtime;
  struct tm *timeinfo;
  char buffer[80];

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  //  strftime(buffer, 80, "[%a,%d-%m-%Y==%T]", timeinfo);
  strftime(buffer, 80, "%T", timeinfo);
  return string(buffer);
}
