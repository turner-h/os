#include "../test/test.h"
#include "../cpu/memory.h"
#include "../drivers/vga.h"

#define CATEGORY "kmalloc"

TestResult test_free() {
    void* addr = kmalloc(sizeof(u32));
    kfree(addr);

    void* new_addr = kmalloc(sizeof(u32));
    ASSERT_EQUALS(addr, new_addr);

    kfree(new_addr);
    return TEST_PASS;
}

TestResult test_multiple_allocs() {
    int* number = kmalloc(sizeof(int));
    TestCase* test = kmalloc(sizeof(TestCase));
    int* number2 = kmalloc(sizeof(int));

    *number = 5;
    test->category_length = 5;
    *number2 = 4;

    ASSERT_EQUALS(*number, 5);
    ASSERT_EQUALS(test->category_length, 5);
    ASSERT_EQUALS(*number2, 4);
    
    kfree(number);
    kfree(test);
    kfree(number2);

    return TEST_PASS;
}

TestResult test_free_same_location() {
    void* n1 = kmalloc(sizeof(u32));
    kfree(n1);

    void* n2 = kmalloc(sizeof(u32));
    ASSERT_EQUALS(n1, n2);

    kfree(n2);
    void* n3 = kmalloc(sizeof(u32));
    ASSERT_EQUALS(n1, n3);

    kfree(n3);
    return TEST_PASS;
}

TestResult test_perfect_fragmentation() {
    void* n1 = kmalloc(sizeof(u32));
    void* n2 = kmalloc(sizeof(u32));

    ASSERT_EQUALS(n2, n1 + (3 * sizeof(u32)));

    kfree(n1);

    void* n3 = kmalloc(sizeof(u32));

    ASSERT_EQUALS(n1, n3);

    void* n4 = kmalloc(sizeof(u32));

    ASSERT_EQUALS(n4, n2 + (3 * sizeof(u32)));

    kfree(n4);
    kfree(n3);
    kfree(n2);

    return TEST_PASS;
}

TestResult test_free_aggregation() {
    void* n1 = kmalloc(sizeof(u32));
    void* n2 = kmalloc(sizeof(u32));
    void* n3 = kmalloc(sizeof(u32));
    
    void* big1 = kmalloc(5 * sizeof(u32));
    ASSERT_EQUALS(big1, n3 + (3 * sizeof(u32)));

    kfree(n1);
    kfree(n2);

    void* big2 = kmalloc(5 * sizeof(u32));
    ASSERT_EQUALS(big2, n1);

    kfree(big2);

    n1 = kmalloc(sizeof(u32));
    n2 = kmalloc(sizeof(u32));
   
    kfree(n2);
    kfree(n1);

    big2 = kmalloc(5 * sizeof(u32));
    ASSERT_EQUALS(big2, n1);

    return TEST_PASS;
}

void init_kmalloc_tests() {
    register_test_case(test_perfect_fragmentation, "fixed size fragmentation");
    register_test_case(test_free_aggregation, "aggregates groups of free space");
    register_test_case(test_free, "frees memory");
    register_test_case(test_multiple_allocs, "mulitple allocations");
    register_test_case(test_free_same_location, "frees from the same location twice");
}
