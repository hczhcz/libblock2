import os

rep_indent = '    '
rep_sep = os.linesep
rep_colon = ' - '


class Node(object):
    def __init__(self, newname):
        self.name = newname


class NodeList(Node):
    def __init__(self, newname, newlist):
        super(NodeList, self).__init__(newname)
        self.list = newlist

    def get_text(self):
        return ''.join([item.get_text() for item in self.list])

    def get_tree(self, indent=0):
        if len(self.list) == 1:
            return self.name + rep_colon + self.list[0].get_tree(indent)
        else:
            return self.name + ''.join([
                rep_sep + rep_indent * (indent + 1) + (
                    item.get_tree(indent + 1)
                ) for item in self.list
            ])

    def len(self):
        return sum([item.len() for item in self.list])


class NodeText(Node):
    def __init__(self, newname, newtext):
        super(NodeText, self).__init__(newname)
        self.text = newtext

    def get_text(self):
        return self.text

    def get_tree(self, indent=0):
        return self.name + rep_colon + self.text.encode('string_escape')

    def len(self):
        return len(self.text)
