#include "output.hpp"
#include "type.hpp"
#include "block.hpp"

void BlockBuiltinFmt::buildContent(Instance &instance, Output &output) {
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

    output.content(
        instance,
        [&](OutputContext &oc) {
            oc.endl();

            for (char i: str) {
                switch (i) {
                    case '$':
                        oc.os << instance.strSelf() << "->";
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
    );
}

BlockBuiltinFmt::BlockBuiltinFmt(
    std::set<BlockOption> &&_options,
    std::vector<std::pair<std::string, ParamMode>> &&_params,
    std::map<std::string, Type &> &&_static_types,
    std::vector<std::pair<std::string, std::string>> &&_dynamic_types,
    std::string &&_str
):
    BlockBuiltin {std::move(_options), std::move(_params)},
    static_types {_static_types},
    dynamic_types {_dynamic_types},
    str {std::move(_str)} {}
