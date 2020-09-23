#include "editor.h"

#define CTRL_KEY(c) (c & 0x1F)

namespace kalam {

bool Editor::ProcessKeyPress() {
  char c = term_.ReadKey();
  switch (c) {
    case CTRL_KEY('q'):
      return false;
  }

  return true;
}

}  // namespace kalam