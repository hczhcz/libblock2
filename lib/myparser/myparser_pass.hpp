#pragma once

#include "myparser_common.hpp"

namespace myparser {

template <class TX = void> // actually not a template
class PassBase {
private:
    size_t id;

protected:
    inline PassBase(const size_t pass): id(pass) {}

    // virtual ~PassBase() {}

public:
    inline size_t getId() const {
        return id;
    }
};

template <size_t I>
class PassProto: public PassBase<> {
public:
    inline PassProto(): PassBase<>(I) {}

    // virtual ~PassProto() {}

    template <class T>
    static MYPARSER_INLINE void call(
        PassBase<> *pass, const size_t target, const T *node
    ) {
        if (target == I) {
            ((Pass<I> *) pass)->run(node);
        } else {
            Pass<I + 1>::call(pass, target, node);
        }
    }
};

template <size_t I>
class Pass: public Pass<I + 1> {};

template <>
class Pass<PASS_FIN> {
public:
    template <class T>
    static MYPARSER_INLINE void call(
        PassBase<> *, const size_t , const T *
    ) {
        // never reach
        throw;
    }
};

template <class T, size_t I = 0>
void passCall(
    PassBase<> *pass, const T *node
) {
    Pass<I>::call(pass, pass->getId(), node); // TODO: ?
}

}
