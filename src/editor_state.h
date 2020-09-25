#ifndef KALAM_SRC_EDITOR_STATE_H_
#define KALAM_SRC_EDITOR_STATE_H_

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
  int screen_rows_;
  int screen_cols_;
  friend class Editor;
};

}  // namespace kalam

#endif  // KALAM_SRC_EDITOR_STATE_H_