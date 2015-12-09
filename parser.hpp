#pragma once

#include "include.hpp"

namespace libblock {

class Node;

Node &parse(std::string &&code);
Node &parseFile(std::string &&file);

}
