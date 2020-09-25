#include "editor.h"

#include <cstdlib>
#include <string>

#define CTRL_KEY(c) (c & 0x1F)

namespace kalam {

void Editor::ProcessKeyPress() const {
  char c = term_.ReadKey();
  switch (c) {
    case CTRL_KEY('q'):
      std::string buffer;
      term_.PrepareBufferClearScreen(buffer);
      term_.Write(buffer);
      std::exit(0);
  }
}

void Editor::PrepareBufferDrawRows(std::string& buffer) const {
  // Draw tilde on every row. Don't do carriage return/line feed on last row to
  // avoid scroll.
  for (int row = 0; row < editor_state_.screen_rows_ - 1; ++row) {
    buffer += "~\r\n";
  }
  buffer += "~";
}

void Editor::RefreshScreen() const {
  std::string buffer = "";
  term_.PrepareBufferHideCursor(buffer);
  term_.PrepareBufferClearScreen(buffer);
  PrepareBufferDrawRows(buffer);
  term_.PrepareBufferMoveCursorToTopLeft(buffer);
  term_.PrepareBufferShowCursor(buffer);
  term_.Write(buffer);
}

}  // namespace kalam