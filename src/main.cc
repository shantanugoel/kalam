#include <cctype>
#include <cstdio>

#include "editor.h"
#include "editor_state.h"
#include "logger.h"
#include "terminal.h"

namespace kalam {

int RunMain() {
  static Terminal term;
  static EditorState editor_state;
  Editor editor(term, editor_state);
  editor.Open();

  while (1) {
    editor.RefreshScreen();
    editor.ProcessKeyPress();
  }

  return 0;
}

}  // namespace kalam

int main() { return kalam::RunMain(); }