
#include "lexer.hpp"
#include "tokens.hpp"

int main()
{
    static lge::TokenMap tmap = lge::TokenMap::get_instance();
    std::vector<lge::Token> tokens = lge::run_lexer("(ME&&TAKE_EXAM->FAIL)");
   
}