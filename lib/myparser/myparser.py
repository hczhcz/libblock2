import os
import re

from myparser_tool import MyParserException
from myparser_rule import RuleList, RuleBuiltin, RuleRegex
from myparser_rule import root_name

re_list = re.compile(r'(?<=^)\w([\w ]*\w)?(?=:$)')
re_builtin = re.compile(r'(?<=^\*\*)\w([\w ]*\w)?(?=\*\*:$)')
re_regex = re.compile(r'(?<=^\*)\w([\w ]*\w)?(?=\*:$)')
re_rule = re.compile(r'(?<=^    )(?!\/\/ ).*(?=$)')


class MyParser(object):
    def __init__(self):
        self.rule_list = list()

    def add_rules(self, rule):
        for item in rule:
            result = re_list.search(item)
            if result:
                self.rule_list.append(RuleList(
                    result.string[result.start():result.end()]
                ))

            result = re_builtin.search(item)
            if result:
                self.rule_list.append(RuleBuiltin(
                    result.string[result.start():result.end()]
                ))

            result = re_regex.search(item)
            if result:
                self.rule_list.append(RuleRegex(
                    result.string[result.start():result.end()]
                ))

            result = re_rule.search(item)
            if result:
                if len(self.rule_list) == 0:
                    raise MyParserException('Rule name undefined')
                else:
                    self.rule_list[-1].add(
                        result.string[result.start():result.end()]
                    )

    def add_file(self, filename):
        self.add_rules(open(filename, 'r').readlines())

    def dump(self):
        return os.linesep.join([
            item.dump() for item in self.rule_list
        ])

    def xdump(self, template):
        return os.linesep.join([
            item.xdump(template) for item in self.rule_list
        ])

    def compile(self):
        self.compiled = dict()
        for item in self.rule_list:
            self.compiled[item.name] = item.compile(self.compiled)

        return self.compiled[root_name]

    def match(self, data):
        result = self.compiled[root_name](data, 0)

        if result:
            return result
        else:
            raise MyParserException('Match nothing')


if __name__ == '__main__':
    import sys

    if len(sys.argv) <= 1 or sys.argv[1] == 'help':
        # show help

        print '============================================='
        print ' MyParser: a lightweight LL parser framework '
        print '============================================='
        print ''
        print 'Usage: ' + __file__ + ' command arguments'
        print ''
        print __file__ + ' help'
        print '    Show help'
        print ''
        print __file__ + ' print SYNTAX'
        print '    Process syntax file and print out'
        print ''
        # print __file__ + ' match SYNTAX [-i INPUT] [-o OUTPUT]'
        # print '    ???'
        # print ''
        print __file__ + ' c++ SYNTAX [-o OUTPUT] [-p HEADER_PATH]'
        print '    Run the c++ code generator'
        print ''

    elif sys.argv[1] == 'print':
        # dump

        index = 2

        syntax = ''

        while index < len(sys.argv):
            if sys.argv[index][:1] == '-':
                raise MyParserException('Unknown argument')
            else:
                syntax = sys.argv[index]
            index += 1

        if not os.path.isfile(syntax):
            raise MyParserException('File not found')

        parser = MyParser()
        parser.add_file(syntax)

        print parser.dump()

    elif sys.argv[1] == 'c++':
        # c++ code generator

        import myparser_cpp

        index = 2

        syntax = ''
        output = ''
        mppath = './'

        while index < len(sys.argv):
            if sys.argv[index][:1] == '-':
                if sys.argv[index] == '-o':
                    index += 1
                    if index < len(sys.argv):
                        output = sys.argv[index]
                elif sys.argv[index] == '-p':
                    index += 1
                    if index < len(sys.argv):
                        mppath = sys.argv[index]
                else:
                    raise MyParserException('Unknown argument')
            else:
                syntax = sys.argv[index]
            index += 1

        if not os.path.isfile(syntax):
            raise MyParserException('File not found')

        parser = MyParser()
        parser.add_file(syntax)

        result = myparser_cpp.cplusplus_gen_auto(
            parser, mppath
        )

        if output == '':
            print result
        else:
            open(output, 'w').write(result)

    else:
        raise MyParserException('Unknown command')
