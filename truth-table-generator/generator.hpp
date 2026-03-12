#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace ttg::detail
{

using u8 = unsigned char;
using u64 = unsigned long long;
using usize = std::size_t;

using i8 = signed char;
using i32 = int;
using isize = std::ptrdiff_t;

enum class TokenType : u8 {
    UNKNOWN = 0,

    // is_operation
    BICONDITIONAL,
    CONDITIONAL, 
    AND,
    XOR,
    OR,
    NOT_POSTFIX,
    NOT_PREFIX,

    // is_terminal && is_alphanumeric
    VARIABLE,
    CONSTANT_TRUE,
    CONSTANT_FALSE,

    // is_terminal
    SEPARATOR,

    // is_parenthesis
    OPEN_PARENTHESIS,
    CLOSED_PARENTHESIS,
};

struct Token {
    std::string data{};
    i32 vcount = -1; // Exclusive for variables.
    bool is_parenthesis : 1 = false;
    bool is_operation : 1 = false;
    bool is_terminal : 1 = false;
    bool is_alphanumeric : 1 = false;
    TokenType type : 4 = TokenType::UNKNOWN;
};

union Data {
    char character;
    i32 index;
};

struct Node {
    i32 token_id = -1;
    i32 left = -1;
    i32 right = -1;
};

struct Parsers {
    std::vector<Node> &ast;
    std::vector<detail::Token> &token_list;
    i32 &tokens_consumed;

    i32 parse_or();
    i32 parse_xor();
    i32 parse_and();
    i32 parse_not_prefix();
    i32 parse_not_postfix();
    i32 parse_atom();
};

}  // namespace ttg::detail

namespace ttg
{

class Table
{
   public:
    Table() = default;

    /// @brief Creates an internal AST based on a given boolean expression.
    /// @note Equivalent to calling Table() then set_expression().
    Table(std::string_view expression);

    /// @brief Replaces the internal AST with a new one based on the given expression.
    void set_expression(std::string_view expression);

    /// @brief Evaluates the table based on the internal AST.
    /// @note The table must have an active expression for this call to succeed.
    void evaluate_table();

    /// @brief Returns a formatted string based on the evaluated table.
    /// @note Table must have been evaluated for this call to succeed.
    std::string get_table();

   private:
    void _run_lexer(std::string_view expression);
    void _run_parser();
    void _run_evaluator();
    void _eval_recursor(detail::isize eval, detail::u64 rows);

    std::vector<detail::Token> _token_list{};
    std::vector<detail::Node> _ast{};
    std::vector<std::string> _expression_substr{};
    std::vector<std::vector<detail::i8>> _entries{};  // entries[i] = evaluations for _ast[i].
    detail::i32 _root = 0;
    detail::i32 _varcount = 0;
};

}  // namespace ttg