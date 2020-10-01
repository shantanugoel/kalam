#ifndef KALAM_SRC_EDITOR_H_
#define KALAM_SRC_EDITOR_H_

#include <string_view>

#include "editor_state.h"
#include "terminal.h"

namespace kalam {

class Editor {
 public:
  Editor(Terminal& term, EditorState& editor_state)
      : term_(term), editor_state_(editor_state) {
    if (term_.GetWindowSize(editor_state_.screen_rows_,
                            editor_state_.screen_cols_) == -1) {
      Logger::Die(LOG_STRING("GetWindowSize"));
    }
  }

  void ProcessKeyPress() const;
  void RefreshScreen() const;
  void Open(const char* filename) const;

 private:
  Terminal& term_;
  EditorState& editor_state_;

  void PrepareBufferDrawRows(std::string& buffer) const;
  void Scroll() const;
  void MoveCursor(int key) const;
  void UpdateRow(EditorState::Row& row) const;
};

}  // namespace kalam

#endif  // KALAM_SRC_EDITOR_H_