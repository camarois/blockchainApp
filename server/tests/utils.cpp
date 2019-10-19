#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "utils.hpp"

namespace Tests {

std::filesystem::path getDir(const std::filesystem::path& suffix) { return std::filesystem::current_path() / "data" / suffix; }

std::filesystem::path createEmptyDir(const std::filesystem::path& suffix) {
  std::filesystem::path path = getDir(suffix);
  std::filesystem::remove_all(path);
  std::filesystem::create_directories(path);

  return path;
}

bool checkFileSHA256Hash(const std::filesystem::path& file, const std::string& hash) {
  const int kReadEnd = 0;
  const int kWriteEnd = 1;

  int resultOut[2];
  if (pipe(resultOut) < 0) {
    return false;
  }

  pid_t pid = fork();
  if (pid < 0) {
    return false;
  }

  char result[64];
  if (pid) {
    if (wait(NULL) < 0) {
      return false;
    }

    if (read(resultOut[kReadEnd], result, 64) != 64) {
      return false;
    }

    if (close(resultOut[kReadEnd]) < 0) {
      return false;
    }

    if (close(resultOut[kWriteEnd]) < 0) {
      return false;
    }
  } else {
    if (dup2(resultOut[kWriteEnd], STDOUT_FILENO) < 0) {
      exit(1);
    }

    if (close(resultOut[kReadEnd]) < 0) {
      exit(1);
    }

    if (close(resultOut[kWriteEnd]) < 0) {
      exit(1);
    }

    if (execlp("sha256sum", "sha256sum", file.c_str(), nullptr) < 0) {
      exit(1);
    }
  }

  return std::string(result, 64) == hash;
}

}  // namespace Tests
