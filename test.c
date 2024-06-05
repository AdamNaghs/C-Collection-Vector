#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

struct test_data
{
    char name[100];
    size_t index;
};

struct test_data new_data(size_t index)
{
    struct test_data ret = {.index = index};
    snprintf(ret.name, 100, "index%zu", index);
    return ret;
}

void test2(void)
{
    Vec *td_vec = VEC(struct test_data);
    int i;
    for (i = 0; i < 101; i++)
    {
        struct test_data td = new_data(i);
        V_ADD(td_vec, &td);
        if (i == 0)
            continue;
        struct test_data *ptr = vec_at(td_vec, i - 1);
        printf("Test1 - %d. %s - %zu\n", i, ptr->name, ptr->index);
        printf("%zu, %zu\n", td_vec->len, td_vec->capacity);
    }
    int c = 0;
    i = 0;
    struct test_data *entry;
    V_FOR_EACH_ANSI(td_vec, entry)
    {
        if (td_vec->fe_idx >= 100)
        {
            i++;
            if (i >= 20)
                break;
        }
        printf("idx: %d\n", td_vec->fe_idx);
        printf("Real idx: %d\n",c);
        printf("Vec Size: %llu\n", td_vec->len);
        printf("Name: %s, Idx: %llu\n", entry->name, entry->index);
        vec_remove(td_vec, td_vec->fe_idx);
        c++;
    }
    
    printf("%llu\n", td_vec->fe_idx);
    vec_free(td_vec);
}

void test1(void)
{
    Vec *int_vec = VEC(int);
    int value, i;
    for (i = 0; i < 101; i++)
    {
        value = i;
        V_ADD(int_vec, &value);
        printf("Test1. %d\n", *V_AT(int_vec, i, int));
        printf("\t%zu, %zu\n", int_vec->len, int_vec->capacity);
    }
    for (i = 0; i < 101; i++)
    {
        V_RM(int_vec, 0);
        printf("Test2. %d\n", *V_AT(int_vec, 0, int));
        printf("\t%zu, %zu\n", int_vec->len, int_vec->capacity);
    }
    for (i = 0; i < 101; i++)
    {
        vec_insert(int_vec, 0, &i);
        printf("Test3. %d\n", *V_AT(int_vec, 0, int));
        printf("\t%zu, %zu\n", int_vec->len, int_vec->capacity);
    }
    while (int_vec->len)
    {
        printf("Test4. %d\n", *V_AT(int_vec, 0, int));
        printf("\t%zu, %zu\n", int_vec->len, int_vec->capacity);
        V_RMF(int_vec, 0);
    }
    vec_free(int_vec);
}

void test3(void)
{
    Vec *int_vec = VEC(int);
    int value, i;
    printf("Test3\n");
    for (i = 0; i < 11; i++)
    {
        value = i;
        V_ADD(int_vec, &value);
        printf("\tidx:%zu, cap:%zu\n", *(int*)vec_at(int_vec,i), int_vec->capacity);
    }
    V_REV(int_vec);
    printf("Reversed\n");
    for (i = 0; i < 11; i++)
    {
        printf("\tidx:%zu, cap:%zu\n", *(int*)vec_at(int_vec,i), int_vec->capacity);
    }
}

int main()
{
    test1();
    test2();
    test3();
    return 0;
}
