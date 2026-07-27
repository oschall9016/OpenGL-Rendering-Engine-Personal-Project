#pragma once
#include <bitset>

using Entity = int;

constexpr int MAX_ENTITIES = 1000;
constexpr int MAX_COMPONENTS = 50;

using Signature = std::bitset<MAX_COMPONENTS>;
