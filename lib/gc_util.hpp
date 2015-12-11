#include <list>
#include <vector>
#include <map>

#include <gc/gc.h>
#include <gc/gc_cpp.h>
#include <gc/gc_allocator.h>

namespace std {

template <class T>
using gc_list = list<T, gc_allocator<T>>;
template <class T>
using gc_vector = vector<T, gc_allocator<T>>;
template <class Key, class T>
using gc_map = map<
    Key, T,
    less<Key>,
    gc_allocator<std::pair<const Key, T>>
>;

template <class T>
class gc_function;

template <class R, class... Args>
class gc_function<R (Args...)>: public function<R (Args...)> {
public:
    template <class Func>
    gc_function(
        Func _func,
        decltype(&Func::operator()) * = nullptr
    ):
        function<R (Args...)> {
            std::allocator_arg,
            gc_allocator<char> {},
            std::forward<Func>(_func)
        } {}

    gc_function(const gc_function &_obj):
        function<R (Args...)> {
            std::allocator_arg,
            gc_allocator<char> {},
            static_cast<const function<R (Args...)> &>(_obj)
        } {}

    gc_function(gc_function &&_obj):
        function<R (Args...)> {
            std::allocator_arg,
            gc_allocator<char> {},
            static_cast<function<R (Args...)> &&>(_obj)
        } {}
};

}
