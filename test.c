#include "vector.h"
#include "../Testing/testing.h"
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

TEST_MAKE(Struct_Vec)
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
        TEST_ASSERT_CLEAN(ptr->index == i - 1, vec_free(td_vec));
        TEST_ASSERT_CLEAN(strcmp(ptr->name, new_data(i - 1).name) == 0, vec_free(td_vec));
        TEST_ASSERT_CLEAN_MSG(td_vec->len == (i + 1), vec_free(td_vec), "len: %zu, i: %d", td_vec->len, i);
    }
    vec_free(td_vec);
    TEST_PASS();
}

TEST_MAKE(Insert_Vec)
{
    Vec *int_vec = VEC(int);
    int value, i;
    for (i = 0; i < 11; i++)
    {
        value = i;
        V_ADD(int_vec, &value);
        TEST_ASSERT_CLEAN(*(int*)vec_at(int_vec,i) == i, vec_free(int_vec));
    }
    int insert_val = 100;
    V_INS(int_vec, 5, &insert_val);
    for (i = 0; i < 11; i++)
    {
        if (i < 5)
            TEST_ASSERT_CLEAN(*(int*)vec_at(int_vec,i) == i, vec_free(int_vec));
        else if (i == 5)
            TEST_ASSERT_CLEAN(*(int*)vec_at(int_vec,i) == 100, vec_free(int_vec));
        else
            TEST_ASSERT_CLEAN(*(int*)vec_at(int_vec,i) == i - 1, vec_free(int_vec));
    }
    vec_free(int_vec);
}


TEST_MAKE(Str_Vec)
{
    Vec* str_vec = VEC(char*);
    str_vec->free_entry = vec_deref_free;
    int i;
    for (i = 0; i < 101; i++)
    {
        char* str = malloc(100);
        snprintf(str, 100, "index%d", i);
        V_ADD(str_vec, &str);
        if (i == 0)
            continue;
        char** ptr = vec_at(str_vec, i);
        TEST_ASSERT_CLEAN(strcmp(*ptr, str) == 0, vec_free(str_vec));
        TEST_ASSERT_CLEAN(str_vec->len == i + 1, vec_free(str_vec));
        TEST_ASSERT_CLEAN(str_vec->capacity >= i + 1, vec_free(str_vec));
    }
    vec_free(str_vec);
    TEST_PASS();
}

TEST_MAKE(Rev_Vec)
{
    Vec* int_vec = VEC(int);
    int value, i;
    for (i = 0; i < 11; i++)
    {
        value = i;
        V_ADD(int_vec, &value);
    }
    V_REV(int_vec);
    for (i = 0; i < 11; i++)
    {
        TEST_ASSERT_CLEAN(*(int*)vec_at(int_vec,i) == 10 - i, vec_free(int_vec));
    }
    vec_free(int_vec);
    TEST_PASS();
}

TEST_SUITE_MAKE(Vec)
{
    TEST_SUITE_LINK(Vec, Struct_Vec);
    TEST_SUITE_LINK(Vec, Insert_Vec);
    TEST_SUITE_LINK(Vec, Str_Vec);
    TEST_SUITE_LINK(Vec, Rev_Vec);
    TEST_SUITE_END(Vec);
}

int main()
{
    TEST_SUITE_RUN(Vec);
    return 0;
}
