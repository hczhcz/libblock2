#include "output.hpp"
#include "type.hpp"
#include "block.hpp"

namespace libblock {

void BlockBuiltin::codeFmt(
    const std::string &str,
    Instance &instance, OutputContext &oc
) {
    oc.endl();

    for (char i: str) {
        switch (i) {
            case '$':
                oc.os << instance.strSelf() << "->data.";
                break;

            case '\t':
                oc.tab();
                break;

            case '\n':
                oc.endl();
                break;

            default:
                oc.os << i;
                break;
        }
    }
}

void BlockBuiltinFmt::buildContent(
    Session &,
    Instance &instance
) {
    // gen type
    // notice: for existing symbols,
    //         Instance::insert covers Instance::check here

    for (const auto &symbol: static_types) {
        instance.insert(
            symbol.first,
            symbol.second
        );
    }

    for (const auto &symbol: dynamic_types) {
        instance.insert(
            symbol.first,
            instance.at(symbol.second)
        );
    }

    // render

    instance.content.insert(
        [&](OutputContext &oc) {
            codeFmt(str, instance, oc);
        }
    );
}

BlockBuiltinFmt::BlockBuiltinFmt(
    std::set<BlockOption> &&_options,
    std::gc_vector<std::pair<std::string, ParamMode>> &&_params,
    std::gc_map<
        std::string,
        std::reference_wrapper<Type>
    > &&_static_types,
    std::gc_vector<std::pair<std::string, std::string>> &&_dynamic_types,
    std::string &&_str
):
    BlockBuiltin {std::move(_options), std::move(_params)},
    static_types {std::move(_static_types)},
    dynamic_types {std::move(_dynamic_types)},
    str {std::move(_str)} {}

}
