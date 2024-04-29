#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdint.h>

/*
    INFO:
        The Vector is an array of bytes. 
        When adding a new member to the vector you must provide a valid memory address to copy the data from.
        The vector will then contain a copy of the data at that index.
        If the vector contains pointers to memory, you need to be careful to free memory before you remove the entry from the array.

    TODO:
        Add another header and c file, copy and paste this vector into there
        and rewrite it to return errors.
*       Don't just change these files because I link directly to to these files in some projects.
*/
/*
    Define DISABLE_ASSERT to not use asserts.
*/

#define VECTOR_DEFAULT_CAP 10

#define INVALID_FE_IDX ((size_t)-1)

typedef uint8_t byte;

typedef struct Vec Vec;

/* 0 if eq, -1 if less, 1 if greater than */
typedef int (*void_cmp_func)(const void *, const void *);

/* returns the new capcaity based on the current vec state */
typedef size_t (*vec_growth_rate_func)(Vec *);

struct Vec
{
    byte *data;
    size_t len;       // current number of entries
    size_t capacity;  // max number of entries
    size_t elem_size; // elem_size * capacity is the current memory allocation in bytes
    void_cmp_func cmp;
    vec_growth_rate_func grow;
    void (*free_entry)(const void*);
    size_t fe_idx; // use by VEC_FOR_EACH to ensure index after altering the vector
};

#define VEC(type) (vec_new(VECTOR_DEFAULT_CAP, sizeof(type), NULL, NULL, NULL))
#define V_ADD(v, data) (vec_push_back(v, data))
#define V_INS(v, idx, data) (vec_insert(v, idx, data))
#define V_RM(v, idx) (vec_remove(v, idx))
#define V_RMF(v, idx) (vec_remove_fast(v, idx))
#define V_AT(v, index, t) ((index < (v)->capacity) ? (t *)vec_at(v, index) : NULL)
#define V_CONT(v, data) (vec_find(v, data) ? 1 : 0)
#define V_POP(v) (vec_pop_back(v))
#define V_POP_FRONT(v) (vec_pop_front(v))
#define V_COPY(v) (vec_copy(v))
#define V_CLEAR(v) (vec_clear(v))
#define V_REV(v) (vec_reverse(v))

/* Accommodates adding and removing entries but not inserting before the current index.
    You can still insert before the current index, but it will make you iterate over the current entry again
    and you will not be able to iterate over the entry you inserted before the current index. */
#define V_FOR_EACH(vec, type, var_name)                 \
    (vec)->fe_idx = 0;                                  \
    for (type *var_name = vec_at_s(vec, (vec)->fe_idx); \
         var_name != NULL;                              \
         var_name = vec_at_s(vec,++((vec)->fe_idx)))

Vec *vec_new(size_t capacity, size_t elem_size, void_cmp_func cmp, vec_growth_rate_func grow, void (*free_entry)(const void*));
void vec_free(Vec *v);
/* data must be a valid memory address, it will be copied into the array */
void vec_push_back(Vec *v, void *data);
/* ensures the current order of the vec */
void vec_remove(Vec *v, size_t index);
/*  doesn't ensure the vec order */
void vec_remove_fast(Vec *v, size_t index);
void vec_resize(Vec *v, size_t new_size);
void vec_sort(Vec *v);
/* data must be a valid memory address, it will be copied into the array */
void vec_insert(Vec *v, size_t index, void *data);
/* removes all entries, does not free memory */
void vec_clear(Vec *v);
/* resizes array to len */
void vec_clamp(Vec *v);
void vec_reverse(Vec *v);
void vec_swap(Vec *v, size_t idx0, size_t idx1);
/* Has return type */
void *vec_at(Vec *v, size_t index);
/* safer */
void *vec_at_s(Vec *v, size_t index);
// "returns" a ptr of type 't' at 'index' from 'v'
/* NULL on fail, ptr to element on success */
void *vec_pop_back(Vec *v);
/* NULL on fail, ptr to element on success */
void *vec_pop_front(Vec *v);
/* NULL on fail, ptr to element on success */
void *vec_find(Vec *v, void *_find);
/* Returns heap allocated deep copy */
Vec *vec_copy(Vec *v);

size_t vec_size(Vec *v);

#endif /* VECTOR_H */