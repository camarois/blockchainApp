#include "common/query.hpp"

#include <cstring>

namespace Common {

std::string Query::val() const { return *query_; }

}  // namespace Common
