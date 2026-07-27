#ifndef TEST_H
#define TEST_H

typedef enum {
    TEST_PASS = 1,
    TEST_FAIL = 0
} TestResult;

typedef struct {
    TestResult (*test)();
    char* test_name;
    char* category;
    int category_length;
} TestCase;

void _register_test(TestCase* test_case);

void test_run(char* category);
void test_run_all();

void init_tests();
void init_kmalloc_tests();

#define register_test_case(test, name)                                          \
    TestCase* test_case_##test = kmalloc(sizeof(TestCase));                     \
    *test_case_##test = (TestCase) {test, name, CATEGORY, sizeof(CATEGORY)};    \
    _register_test(test_case_##test);                                                   

#define ASSERT(condition)   \
if (!condition) {           \
    return TEST_FAIL;       \
}       

#define ASSERT_FALSE(condition) \
if (condition) {                \
    return TEST_FAIL;           \
}       

#define ASSERT_EQUALS(con1, con2)   \
if (con1 != con2) {                 \
    return TEST_FAIL;               \
}

#endif
