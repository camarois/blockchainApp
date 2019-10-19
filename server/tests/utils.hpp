#ifndef TESTS_UTILS_HPP
#define TESTS_UTILS_HPP

#include <filesystem>

namespace Tests {

std::filesystem::path getDir(std::filesystem::path suffix);
std::filesystem::path createEmptyDir(std::filesystem::path suffix);
bool checkFileSHA256Hash(const std::filesystem::path& file, const std::string& hash);

} // namespace Tests

#endif // TESTS_UTILS_HPP
