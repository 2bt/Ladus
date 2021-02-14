#include "util.hpp"

namespace {

Random g_random;

} // namespace


int   rand_int(int a, int b)       { return g_random.rand_int(a, b); }
float rand_float(float a, float b) { return g_random.rand_float(a, b); }
