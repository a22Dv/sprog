#include "ast.hpp"

#include <map>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace lge
{

ASTTree get_ast(std::vector<Token> &tokens)
{
    ASTTree tree{};
    static ASTNode start = ASTNodeBiconditional{};

    ASTNodeParsing nparse{tree.nodes, tokens, 0};
    const isize root = std::visit(nparse, start);  // Start parsing at highest level.

    // Now, the number of nodes is defined
    tree.subexpressions.resize(tree.nodes.size());
    ASTNodeFormatter nformat{tree.nodes, tokens, tree.subexpressions};
    const std::string lstr = std::visit(nformat, start);

    // Each subexpression string maps to several
    // indices if duplicate subexpressions are
    // found, along with a boolean evaluation state.
    isize varcount = 0;
    std::map<std::string, std::vector<isize>> mapped_subexprs{};
    for (isize i = 0; i < tree.subexpressions.size(); ++i) {
        std::string &str = tree.subexpressions[i];
        auto it = mapped_subexprs.find(str);

        if (it == mapped_subexprs.end()) {
            // Left, Right, Token ID, Node ID.
            const auto [left, right, tid, nid] = std::visit(ASTNodeVisitor{}, tree.nodes[i]);
            if (tokens[tid].type == TokenType::VARIABLE) {
                ++varcount;
            }
            mapped_subexprs[str] = {i};
        } else {
            it->second.push_back(i);
        }
    }
    // Number of variables multiplied by number of unique subexpressions.
    const isize n_exprs = mapped_subexprs.size();
    const isize n_entries = 1 << varcount;
    tree.values.resize(n_entries * n_exprs);
    tree.columns.resize(n_exprs);

    std::vector<isize> remap(tree.nodes.size());
    std::vector<bool> evaluated(n_exprs);
    for (isize cvar = 0, idx = 0; auto &entry : mapped_subexprs) {
        ASTNode &node = tree.nodes[entry.second[0]];
        const auto [left, right, tid, nid] = std::visit(ASTNodeVisitor{}, node);
        for (auto i : entry.second) {
            remap[i] = idx;
        }
        if (tokens[tid].type != TokenType::VARIABLE) {
            ++idx;
            continue;
        }
        const isize seqlen = n_entries >> cvar;
        for (isize i = 0; i < n_entries; ++i) {
            tree.values[idx * n_entries + i] = (i & (seqlen - 1)) < (seqlen >> 1);
        }
        ++cvar;
        ++idx;
    }

    ASTNodeEvaluator eval{n_entries, tree.values, remap, evaluated};
    for (isize i = 0; i < tree.nodes.size(); ++i) {
        std::visit(eval, tree.nodes[i]);  // Variables are skipped.
    }

    // (Subexpression, Values) index in sorted order, by length,
    // then variables at leftmost sorted alphabetically.

    /// TODO: Implementation. 
    /// Iterate through each unique subexpression in the map, find its index
    /// in the subexpression string array, use said index to get the corresponding node,
    /// use remap to get the corresponding column index of said node index.
    /// Skip variables. Sort by string length then alphabetically, but before the variables
    /// are put at the beginning, sorted alphabetically.
    return tree;
}

}  // namespace lge