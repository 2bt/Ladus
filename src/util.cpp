#include "util.hpp"

namespace {


Random g_random;


template<int C>
float my_sin(float x) {
    const float a0 = 0.174391564f;
    const float a1 = -0.790082954f;
    const float a2 = 0.0515932249f;
    const float a3 = 1.56366766f;
    const float a4 = 0.000230609556f;
    x *= 2.0f / M_PI;
    int ix = floorf(x);
    x -= ix;
    ix += C;
    if (ix & 1) x = 1.0f - x;
    float y = (((a0 * x + a1) * x + a2) * x + a3) * x + a4;
    return ix & 2 ? -y : y;
}


} // namespace


int   rand_int(int a, int b)       { return g_random.rand_int(a, b); }
float rand_float(float a, float b) { return g_random.rand_float(a, b); }


float my_sin(float x) { return my_sin<0>(x); }
float my_cos(float x) { return my_sin<1>(x); }


