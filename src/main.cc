#include <cctype>
#include <cstdio>

#include "editor.h"
#include "logger.h"
#include "terminal.h"

namespace kalam {

int RunMain() {
  static Terminal term;
  Editor editor(term);

  while (1) {
    term.RefreshScreen();
    editor.ProcessKeyPress();
  }

  return 0;
}

}  // namespace kalam

int main() { return kalam::RunMain(); }