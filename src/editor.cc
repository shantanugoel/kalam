#include "editor.h"

#include <cstdlib>

#define CTRL_KEY(c) (c & 0x1F)

namespace kalam {

void Editor::ProcessKeyPress() const {
  char c = term_.ReadKey();
  switch (c) {
    case CTRL_KEY('q'):
      term_.ClearScreen();
      std::exit(0);
  }
}

void Editor::DrawRows() const {
  // Draw tilde on every row. Don't do carriage return/line feed on last row to
  // avoid scroll.
  for (int row = 0; row < editor_state_.screen_rows_ - 1; ++row) {
    term_.Write("~\r\n");
  }
  term_.Write("~");
}

void Editor::RefreshScreen() const {
  term_.ClearScreen();
  DrawRows();
  term_.MoveCursorToTopLeft();
}

}  // namespace kalam