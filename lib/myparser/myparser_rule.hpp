#pragma once

#include "myparser_ast_spec.hpp"

namespace myparser {

// builtin names -> myparser_ast.hpp

using ErrorList = MP_STR("Nothing matched");
using ErrorRegex = MP_STR("Regex not matched");
using ErrorChecking = MP_STR("Match not accepted");
using ErrorKeyword = MP_STR("Bad keyword");
using ErrorTail = MP_STR("Parsing not finished");

template <size_t L, size_t M>
class Tag {
public:
    static const size_t least = L;
    static const size_t most = M;
};
using TagNormal = Tag<1, 1>;
using TagMaybe = Tag<0, 1>;
using TagAny0 = Tag<0, std::numeric_limits<size_t>::max()>;
using TagAny1 = Tag<1, std::numeric_limits<size_t>::max()>;

template <class N>
class Rule {
private:
    inline Rule() = delete; // force static

    // virtual ~Rule() = delete;
};

// need specialize
template <class N>
class RuleDef;

template <>
class RuleDef<BuiltinError>: public Rule<BuiltinError> {};

//////// Named ////////

template <class N, class... RL>
class RuleList: public Rule<N> {
private:
    template <size_t I, class R, class... Rx>
    static MYPARSER_INLINE std::pair<Node<> *, Node<> *> runRule(
        Input &input, const Input &end
    ) {
        using Line = typename R::template Helper<N, I>;

        Input input_rev = input;

        auto current = Line::parse(input, end);

        if (current.first) {
            return current;
        } else {
            input = input_rev;

            auto next = runRule<I + 1, Rx...>(input, end);

            if (!next.second) {
                return {next.first, current.second};
            } else {
                return {
                    next.first,
                    next.second->challengeLonger(current.second)
                };
            }
        }
    }

    template <size_t I> // iteration finished
    static MYPARSER_INLINE std::pair<Node<> *, Node<> *> runRule(
        Input &input, const Input &
    ) {
        return {
            nullptr,
            new NodeTypedError<N, ErrorList>(input)
        };
    }

public:
    static std::pair<Node<> *, Node<> *> parse(
        Input &input, const Input &end
    ) {
        return runRule<0, RL...>(input, end);
    }
};

template <class N, class RX>
class RuleRegex: public Rule<N> {
public:
    using Result = NodeTypedText<N>;

private:
    static MYPARSER_INLINE std::pair<Node<> *, Node<> *> runRegex(
        Input &input, const Input &end
    ) {
        #if defined(MYPARSER_BOOST_XPRESSIVE)
            static const regex_lib::basic_regex<Input> re =
                regex_lib::basic_regex<Input>::compile<Input>(
                    RX::getStr().cbegin(),
                    RX::getStr().cend()
                );
        #else
            static const regex_lib::regex re(
                RX::getStr()
            );
        #endif

        regex_lib::match_results<Input> mdata;

        if (
            regex_lib::regex_search(
                input, end, mdata, re,
                regex_lib::regex_constants::match_continuous
            )
        ) {
            Result *result =
                new Result(input, input + mdata.length());

            if (result->accepted()) {
                input += mdata.length();

                return {result, nullptr};
            } else {
                return {nullptr, result};
            }
        } else {
            return {
                nullptr,
                new NodeTypedError<N, ErrorRegex>(input)
            };
        }
    }

public:
    static std::pair<Node<> *, Node<> *> parse(
        Input &input, const Input &end
    ) {
        return runRegex(input, end);
    }
};

//////// Cell ////////

template <class N = BuiltinSpace, class TAG = TagNormal>
class RuleItemSpace: public TAG {
public:
    static MYPARSER_INLINE std::pair<Node<> *, Node<> *> parse(
        Input &input, const Input &end
    ) {
        return RuleDef<N>::parse(input, end);
    }
};

template <class KW, class N = BuiltinKeyword, class TAG = TagNormal>
class RuleItemKeyword: public TAG {
public:
    static MYPARSER_INLINE std::pair<Node<> *, Node<> *> parse(
        Input &input, const Input &end
    ) {
        auto current = RuleDef<N>::parse(input, end);

        if (
            current.first
            &&
            current.first->getFullText() == KW::getStr()
        ) {
            return current;
        } else {
            if (current.first) {
                current.first->free();
            }
            if (current.second) {
                current.second->free();
            }

            return {
                nullptr,
                new NodeTypedError<N, ErrorKeyword>(input)
            };
        }
    }
};

template <class N, class TAG = TagNormal>
class RuleItemRef: public TAG {
public:
    static MYPARSER_INLINE std::pair<Node<> *, Node<> *> parse(
        Input &input, const Input &end
    ) {
        return RuleDef<N>::parse(input, end);
    }
};

template <class E, class TAG = TagNormal>
class RuleItemError: public TAG {
public:
    static MYPARSER_INLINE std::pair<Node<> *, Node<> *> parse(
        Input &input, const Input &
    ) {
        return {
            nullptr,
            new NodeTypedError<BuiltinError, E>(input)
        };
    }
};

//////// Misc ////////

template <class... RL>
class RuleLine {
public:
    template <class N, size_t I>
    class Helper {
    public:
        using Result = NodeTypedList<N, I>;

