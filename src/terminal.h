#ifndef KALAM_SRC_TERMINAL_H_
#define KALAM_SRC_TERMINAL_H_

extern "C" {
#include <termios.h>
#include <unistd.h>
}

#include "logger.h"

namespace kalam {

class Terminal {
 public:
  Terminal() {
    if (tcgetattr(STDIN_FILENO, &orig_termios_) == -1) Logger::Die("tcgetattr");
    // TODO: Bind atexit() with disabling raw mode.

    termios raw = orig_termios_;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  }

  ~Terminal() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios_) == -1)
      Logger::Die("tcsetattr");
  }

  // Remove copy/assignment
  Terminal(const Terminal&) = delete;
  Terminal& operator=(const Terminal&) = delete;

  char ReadKey() const {
    char c;
    int nread = 0;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
      if (nread == -1 && errno != EAGAIN) Logger::Die("read");
    }
    return c;
  }

  void RefreshScreen() const {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
  }

 private:
  termios orig_termios_;
};

}  // namespace kalam

#endif  // KALAM_SRC_TERMINAL_H_