#include "editor.h"

#include <cstdlib>

#define CTRL_KEY(c) (c & 0x1F)

namespace kalam {

void Editor::ProcessKeyPress() {
  char c = term_.ReadKey();
  switch (c) {
    case CTRL_KEY('q'):
      std::exit(0);
  }
}

}  // namespace kalam