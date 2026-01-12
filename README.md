# Observant

## Description

Observant is a header-only library that provides a general-purpose event system for C++ inspired by that of C#. With it, you can implement the Observer pattern using the power of templates.

## Features

Observant features two kinds of events:
- `Event`: A class that is held by the subject as a field that is designed to interact with objects of the `Callback` type that are themselves held by observers as fields.
- `Raw_Event`: A class that holds raw pointers to free-floating functions and `static` methods.

## Requirements
### Source Files

Observant consists of a single header file: `observant.hpp`. Just `#include` it wherever you need and you're good to go! 

Observant compiles as C++11 at minimum.

### Unit Tests

For building the unit tests, have [Premake](https://premake.github.io/) installed and use it to run the `premake5.lua` file at the root of the repository with your preferred toolset. Below is an example using Visual Studio 2022.

```
premake5 vs2022
```

## Usage
### Overview

`Callback` and `Event` together provide the classic Observer Pattern:
	1. A `Callback` object is held by the observer as a field, and an `Event` object with the same type parameters is held as a field by the subject.
	2. Have the `Callback` register to the `Event` object.

Benefits of this system:
	1. Flexible - Observers can have multiple Callback fields and can thus observe multiple subjects.
	2. Safe - Both `Event` and `Callback` are capable of performing cleanup action regardless of whether the subject or observer falls out of scope. Should the subject fall away first, the `Event` will signal to all `Callbacks` it contains to drop any references to it, and likewise, should a `Callback`'s owner fall away first, it will alert any and all events to which it is subscribed to remove all references to it. In that sense, it holds an advantage over C#'s event system, where the user must be careful based on whether the observer falls out of scope or not.

`Event` and `Raw_Event` are able to register and deregister callbacks using either function-like (`register_callback`/`remove_callback`) or operator-like (`+=`/`-=`) syntax. The latter style is for both conciseness and an aid for those used to the C# style of registering and unregistering callbacks.

### Example

```cpp
#include "observant.hpp"
#include <iostream>
#include <string>

using namespace observant;

class Subject {
public:
    void do_work() {
        std::cout << "Did work. (Count: " << ++m_count << ')' << '\n';
        m_did_work.invoke(m_count);
    }

    void register_did_work(Callback<int>& callback) {
        m_did_work.add_callback(&callback);
    }

    void unregister_did_work(Callback<int>& callback) {
        m_did_work.remove_callback(&callback);
    }

    void print_registee_count() const {
        std::cout << m_did_work.get_callback_count() << '\n';
    }

private:
    int m_count { 0 };
    Event<int> m_did_work {};
};

class Observer {
public:
    Observer(std::string id) : m_id { name } {}

    const Callback<int>& get_callback() const {
        return m_callback;
    }

private:
    std::string m_id {};
    Callback<int> m_callback {
		[this](int data) mutable { this->on_did_work(int); }
	};

	void on_did_work(int data) {
		std::cout
			<< "Observer "
			<< m_host.id
			<< " took notice of the event. (Data obtained from event: "
			<< data
			<< ")\n";
	}
};

int main() {
    Subject subject {};
    Observer observer_1 { "1234" };

    subject.register_did_work(observer_1.get_callback());
    subject.print_registee_count();

    subject.do_work();

    {
        Observer observer_2 { "5678" };
        subject.register_did_work(observer_2.get_callback());

        subject.do_work();

        subject.print_registee_count();
    }

    subject.print_registee_count();

    subject.do_work();

    return 0;
}
```
