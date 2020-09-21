#include <cctype>
#include <cstdio>

#include "logger.h"
#include "terminal.h"

namespace kalam {

int RunMain() {
  Terminal term;

  // TODO: Abstract read to Terminal class.
  while (1) {
    char c = '\0';
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) {
      Logger::Die("Read");
    }
    if (std::iscntrl(c)) {
      printf("%d\r\n", c);
    } else {
      printf("%d ('%c')\r\n", c, c);
    }
    if (c == 'q') break;
  }

  return 0;
}

}  // namespace kalam

int main() { return kalam::RunMain(); }