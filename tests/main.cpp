#include "test_event.hpp"
#include "test_raw_event.hpp"

int main() {
    test::run_raw_event_tests();
    test::run_event_tests();
    return 0;
}
