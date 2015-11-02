#include "node.hpp"

uintptr_t Node::nuid() const {
    return (uintptr_t) this;
}

std::string Node::nuidOut() const {
    return "result_" + std::to_string(nuid());
}

std::string Node::nuidIn() const {
    return "input_" + std::to_string(nuid());
}
