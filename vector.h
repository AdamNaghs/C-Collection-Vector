#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdint.h>

#define VECTOR_DEFAULT_CAP 10

typedef uint8_t byte;

typedef struct Vector Vector;

/* 0 if eq, -1 if less, 1 if greater than */
typedef int (*void_cmp_func)(const void *, const void *);

/* returns the new capcaity based on the current vector state */
typedef size_t (*vector_growth_rate_func)(Vector *);

struct Vector
{
    byte *data;
    size_t len;       // current number of entries
    size_t capacity;  // max number of entries
    size_t elem_size; // elem_size * capacity is the current memory allocation in bytes
    void_cmp_func cmp;
    vector_growth_rate_func grow;
    size_t fe_idx;
};

#define V(type) (vector_new(VECTOR_DEFAULT_CAP, sizeof(type), NULL, NULL))
#define V_ADD(v, data) (vector_push_back(v, data))
#define V_INS(v, idx, data) (vector_insert(v, idx, data))
#define V_RM(v, idx) (vector_remove(v, idx))
#define V_RMF(v, idx) (vector_remove_fast(v, idx))
#define V_AT(v, index, t) ((index < (v)->capacity) ? (t *)vector_at(v, index) : NULL)
#define V_CONT(v, data) (vector_find(v, data) ? 1 : 0)
#define V_POP(v) (vector_pop_back(v))
#define V_POP_FRONT(v) (vector_pop_front(v))
#define V_COPY(v) (vector_copy(v))
#define V_CLEAR(v) (vector_clear(v))
#define V_REV(v) (vector_reverse(v))

/* Accommodates adding and removing entries but not inserting before the current index.
    You can still insert before the current index, but it will make you iterate over the current entry again
    and you will not be able to iterate over the entry you inserted before the current index. */
#define V_FOR_EACH(vec, type, var_name) \
(vec)->fe_idx = 0; \
    for (type * var_name = vector_at_s(vec, (vec)->fe_idx); \
         var_name != NULL; \
         var_name = ((++(vec)->fe_idx), vector_at_s(vec, (vec)->fe_idx)) ?: ((vec)->fe_idx = (size_t)-1, NULL))

Vector *vector_new(size_t capacity, size_t elem_size, void_cmp_func cmp, vector_growth_rate_func grow);
void vector_free(Vector *v);
/* data must be a valid memory address, it will be copied into the array */
void vector_push_back(Vector *v, void *data);
/* ensures the current order of the vector */
void vector_remove(Vector *v, size_t index);
/*  doesn't ensure the vector order */
void vector_remove_fast(Vector *v, size_t index);
void vector_resize(Vector *v, size_t new_size);
void vector_sort(Vector *v);
/* data must be a valid memory address, it will be copied into the array */
void vector_insert(Vector *v, size_t index, void *data);
/* removes all entries, does not free memory */
void vector_clear(Vector *v);
/* resizes array to len */
void vector_clamp(Vector *v);
void vector_reverse(Vector *v);
void vector_swap(Vector *v, size_t idx0, size_t idx1);
/* Has return type */
void *vector_at(Vector *v, size_t index);
/* safer */
void *vector_at_s(Vector *v, size_t index);
// "returns" a ptr of type 't' at 'index' from 'v'
/* NULL on fail, ptr to element on success */
void *vector_pop_back(Vector *v);
/* NULL on fail, ptr to element on success */
void *vector_pop_front(Vector *v);
/* NULL on fail, ptr to element on success */
void *vector_find(Vector *v, void *_find);
/* Returns heap allocated deep copy */
Vector *vector_copy(Vector *v);

size_t vector_size(Vector *v);

#endif /* VECTOR_H */