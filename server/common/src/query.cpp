#include "common/query.hpp"
#include <cstring>

namespace Common {

std::string Query::val() const { return std::string(query_.get()); }

}  // namespace Common
