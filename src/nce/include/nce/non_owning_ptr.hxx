#pragma once
#include <iostream>



/**
 * @brief Pointer that does not own any memory nor requires any cleanup
 */
template<typename T> class NonOwningPtr {
    public:
    using ValueType = T;

    using Pointer = ValueType*;
    using ConstPointer = const ValueType*;

    using Reference = ValueType&;
    using ConstReference = const ValueType&;
    using RReference = ValueType&&;
    using RConstReference = const ValueType&&;


    // Constructor
    NonOwningPtr() : m_data(nullptr) {}
    NonOwningPtr(T* data) : m_data(data) {}

    // Destructor
    ~NonOwningPtr() = default;

    // Copy constructor
    NonOwningPtr(NonOwningPtr& o) : m_data(o.m_data) {};
    // Copy assignment
    NonOwningPtr& operator=(NonOwningPtr& o) { m_data = o.m_data; return *this; };
    NonOwningPtr& operator=(Pointer o) { m_data = o; return *this; };

    // Move constructor
    NonOwningPtr(NonOwningPtr&& o) : m_data(std::move(o.m_data)) {
        o.m_data = nullptr;
    }
    // Move assignment
    NonOwningPtr& operator=(NonOwningPtr&& o) {
        o.m_data = nullptr;
        return *this;
    }
    operator bool() { return m_data != nullptr; }
    operator bool() const { return m_data != nullptr; }
    operator Pointer() { return m_data; }
    operator Pointer() const { return m_data; }
    Reference operator *() const { return *(m_data); }
    Pointer operator ->() const { return m_data; }

    friend std::ostream &operator<<(std::ostream &os, NonOwningPtr<T>& m) { return os << m.m_data; }

    [[nodiscard]] Pointer get() { return m_data; }


    private:
    Pointer m_data;

};
