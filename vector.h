#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdint.h>
#include <stddef.h>

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

#define INVALID_FE_IDX ((size_t) -1)

typedef uint8_t byte;

typedef struct Vec Vec;

/* 0 if eq, -1 if less, 1 if greater than */
typedef int (*void_cmp_func)(const void * a, const void *b);

int vec_char_cmp(const void *data0, const void *data1);

int vec_int_cmp(const void *data0, const void *data1);

int vec_uint_cmp(const void *data0, const void *data1);

int vec_ll_cmp(const void *data0, const void *data1);

int vec_ull_cmp(const void *data0, const void *data1);


/* returns the new capcaity based on the current vec state */
typedef size_t (*vec_growth_rate_func)(Vec *);

struct Vec
{
    byte *data;
    size_t len;       /* current number of entries */
    size_t capacity;  /* max number of entries */
    size_t elem_size; /* elem_size * capacity is the current memory allocation in bytes */
    void_cmp_func cmp;
    vec_growth_rate_func grow;
    void (*free_entry)(const void *);
    size_t fe_idx;    /* use by VEC_FOR_EACH to ensure index after altering the vector */
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

#ifndef __STDC__
/* Accommodates adding and removing entries but not inserting before the current index.
    You can still insert before the current index, but it will make you iterate over the current entry again
    and you will not be able to iterate over the entry you inserted before the current index. */
#define V_FOR_EACH(vec, type, var_name)                 \
    (vec)->fe_idx = 0;                                  \
    for (type *var_name = vec_at_s(vec, (vec)->fe_idx); \
         var_name != NULL;                              \
         var_name = vec_at_s(vec, ++((vec)->fe_idx)))
#endif
/* Same as V_FOR_EACH, but expects the user to allocate the var_name as a pointer to type */
#define V_FOR_EACH_ANSI(vec, var_name)            \
    (vec)->fe_idx = 0;                            \
    for (var_name = vec_at_s(vec, (vec)->fe_idx); \
         var_name != NULL;                        \
         var_name = vec_at_s(vec, ++((vec)->fe_idx)))
/**
 * @brief Dynamic array of bytes.
 * 
 * @param capacity Capacity of the vector 
 * @param elem_size Size of each element in bytes
 * @param cmp Function to compare elements
 * @param grow Function to determine the new capacity of the vector
 * @param free_entry Function to free the memory of an element
 * @return Vec* 
 */
Vec *vec_new(size_t capacity, size_t elem_size, void_cmp_func cmp, vec_growth_rate_func grow, void (*free_entry)(const void *));

/**
 * @brief Free the memory of the vector.
 * 
 * @param v Vector to free 
 */
void vec_free(Vec *v);
/**
 * @brief Data must be a valid memory address, it will be copied into the vector.
 * 
 * @param v Vector to push data into.
 * @param data A valid memory address which will be copied into the vector.
 * @return void 
 */
void vec_push_back(Vec *v, void *data);

/**
 * @brief Ensures the current order of the vec
 * 
 * @param v Vector to remove data from.
 * @param index Index of the data to remove.
 */
void vec_remove(Vec *v, size_t index);

/**
 * @brief doesn't ensure the vec order
 * 
 * @param v 
 * @param index 
 */
void vec_remove_fast(Vec *v, size_t index);

/**
 * @brief Resizes the vector to the new size.
 * 
 * @param v Vector to resize.
 * @param new_size New size of the vector.
 */
void vec_resize(Vec *v, size_t new_size);

/**
 * @brief Sorts the vector using libc qsort.
 * 
 * @param v Vector to sort.
 */
void vec_sort(Vec *v);

/**
 * @brief Inserts data into the vector at the specified index.
 * 
 * @param v Vector to insert data into.
 * @param index Index to insert data at.
 * @param data Data must be a valid memory address.
 */
void vec_insert(Vec *v, size_t index, void *data);
/**
 * @brief Removes all entries from the vector, calls their free functions if one was given.
 * 
 * @param v Vector to clear.
 */
void vec_clear(Vec *v);

/**
 * @brief Resizes the vector to the current length.
 * 
 * @param v Vector to resize.
 */
void vec_clamp(Vec *v);

/**
 * @brief Reverses the order of the vector.
 * 
 * @param v Vector to reverse.
 */
void vec_reverse(Vec *v);

/**
 * @brief Swaps the elements at the specified indices.
 * 
 * @param v Vector to swap elements in.
 * @param idx0 Index of the first element.
 * @param idx1 Index of the second element.
 */
void vec_swap(Vec *v, size_t idx0, size_t idx1);

/**
 * @brief Returns a pointer to the element at the specified index.
 * 
 * @param v Vector to get the element from.
 * @param index Index of the element.
 * @return void* Pointer to the element.
 * 
 * @warning Not as safe as vec_at_s.
 */
void *vec_at(Vec *v, size_t index);

/**
 * @brief Returns a pointer to the element at the specified index.
 * 
 * @param v Vector to get the element from.
 * @param index Index of the element.
 * @return void* Pointer to the element.
 */
void *vec_at_s(Vec *v, size_t index);
/**
 * @brief Removes the last element from the vector.
 * 
 * @param v Vector to remove the element from.
 * @return void* Pointer to the element. NULL on fail.
 */
void *vec_pop_back(Vec *v);
/* NULL on fail, ptr to element on success */
/**
 * 
 * @param v Vector to remove the element from.
 * @return void* Pointer to the first element. NULL on fail.
 */
void *vec_pop_front(Vec *v);

/**
 * @brief Finds the first element that matches the data.
 * 
 * @param v Vector to search.
 * @param _find Data to search for.
 * @return void* Pointer to the element. NULL on fail.
 * 
 * @warning Expects a cmp function to be assigned to the vector.
 */
void *vec_find(Vec *v, void *_find);
/**
 * @brief Returns a heap allocated deep copy of the vector.
 * 
 * @param v Vector to copy.
 * @return Vec* Pointer to the new vector.
 */
Vec *vec_copy(Vec *v);
/**
 * @brief Returns the size of the vector.
 * 
 * @param v Vector to get the size of.
 * @return size_t Size of the vector.
 */
size_t vec_size(Vec *v);
/**
 * @brief Adds all of the source vector to the destination vector.
 * 
 * @param dest Destination vector.
 * @param source Source vector.
 * @return int 0 on success, 1 on fail, and 2 on partial fail.
 */
int vec_append(Vec *dest, Vec *source);
/**
 * @brief Returns a heap allocated deep copy of the vector.
 * 
 * @param v Vector to copy.
 * @param ret_elem_count Pointer to the size of the vector.
 * @return void* Pointer to the new vector. Must be freed using "free".
 * 
 * @warning User must free the returned pointer.
 */
void *vec_arr_copy(Vec *v, size_t *ret_elem_count);

#endif /* VECTOR_H */