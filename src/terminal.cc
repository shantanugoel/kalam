#include "terminal.h"

#include "logger.h"

namespace kalam {

int Terminal::ReadKey() const {
  char c;
  int nread = 0;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) Logger::Die(LOG_STRING("read"));
  }

  // Handle escape sequences/special keys
  // TODO: Think about better ways of doing this. Currently this adds 0.1-0.2
  // seconds of delay
  if (c == '\x1b') {
    char seq[3];
    if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
    if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

    // Handle arrow keys and treat them as our internal movement keys
    // TODO: Think about whether to do this replacement in editor instead.
    if (seq[0] == '[') {
      if (seq[1] >= '0' && seq[1] <= '9') {
        if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
        if (seq[2] == '~') {
          switch (seq[1]) {
            case '1':
              return ToUnderlying(Key::kHome);
            case '3':
              return ToUnderlying(Key::kDel);
            case '4':
              return ToUnderlying(Key::kEnd);
            case '5':
              return ToUnderlying(Key::kPageUp);
            case '6':
              return ToUnderlying(Key::kPageDown);
            case '7':
              return ToUnderlying(Key::kHome);
            case '8':
              return ToUnderlying(Key::kEnd);
          }
        }
      } else {
        switch (seq[1]) {
          case 'A':
            return ToUnderlying(Key::kArrowUp);
          case 'B':
            return ToUnderlying(Key::kArrowDown);
          case 'C':
            return ToUnderlying(Key::kArrowRight);
          case 'D':
            return ToUnderlying(Key::kArrowLeft);
          case 'H':
            return ToUnderlying(Key::kHome);
          case 'F':
            return ToUnderlying(Key::kEnd);
        }
      }
    } else if (seq[0] == 'O') {
      switch (seq[1]) {
        case 'H':
          return ToUnderlying(Key::kHome);
        case 'F':
          return ToUnderlying(Key::kEnd);
      }
    }
    return '\x1b';
  } else {
    return c;
  }
}

}  // namespace kalam