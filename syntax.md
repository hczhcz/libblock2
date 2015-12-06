Syntax Definition
===

Root
---

**root**:

    <> <stmt> <>

Statements
---

stmt:

    <expr> ; <stmt>
    <>

Expressions
---

expr:

    <expr assign>

expr assign:

    <expr logic 1> <action assign>

action assign:

    = <expr logic 1>
    or= <expr logic 1>
    xor= <expr logic 1>
    and= <expr logic 1>
    += <expr logic 1>
    -= <expr logic 1>
    |= <expr logic 1>
    ^= <expr logic 1>
    &= <expr logic 1>
    *= <expr logic 1>
    /= <expr logic 1>
    div= <expr logic 1>
    mod= <expr logic 1>
    shl= <expr logic 1>
    shr= <expr logic 1>
    ushr= <expr logic 1>
    rol= <expr logic 1>
    ror= <expr logic 1>
    <>

expr logic 1:

    <expr logic 2> <action logic 1>

action logic 1:

    or <expr logic 2> <action logic 1 or>
    xor <expr logic 2> <action logic 1 xor>
    and <expr logic 2> <action logic 1 and>
    <>

action logic 1 or:

    or <expr logic 2> <action logic 1 or>
    <>

action logic 1 xor:

    xor <expr logic 2> <action logic 1 xor>
    <>

action logic 1 and:

    and <expr logic 2> <action logic 1 and>
    <>

expr logic 2:

    not <expr logic 2>
    <expr comp>

expr comp:

    <expr math 1> <action comp>

action comp:

    == <expr math 1>
    != <expr math 1>
    \< <expr math 1>
    \<= <expr math 1>
    \> <expr math 1>
    \>= <expr math 1>
    is <expr math 1>
    in <expr math 1>
    <>

expr math 1:

    <expr math 2> <action math 1>

action math 1:

    + <expr math 2> <action math 1 add>
    - <expr math 2> <action math 1 add>
    | <expr math 2> <action math 1 or>
    ^ <expr math 2> <action math 1 xor>
    & <expr math 2> <action math 1 and>
    <>

action math 1 add:

    + <expr math 2> <action math 1 add>
    - <expr math 2> <action math 1 add>
    <>

action math 1 or:

    | <expr math 2> <action math 1 or>
    <>

action math 1 xor:

    ^ <expr math 2> <action math 1 xor>
    <>

action math 1 and:

    & <expr math 2> <action math 1 and>
    <>

expr math 2:

    <expr math 3> <action math 2>

action math 2:

    * <expr math 3> <action math 2 mul>
    / <expr math 3> <action math 2 mul>
    div <expr math 3> <action math 2 mul>
    mod <expr math 3> <action math 2 mul>
    shl <expr math 3> <action math 2 shift>
    shr <expr math 3> <action math 2 shift>
    ushr <expr math 3> <action math 2 shift>
    rol <expr math 3> <action math 2 shift>
    ror <expr math 3> <action math 2 shift>
    <>

action math 2 mul:

    * <expr math 3> <action math 2 mul>
    / <expr math 3> <action math 2 mul>
    div <expr math 3> <action math 2 mul>
    mod <expr math 3> <action math 2 mul>
    <>

action math 2 shift:

    shl <expr math 3> <action math 2 shift>
    shr <expr math 3> <action math 2 shift>
    ushr <expr math 3> <action math 2 shift>
    rol <expr math 3> <action math 2 shift>
    ror <expr math 3> <action math 2 shift>
    <>

expr math 3:

    + <expr math 3>
    - <expr math 3>
    ~ <expr math 3>
    <expr unary>

expr unary:

    ( <expr> ) <action unary>
    <literal> <action unary>
    <symbol> <action unary>
    <blocks 1> <action unary>

action unary:

    <path> <action unary>
    <call> <action unary>
    <blocks 2> <action unary>
    <>

Literals
---

literal:

    <int>
    <real>
    <str>

*int*:

    [0-9]+(?!\.)

*real*:

    [0-9]+\.[0-9]*([Ee][+\-]?[0-9]+)?

*str*:

    "([^"]|\\")*"|'([^']|\\')*'

Symbols, Paths, Calls
---

symbol:

    <id>

path:

    . <id>

call:

    ( <args> )

args:

    <expr> <args more>
    <>

args more:

    , <args>
    <>

Blocks
---

blocks 1:

    \\ <block> <blocks more>

blocks 2:

    \\\\ <block> <blocks more>

blocks more:

    \\ <block> <blocks more>
    <>

block:

    <params> { <stmt> }

params:

    <param> <params more>
    <>

params more:

    , <params>
    <>

param:

    in <id>
    out <id>
    var <id>
    <id>

Special
---

**space**:

    <ignored>

**keyword**:

    <sign>

*id*:

    [A-Za-z_][A-Za-z0-9_]*

*sign*:

    [+\-*/%|^&~=!<>]=?|[A-Za-z_][A-Za-z0-9_]*=?|\\\\?|[(){},.;]

*ignored*:

    (\/\/[^\n]*|[ \t\r\n])*
    // including comments
