#pragma once

#include "tokens.hpp"

#include <vector>
#include <string_view>

namespace lge
{

std::vector<Token> run_lexer(std::string_view expression);


}  // namespace lge
