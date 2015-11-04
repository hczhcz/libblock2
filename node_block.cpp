#include "exception.hpp"
#include "output.hpp"
#include "node.hpp"

Instance &Block::getInstance(Instance &&instance, Output &output) {
    for (Instance &target: instances) {
        bool ok {true};

        for (const auto &symbol: instance.symbol_types) {
            if (symbol.second != target.at(symbol.first)) {
                ok = false;
                break;
            }
        }

        if (ok) {
            // found

            return target;
        }
    }

    // not found

    instances.push_back(std::move(instance));

    // build

    output.insert(instances.back());
    buildContent(instances.back(), output);

    return instances.back();
}

void Block::buildProc(Instance &, Output &) {
    // nothing
}

Type &Block::buildOut(Instance &instance, Output &output) {
    // get type

    instance.children.push_back({instance, *this});
    Type &type {instance.children.back()};

    // render

    output.at(instance).content
        << "    " << type.renderDecl(nuidOut())
        << " = self;\n";

    // return

    return type;
}

void Block::buildIn(Instance &, Type &, Output &) {
    throw ErrorWriteNotAllowed {};
}

std::map<std::string, BlockBuiltin &> BlockBuiltin::builtins;

void BlockBuiltin::applyBuiltin(Instance &instance) {
    for (const auto &builtin: builtins) {
        instance.children.push_back({instance, builtin.second});
        Type &type {instance.children.back()};

        instance.insert(builtin.first, type);
    }
}

void BlockBuiltin::buildContent(Instance &instance, Output &output) {
    // render (before body)

    output.at(instance).header
        << "builtin_" << name << "(block_" << instance.tuid() << " *self);\n";

    // gen type

    func(instance);
}

void BlockUser::buildContent(Instance &instance, Output &output) {
    // render (before body)

    output.at(instance).header
        << "func_" << instance.tuid() << "(block_" << instance.tuid() << " *self);\n";

    std::ostringstream &os {output.at(instance).content};

    os << "func_" << instance.tuid() << "(block_" << instance.tuid() << " *self) {\n";

    // gen type

    ast->buildProc(instance, output);

    // render (after body)

    os << "}\n\n";
}
