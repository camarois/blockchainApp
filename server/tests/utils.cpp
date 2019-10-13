#include "utils.hpp"

namespace Tests {

std::filesystem::path getDir(std::filesystem::path suffix) {
  return std::filesystem::current_path()/"data"/suffix;
}

std::filesystem::path createEmptyDir(std::filesystem::path suffix) {
  std::filesystem::path path = getDir(suffix);
  std::filesystem::remove_all(path);
  std::filesystem::create_directories(path);

  return path;
}

} // namespace Tests
