#pragma once
#include <bitset>

using Entity = int;
using entity = int;

constexpr int MAX_ENTITIES = 1000;
constexpr int MAX_COMPONENTS = 20;

using Signature = std::bitset<MAX_COMPONENTS>;
