module;
#include <iostream>

export module move_only_ptr;

export {
template<typename T> struct MoveOnlyPointer {
    using ValueType = T;

    using Pointer = ValueType*;
    using ConstPointer = const ValueType*;

    using Reference = ValueType&;
    using ConstReference = const ValueType&;
    using RReference = ValueType&;
    using RConstReference = const ValueType&;

    Pointer data;

    // Constructor
    MoveOnlyPointer() : data(nullptr) {}
    MoveOnlyPointer(T* data) : data(data) {}
    // Destructor
    ~MoveOnlyPointer() = default;

    // Move constructor
    MoveOnlyPointer(MoveOnlyPointer&& o) : data(std::move(o.data)) {
        o.data = nullptr;
    }
    // Move assignment
    MoveOnlyPointer& operator=(MoveOnlyPointer&& o) {
        o.data = nullptr;
        return *this;
    }
    operator bool() { return this->data != nullptr; }
    operator bool() const { return this->data != nullptr; }
    operator Pointer() { return this->data; }
    operator Pointer() const { return this->data; }
    Reference operator *() const { return *(this->data); }
    Pointer operator ->() const { return this->data; }

    friend std::ostream &operator<<(std::ostream &os, MoveOnlyPointer<T>& m) { return os << m.data; }

    // Copy constructor
    MoveOnlyPointer(MoveOnlyPointer& o) : data(o.data) {};
    // Copy assignment
    MoveOnlyPointer& operator=(MoveOnlyPointer& o) { this->data = o.data; return *this; };

    // Copy assignment
    void operator=(Pointer p) { this->data = p; };

};
}
