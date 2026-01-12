#include "observant.hpp"
#include "utilities.hpp"

namespace test {

void run_raw_event_tests() {
    test::run("Raw_Event::register_callback", "registers callback", []() {
        test::Raw_Subject subject {};

        std::size_t expected { 0 };

        test::assert_false(subject.did_work.has_callbacks());
        test::assert_equal(subject.did_work.get_callback_count(), expected);

        test::assert_true(subject.did_work.add_callback(test::raw::callback_1));

        test::assert_true(subject.did_work.has_callbacks());
        test::assert_equal(subject.did_work.get_callback_count(), ++expected);

        test::assert_true(subject.did_work.add_callback(test::raw::callback_2));

        test::assert_true(subject.did_work.has_callbacks());
        test::assert_equal(subject.did_work.get_callback_count(), ++expected);

        test::assert_true(subject.did_work.add_callback(test::raw::callback_3));

        test::assert_true(subject.did_work.has_callbacks());
        test::assert_equal(subject.did_work.get_callback_count(), ++expected);
    });

    test::run("Raw_Event::register_callback", "does not register the same callback more than once", []() {
        test::Raw_Subject subject {};
        constexpr std::size_t expected { 1 };

        test::assert_true(subject.did_work.add_callback(test::raw::callback_1));
        test::assert_equal(subject.did_work.get_callback_count(), expected);

        test::assert_false(subject.did_work.add_callback(test::raw::callback_1));
        test:assert_equal(subject.did_work.get_callback_count(), expected);
    });

    test::run("Raw_Event::operator+=", "registers callback", []() {
        test::Raw_Subject subject {};

        std::size_t expected { 0 };

        test::assert_false(subject.did_other_work.has_callbacks());
        test::assert_equal(subject.did_other_work.get_callback_count(), expected);

        subject.did_other_work += test::raw::callback_1;

        test::assert_true(subject.did_other_work.has_callbacks());
        test::assert_equal(subject.did_other_work.get_callback_count(), ++expected);

        subject.did_other_work += test::raw::callback_2;

        test::assert_true(subject.did_other_work.has_callbacks());
        test::assert_equal(subject.did_other_work.get_callback_count(), ++expected);

        subject.did_other_work += test::raw::callback_3;

        test::assert_true(subject.did_other_work.has_callbacks());
        test::assert_equal(subject.did_other_work.get_callback_count(), ++expected);
    });

    test::run("Raw_Event::remove_callback", "removes callback", []() {
        test::Raw_Subject subject {};

        subject.did_work.add_callback(test::raw::callback_1);
        subject.did_work.add_callback(test::raw::callback_2);
        subject.did_work.add_callback(test::raw::callback_3);

        std::size_t expected { 3 };

        test::assert_true(subject.did_work.has_callbacks());
        test::assert_equal(subject.did_work.get_callback_count(), expected);

        test::assert_true(subject.did_work.remove_callback(test::raw::callback_2));
        test::assert_equal(subject.did_work.get_callback_count(), --expected);

        test::assert_true(subject.did_work.remove_callback(test::raw::callback_1));
        test::assert_equal(subject.did_work.get_callback_count(), --expected);

        test::assert_true(subject.did_work.remove_callback(test::raw::callback_3));
        test::assert_equal(subject.did_work.get_callback_count(), --expected);

        test::assert_false(subject.did_other_work.has_callbacks());
    });

    test::run("Raw_Event::operation-=", "removes callback", []() {
        test::Raw_Subject subject {};

        subject.did_other_work += test::raw::callback_1;
        subject.did_other_work += test::raw::callback_2;
        subject.did_other_work += test::raw::callback_3;

        std::size_t expected { 3 };

        test::assert_true(subject.did_other_work.has_callbacks());
        test::assert_equal(subject.did_other_work.get_callback_count(), expected);

        subject.did_other_work -= test::raw::callback_2;
        test::assert_equal(subject.did_other_work.get_callback_count(), --expected);

        subject.did_other_work -= test::raw::callback_1;
        test::assert_equal(subject.did_other_work.get_callback_count(), --expected);

        subject.did_other_work -= test::raw::callback_3;
        test::assert_equal(subject.did_other_work.get_callback_count(), --expected);

        test::assert_false(subject.did_other_work.has_callbacks());
    });

    test::run("Raw_Event::clear_callbacks", "removes all callbacks", []() {
        test::Raw_Subject subject {};

        subject.sent_message.add_callback(test::raw::callback_1);
        subject.sent_message.add_callback(test::raw::callback_2);

        test::assert_true(subject.sent_message.has_callbacks());
        test::assert_equal(subject.sent_message.get_callback_count(), 2);

        subject.sent_message.clear_callbacks();

        test::assert_false(subject.sent_message.has_callbacks());
        test::assert_equal(subject.sent_message.get_callback_count(), 0);
    });

    test::run("Raw_Event::operator()", "invokes all callbacks", []() {
        test::Raw_Subject subject {};

        subject.did_work += test::raw::callback_1;
        subject.did_work += test::raw::callback_2;
        subject.did_work += test::raw::callback_3;
        subject.do_work();
        subject.do_work();
        subject.do_work();

        subject.did_other_work += test::raw::callback_1;
        subject.did_other_work += test::raw::callback_2;
        subject.did_other_work += test::raw::callback_3;
        subject.do_other_work(8, 5);
        subject.do_other_work(7, 2);
        subject.do_other_work(10, 2);

        subject.sent_message += test::raw::callback_1;
        subject.sent_message += test::raw::callback_2;
        subject.send_message();
        subject.send_message();
    });
}

}
