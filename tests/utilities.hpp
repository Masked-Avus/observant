#pragma once

#include "observant.hpp"

#include <string>
#include <functional>
#include <stdexcept>

namespace test {
namespace raw {

void callback_1();
void callback_2();
void callback_3();

void callback_1(int, int);
void callback_2(int, int);
void callback_3(int, int);

void callback_1(const std::string&);
void callback_2(const std::string&);

}

class Raw_Subject {
public:
    observant::Raw_Event<> did_work {};
    observant::Raw_Event<int, int> did_other_work {};
    observant::Raw_Event<const std::string&> sent_message {};

    void do_work();
    void do_other_work(int x, int y);
    void send_message();

private:
    int m_did_work_count {};
    int m_did_other_work_count {};
    int m_sent_message_count {};
};

class Subject {
public:
    observant::Event<int> did_work {};

    void do_work();

    
private:
    int m_did_work_count {};
};

class Observer {
public:
    observant::Callback<int>& get_callback() { return m_callback; }

    void register_event(Subject& subject);

private:
    observant::Callback<int> m_callback { [this](int data) mutable { this->on_do_work(data); } };
    int m_incrementor {};

    void on_do_work(int data);
};

class Assertion_Failure : public std::runtime_error {
public:
    Assertion_Failure(const char* message) : std::runtime_error { message } { }
    Assertion_Failure(const std::string& message) : std::runtime_error { message } { }
};

void run(const char* subject, const char* expected_result, std::function<void()> test);

void assert_true(bool is_true);
void assert_false(bool is_false);
void assert_equal(std::size_t expected, std::size_t actual);

}
