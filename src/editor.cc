#include "editor.h"

#define CTRL_KEY(c) (c & 0x1F)

namespace kalam {

void Editor::ProcessKeyPress() {
  char c = term_.ReadKey();
  switch (c) {
    case CTRL_KEY('q'):
      exit(0);
  }
}

}  // namespace kalam