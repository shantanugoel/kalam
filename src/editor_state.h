#ifndef KALAM_SRC_EDITOR_STATE_H_
#define KALAM_SRC_EDITOR_STATE_H_

#include <string>
#include <vector>

namespace kalam {

// Forward declaration for making it a friend without adding a dependency to it.
class Editor;

// TODO: Potentially make singleton.
class EditorState {
 public:
  EditorState() = default;
  ~EditorState() = default;

  EditorState(const EditorState&) = delete;
  EditorState& operator=(const EditorState&) = delete;

 private:
  // Cursor State;
  size_t cx_ = 0;
  size_t cy_ = 0;

  // Screen State
  size_t screen_rows_;
  size_t screen_cols_;

  // Offset/Row at the top of the screen.
  size_t row_offset_ = 0;
  // Offset/col at the left of screen.
  size_t col_offset_ = 0;

  // Text data
  std::vector<std::string> rows_ = {};

  friend class Editor;
};

}  // namespace kalam

#endif  // KALAM_SRC_EDITOR_STATE_H_