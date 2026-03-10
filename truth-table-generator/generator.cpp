// #define _TTG_CATCH_EXCEPTION_AT_MAIN

#include <cstdlib>
#include <iostream>

#ifdef _TTG_CATCH_EXCEPTION_AT_MAIN
#include <exception>
#endif

#include "generator.hpp"

namespace ttg {}

int main(int argc, char **argv) {
  using namespace ttg;
  if (argc != 2) {
    std::cerr << "Usage: ./generator <BOOLEAN EXPRESSION>";
    return EXIT_FAILURE;
  }
#ifdef _TTG_CATCH_EXCEPTION_AT_MAIN
  try {
#endif

    Table table = create_table(argv[1]);
    display_table(table);
    return EXIT_SUCCESS;

#ifdef _TTG_CATCH_EXCEPTION_AT_MAIN
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
#endif
}
