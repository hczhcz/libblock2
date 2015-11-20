#include "output.hpp"
#include "type.hpp"
#include "node.hpp"
#include "block.hpp"

std::map<std::string, std::shared_ptr<NodeBlock>> &Builtin::builtins() {
    static std::map<std::string, std::shared_ptr<NodeBlock>> map;

    return map;
}

void Builtin::apply(Instance &instance) {
    for (const auto &builtin: builtins()) {
        instance.insert(
            builtin.first,
            instance.addClosure(*builtin.second)
        );
    }
}

void BlockBuiltinFmt::buildContent(Instance &instance, Output &output) {
    // gen type
    // notice: for existing symbols,
    //         Instance::insert would call Instance::check

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
                        oc.os << instance.strCast("self") << "->";
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
