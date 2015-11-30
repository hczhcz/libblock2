import myparser
import myparser_cpp

parser = myparser.MyParser()
parser.add_file('parser_syntax.md')

with open('../parser.hpp', 'w') as hpp:
    hpp.write(
        myparser_cpp.cplusplus_gen(
            parser.xdump(myparser_cpp.cplusplus_dump),
            'parser/'
        )
    )
