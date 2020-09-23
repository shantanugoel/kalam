#include "editor.h"

#include <cstdlib>

#define CTRL_KEY(c) (c & 0x1F)

namespace kalam {

void Editor::ProcessKeyPress() const {
  char c = term_.ReadKey();
  switch (c) {
    case CTRL_KEY('q'):
      term_.RefreshScreen();
      std::exit(0);
  }
}

}  // namespace kalam