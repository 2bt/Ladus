#include "util.hpp"

namespace {

uint32_t g_next = 1;

} // namespace

void* operator new(size_t size) {
    return malloc(size);
}

void operator delete(void* ptr) {
    free(ptr);
}

int rand() {
    g_next = g_next * 1103515245 + 12345;
    return (uint32_t)(g_next / 65536) % (RAND_MAX + 1);
}

void srand(uint32_t seed) {
    g_next = seed;
}
