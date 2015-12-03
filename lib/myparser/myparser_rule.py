import os
import re

from myparser_tool import char_error, char_maybe, char_any0, char_any1
from myparser_tool import MyParserException
from myparser_tool import map_one, map_one_deep, map_all
from myparser_ast import NodeList, NodeText

syntax_indent = '    '
syntax_return = os.linesep
syntax_sep = os.linesep
syntax_colon = ':'

syntax_list = ''
syntax_builtin = '**'
syntax_regex = '*'

syntax_space = ' '
syntax_ref_l = '<'
syntax_ref_r = '>'
syntax_escape = '\\'

escape_dict = {
    '0': '\0',
    'b': '\b',
    't': '\t',
    'n': '\n',
    'v': '\v',
    'f': '\f',
    'r': '\r',
    syntax_space: syntax_space,
    syntax_ref_l: syntax_ref_l,
    syntax_ref_r: syntax_ref_r,
    syntax_escape: syntax_escape
}

inv_escape_dict = {b: a for a, b in escape_dict.items()}

re_name = re.compile(r'[\w ]+')

parser_deep = False

root_name = 'root'
space_name = 'space'
keyword_name = 'keyword'
ignore_name = ''
builtin_name = {root_name, space_name, keyword_name}


class RuleItemSpace(object):
    def __init__(self):
        pass

    def dump(self):
        return syntax_space

    def xdump(self, template):
        return template['space']()

    def compile(self, env):
        return lambda val, pos: env[space_name](val, pos)


class RuleItemKeyword(object):
    def __init__(self, newtext):
        self.text = newtext

    def dump(self):
        return ''.join([
            (
                syntax_escape + inv_escape_dict[char]
                if char in inv_escape_dict else char
            ) for char in self.text
        ])

    def xdump(self, template):
        return template['keyword'](self.text)

    def compile(self, env):
        checker = lambda x: x if x.get_text() == self.text else None

        return lambda val, pos: checker(env[keyword_name](val, pos))


class RuleItemRef(object):
    def __init__(self, newtarget):
        if re_name.match(newtarget).end() != len(newtarget):
            raise MyParserException('Bad item reference')
        self.target = newtarget

    def dump(self):
        return syntax_ref_l + self.target + syntax_ref_r

    def dump_tag(self, tag):
        return syntax_ref_l + tag + self.target + syntax_ref_r

    def xdump(self, template):
        return template['ref'](self.target)

    def xdump_tag(self, template, tag):
        return template['ref' + tag](self.target)

    def compile(self, env):
        return lambda val, pos: env[self.target](val, pos)


class RuleItemError(object):
    def __init__(self, newerror):
        self.error = newerror

    def dump_tag(self, tag):
        return syntax_ref_l + tag + self.error + syntax_ref_r

    def xdump_tag(self, template, tag):
        return template['error'](self.error)

    def compile(self, env):
        return lambda val, pos: MyParserException(self.error).do_raise()


class Rule(object):
    def __init__(self, newname):
        self.name = newname


class RuleList(Rule):
    def __init__(self, newname):
        super(RuleList, self).__init__(newname)
        self.rule = list()

    def add_space(self):
        self.rule[-1].append(
            (None, RuleItemSpace())
        )

    def add_text(self, newtext):
        self.rule[-1].append(
            (None, RuleItemKeyword(newtext))
        )

    def add_ref(self, newtarget):
        if newtarget == ignore_name:
            self.rule[-1].append((None, None))
        elif newtarget[0] == char_error:
            self.rule[-1].append(
                (newtarget[0], RuleItemError(newtarget[1:]))
            )
        elif newtarget[0] in {char_maybe, char_any0, char_any1}:
            self.rule[-1].append(
                (newtarget[0], RuleItemRef(newtarget[1:]))
            )
        else:
            self.rule[-1].append((None, RuleItemRef(newtarget)))

    def add(self, newline):
        self.rule.append(list())

        buf = ''
        mode = 0
        newmode = 0
        escaped = False

        # mode 1: space
        # mode 2: keyword
        # mode 3: ref / error

        for char in newline:
            if escaped:
                escaped = False
                if char in escape_dict:
                    buf += escape_dict[char]
                else:
                    buf += char
            elif mode == 3:
                if char == syntax_ref_r:
                    newmode = 0
                else:
                    buf += char
            elif char == syntax_space:
                newmode = 1
            elif char == syntax_ref_l:
                newmode = 3
            elif char == syntax_escape:
                newmode = 2
                escaped = True
            else:
                newmode = 2
                buf += char

            if newmode != mode:
                if mode == 1:
                    self.add_space()
                elif mode == 2:
                    self.add_text(buf)
                    buf = ''
                elif mode == 3:
                    self.add_ref(buf)
                    buf = ''
                mode = newmode

        if mode == 1:
            self.add_space()
        elif mode == 2:
            self.add_text(buf)
        elif mode == 3:
            raise MyParserException(
                '"' + syntax_ref_r + '" expected but not found'
            )

    def dump_list(self):
        return syntax_sep.join([
            syntax_indent + ''.join([
                (
                    item[1].dump_tag(item[0])
                    if item[0] else item[1].dump()
                    if item[1] else syntax_ref_l + syntax_ref_r
                ) for item in line
            ]) for line in self.rule
        ])

    def dump(self):
        return syntax_list + self.name + syntax_list\
            + syntax_colon + syntax_return\
            + syntax_return\
            + self.dump_list()\
            + syntax_return

    def xdump(self, template):
        return template['list'](self.name, [
            template['line']([
                (
                    item[1].xdump_tag(template, item[0])
                    if item[0] else item[1].xdump(template)
                ) for item in line if item[1]
            ]) for line in self.rule
        ])

    def compile(self, env):
        self.compiled = [
            [
                (
                    item[0], item[1].compile(env)
                ) for item in line if item[1]
            ] for line in self.rule
        ]

        list_match = lambda val, pos, l: map_all(
            l, pos,
            lambda x, xpos: x(val, xpos),
            lambda x, xpos: xpos + x.len(),
            lambda x: NodeList(self.name, x)
        )

        if parser_deep:
            return lambda val, pos: map_one_deep(
                self.compiled, lambda x: list_match(val, pos, x)
            )
        else:
            return lambda val, pos: map_one(
                self.compiled, lambda x: list_match(val, pos, x)
            )


class RuleBuiltin(RuleList):
    def __init__(self, newname):
        super(RuleBuiltin, self).__init__(newname)
        if not newname in builtin_name:
            raise MyParserException('Bad builtin rule name')

    def dump(self):
        return syntax_builtin + self.name + syntax_builtin\
            + syntax_colon + syntax_return\
            + syntax_return\
            + self.dump_list()\
            + syntax_return


class RuleRegex(Rule):
    def __init__(self, newname):
        super(RuleRegex, self).__init__(newname)

    def add(self, newregex):
        if hasattr(self, 'regex'):
            raise MyParserException('Too much regex')
        self.regex = newregex

    def dump(self):
        return syntax_regex + self.name + syntax_regex\
            + syntax_colon + syntax_return\
            + syntax_return\
            + syntax_indent + self.regex\
            + syntax_return

    def xdump(self, template):
        return template['regex'](self.name, self.regex)

    def compile(self, env):
        self.compiled = re.compile(self.regex)

        checker = lambda x: NodeText(
            self.name, x.string[x.start():x.end()]
        ) if x else None

        return lambda val, pos: checker(self.compiled.match(val, pos))
