#ifndef KALAM_SRC_EDITOR_H_
#define KALAM_SRC_EDITOR_H_

#include <cstdlib>

#include "terminal.h"

namespace kalam {

class Editor {
 public:
  Editor(Terminal& term) : term_(term) {}

  void ProcessKeyPress();

 private:
  Terminal& term_;
};

}  // namespace kalam
#endif  // KALAM_SRC_EDITOR_H_