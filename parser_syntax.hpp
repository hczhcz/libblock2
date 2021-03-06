// generated by MyParser C++ Code Generator

#pragma once

#include "./lib/myparser/myparser.hpp"

namespace myparser {

template<>
class RuleDef<MP_STR("root")>:
public RuleList<MP_STR("root"),
    RuleLine<
        RuleItemSpace<>,
        RuleItemRef<MP_STR("stmt")>,
        RuleItemSpace<>
    >
> {};

template<>
class RuleDef<MP_STR("stmt")>:
public RuleList<MP_STR("stmt"),
    RuleLine<
        RuleItemRef<MP_STR("expr")>,
        RuleItemSpace<>,
        RuleItemKeyword<MP_STR(";")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("stmt")>
    >,
    RuleLine<
        
    >
> {};

template<>
class RuleDef<MP_STR("expr")>:
public RuleList<MP_STR("expr"),
    RuleLine<
        RuleItemRef<MP_STR("expr assign")>
    >
> {};

template<>
class RuleDef<MP_STR("expr assign")>:
public RuleList<MP_STR("expr assign"),
    RuleLine<
        RuleItemRef<MP_STR("expr logic 1")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action assign")>
    >
> {};

template<>
class RuleDef<MP_STR("action assign")>:
public RuleList<MP_STR("action assign"),
    RuleLine<
        RuleItemKeyword<MP_STR("=")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("or=")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("xor=")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("and=")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("+=")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("-=")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("|=")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("^=")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("&=")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("*=")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("/=")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("div=")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("mod=")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("shl=")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("shr=")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("ushr=")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("rol=")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("ror=")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 1")>
    >,
    RuleLine<
        
    >
> {};

template<>
class RuleDef<MP_STR("expr logic 1")>:
public RuleList<MP_STR("expr logic 1"),
    RuleLine<
        RuleItemRef<MP_STR("expr logic 2")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action logic 1")>
    >
> {};

template<>
class RuleDef<MP_STR("action logic 1")>:
public RuleList<MP_STR("action logic 1"),
    RuleLine<
        RuleItemKeyword<MP_STR("or")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 2")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action logic 1 or")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("xor")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 2")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action logic 1 xor")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("and")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 2")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action logic 1 and")>
    >,
    RuleLine<
        
    >
> {};

template<>
class RuleDef<MP_STR("action logic 1 or")>:
public RuleList<MP_STR("action logic 1 or"),
    RuleLine<
        RuleItemKeyword<MP_STR("or")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 2")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action logic 1 or")>
    >,
    RuleLine<
        
    >
> {};

template<>
class RuleDef<MP_STR("action logic 1 xor")>:
public RuleList<MP_STR("action logic 1 xor"),
    RuleLine<
        RuleItemKeyword<MP_STR("xor")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 2")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action logic 1 xor")>
    >,
    RuleLine<
        
    >
> {};

template<>
class RuleDef<MP_STR("action logic 1 and")>:
public RuleList<MP_STR("action logic 1 and"),
    RuleLine<
        RuleItemKeyword<MP_STR("and")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 2")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action logic 1 and")>
    >,
    RuleLine<
        
    >
> {};

template<>
class RuleDef<MP_STR("expr logic 2")>:
public RuleList<MP_STR("expr logic 2"),
    RuleLine<
        RuleItemKeyword<MP_STR("not")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr logic 2")>
    >,
    RuleLine<
        RuleItemRef<MP_STR("expr comp")>
    >
> {};

template<>
class RuleDef<MP_STR("expr comp")>:
public RuleList<MP_STR("expr comp"),
    RuleLine<
        RuleItemRef<MP_STR("expr math 1")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action comp")>
    >
> {};

template<>
class RuleDef<MP_STR("action comp")>:
public RuleList<MP_STR("action comp"),
    RuleLine<
        RuleItemKeyword<MP_STR("==")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("!=")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("<")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("<=")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR(">")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR(">=")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("is")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 1")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("in")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 1")>
    >,
    RuleLine<
        
    >
> {};

template<>
class RuleDef<MP_STR("expr math 1")>:
public RuleList<MP_STR("expr math 1"),
    RuleLine<
        RuleItemRef<MP_STR("expr math 2")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 1")>
    >
> {};

template<>
class RuleDef<MP_STR("action math 1")>:
public RuleList<MP_STR("action math 1"),
    RuleLine<
        RuleItemKeyword<MP_STR("+")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 2")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 1 add")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("-")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 2")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 1 add")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("|")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 2")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 1 or")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("^")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 2")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 1 xor")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("&")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 2")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 1 and")>
    >,
    RuleLine<
        
    >
> {};

