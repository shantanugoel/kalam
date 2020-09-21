#ifndef KALAM_SRC_TERMINAL_H_
#define KALAM_SRC_TERMINAL_H_

extern "C" {
#include <termios.h>
#include <unistd.h>
}

namespace kalam {

class Terminal {
 public:
  Terminal() {
    tcgetattr(STDIN_FILENO, &raw_termios_);
    raw_termios_.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_termios_);
  }

  ~Terminal() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_termios_); }

 private:
  termios raw_termios_;
};

}  // namespace kalam

#endif  // KALAM_SRC_TERMINAL_H_