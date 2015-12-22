#pragma once

#include "exception.hpp"
#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

namespace myparser {

enum {
    PASS_AST = PASS_USER
};

template <>
class Pass<PASS_AST>: public PassProto<PASS_AST> {
private:
    std::gc_vector<std::reference_wrapper<libblock::Node>> lb_nodes;
    std::gc_vector<std::reference_wrapper<libblock::Block>> *lb_blocks_p;
    std::gc_vector<std::pair<std::string, libblock::ParamMode>> *lb_params_p;

    void putArg(const Node<> *node) {
        Pass<PASS_AST> child_pass;

        node->runPass(&child_pass);

        lb_nodes.push_back(
            child_pass.getOne()
        );
    }

    void putBlocks(
        const Node<> *node_first,
        const Node<> *node_more,
        std::gc_vector<std::reference_wrapper<libblock::Block>> &blocks
    ) {
        Pass<PASS_AST> child_pass;
        child_pass.lb_blocks_p = &blocks;

        node_first->runPass(&child_pass);
        node_more->runPass(&child_pass);
    }

    void makeCall(std::string &&name) {
        std::gc_vector<std::reference_wrapper<libblock::Node>> args {
            std::move(lb_nodes)
        };
        lb_nodes.clear();

        lb_nodes.push_back(
            *new (GC) libblock::NodeCall {
                *new (GC) libblock::NodeSymbol {
                    libblock::LookupMode::mixed,
                    std::move(name)
                },
                std::move(args)
            }
        );
    }

public:
    inline Pass() {}

    // virtual ~Pass() {}

    libblock::Node &getOne() {
        if (lb_nodes.size() != 1) {
            throw libblock::ErrorParserInternalError {};
        }

        return lb_nodes.front();
    }

    #define RUN_LIST(name, I) \
        void run(const NodeTypedList<MP_STR(name), I> *node)

    #define RUN_TEXT(name) \
        void run(const NodeTypedText<MP_STR(name)> *node)

    RUN_LIST("root", 0) {
        node->getChildren()[1]->runPass(this);
        makeCall("__do__");
    }