template<>
class RuleDef<MP_STR("action math 1 add")>:
public RuleList<MP_STR("action math 1 add"),
    RuleLine<
        RuleItemKeyword<MP_STR("+")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 2")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 1 add")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("-")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 2")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 1 add")>
    >,
    RuleLine<
        
    >
> {};

template<>
class RuleDef<MP_STR("action math 1 or")>:
public RuleList<MP_STR("action math 1 or"),
    RuleLine<
        RuleItemKeyword<MP_STR("|")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 2")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 1 or")>
    >,
    RuleLine<
        
    >
> {};

template<>
class RuleDef<MP_STR("action math 1 xor")>:
public RuleList<MP_STR("action math 1 xor"),
    RuleLine<
        RuleItemKeyword<MP_STR("^")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 2")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 1 xor")>
    >,
    RuleLine<
        
    >
> {};

template<>
class RuleDef<MP_STR("action math 1 and")>:
public RuleList<MP_STR("action math 1 and"),
    RuleLine<
        RuleItemKeyword<MP_STR("&")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 2")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 1 and")>
    >,
    RuleLine<
        
    >
> {};

template<>
class RuleDef<MP_STR("expr math 2")>:
public RuleList<MP_STR("expr math 2"),
    RuleLine<
        RuleItemRef<MP_STR("expr math 3")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 2")>
    >
> {};

template<>
class RuleDef<MP_STR("action math 2")>:
public RuleList<MP_STR("action math 2"),
    RuleLine<
        RuleItemKeyword<MP_STR("*")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 3")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 2 mul")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("/")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 3")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 2 mul")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("div")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 3")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 2 mul")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("mod")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 3")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 2 mul")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("shl")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 3")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 2 shift")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("shr")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 3")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 2 shift")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("ushr")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 3")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 2 shift")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("rol")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 3")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 2 shift")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("ror")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 3")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 2 shift")>
    >,
    RuleLine<
        
    >
> {};

template<>
class RuleDef<MP_STR("action math 2 mul")>:
public RuleList<MP_STR("action math 2 mul"),
    RuleLine<
        RuleItemKeyword<MP_STR("*")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 3")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 2 mul")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("/")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 3")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 2 mul")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("div")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 3")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 2 mul")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("mod")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 3")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 2 mul")>
    >,
    RuleLine<
        
    >
> {};

template<>
class RuleDef<MP_STR("action math 2 shift")>:
public RuleList<MP_STR("action math 2 shift"),
    RuleLine<
        RuleItemKeyword<MP_STR("shl")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 3")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 2 shift")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("shr")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 3")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 2 shift")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("ushr")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 3")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 2 shift")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("rol")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 3")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 2 shift")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("ror")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 3")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action math 2 shift")>
    >,
    RuleLine<
        
    >
> {};

template<>
class RuleDef<MP_STR("expr math 3")>:
public RuleList<MP_STR("expr math 3"),
    RuleLine<
        RuleItemKeyword<MP_STR("+")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 3")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("-")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 3")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("~")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr math 3")>
    >,
    RuleLine<
        RuleItemRef<MP_STR("expr unary")>
    >
> {};

template<>
class RuleDef<MP_STR("expr unary")>:
public RuleList<MP_STR("expr unary"),
    RuleLine<
        RuleItemKeyword<MP_STR("(")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("expr")>,
        RuleItemSpace<>,
        RuleItemKeyword<MP_STR(")")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action unary")>
    >,
    RuleLine<
        RuleItemRef<MP_STR("literal")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action unary")>
    >,
    RuleLine<
        RuleItemRef<MP_STR("symbol")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action unary")>
    >,
    RuleLine<
        RuleItemRef<MP_STR("blocks 1")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action unary")>
    >
> {};

template<>
class RuleDef<MP_STR("action unary")>:
public RuleList<MP_STR("action unary"),
    RuleLine<
        RuleItemRef<MP_STR("path")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action unary")>
    >,
    RuleLine<
        RuleItemRef<MP_STR("call")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action unary")>
    >,
    RuleLine<
        RuleItemRef<MP_STR("blocks 2")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("action unary")>
    >,
    RuleLine<
        
    >
> {};

template<>
class RuleDef<MP_STR("literal")>:
public RuleList<MP_STR("literal"),
    RuleLine<
        RuleItemRef<MP_STR("int")>
    >,
    RuleLine<
        RuleItemRef<MP_STR("real")>
    >,
    RuleLine<
        RuleItemRef<MP_STR("str")>
    >
> {};

template<>
class RuleDef<MP_STR("int")>:
public RuleRegex<MP_STR("int"),
    MP_STR("[0-9]+(?!\\.)")
> {};

