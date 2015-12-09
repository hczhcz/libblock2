#include "parser.hpp"

#include "parser_special_node.hpp"
#include "parser_syntax.hpp"
#include "parser_visitor.hpp"

namespace libblock {

Node &parse(std::string &&code) {
    myparser::Parser<> parser;
    myparser::Pass<myparser::PASS_AST> visitor;

    myparser::Node<> *parse_tree {
        parser.parse(code, true)
    };
    parse_tree->runPass(&visitor);

    return visitor.getOne();
}

Node &parseFile(std::string &&file) {
    myparser::Parser<> parser;
    myparser::Pass<myparser::PASS_AST> visitor;

    myparser::Node<> *parse_tree {
        parser.parseFile(file, true)
    };
    parse_tree->runPass(&visitor);

    return visitor.getOne();
}

}
