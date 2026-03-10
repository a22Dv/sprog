#pragma once

#include <string_view>

namespace ttg {


struct Table {

};

Table create_table(std::string_view expression);
void display_table(const Table& table);

} // namespace ttg