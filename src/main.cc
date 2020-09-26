#include <cctype>
#include <cstdio>

#include "editor.h"
#include "editor_state.h"
#include "logger.h"
#include "terminal.h"

namespace kalam {

int RunMain(const char *filename) {
  static Terminal term;
  static EditorState editor_state;
  Editor editor(term, editor_state);
  editor.Open(filename);

  while (1) {
    editor.RefreshScreen();
    editor.ProcessKeyPress();
  }

  return 0;
}

}  // namespace kalam

int main(int argc, char *argv[]) {
  char *filename = nullptr;
  if (argc >= 2) filename = argv[1];
  return kalam::RunMain(filename);
}