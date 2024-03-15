#include "vector.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define EXIT_IF_NULL(value)                                                          \
    if (value == NULL)                                                               \
    {                                                                                \
        fprintf(stderr, "Given null on line %d in file \"%s\"", __LINE__, __FILE__); \
        exit(1);                                                                     \
    }

#define RET_IF_NULL(value, return_value)                                             \
    if (value == NULL)                                                               \
    {                                                                                \
        fprintf(stderr, "Given null on line %d in file \"%s\"", __LINE__, __FILE__); \
        return return_value;                                                         \
    }

#define VALIDATE_VECTOR(vec) EXIT_IF_NULL(vec)

size_t default_growth_rate(Vector *v)
{
    return v->capacity * 2;
}

Vector *vector_new(size_t capacity, size_t elem_size, void_cmp_func cmp, vector_growth_rate_func grow)
{
    Vector *p = (Vector *)malloc(sizeof(Vector));
    assert(p);
    p->len = 0;
    p->capacity = 0;
    p->elem_size = elem_size;
    p->data = NULL;
    p->cmp = cmp;
    p->grow = grow ? grow : default_growth_rate;
    vector_resize(p, capacity);
    return p;
}

void vector_free(Vector *v)
{
    VALIDATE_VECTOR(v);
    free(v->data);
    free(v);
}

void *vector_at(Vector *v, size_t index)
{
    return (void *)(&v->data[index * v->elem_size]);
}

void *vector_at_s(Vector *v, size_t index)
{
    VALIDATE_VECTOR(v);
    if (index >= v->len)
    {
        return NULL;
    }
    return vector_at(v, index);
}

void vector_resize(Vector *v, size_t new_cap)
{
    VALIDATE_VECTOR(v);
    if (new_cap == v->capacity)
        return;
    if (!new_cap)
        new_cap++;
    byte *new_data = (byte *)realloc(v->data, new_cap * v->elem_size * sizeof(byte));
    assert(new_data != NULL && "vector_resize: Failed to resize vector array.");

    // Initialize the newly allocated memory
    size_t old_cap_start = v->capacity * v->elem_size * sizeof(byte);
    if (old_cap_start < new_cap)
    {
        size_t region_len = new_cap - v->capacity;
        memset(new_data + old_cap_start, 0, region_len * v->elem_size * sizeof(byte));
    }
    v->capacity = new_cap;
    v->data = new_data;
}

void vector_push_back(Vector *v, void *data)
{
    VALIDATE_VECTOR(v);
    if (!data) return;
    if (v->len >= v->capacity)
        vector_resize(v, v->grow(v));
    memcpy(vector_at(v, v->len), data, v->elem_size * sizeof(byte));
    v->len++;
}

void vector_sort(Vector *v)
{
    VALIDATE_VECTOR(v);
    if (!v->cmp)
    {
        perror("vector_sort: Compare function is undefined.");
        return;
    }
    qsort(v->data, v->len, v->elem_size, v->cmp);
}

void vector_remove(Vector *v, size_t index)
{
    VALIDATE_VECTOR(v);
    if (index >= v->len)
        return;

    if (index < v->len - 1)
    {
        memmove(vector_at(v, index), vector_at(v, index + 1), (v->len - index - 1) * v->elem_size * sizeof(byte));
    }

    v->len--;
}

void vector_insert(Vector *v, size_t index, void *data)
{
    VALIDATE_VECTOR(v);
    if (index > v->len)
        return;

    if (v->len >= v->capacity)
        vector_resize(v, v->grow(v));

    memmove(vector_at(v, index + 1), vector_at(v, index), (v->len - index) * v->elem_size * sizeof(byte));
    memcpy(vector_at(v, index), data, v->elem_size);

    v->len++;
}

void vector_clear(Vector *v)
{
    VALIDATE_VECTOR(v);
    memset(v->data, 0, v->capacity * v->elem_size * sizeof(byte));
    v->len = 0;
}

void vector_clamp(Vector *v)
{
    VALIDATE_VECTOR(v);
    byte *tmp;
    if (!v->len)
        tmp = (byte *)realloc(v->data, v->elem_size * sizeof(byte));
    else
        tmp = (byte *)realloc(v->data, v->len * v->elem_size * sizeof(byte));
    assert(tmp);
    v->data = tmp;
    v->capacity = v->len;
}

void *vector_pop_back(Vector *v)
{
    VALIDATE_VECTOR(v);
    if (v->len < 1)
        return NULL;
    return vector_at(v, --v->len);
}

void *vector_find(Vector *v, void *_find)
{
    VALIDATE_VECTOR(v);
    if (!v->cmp)
    {
        perror("vector_find: Compare function is undefined.");
        return NULL;
    }
    for (size_t i = 0; i < v->len; i++)
    {
        if (v->cmp(vector_at(v, i), _find) == 0)
        {
            return vector_at(v, i);
        }
    }
    return NULL;
}

void vector_remove_fast(Vector *v, size_t index)
{
    VALIDATE_VECTOR(v);
    if (index >= v->len)
        return;
    memcpy(vector_at(v, index), vector_at(v, v->len - 1), v->elem_size * sizeof(byte));
    v->len--;
}

/* Returns heap allocated deep copy */
Vector *vector_copy(Vector *v)
{
    VALIDATE_VECTOR(v);
    Vector *ret = (Vector *)malloc(sizeof(Vector));
    assert(ret);
    ret->grow = v->grow;
    ret->cmp = v->cmp;
    ret->elem_size = v->elem_size;
    ret->len = 0;
    ret->data = NULL;
    ret->capacity = 0;
    vector_resize(ret, v->capacity);
    assert(ret->data && ret->capacity == v->capacity);
    memcpy(ret->data, v->data, v->capacity);
    return ret;
}

void vector_reverse(Vector *v)
{
    VALIDATE_VECTOR(v);
    byte *start = v->data;
    byte *end = start + v->capacity - 1;

    while (start < end)
    {
        byte temp = *start;
        *start++ = *end;
        *end-- = temp;
    }
}

void vector_swap(Vector *v, size_t idx0, size_t idx1)
{
    VALIDATE_VECTOR(v);
    if (idx0 >= v->len)
        return;
    if (idx1 >= v->len)
        return;

    size_t write_size = v->elem_size;
    byte *idx0_entry = vector_at(v, idx0);
    byte *idx1_entry = vector_at(v, idx1);
    while (write_size)
    {
        size_t idx = write_size - 1;
        byte tmp = idx0_entry[idx];
        idx0_entry[idx] = idx1_entry[idx];
        idx1_entry[idx] = tmp;
        write_size--;
    }
}

void *vector_pop_front(Vector *v)
{
    VALIDATE_VECTOR(v);
    if (v->len < 1)
        return NULL;
    void *ret = vector_at(v, 0);
    vector_remove(v, 0);
    return ret;
}