#include "testing/common.h"

#include <memory.h>
#include <stdlib.h>

static const unsigned char block_pad[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF,
    0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF,
};
#define PAD_LENGTH sizeof(block_pad)

// functions wrapped further down
REAL_FUNC_DECL(void *, malloc(size_t size));
REAL_FUNC_DECL(void, free(void* ptr));

static void *create_block(size_t size) {
    unsigned char *in = __real_malloc(size + PAD_LENGTH * 2);
    memcpy(in, block_pad, PAD_LENGTH);
    in += PAD_LENGTH;
    memcpy(in + size, block_pad, PAD_LENGTH);
    return in;
}

static void check_block_integrity(void *input, size_t size) {
    unsigned char *in = input;
    assert_memory_equal(in - PAD_LENGTH, block_pad, PAD_LENGTH);
    assert_memory_equal(in + size, block_pad, PAD_LENGTH);
}

static void free_block(void *input) {
    unsigned char *in = input;
    __real_free(in - PAD_LENGTH);
}

static struct {
    unsigned int past_leaks;
    unsigned int count;
    size_t sizes[MAX_ALLOCS];
    void *ptrs[MAX_ALLOCS];
} allocations;

static void add_alloc(void *addr, size_t size) {
    assert_in_range(allocations.count, 0, MAX_ALLOCS);
    allocations.sizes[allocations.count] = size;
    allocations.ptrs[allocations.count++] = addr;
}

static size_t remove_alloc(void *addr) {
    for (unsigned int i = 0; i < allocations.count; ++i) {
        if (allocations.ptrs[i] == addr) {
            size_t ret = allocations.sizes[i];
            --allocations.count;
            for (unsigned int j = i; j < allocations.count; ++j) {
                allocations.ptrs[j] = allocations.ptrs[j + 1];
                allocations.sizes[j] = allocations.sizes[j + 1];
            }
            check_block_integrity(addr, ret);
            return ret;
        }
    }
    int ptr_not_found = 1;
    assert_false(ptr_not_found);
    return 0;
}

static void accept_past_leaks(void) {
    allocations.past_leaks = allocations.count;
}

static void check_mem_integrity(void) {
    for (unsigned int i = allocations.past_leaks; i < allocations.count; ++i) {
        check_block_integrity(allocations.ptrs[i], allocations.sizes[i]);
    }
}

static int alloc_checks_enabled = 0;

FUNC_WRAPPER(void *, malloc(size_t size)) {
    if (!alloc_checks_enabled) {
        return __real_malloc(size);
    }
    void *ret = create_block(size);
    add_alloc(ret, size);
    return ret;
}

FUNC_WRAPPER(void *, calloc(size_t n, size_t size)) {
    if (!alloc_checks_enabled) {
        return __real_calloc(n, size);
    }
    void *ret = create_block(n * size);
    add_alloc(ret, n * size);
    return ret;
}

FUNC_WRAPPER(void *, realloc(void *old, size_t size)) {
    if (!alloc_checks_enabled) {
        return __real_realloc(old, size);
    }
    if (!size) {
        free(old);
    } else if (old) {
        void *ret = create_block(size);
        add_alloc(ret, size);
        size_t old_size = remove_alloc(old);
        memcpy(ret, old, old_size < size ? old_size : size);
        free_block(old);
        return ret;
    }
    return __wrap_malloc(size);
}

FUNC_WRAPPER(void, free(void* ptr)) {
    if (!alloc_checks_enabled) {
        __real_free(ptr);
    }
    if (ptr) {
        remove_alloc(ptr);
        free_block(ptr);
    }
}

void enable_alloc_checks(void) {
    alloc_checks_enabled = 1;
}

void disable_alloc_checks(void) {
    alloc_checks_enabled = 0;
}

unsigned int leftover_mem_blocks(void) {
    return allocations.count;
}

int alloc_checks_test_setup(void **state) {
    UNUSED(state);
    if (alloc_checks_enabled) {
        accept_past_leaks();
    }
    return 0;
}
int alloc_checks_test_teardown(void **state) {
    UNUSED(state);
    if (alloc_checks_enabled) {
        check_mem_integrity();
    }
    return 0;
}
