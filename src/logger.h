#ifndef KALAM_SRC_LOGGING_H_
#define KALAM_SRC_LOGGING_H_

#include <cerrno>
#include <cstdio>
#include <cstdlib>

#define STRINGIFY(X) #X
#define TOSTRING(X) STRINGIFY(X)
#define LOG_STRING(s) (__FILE__ ":" TOSTRING(__LINE__) ":" s)

namespace kalam {
class Logger {
 public:
  Logger() = default;
  ~Logger() = default;

  static void Die(const char* s) {
    // Clear Screen
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    perror(s);
    exit(1);
  }

 private:
};
}  // namespace kalam

#endif  // KALAM_SRC_LOGGING_H_