    RUN_LIST("stmt", 0) {
        putArg(node->getChildren()[0]);
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("stmt", 1) {
        // nothing
        (void) node;
    }

    RUN_LIST("expr", 0) {
        node->getChildren()[0]->runPass(this);
    }

    RUN_LIST("expr assign", 0) {
        node->getChildren()[0]->runPass(this);
        node->getChildren()[2]->runPass(this);
    }

    RUN_LIST("action assign", 0) {
        putArg(node->getChildren()[2]);
        makeCall("__assign__");
    }

    RUN_LIST("action assign", 1) {
        putArg(node->getChildren()[2]);
        makeCall("__assign_or__");
    }

    RUN_LIST("action assign", 2) {
        putArg(node->getChildren()[2]);
        makeCall("__assign_xor__");
    }

    RUN_LIST("action assign", 3) {
        putArg(node->getChildren()[2]);
        makeCall("__assign_and__");
    }

    RUN_LIST("action assign", 4) {
        putArg(node->getChildren()[2]);
        makeCall("__assign_add__");
    }

    RUN_LIST("action assign", 5) {
        putArg(node->getChildren()[2]);
        makeCall("__assign_sub__");
    }

    RUN_LIST("action assign", 6) {
        putArg(node->getChildren()[2]);
        makeCall("__assign_bit_or__");
    }

    RUN_LIST("action assign", 7) {
        putArg(node->getChildren()[2]);
        makeCall("__assign_bit_xor__");
    }

    RUN_LIST("action assign", 8) {
        putArg(node->getChildren()[2]);
        makeCall("__assign_bit_and__");
    }

    RUN_LIST("action assign", 9) {
        putArg(node->getChildren()[2]);
        makeCall("__assign_mul__");
    }

    RUN_LIST("action assign", 10) {
        putArg(node->getChildren()[2]);
        makeCall("__assign_div__");
    }

    RUN_LIST("action assign", 11) {
        putArg(node->getChildren()[2]);
        makeCall("__assign_int_div__");
    }

    RUN_LIST("action assign", 12) {
        putArg(node->getChildren()[2]);
        makeCall("__assign_int_mod__");
    }

    RUN_LIST("action assign", 13) {
        putArg(node->getChildren()[2]);
        makeCall("__assign_shl__");
    }

    RUN_LIST("action assign", 14) {
        putArg(node->getChildren()[2]);
        makeCall("__assign_shr__");
    }

    RUN_LIST("action assign", 15) {
        putArg(node->getChildren()[2]);
        makeCall("__assign_ushr__");
    }

    RUN_LIST("action assign", 16) {
        putArg(node->getChildren()[2]);
        makeCall("__assign_rol__");
    }

    RUN_LIST("action assign", 17) {
        putArg(node->getChildren()[2]);
        makeCall("__assign_ror__");
    }

    RUN_LIST("action assign", 18) {
        // nothing
        (void) node;
    }

    RUN_LIST("expr logic 1", 0) {
        node->getChildren()[0]->runPass(this);
        node->getChildren()[2]->runPass(this);
    }

    RUN_LIST("action logic 1", 0) {
        putArg(node->getChildren()[2]);
        makeCall("__or__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action logic 1", 1) {
        putArg(node->getChildren()[2]);
        makeCall("__xor__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action logic 1", 2) {
        putArg(node->getChildren()[2]);
        makeCall("__and__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action logic 1", 3) {
        // nothing
        (void) node;
    }

    RUN_LIST("action logic 1 or", 0) {
        putArg(node->getChildren()[2]);
        makeCall("__or__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action logic 1 or", 1) {
        // nothing
        (void) node;
    }

    RUN_LIST("action logic 1 xor", 0) {
        putArg(node->getChildren()[2]);
        makeCall("__xor__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action logic 1 xor", 1) {
        // nothing
        (void) node;
    }

    RUN_LIST("action logic 1 and", 0) {
        putArg(node->getChildren()[2]);
        makeCall("__and__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action logic 1 and", 1) {
        // nothing
        (void) node;
    }

    RUN_LIST("expr logic 2", 0) {
        node->getChildren()[2]->runPass(this);
        makeCall("__not__");
    }

    RUN_LIST("expr logic 2", 1) {
        node->getChildren()[0]->runPass(this);
    }

    RUN_LIST("expr comp", 0) {
        node->getChildren()[0]->runPass(this);
        node->getChildren()[2]->runPass(this);
    }

    RUN_LIST("action comp", 0) {
        putArg(node->getChildren()[2]);
        makeCall("__eq__");
    }

    RUN_LIST("action comp", 1) {
        putArg(node->getChildren()[2]);
        makeCall("__ne__");
    }

    RUN_LIST("action comp", 2) {
        putArg(node->getChildren()[2]);
        makeCall("__lt__");
    }

    RUN_LIST("action comp", 3) {
        putArg(node->getChildren()[2]);
        makeCall("__lte__");
    }

    RUN_LIST("action comp", 4) {
        putArg(node->getChildren()[2]);
        makeCall("__gt__");
    }

    RUN_LIST("action comp", 5) {
        putArg(node->getChildren()[2]);
        makeCall("__gte__");
    }

    RUN_LIST("action comp", 6) {
        putArg(node->getChildren()[2]);
        makeCall("__is__");
    }

    RUN_LIST("action comp", 7) {
        putArg(node->getChildren()[2]);
        makeCall("__in__");
    }

    RUN_LIST("action comp", 8) {
        // nothing
        (void) node;
    }

    RUN_LIST("expr math 1", 0) {
        node->getChildren()[0]->runPass(this);
        node->getChildren()[2]->runPass(this);
    }

    RUN_LIST("action math 1", 0) {
        putArg(node->getChildren()[2]);
        makeCall("__add__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 1", 1) {
        putArg(node->getChildren()[2]);
        makeCall("__sub__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 1", 2) {
        putArg(node->getChildren()[2]);
        makeCall("__bit_or__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 1", 3) {
        putArg(node->getChildren()[2]);
        makeCall("__bit_xor__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 1", 4) {
        putArg(node->getChildren()[2]);
        makeCall("__bit_and__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 1", 5) {
        // nothing
        (void) node;
    }

    RUN_LIST("action math 1 add", 0) {
        putArg(node->getChildren()[2]);
        makeCall("__add__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 1 add", 1) {
        putArg(node->getChildren()[2]);
        makeCall("__sub__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 1 add", 2) {
        // nothing
        (void) node;
    }

    RUN_LIST("action math 1 or", 0) {
        putArg(node->getChildren()[2]);
        makeCall("__bit_or__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 1 or", 1) {
        // nothing
        (void) node;
    }

    RUN_LIST("action math 1 xor", 0) {
        putArg(node->getChildren()[2]);
        makeCall("__bit_xor__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 1 xor", 1) {
        // nothing
        (void) node;
    }

    RUN_LIST("action math 1 and", 0) {
        putArg(node->getChildren()[2]);
        makeCall("__bit_and__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 1 and", 1) {
        // nothing
        (void) node;
    }

    RUN_LIST("expr math 2", 0) {
        node->getChildren()[0]->runPass(this);
        node->getChildren()[2]->runPass(this);
    }

    RUN_LIST("action math 2", 0) {
        putArg(node->getChildren()[2]);
        makeCall("__mul__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 2", 1) {
        putArg(node->getChildren()[2]);
        makeCall("__div__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 2", 2) {
        putArg(node->getChildren()[2]);
        makeCall("__int_div__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 2", 3) {
        putArg(node->getChildren()[2]);
        makeCall("__int_mod__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 2", 4) {
        putArg(node->getChildren()[2]);
        makeCall("__shl__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 2", 5) {
        putArg(node->getChildren()[2]);
        makeCall("__shr__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 2", 6) {
        putArg(node->getChildren()[2]);
        makeCall("__ushr__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 2", 7) {
        putArg(node->getChildren()[2]);
        makeCall("__rol__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 2", 8) {
        putArg(node->getChildren()[2]);
        makeCall("__ror__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 2", 9) {
        // nothing
        (void) node;
    }

    RUN_LIST("action math 2 mul", 0) {
        putArg(node->getChildren()[2]);
        makeCall("__mul__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 2 mul", 1) {
        putArg(node->getChildren()[2]);
        makeCall("__div__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 2 mul", 2) {
        putArg(node->getChildren()[2]);
        makeCall("__int_div__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 2 mul", 3) {
        putArg(node->getChildren()[2]);
        makeCall("__int_mod__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 2 mul", 4) {
        // nothing
        (void) node;
    }

    RUN_LIST("action math 2 shift", 0) {
        putArg(node->getChildren()[2]);
        makeCall("__shl__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 2 shift", 1) {
        putArg(node->getChildren()[2]);
        makeCall("__shr__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 2 shift", 2) {
        putArg(node->getChildren()[2]);
        makeCall("__ushr__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 2 shift", 3) {
        putArg(node->getChildren()[2]);
        makeCall("__rol__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 2 shift", 4) {
        putArg(node->getChildren()[2]);
        makeCall("__ror__");
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("action math 2 shift", 5) {
        // nothing
        (void) node;
    }

    RUN_LIST("expr math 3", 0) {
        node->getChildren()[2]->runPass(this);
        makeCall("__pos__");
    }

    RUN_LIST("expr math 3", 1) {
        node->getChildren()[2]->runPass(this);
        makeCall("__neg__");
    }

    RUN_LIST("expr math 3", 2) {
        node->getChildren()[2]->runPass(this);
        makeCall("__bit_not__");
    }

    RUN_LIST("expr math 3", 3) {
        node->getChildren()[0]->runPass(this);
    }

    RUN_LIST("expr unary", 0) {
        node->getChildren()[2]->runPass(this);
        node->getChildren()[6]->runPass(this);
    }

    RUN_LIST("expr unary", 1) {
        node->getChildren()[0]->runPass(this);
        node->getChildren()[2]->runPass(this);
    }

    RUN_LIST("expr unary", 2) {
        node->getChildren()[0]->runPass(this);
        node->getChildren()[2]->runPass(this);
    }

    RUN_LIST("expr unary", 3) {
        node->getChildren()[0]->runPass(this);
        node->getChildren()[2]->runPass(this);
    }

    RUN_LIST("action unary", 0) {
        node->getChildren()[0]->runPass(this);
        node->getChildren()[2]->runPass(this);
    }

    RUN_LIST("action unary", 1) {
        node->getChildren()[0]->runPass(this);
        node->getChildren()[2]->runPass(this);
    }

    RUN_LIST("action unary", 2) {
        node->getChildren()[0]->runPass(this);
        node->getChildren()[2]->runPass(this);
    }

    RUN_LIST("action unary", 3) {
        // nothing
        (void) node;
    }

    RUN_LIST("literal", 0) {
        node->getChildren()[0]->runPass(this);
    }

    RUN_LIST("literal", 1) {
        node->getChildren()[0]->runPass(this);
    }

    RUN_LIST("literal", 2) {
        node->getChildren()[0]->runPass(this);
    }

    RUN_TEXT("int") {
        lb_nodes.push_back(
            *new (GC) libblock::NodeLiteralInt {
                node->getData()
            }
        );
    }

    RUN_TEXT("real") {
        lb_nodes.push_back(
            *new (GC) libblock::NodeLiteralReal {
                node->getData()
            }
        );
    }

    RUN_TEXT("str") {
        lb_nodes.push_back(
            *new (GC) libblock::NodeLiteralStr {
                node->getRaw()
            }
        );
    }

    RUN_LIST("symbol", 0) {
        lb_nodes.push_back(
            *new (GC) libblock::NodeSymbol {
                libblock::LookupMode::mixed,
                node->getChildren()[0]->getFullText()
            }
        );
    }

    RUN_LIST("path", 0) {
        libblock::Node &lb_source {
            getOne()
        };
        lb_nodes.clear();

        lb_nodes.push_back(
            *new (GC) libblock::NodePath {
                lb_source,
                libblock::LookupMode::local,
                node->getChildren()[2]->getFullText()
            }
        );
    }

    RUN_LIST("call", 0) {
        // callee

        libblock::Node &lb_source {
            getOne()
        };
        lb_nodes.clear();

        // args

        node->getChildren()[2]->runPass(this);

        std::gc_vector<std::reference_wrapper<libblock::Node>> args {
            std::move(lb_nodes)
        };
        lb_nodes.clear();

        // the ast node

        lb_nodes.push_back(
            *new (GC) libblock::NodeCall {
                lb_source,
                std::move(args)
            }
        );
    }

    RUN_LIST("args", 0) {
        putArg(node->getChildren()[0]);
        node->getChildren()[2]->runPass(this);
    }

    RUN_LIST("args", 1) {
        // nothing
        (void) node;
    }

    RUN_LIST("args more", 0) {
        node->getChildren()[2]->runPass(this);
    }

    RUN_LIST("args more", 1) {
        // nothing
        (void) node;
    }

    RUN_LIST("blocks 1", 0) {
        // self as closure

        if (!lb_nodes.empty()) {
            throw libblock::ErrorParserInternalError {};
        }
        libblock::Node &lb_source {
            *new (GC) libblock::NodeSymbol {
                libblock::LookupMode::local,
                "self"
            }
        };

        // blocks

        std::gc_vector<std::reference_wrapper<libblock::Block>> blocks;

        putBlocks(
            node->getChildren()[2],
            node->getChildren()[4],
            blocks
        );

        // the ast node

        lb_nodes.push_back(
            *new (GC) libblock::NodeBlock {
                lb_source,
                std::move(blocks)
            }
        );
    }

    RUN_LIST("blocks 2", 0) {
        // a specified closure

        libblock::Node &lb_source {
            getOne()
        };
        lb_nodes.clear();

        // blocks

        std::gc_vector<std::reference_wrapper<libblock::Block>> blocks;

        putBlocks(
            node->getChildren()[2],
            node->getChildren()[4],
            blocks
        );

        // the ast node

        lb_nodes.push_back(
            *new (GC) libblock::NodeBlock {
                lb_source,
                std::move(blocks)
            }
        );
    }

    RUN_LIST("blocks more", 0) {
        node->getChildren()[2]->runPass(this);
        node->getChildren()[4]->runPass(this);
    }

    RUN_LIST("blocks more", 1) {
        // nothing
        (void) node;
    }

    RUN_LIST("block", 0) {
        // params

        std::gc_vector<std::pair<std::string, libblock::ParamMode>> params;
        lb_params_p = &params;

        node->getChildren()[0]->runPass(this);

        // body

        node->getChildren()[4]->runPass(this);
        makeCall("__do__");

        libblock::Node &lb_ast {
            getOne()
        };
        lb_nodes.clear();

        // block

        lb_blocks_p->push_back(
            *new (GC) libblock::BlockUser {
                {
                    libblock::BlockOption::allow_proc,
                    libblock::BlockOption::allow_out,
                    libblock::BlockOption::allow_in
                },
                std::move(params),
                lb_ast
            }
        );
    }

    RUN_LIST("params", 0) {
        node->getChildren()[0]->runPass(this);
        node->getChildren()[2]->runPass(this);
    }

    RUN_LIST("params", 1) {
        // nothing
        (void) node;
    }

    RUN_LIST("params more", 0) {
        node->getChildren()[2]->runPass(this);
    }

    RUN_LIST("params more", 1) {
        // nothing
        (void) node;
    }

    RUN_LIST("param", 0) {
        lb_params_p->push_back({
            node->getChildren()[2]->getFullText(),
            libblock::ParamMode::in
        });
    }

    RUN_LIST("param", 1) {
        lb_params_p->push_back({
            node->getChildren()[2]->getFullText(),
            libblock::ParamMode::out
        });
    }

    RUN_LIST("param", 2) {
        lb_params_p->push_back({
            node->getChildren()[2]->getFullText(),
            libblock::ParamMode::var
        });
    }

    RUN_LIST("param", 3) {
        lb_params_p->push_back({
            node->getChildren()[0]->getFullText(),
            libblock::ParamMode::in
        });
    }

    RUN_LIST("space", 0) {
        // never reach
        (void) node;

        throw libblock::ErrorParserInternalError {};
    }

    RUN_LIST("keyword", 0) {
        // never reach
        (void) node;

        throw libblock::ErrorParserInternalError {};
    }

    RUN_TEXT("id") {
        // never reach
        (void) node;

        throw libblock::ErrorParserInternalError {};
    }

    RUN_TEXT("sign") {
        // never reach
        (void) node;

        throw libblock::ErrorParserInternalError {};
    }

    RUN_TEXT("ignored") {
        // never reach
        (void) node;

        throw libblock::ErrorParserInternalError {};
    }

    #undef RUN_LIST
    #undef RUN_TEXT

    // ignored nodes

    template <class N, class E>
    void run(const NodeTyped<N, NodeError<E>> *node) {
        // never reach if no parsing error
        (void) node;

        throw libblock::ErrorParserFailed {};
    }
};

}
