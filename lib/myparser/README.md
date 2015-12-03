MyParser
===

A parser generator.

The workflow: `Markdown ===(Python)==> C++11.`

Usages
---

### Generate a parser using the driver program

1. Run `python myparser.py c++ <filename>.md`.

### Generate a parser by hand (using a Python script)

1. Import `myparser` and `myparser_cpp` in python.
2. Create a parser object: `parser = myparser.MyParser()`.
3. Load a syntax file: `parser.add_file(filename)`.
4. Generate the parser: `myparser_cpp.cplusplus_gen_auto(parser, './', 'SYNTAX_HPP')`.
5. Save the generated code as a C++ header file.

### Use the generated parser

1. Import the generated code and use the namespace `myparser`.
2. Parse a file: `auto ast = Parser<>::parseFile(filename)`, or a string: `Parser<>::parse(text)`.
3. Visit the AST via RTTI, or use a visitor `myparser::Pass<>`: `ast->runPass(&pass)`.
4. Compile your code with `-std=c++11`.

Syntax Files
---

MyParser accepts a special Markdown ("MyParser Markdown") file.

The `README.md` (this file) is not only a document but also an example of a syntax file.

You could run `python self_syntax.py` or `python myparser.py c++ README.md -o self_syntax.hpp`. That loads `README.md` and generates `self_syntax.hpp`.

Then, just compile `self_bootstrap.cpp`. A syntax highlighter that accepts MyParser Markdown files as its input is prepared now. You could see `README.md` printed with syntax highlights.

Definition of MyParser Markdown
---

**root**:

    <> <blocks> <>

**space**:

    <*comment line>

**keyword**:

    <id>
    <sign>

blocks:

    <+block>

block:

    <list head> <list body> <>
    <builtin head> <list body> <>
    <regex head> <regex line> <>

list head:

    <id>:<line break>

builtin head:

    **<id>**:<line break>

regex head:

    *<id>*:<line break>

list body:

    <list line> <list body>
    <list line>

list line:

    <indent><list items><line break>

regex line:

    <indent><anything><line break>

comment line:

    <indent>//<anything><line break>
    <other markdown><line break>
    <line break>

list items:

    <*list item>

list item:

    \ <>
    <empty item>
    <keyword item>
    <ref item>
    <error item>

empty item:

    \<<>\>

*keyword item*:

    ([^ \r\n\\<]|\\.)+

ref item:

    \<<id>\>
    \<?<id>\>
    \<*<id>\>
    \<+<id>\>

error item:

    \<!<id>\>

*id*:

    \w([\w ]*\w)?

*sign*:

    \**:|\*+| +|\/\/|<[\*\+\?\!]?|>

*other markdown*:

    ( ? ? ?)[^ \r\n].*[^:\r\n]|.|

*indent*:

    (    )

*line break*:

    [\r\n]+

*anything*:

    .*

Example
---

[gaocegege/CompilerLab (MyLang Project)](https://github.com/gaocegege/CompilerLab)
