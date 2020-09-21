#include "terminal.h"

namespace kalam {

int RunMain() {
  Terminal term;
  char c;

  // TODO: Abstract read to Terminal class.
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q')
    ;
  return 0;
}

}  // namespace kalam

int main() { return kalam::RunMain(); }