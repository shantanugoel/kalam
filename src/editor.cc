#include "editor.h"

#include <cstdlib>
#include <string>

#define CTRL_KEY(c) (c & 0x1F)

namespace kalam {

const std::string kKalamVersion = "0.0.1";
const std::string kWelcomeMessage = "Kalam Editor -- version " + kKalamVersion;

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
    if (row == editor_state_.screen_rows_ / 3) {
      int padding = (editor_state_.screen_cols_ - kWelcomeMessage.length()) / 2;
      if (padding) {
        buffer += "~";
        padding--;
      }
      while (padding--) buffer += " ";
      buffer += kWelcomeMessage;
    } else {
      buffer += "~";
    }
    term_.PrepareBufferClearLine(buffer);
    buffer += "\r\n";
  }
  buffer += "~";
  term_.PrepareBufferClearLine(buffer);
}

void Editor::RefreshScreen() const {
  std::string buffer = "";
  term_.PrepareBufferHideCursor(buffer);
  PrepareBufferDrawRows(buffer);
  term_.PrepareBufferMoveCursorToTopLeft(buffer);
  term_.PrepareBufferShowCursor(buffer);
  term_.Write(buffer);
}

}  // namespace kalam