#ifndef KALAM_SRC_TERMINAL_H_
#define KALAM_SRC_TERMINAL_H_

extern "C" {
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
}

#include <string>
#include <string_view>

#include "logger.h"
#include "utils.h"

namespace kalam {

// TODO: Move this to a better module. Should not be in terminal
enum class Key : int {
  kArrowLeft = 1000,
  kArrowRight,
  kArrowUp,
  kArrowDown,
  kDel,
  kHome,
  kEnd,
  kPageUp,
  kPageDown,
};

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

  int ReadKey() const;

  void Write(std::string_view string) const {
    write(STDOUT_FILENO, string.data(), string.length());
  }

  void PrepareBufferResetCursor(std::string& buffer) const {
    buffer += "\x1b[H";
  }

  void PrepareBufferMoveCursorToYX(std::string& buffer, size_t y,
                                   size_t x) const {
    buffer += "\x1b[" + std::to_string(y) + ";" + std::to_string(x) + "H";
  }

  void PrepareBufferHideCursor(std::string& buffer) const {
    buffer += "\x1b[?25l";
  }

  void PrepareBufferShowCursor(std::string& buffer) const {
    buffer += "\x1b[?25h";
  }

  void PrepareBufferClearScreen(std::string& buffer) const {
    buffer += "\x1b[2J";
  }

  void PrepareBufferClearLine(std::string& buffer) const { buffer += "\x1b[K"; }

  int GetCursorPosition(size_t& rows, size_t& cols) const {
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
    if (sscanf(&buf[2], "%zu;%zu", &rows, &cols) != 2) return -1;
    return 0;
  }

  int GetWindowSize(size_t& rows, size_t& cols) const {
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