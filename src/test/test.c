#include "../test/test.h"
#include "../lib/list.h"
#include "../lib/util.h"
#include "../drivers/vga.h"

LinkedList g_tests;

void _register_test(TestCase* test_case) {
    ll_add(&g_tests, test_case);
}

void print_pass() {
    kprintf("[");
    vga_set_color(2, 0);
    kprintf("PASS");
    vga_set_color(15, 0);
    kprintf("]");
}

void print_fail() {
    kprintf("[");
    vga_set_color(4, 0);
    kprintf("FAIL");
    vga_set_color(15, 0);
    kprintf("]");
}

void init_tests() {
    init_kmalloc_tests();
}

void run_case(TestCase* test_case) {
    TestResult result = test_case->test();
    if (result == TEST_PASS) {
        print_pass();
    } else {
        print_fail();
    }

    kprintf(" %s -- %s\n", test_case->category, test_case->test_name);
}

void test_run_all() {
    struct LinkedListNode* node = g_tests.front;
    for (int i = 0; i < g_tests.size; i++) {
        TestCase* test_case = ((TestCase*) (node->data));
        run_case(test_case);

        node = node->next;
    }
}

void test_run(char* category) {
    struct LinkedListNode* node = g_tests.front;
    for (int i = 0; i < g_tests.size; i++) {
        TestCase* test_case = ((TestCase*) (node->data));

        if (string_cmp(category, test_case->category, test_case->category_length)) {
            run_case(test_case);
        }

        node = node->next;
    }
}
