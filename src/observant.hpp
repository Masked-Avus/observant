// Version: 1.0.2

#pragma once

#include <vector>
#include <functional>


namespace observant {

// Necessary forward declaration for Callback.
template<typename ... Args>
class Event;

template<typename ... Args>
class Callback {
public:
    using Event_Type = Event<Args...>;
    using Function_Type = std::function<void(Args...)>;

    Callback() = default;

    Callback(Function_Type on_invoke);

    // Allow the base class to handle clearing any registered events.
    virtual ~Callback();

    void set_invoke_function(Function_Type on_event);
    void operator ()(Args ... args);
    std::size_t get_registered_event_count() const;
    bool is_registered() const;
    bool has_invoke_function() const;

    friend class Event<Args...>;

private:
    std::vector<Event_Type*> m_registered_events {};
    Function_Type m_invoke_function {};

    void register_event(Event_Type& event);

    /*
    We use this in conjunction with Event's "remove_callback()" to not be potentially loaded
        up with pointers to dead events should the Callback get registered with multiple Events
        that die off before it does.

    Not having this does not cause any unpredictable behavior, but doing so can potentially
        prevent memory leaks in specific circumstances.
    */
    void remove_event(const Event_Type& target);
};

template<typename ... Args>
class Event {
public:
    using Callback_Type = Callback<Args...>;

    Event() = default;

    ~Event();

    void operator ()(Args ... args);

    bool add_callback(Callback_Type& callback);
    Event& operator +=(Callback_Type& callback);
    bool remove_callback(Callback_Type& callback);
    Event& operator -=(Callback_Type& callback);
    void clear_callbacks();
    std::size_t get_callback_count() const;
    bool has_callbacks() const;

    // Deleted for safety reasons.
    Event(const Event& other) = delete;
    Event& operator =(const Event& other) = delete;

private:
    std::vector<Callback_Type*> m_callbacks {};
};

template<typename ... Args>
Callback<Args...>::~Callback() {
    for (std::size_t i {}; i < m_registered_events.size(); ++i) {
        m_registered_events[i]->remove_callback(*this);
    }

    // We call this after and removing ourselves from all events (rather than
    //     within the prior loop) for simplicity.
    m_registered_events.clear();
}

template<typename ... Args>
Callback<Args...>::Callback(Function_Type on_invoke) : m_invoke_function { on_invoke } {}

template<typename ... Args>
void Callback<Args...>::set_invoke_function(Function_Type on_event) {
    m_invoke_function = on_event;
}

template<typename ... Args>
void Callback<Args...>::operator ()(Args ... args) {
    m_invoke_function(args...);
}

template<typename ... Args>
std::size_t Callback<Args...>::get_registered_event_count() const {
    return m_registered_events.size();
}

template<typename ... Args>
bool Callback<Args...>::is_registered() const {
    return m_registered_events.size() > 0;
}

template<typename ... Args>
bool Callback<Args...>::has_invoke_function() const {
    return static_cast<bool>(m_invoke_function);
}

template<typename ... Args>
void Callback<Args...>::register_event(Event_Type& event) {
    Event_Type* p_event { &event };

    // Prevent double-inclusion.
    for (const Event_Type* registered_event : m_registered_events) {
        if (p_event == registered_event) {
            return;
        }
    }

    m_registered_events.push_back(p_event);
}

template<typename ... Args>
void Callback<Args...>::remove_event(const Event_Type& target) {
    const Event_Type* p_target { &target };

    for (std::size_t i {}; i < m_registered_events.size(); ++i) {
        if (m_registered_events[i] == p_target) {
            m_registered_events.erase(m_registered_events.begin() + i);
            return;
        }
    }
}

template<typename ... Args>
Event<Args...>::~Event() {
    for (Callback_Type* callback : m_callbacks) {
        callback->remove_event(*this);
    }

    m_callbacks.clear();
}

template<typename ... Args>
void Event<Args...>::operator ()(Args ... args) {
    for (Callback_Type* callback : m_callbacks) {
        (*callback)(args...);
    }
}

template<typename ... Args>
bool Event<Args...>::add_callback(Callback_Type& callback) {
    Callback_Type* p_callback { &callback };

    // This check is to prevent double-inclusion of the same observer.
    for (std::size_t i {}; i < m_callbacks.size(); ++i) {
        if (p_callback == m_callbacks[i]) {
            return false;
        }
    }

    m_callbacks.push_back(p_callback);
    p_callback->register_event(*this);

    return true;
}

template<typename ... Args>
Event<Args...>& Event<Args...>::operator +=(Callback_Type& callback) {
    add_callback(callback);
    return *this;
}

template<typename ... Args>
bool Event<Args...>::remove_callback(Callback_Type& callback) {
    Callback_Type* p_callback { &callback };

    for (std::size_t i {}; i < m_callbacks.size(); ++i) {
        if (p_callback == m_callbacks.at(i)) {
            m_callbacks.erase(m_callbacks.begin() + i);

            // For safety and memory efficiency.
            // The former is especially important should any observers die before the Event.
            p_callback->remove_event(*this);

            return true;
        }
    }

    return false;
}

template<typename ... Args>
Event<Args...>& Event<Args...>::operator -=(Callback_Type& callback) {
    remove_callback(callback);
    return *this;
}

template<typename ... Args>
void Event<Args...>::clear_callbacks() {
    m_callbacks.clear();
}

template<typename ... Args>
std::size_t Event<Args...>::get_callback_count() const {
    return m_callbacks.size();
}

template<typename ... Args>
bool Event<Args...>::has_callbacks() const {
    return m_callbacks.size() > 0;
}

}

