#pragma once
#include <bitset>

using Entity = int;
using entity = int;

constexpr int MAX_COMPONENTS = 5; // for testing

using Signature = std::bitset<MAX_COMPONENTS>;