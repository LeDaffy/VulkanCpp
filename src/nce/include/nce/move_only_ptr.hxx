#pragma once
#include <iostream>

template<typename T, typename D> class MoveOnlyPtr {
    public:
    using ValueType = T;

    using Pointer = ValueType*;
    using ConstPointer = const ValueType*;

    using Reference = ValueType&;
    using ConstReference = const ValueType&;
    using RReference = ValueType&&;
    using RConstReference = const ValueType&&;


    // Constructor
    MoveOnlyPtr() : m_data(nullptr) {}
    MoveOnlyPtr(T* data) : m_data(data) {}
    // Destructor
    ~MoveOnlyPtr() = default;

    // Move constructor
    MoveOnlyPtr(MoveOnlyPtr&& o) : m_data(std::move(o.m_data)) {
        o.m_data = nullptr;
    }
    // Move assignment
    MoveOnlyPtr& operator=(MoveOnlyPtr&& o) {
        o.m_data = nullptr;
        return *this;
    }
    operator bool() { return m_data != nullptr; }
    operator bool() const { return m_data != nullptr; }
    operator Pointer() { return m_data; }
    operator Pointer() const { return m_data; }
    Reference operator *() const { return *(m_data); }
    Pointer operator ->() const { return m_data; }

    friend std::ostream &operator<<(std::ostream &os, MoveOnlyPtr<T, D>& m) { return os << m.data; }

    // Copy constructor
    MoveOnlyPtr(MoveOnlyPtr& o) = delete;
    // Copy assignment
    MoveOnlyPtr& operator=(MoveOnlyPtr& o) = delete;

    // Copy assignment
    void operator=(Pointer p) { m_data = p; };

    private:
    Pointer m_data;

};
