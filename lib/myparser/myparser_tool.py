char_error = '!'
char_maybe = '?'
char_any0 = '*'
char_any1 = '+'


class MyParserException(BaseException):
    def do_raise(self):
        raise self


def debug_print(x):
    print(x)
    return x


def map_one(data, func):
    for item in data:
        result = func(item)
        if result:
            return result


def map_one_deep(data, func):
    result = None

    for item in data:
        new_result = func(item)
        if new_result and result:
            raise MyParserException('Rule collision')
        else:
            result = new_result

    return result


def map_all(data, info, func, update, succeed):
    result = None
    result_list = []

    for (mode, item) in data:
        while True:
            result = func(item, info)

            if result:
                info = update(result, info)
                result_list.append(result)

                if mode == char_maybe:
                    break
                elif mode == char_any0:
                    continue
                elif mode == char_any1:
                    mode = char_any0
                    continue
                else:
                    break
            else:
                if mode == char_maybe:
                    break
                elif mode == char_any0:
                    break
                elif mode == char_any1:
                    return None
                else:
                    return None

    return succeed(result_list)
