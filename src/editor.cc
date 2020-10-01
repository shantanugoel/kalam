#include "editor.h"

#include <cstdlib>
#include <fstream>
#include <string>
#include <string_view>

#include "terminal.h"
#include "utils.h"

#define CTRL_KEY(c) (c & 0x1F)

namespace kalam {

const std::string kKalamVersion = "0.0.1";
const std::string kWelcomeMessage = "Kalam Editor -- version " + kKalamVersion;

void Editor::MoveCursor(int key) const {
  switch (key) {
    case ToUnderlying(Key::kArrowLeft):
      if (editor_state_.cx_ != 0) editor_state_.cx_--;
      break;

    case ToUnderlying(Key::kArrowRight):
      // If the cursor is not at the bottom of file (i.e. past the last line),
      // and not at the end of current line, i.e. past the characters in the
      // line, then we can allow cursor to move right.
      if ((editor_state_.cy_ < editor_state_.rows_.size()) &&
          (editor_state_.cx_ < editor_state_.rows_[editor_state_.cy_].size())) {
        editor_state_.cx_++;
      }
      break;

    case ToUnderlying(Key::kArrowUp):
      if (editor_state_.cy_ != 0) editor_state_.cy_--;
      break;

    case ToUnderlying(Key::kArrowDown):
      // Don't scroll past end of file.
      if (editor_state_.cy_ < editor_state_.rows_.size()) editor_state_.cy_++;
      break;
  }

  // Snap cursor to 0 if it's at the bottom of the file, i.e. past the last
  // line.
  if (editor_state_.cy_ >= editor_state_.rows_.size()) {
    editor_state_.cx_ = 0;
  } else if (editor_state_.cx_ >
             editor_state_.rows_[editor_state_.cy_].size()) {
    // Otherwise snap cursor to end of line if it is past the length of the
    // current line.
    editor_state_.cx_ = editor_state_.rows_[editor_state_.cy_].size();
  }
}

void Editor::ProcessKeyPress() const {
  int c = term_.ReadKey();
  switch (c) {
    case CTRL_KEY('q'): {
      std::string buffer;
      term_.PrepareBufferClearScreen(buffer);
      term_.Write(buffer);
      std::exit(0);
    } break;

    case ToUnderlying(Key::kHome):
      editor_state_.cx_ = 0;
      break;

    case ToUnderlying(Key::kEnd):
      editor_state_.cx_ = editor_state_.screen_cols_ - 1;
      break;

    case ToUnderlying(Key::kPageUp):
    case ToUnderlying(Key::kPageDown): {
      int times = editor_state_.screen_rows_;
      // TODO: Improve perf by calculating once directly and moving.
      // But this may be better for scrolling.
      while (times--) {
        MoveCursor(c == ToUnderlying(Key::kPageUp)
                       ? ToUnderlying(Key::kPageUp)
                       : ToUnderlying(Key::kPageDown));
      }
    } break;

    case ToUnderlying(Key::kArrowLeft):
    case ToUnderlying(Key::kArrowRight):
    case ToUnderlying(Key::kArrowUp):
    case ToUnderlying(Key::kArrowDown):
      MoveCursor(c);
      break;
  }
}

void Editor::PrepareBufferDrawRows(std::string& buffer) const {
  // Draw tilde on every row. Don't do carriage return/line feed on last row to
  // avoid scroll.
  for (size_t row = 0; row < editor_state_.screen_rows_ - 1; ++row) {
    size_t file_row = row + editor_state_.row_offset_;
    if (file_row >= editor_state_.rows_.size()) {
      // Print welcome message only if there's not content being displayed.
      if (editor_state_.rows_.size() == 0 &&
          row == editor_state_.screen_rows_ / 3) {
        int padding =
            (editor_state_.screen_cols_ - kWelcomeMessage.length()) / 2;
        if (padding) {
          buffer += "~";
          padding--;
        }
        while (padding--) buffer += " ";
        buffer += kWelcomeMessage;
      } else {
        buffer += "~";
      }
    } else {
      size_t len =
          editor_state_.rows_[file_row].size() - editor_state_.col_offset_;
      if (len < 0) len = 0;
      if (len > editor_state_.screen_cols_) len = editor_state_.screen_cols_;
      std::string_view temp = std::string_view(
          editor_state_.rows_[file_row].data() + editor_state_.col_offset_,
          len);
      buffer += temp;
    }
    term_.PrepareBufferClearLine(buffer);
    buffer += "\r\n";
  }
  buffer += "~";
  term_.PrepareBufferClearLine(buffer);
}

void Editor::Scroll() const {
  if (editor_state_.cy_ < editor_state_.row_offset_) {
    editor_state_.row_offset_ = editor_state_.cy_;
  }
  if (editor_state_.cy_ >=
      editor_state_.row_offset_ + editor_state_.screen_rows_) {
    editor_state_.row_offset_ =
        editor_state_.cy_ - editor_state_.screen_rows_ + 1;
  }
  if (editor_state_.cx_ < editor_state_.col_offset_) {
    editor_state_.col_offset_ = editor_state_.cx_;
  }
  if (editor_state_.cx_ >=
      editor_state_.col_offset_ + editor_state_.screen_cols_) {
    editor_state_.col_offset_ =
        editor_state_.cx_ - editor_state_.screen_cols_ + 1;
  }
}

void Editor::RefreshScreen() const {
  std::string buffer = "";

  Scroll();

  term_.PrepareBufferHideCursor(buffer);
  term_.PrepareBufferClearScreen(buffer);
  term_.PrepareBufferResetCursor(buffer);

  PrepareBufferDrawRows(buffer);

  term_.PrepareBufferMoveCursorToYX(
      buffer, (editor_state_.cy_ - editor_state_.row_offset_) + 1,
      (editor_state_.cx_ - editor_state_.col_offset_) + 1);
  term_.PrepareBufferShowCursor(buffer);
  term_.Write(buffer);
}

void Editor::Open(const char* filename) const {
  std::fstream file;
  // TODO: set current working directory to open local files
  file.open(filename);
  if (!file.is_open()) Logger::Die(filename);

  std::string line;
  while (std::getline(file, line)) {
    editor_state_.rows_.emplace_back(line);
  }
}

}  // namespace kalam