namespace observant {

// Similar to "Event" but stores raw function pointers for both free-floating and static class functions.
template<typename ... Args>
class Raw_Event {
public:
    using Callback_Type = void(*)(Args...);

    Raw_Event() = default;

    ~Raw_Event();

    void operator ()(Args ... args);
    bool add_callback(Callback_Type callback);
    Raw_Event& operator +=(Callback_Type callback);
    bool remove_callback(Callback_Type callback);
    Raw_Event& operator -=(Callback_Type callback);
    void clear_callbacks();
    std::size_t get_callback_count() const;
    bool has_callbacks() const;

    // Deleted for safety reasons.
    Raw_Event(const Raw_Event& other) = delete;
    Raw_Event& operator =(const Raw_Event& other) = delete;


private:
    std::vector<Callback_Type> m_callbacks {};
};

template<typename ... Args>
Raw_Event<Args...>::~Raw_Event() {
    m_callbacks.clear();
}

template<typename ... Args>
void Raw_Event<Args...>::operator ()(Args ... args) {
    for (Callback_Type procedure : m_callbacks) {
        procedure(args...);
    }
}

template<typename ... Args>
bool Raw_Event<Args...>::add_callback(Callback_Type callback) {
    // This check is to prevent double-inclusion of the same observer.
    for (std::size_t i {}; i < m_callbacks.size(); ++i) {
        if (callback == m_callbacks[i]) {
            return false;
        }
    }

    m_callbacks.push_back(callback);

    return true;
}

template<typename ... Args>
Raw_Event<Args...>& Raw_Event<Args...>::operator +=(Callback_Type callback) {
    add_callback(callback);
    return *this;
}

template<typename ... Args>
bool Raw_Event<Args...>::remove_callback(Callback_Type callback) {
    for (std::size_t i {}; i < m_callbacks.size(); ++i) {
        if (callback == m_callbacks.at(i)) {
            m_callbacks.erase(m_callbacks.begin() + i);
            return true;
        }
    }

    return false;
}

template<typename ... Args>
Raw_Event<Args...>& Raw_Event<Args...>::operator -=(Callback_Type callback) {
    remove_callback(callback);
    return *this;
}

template<typename ... Args>
void Raw_Event<Args...>::clear_callbacks() {
    m_callbacks.clear();
}

template<typename ... Args>
std::size_t Raw_Event<Args...>::get_callback_count() const {
    return m_callbacks.size();
}

template<typename ... Args>
bool Raw_Event<Args...>::has_callbacks() const {
    return m_callbacks.size() > 0;
}

}
