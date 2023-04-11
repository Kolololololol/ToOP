#ifndef LOGGER2_H
#define LOGGER2_H

#define Log(level)                                                             \
  if (level > loglevel)                                                        \
    ;                                                                          \
  else                                                                         \
    Logger2(level)
#define stringify(name) #name

#include <iostream>
#include <sstream>
using namespace std;

/* consider adding boost thread id since we'll want to know whose writting and
 * won't want to repeat it for every single call */

/* consider adding policy class to allow users to redirect logging to specific
 * files via the command line
 */

enum loglevel_e { INFO = 0, WARN, ERROR, FATAL, DEBUG };

class Logger2 {
public:
  Logger2(loglevel_e _loglevel = INFO) {
    _buffer << "-" << nowTime() << " [" << TLogLevelNames[_loglevel] << "]: "
        /*<< std::string(
            _loglevel > logDEBUG
            ? (_loglevel - logDEBUG) * 4
            : 1
            , ' ')*/
        ;
  }

  template <typename T> Logger2 &operator<<(T const &value) {
    _buffer << value;
    return *this;
  }

  ~Logger2() {
    _buffer << std::endl;
    // This is atomic according to the POSIX standard
    // http://www.gnu.org/s/libc/manual/html_node/Streams-and-Threads.html
    cerr << _buffer.str();
  }

private:
  ostringstream _buffer;
  string nowTime();
  const char *TLogLevelNames[5] = {" INFO  ", "  WARN ", " ERROR ", " FATAL ",
                                   " DEBUG "};
};

extern loglevel_e loglevel;

#endif // LOGGER2_H