    private:
        template <class R, class... Rx>
        static MYPARSER_INLINE bool runRule(
            Result *&result, Node<> *&err, size_t &errpos,
            Input &input, const Input &end
        ) {
            for (size_t i = 0; i < R::most; ++i) {
                auto current = R::parse(input, end);

                if (current.second) {
                    err = current.second->challengeLonger(err);

                    // if err updated
                    if (err == current.second) {
                        errpos = result->getChildren().size();
                    }
                }

                if (!current.first) {
                    if (i < R::least) {
                        return false;
                    } else {
                        break;
                    }
                } else {
                    result->putChild(current.first);
                }
            }

            return runRule<Rx...>(result, err, errpos, input, end);
        }

        template <std::nullptr_t P = nullptr> // iteration finished
        static MYPARSER_INLINE bool runRule(
            Result *&, Node<> *&, size_t &,
            Input &, const Input &
        ) {
            return true;
        }

    public:
        static MYPARSER_INLINE std::pair<Node<> *, Node<> *> parse(
            Input &input, const Input &end
        ) {
            Result *result = new Result(input);
            Result *result_err = new Result(input);

            Node<> *err = nullptr;
            size_t errpos = 0;

            bool succeed = runRule<RL...>(result, err, errpos, input, end);

            result->bind(result_err, errpos);
            result_err->bind(result, errpos);

            if (err) {
                for (size_t i = 0; i < errpos; ++i) {
                    result_err->putChild(result->getChildren()[i]);
                }
                result_err->putChild(err);
            } else {
                result_err->free();
                result_err = nullptr;
            }

            if (!succeed) {
                result->free();
                result = nullptr;
            }

            return {result, result_err};
        }
    };
};

template <class N = BuiltinRoot>
class Parser {
public:
    static inline Node<> *parse(
        Input &input, const Input &end, const bool dothrow = false
    ) {
        auto current = RuleDef<N>::parse(input, end);

        if (input != end) {
            current.first->free();

            return new NodeTypedError<N, ErrorList>(input);
        };

        if (current.first) {
            if (current.second) {
                current.second->free();
            }

            return current.first;
        } else {
            if (dothrow) {
                throw current.second;
            } else {
                return current.second;
            }
        }
    }

    static inline Node<> *parse(
        const std::string &text, const bool dothrow = false
    ) {
        Input input = text.cbegin();

        return parse(input, text.cend(), dothrow);
    }

    static inline Node<> *parseFile(
        const std::string &filename, const bool dothrow = false
    ) {
        static std::vector<std::string> storage;

        std::ifstream fs(filename);
        std::istreambuf_iterator<char> input(fs);
        std::istreambuf_iterator<char> end;
        storage.push_back(std::string(input, end));

        return parse(storage.back(), dothrow);
    }
};

}
