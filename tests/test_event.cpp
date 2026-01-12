#include "observant.hpp"
#include "utilities.hpp"

namespace test {

void run_event_tests() {
    test::run("Event::register_callback", "registers callback", []() {
        test::Subject subject {};
        test::Observer observer_1 {};
        test::Observer observer_2 {};

        std::size_t expected { 0 };

        test::assert_false(subject.did_work.has_callbacks());
        test::assert_equal(subject.did_work.get_callback_count(), expected);

        test::assert_true(subject.did_work.add_callback(observer_1.get_callback()));

        test::assert_true(subject.did_work.has_callbacks());
        test::assert_equal(subject.did_work.get_callback_count(), ++expected);

        test::assert_true(subject.did_work.add_callback(observer_2.get_callback()));

        test::assert_true(subject.did_work.has_callbacks());
        test::assert_equal(subject.did_work.get_callback_count(), ++expected);
    });

    test::run("Event::register_callback", "does not register same callback more than once", []() {
        test::Subject subject {};
        test::Observer observer {};

        test::assert_false(subject.did_work.has_callbacks());
        test::assert_equal(subject.did_work.get_callback_count(), 0);

        subject.did_work.add_callback(observer.get_callback());

        constexpr std::size_t expected { 1 };

        test::assert_true(subject.did_work.has_callbacks());
        test::assert_equal(subject.did_work.get_callback_count(), expected);

        test::assert_false(subject.did_work.add_callback(observer.get_callback()));
        test::assert_equal(subject.did_work.get_callback_count(), expected);
    });

    test::run("Event::operator+=", "registers callback", []() {
        test::Subject subject {};
        test::Observer observer_1 {};
        test::Observer observer_2 {};

        std::size_t expected { 0 };

        test::assert_false(subject.did_work.has_callbacks());
        test::assert_equal(subject.did_work.get_callback_count(), expected);

        subject.did_work += observer_1.get_callback();

        test::assert_true(subject.did_work.has_callbacks());
        test::assert_equal(subject.did_work.get_callback_count(), ++expected);

        subject.did_work += observer_2.get_callback();

        test::assert_true(subject.did_work.has_callbacks());
        test::assert_equal(subject.did_work.get_callback_count(), ++expected);
    });

    test::run("Event::remove_callback", "removes callback", []() {
        test::Subject subject {};
        test::Observer observer_1 {};
        test::Observer observer_2 {};

        subject.did_work += observer_1.get_callback();
        subject.did_work += observer_2.get_callback();

        std::size_t expected { 2 };

        test::assert_true(subject.did_work.has_callbacks());
        test::assert_equal(subject.did_work.get_callback_count(), expected);

        test::assert_true(subject.did_work.remove_callback(observer_1.get_callback()));

        test::assert_true(subject.did_work.has_callbacks());
        test::assert_equal(subject.did_work.get_callback_count(), --expected);

        test::assert_true(subject.did_work.remove_callback(observer_2.get_callback()));

        test::assert_false(subject.did_work.has_callbacks());
        test::assert_equal(subject.did_work.get_callback_count(), --expected);
    });

    test::run("Event::operator-=", "removes callback", []() {
        test::Subject subject {};
        test::Observer observer_1 {};
        test::Observer observer_2 {};

        subject.did_work += observer_1.get_callback();
        subject.did_work += observer_2.get_callback();

        std::size_t expected { 2 };

        test::assert_true(subject.did_work.has_callbacks());
        test::assert_equal(subject.did_work.get_callback_count(), expected);

        subject.did_work -= observer_1.get_callback();

        test::assert_true(subject.did_work.has_callbacks());
        test::assert_equal(subject.did_work.get_callback_count(), --expected);

        subject.did_work -= observer_2.get_callback();

        test::assert_false(subject.did_work.has_callbacks());
        test::assert_equal(subject.did_work.get_callback_count(), --expected);
    });

    test::run("Event::operator()", "invokes all callbacks", []() {
        test::Subject subject {};
        test::Observer observer_1 {};
        test::Observer observer_2 {};

        subject.did_work += observer_1.get_callback();
        subject.did_work += observer_2.get_callback();

        subject.do_work();
        subject.do_work();
    });

    test::run("Event::clear_callbacks", "removes all callbacks", []() {
        test::Subject subject {};
        test::Observer observer_1 {};
        test::Observer observer_2 {};
        test::Observer observer_3 {};

        subject.did_work += observer_1.get_callback();
        subject.did_work += observer_2.get_callback();
        subject.did_work += observer_3.get_callback();

        test::assert_equal(subject.did_work.get_callback_count(), 3);

        subject.did_work.clear_callbacks();

        test::assert_equal(subject.did_work.get_callback_count(), 0);
    });

    test::run("Event::~Event", "cleans up callbacks upon being destroyed", []() {
        test::Observer observer_1 {};
        test::Observer observer_2 {};

        test::assert_true(observer_1.get_callback().get_registered_event_count() == 0);
        test::assert_true(observer_2.get_callback().get_registered_event_count() == 0);

        {
            test::Subject subject {};

            subject.did_work += observer_1.get_callback();
            subject.did_work += observer_2.get_callback();

            test::assert_true(observer_1.get_callback().get_registered_event_count() == 1);
            test::assert_true(observer_2.get_callback().get_registered_event_count() == 1);
        }

        test::assert_true(observer_1.get_callback().get_registered_event_count() == 0);
        test::assert_true(observer_2.get_callback().get_registered_event_count() == 0);
    });

    test::run("CallBack", "registers events", []() {
        test::Subject subject {};
        test::Observer observer_1 {};
        test::Observer observer_2 {};

        subject.did_work += observer_1.get_callback();
        subject.did_work += observer_2.get_callback();

        test::assert_true(observer_1.get_callback().is_registered());
        test::assert_true(observer_1.get_callback().get_registered_event_count() > 0);
        test::assert_true(observer_2.get_callback().is_registered());
        test::assert_true(observer_2.get_callback().get_registered_event_count() > 0);
    });

    test::run("Callback::~CallBack", "cleans up events upon being destroyed", []() {
        test::Subject subject {};

        test::assert_true(subject.did_work.get_callback_count() == 0);

        {
            test::Observer observer_1 {};
            test::Observer observer_2 {};

            subject.did_work += observer_1.get_callback();
            subject.did_work += observer_2.get_callback();
            
            test::assert_true(subject.did_work.get_callback_count() == 2);
        }

        test::assert_true(subject.did_work.get_callback_count() == 0);
    });
}

}
