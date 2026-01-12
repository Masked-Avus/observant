#include "utilities.hpp"
#include <iostream>

namespace test {
namespace raw {

void callback_1() {
    std::cout << "[Callback 1] Hello!\n";
}

void callback_2() {
    std::cout << "[Callback 2] Hola!\n";
}

void callback_3() {
    std::cout << "[Callback 3] Konichiwa!\n";
}

void callback_1(int x, int y) {
    std::cout << "[Callback 1] " << x << " + " << y << " = " << (x + y) << '\n';
}

void callback_2(int x, int y) {
    std::cout << "[Callback 2] " << x << " - " << y << " = " << (x - y) << '\n';
}

void callback_3(int x, int y) {
    std::cout << "[Callback 3] " << x << " * " << y << " = " << (x * y) << '\n';
}

void callback_1(const std::string& message) {
    std::cout << "[CALLBACK 1] " << message << '\n';
}

void callback_2(const std::string& message) {
    std::cout << "[CALLBACK 2] " << message << '\n';
}

}

void Raw_Subject::do_work() {
    std::cout
        << "[Raw_Subject::do_work] Did work. (Count: "
        << ++m_did_work_count
        << ')' << '\n';
    did_work();
}

void Raw_Subject::do_other_work(int x, int y) {
    std::cout
        << "[Raw_Subject::do_otherwork] Did work with "
        << x
        << " and "
        << y
        << ". (Count: "
        << ++m_did_other_work_count
        << ')' << '\n';
    did_other_work(x, y);
}

void Raw_Subject::send_message() {
    std::cout
        << "[Raw_Subject::send_message] Did work. (Count: "
        << ++m_sent_message_count
        << ')' << '\n';
    sent_message("Hey there!");
}

void Subject::do_work() {
    std::cout << "[Subject] Did work. (Count: " << ++m_did_work_count << ')' << '\n';
    did_work(m_did_work_count);
}

void Observer::register_event(Subject& subject) {
    subject.did_work.add_callback(m_callback);
}

void Observer::on_do_work(int data) {
    std::cout
        << "Event firing recognized by [Observer]. (Times observed: "
        << ++m_incrementor
        << ')' << '\n';
}

void assert_true(bool is_true) {
    if (!is_true) {
        throw Assertion_Failure("Expected true but got false instead");
    }
}

void assert_false(bool is_false) {
    if (is_false) {
        throw Assertion_Failure("Expected false but got true instead");
    }
}

void assert_equal(std::size_t expected, std::size_t actual) {
    if (expected != actual) {
        throw Assertion_Failure(
            std::string("Expected ")
                .append(std::to_string(expected))
                .append(" but got ")
                .append(std::to_string(actual))
                .append(" instead")
        );
    }
}

void run(const char* subject, const char* expected_result, std::function<void()> test) {
    try {
        test();
        std::cout << subject << ": " << expected_result << " -- " << "SUCCESS";
    }
    catch (const Assertion_Failure& exception) {
        std::cout << subject << ": " << expected_result << " -- " << "ASSERTION FAILURE (" << exception.what() << ')';
    }
    catch (...) {
        std::cout << subject << ": " << expected_result << " -- " << "UNKNOWN EXCEPTION";
    }

    std::cout << '\n';
}

}
