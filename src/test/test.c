#include "../test/test.h"
#include "../lib/list.h"
#include "../lib/util.h"
#include "../lib/types.h"
#include "../drivers/vga.h"
#include "../cpu/port.h"

LinkedList g_tests;

void qemu_exit(bool ok) {
    port_byte_out(0xF4, ok? 0x10 : 0x11);
}

void _register_test(TestCase* test_case) {
    ll_add(&g_tests, test_case);
}

void init_tests() {
    init_kmalloc_tests();
}

TestResult run_case(TestCase* test_case) {
    TestResult result = test_case->test();
    if (result == TEST_PASS) {
        sprintf("[\033[32mPASS\033[0m]");
    } else {
        sprintf("[\033[31mFAIL\033[0m]");
    }

    sprintf(" %s -- %s\n", test_case->category, test_case->test_name);

    return result;
}

void test_run_all() {
    struct LinkedListNode* node = g_tests.front;
    int failing_tests = 0;
    for (int i = 0; i < g_tests.size; i++) {
        TestCase* test_case = ((TestCase*) (node->data));
        if (run_case(test_case) == TEST_FAIL) {
            failing_tests++;
        }

        node = node->next;
    }

    sprintf("\n%d/%d tests passing\n", g_tests.size - failing_tests, g_tests.size);
    qemu_exit(failing_tests == 0);
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