template<>
class RuleDef<MP_STR("real")>:
public RuleRegex<MP_STR("real"),
    MP_STR("[0-9]+\\.[0-9]*([Ee][+\\-]?[0-9]+)?")
> {};

template<>
class RuleDef<MP_STR("str")>:
public RuleRegex<MP_STR("str"),
    MP_STR("\"([^\"]|\\\\\")*\"|\'([^\']|\\\\\')*\'")
> {};

template<>
class RuleDef<MP_STR("symbol")>:
public RuleList<MP_STR("symbol"),
    RuleLine<
        RuleItemRef<MP_STR("id")>
    >
> {};

template<>
class RuleDef<MP_STR("path")>:
public RuleList<MP_STR("path"),
    RuleLine<
        RuleItemKeyword<MP_STR(".")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("id")>
    >
> {};

template<>
class RuleDef<MP_STR("call")>:
public RuleList<MP_STR("call"),
    RuleLine<
        RuleItemKeyword<MP_STR("(")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("args")>,
        RuleItemSpace<>,
        RuleItemKeyword<MP_STR(")")>
    >
> {};

template<>
class RuleDef<MP_STR("args")>:
public RuleList<MP_STR("args"),
    RuleLine<
        RuleItemRef<MP_STR("expr")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("args more")>
    >,
    RuleLine<
        
    >
> {};

template<>
class RuleDef<MP_STR("args more")>:
public RuleList<MP_STR("args more"),
    RuleLine<
        RuleItemKeyword<MP_STR(",")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("args")>
    >,
    RuleLine<
        
    >
> {};

template<>
class RuleDef<MP_STR("blocks 1")>:
public RuleList<MP_STR("blocks 1"),
    RuleLine<
        RuleItemKeyword<MP_STR("\\")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("block")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("blocks more")>
    >
> {};

template<>
class RuleDef<MP_STR("blocks 2")>:
public RuleList<MP_STR("blocks 2"),
    RuleLine<
        RuleItemKeyword<MP_STR("\\\\")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("block")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("blocks more")>
    >
> {};

template<>
class RuleDef<MP_STR("blocks more")>:
public RuleList<MP_STR("blocks more"),
    RuleLine<
        RuleItemKeyword<MP_STR("\\")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("block")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("blocks more")>
    >,
    RuleLine<
        
    >
> {};

template<>
class RuleDef<MP_STR("block")>:
public RuleList<MP_STR("block"),
    RuleLine<
        RuleItemRef<MP_STR("params")>,
        RuleItemSpace<>,
        RuleItemKeyword<MP_STR("{")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("stmt")>,
        RuleItemSpace<>,
        RuleItemKeyword<MP_STR("}")>
    >
> {};

template<>
class RuleDef<MP_STR("params")>:
public RuleList<MP_STR("params"),
    RuleLine<
        RuleItemRef<MP_STR("param")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("params more")>
    >,
    RuleLine<
        
    >
> {};

template<>
class RuleDef<MP_STR("params more")>:
public RuleList<MP_STR("params more"),
    RuleLine<
        RuleItemKeyword<MP_STR(",")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("params")>
    >,
    RuleLine<
        
    >
> {};

template<>
class RuleDef<MP_STR("param")>:
public RuleList<MP_STR("param"),
    RuleLine<
        RuleItemKeyword<MP_STR("in")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("id")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("out")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("id")>
    >,
    RuleLine<
        RuleItemKeyword<MP_STR("var")>,
        RuleItemSpace<>,
        RuleItemRef<MP_STR("id")>
    >,
    RuleLine<
        RuleItemRef<MP_STR("id")>
    >
> {};

template<>
class RuleDef<MP_STR("space")>:
public RuleList<MP_STR("space"),
    RuleLine<
        RuleItemRef<MP_STR("ignored")>
    >
> {};

template<>
class RuleDef<MP_STR("keyword")>:
public RuleList<MP_STR("keyword"),
    RuleLine<
        RuleItemRef<MP_STR("sign")>
    >
> {};

template<>
class RuleDef<MP_STR("id")>:
public RuleRegex<MP_STR("id"),
    MP_STR("[A-Za-z_][A-Za-z0-9_]*")
> {};

template<>
class RuleDef<MP_STR("sign")>:
public RuleRegex<MP_STR("sign"),
    MP_STR("[+\\-*/%|^&~=!<>]=?|[A-Za-z_][A-Za-z0-9_]*=?|\\\\\\\\?|[(){},.;]")
> {};

template<>
class RuleDef<MP_STR("ignored")>:
public RuleRegex<MP_STR("ignored"),
    MP_STR("(\\/\\/[^\\n]*|[ \\t\\r\\n])*")
> {};

}
