#ifndef KALAM_SRC_TERMINAL_H_
#define KALAM_SRC_TERMINAL_H_

extern "C" {
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
}

#include <string_view>

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
      if (nread == -1 && errno != EAGAIN) Logger::Die(LOG_STRING("read"));
    }
    return c;
  }

  void Write(std::string_view string) const {
    write(STDOUT_FILENO, string.data(), string.length());
  }

  void MoveCursorToTopLeft() const { Write("\x1b[H"); }

  int GetCursorPosition(int& rows, int& cols) const {
    char buf[32];
    unsigned int i = 0;
    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;
    while (i < sizeof(buf) - 1) {
      if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
      if (buf[i] == 'R') break;
      i++;
    }
    buf[i] = '\0';
    if (buf[0] != '\x1b' || buf[1] != '[') return -1;
    if (sscanf(&buf[2], "%d;%d", &rows, &cols) != 2) return -1;
    return 0;
  }

  void ClearScreen() const { Write("\x1b[2J"); }

  int GetWindowSize(int& rows, int& cols) const {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
      if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
      return GetCursorPosition(rows, cols);
    } else {
      cols = ws.ws_col;
      rows = ws.ws_row;
      return 0;
    }
  }

 private:
  termios orig_termios_;
};

}  // namespace kalam

#endif  // KALAM_SRC_TERMINAL_H_