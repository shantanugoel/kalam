#include <cctype>
#include <cstdio>

#include "editor.h"
#include "logger.h"
#include "terminal.h"

namespace kalam {

int RunMain() {
  Terminal term;
  Editor editor(term);

  bool result = true;
  do {
    result = editor.ProcessKeyPress();
  } while (result);

  return 0;
}

}  // namespace kalam

int main() { return kalam::RunMain(); }