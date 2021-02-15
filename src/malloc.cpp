/*
    NOTE: I took this code from
    https://github.com/maxkl/wasm-malloc/blob/master/src/malloc.c
    and modified it a bit.

    MIT License

    Copyright (c) 2019 Max Klein

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/


#include "stdwasm.hpp"

#define PAGE_SIZE (64 * 1024)
#define BLOCK_INFO_SIZE (sizeof(block_info))

struct block_info {
    block_info* previous;
    block_info* next;
    size_t      size;
    bool        free;
};

static bool        initialized = false;
static size_t      current_pages;
static uintptr_t   heap_top;
static block_info* first_block;
static block_info* last_block;
static block_info* first_free_block;


static void initialize() {
    if (initialized) return;
    initialized      = true;
    current_pages    = __builtin_wasm_memory_grow(0, 0);
    heap_top         = current_pages * PAGE_SIZE;
    first_block      = nullptr;
    last_block       = nullptr;
    first_free_block = nullptr;
}

static uintptr_t grow_heap(size_t inc) {
    uintptr_t old_heap_top = heap_top;
    size_t    old_pages    = current_pages;
    heap_top += inc;
    current_pages = (heap_top + PAGE_SIZE - 1) / PAGE_SIZE;
    if (current_pages > old_pages) {
        __builtin_wasm_memory_grow(0, current_pages - old_pages);
    }
    return old_heap_top;
}


//static void XXX(char const* label) {
//    int i  = 0;
//    int is = 0;
//    int j  = 0;
//    int js = 0;
//    for (block_info* block = first_block; block; block = block->next) {
//        if (block->free) {
//            j  += 1;
//            js += block->size + BLOCK_INFO_SIZE;
//        }
//        else {
//            i  += 1;
//            is += block->size + BLOCK_INFO_SIZE;
//        }
//    }
//    printf("%s %d %d %d %d\n", label, i, is, j, js);
//}


void* malloc(size_t size) {
    initialize();

    // TODO: alignment

    for (block_info* block = first_free_block; block; block = block->next) {
        if (!block->free) continue;
        if (block->size >= size) {

            if (block->size - size > BLOCK_INFO_SIZE) {
                block_info* next_block = (block_info*)((uintptr_t)block + BLOCK_INFO_SIZE + size);
                next_block->previous   = block;
                next_block->next       = block->next;
                next_block->free       = true;
                next_block->size       = block->size - size - BLOCK_INFO_SIZE;
                if (next_block->next) next_block->next->previous = next_block;
                block->next = next_block;
                block->size = size;
                if (block == first_free_block) first_free_block = next_block;
                if (block == last_block) last_block = next_block;
            }
            else {
                if (block == first_free_block) {
                    first_free_block = block->next;
                    while (first_free_block && !first_free_block->free) {
                        first_free_block = first_free_block->next;
                    }
                }
            }
            block->free = false;
            return (void*)((uintptr_t)block + BLOCK_INFO_SIZE);
        }
        else if ((uintptr_t)block == (uintptr_t)last_block) {
            grow_heap(size - block->size);
            block->size = size;
            block->free = false;
            return (void*)((uintptr_t)block + BLOCK_INFO_SIZE);
        }
    }


    block_info* new_block = (block_info*)grow_heap(BLOCK_INFO_SIZE + size);
    new_block->previous   = last_block;
    new_block->next       = nullptr;
    new_block->free       = false;
    new_block->size       = size;

    if (!first_block) first_block = new_block;

    if (last_block) last_block->next = new_block;
    last_block = new_block;

//    XXX("m");

    return (void*)((uintptr_t)new_block + BLOCK_INFO_SIZE);
}

void free(void* ptr) {
    if (!ptr) return;
    if (!initialized) return;

    block_info* block = (block_info*)((uintptr_t)ptr - BLOCK_INFO_SIZE);
    assert(!block->free);
    block->free = true;

    // Merge consecutive free blocks
    if (block->previous && block->previous->free) {
        block->previous->size += BLOCK_INFO_SIZE + block->size;
        block->previous->next = block->next;
        if (block->next) block->next->previous = block->previous;
        if (block == last_block) last_block = block->previous;
        block = block->previous;
    }
    if (block->next && block->next->free) {
        block->size += BLOCK_INFO_SIZE + block->next->size;
        if (block->next == last_block) last_block = block;
        block->next = block->next->next;
        if (block->next) block->next->previous = block;
    }
    if (!first_free_block || block < first_free_block) first_free_block = block;

//    XXX("f");

    // TODO: if this is the last block, release it
}
