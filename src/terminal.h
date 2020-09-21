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
    tcgetattr(STDIN_FILENO, &orig_termios_);
    // TODO: Bind atexit() with disabling raw mode.

    termios raw = orig_termios_;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  }

  ~Terminal() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios_); }

  // Remove copy/assignment
  Terminal(const Terminal&) = delete;
  Terminal& operator=(const Terminal&) = delete;

 private:
  termios orig_termios_;
};

}  // namespace kalam

#endif  // KALAM_SRC_TERMINAL_H_