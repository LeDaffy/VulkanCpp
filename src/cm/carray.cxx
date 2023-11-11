module;

#include <optional>
#include <functional>

export module carray;

export {
/*!
 * @brief CArray: Templated class used to wrap a C-style array from a pointer and a length
 * Example:
 * ```
 * CArray<i32, i32> depths([display, screen_id]() -> std::tuple<i32*, i32> {
 *     auto depths_size = 0;
 *     auto depths_array = XListDepths(display, screen_id, &depths_size);
 *     return std::make_tuple(depths_array, depths_size);
 *     }, XFree);
 * ```
 */
template<typename T, typename SizeType=size_t, typename D=int(*)(void*)> struct CArray {
    using ValueType = T;
    using DeleterType = D;

    using Pointer = ValueType*;
    using ConstPointer = const ValueType*;

    using Reference = ValueType&;
    using ConstReference = const ValueType&;

    using Iterator = ValueType*;
    using ConstIterator = const ValueType*;

    using DifferenceType = std::ptrdiff_t;
    using ReverseIterator = std::reverse_iterator<Iterator>;
    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;


    // Iterators
    const Iterator begin()  const { return Iterator(this->d); }
          Iterator begin()        { return Iterator(this->d); }
    const Iterator end()    const { return Iterator(this->d + this->nm); }
          Iterator end()          { return Iterator(this->d + this->nm); }

    const Iterator cbegin() const { return ConstIterator(this->d); }
    const Iterator cend()   const { return ConstIterator(this->d + this->nm); }

    const ReverseIterator rbegin() const { return ReverseIterator(this->end()); }
          ReverseIterator rbegin()       { return ReverseIterator(this->end()); }
    const ReverseIterator rend()   const { return ReverseIterator(this->begin()); }
          ReverseIterator rend()         { return ReverseIterator(this->begin()); }

    ConstReverseIterator crbegin() const { return ReverseIterator(this->end()); }
    ConstReverseIterator crend()   const { return ReverseIterator(this->begin()); }


    /** 
     *  @brief Initialize a CArray from a function and a custom deleter. The function should return a pointer the 
     *  array and the number of elements within the array. The deleter d will be called on the data in the destructor.
     *
     *  @param f Any function whose return value can be destructured into [data, size]. Data and size are used to
     *  initialize the array pointer and array size respectively.
     *
     *  @param d Any function whose return value can be destructured into [data, size]. Data and size are used to
     *  initialize the array pointer and array size respectively.
     */
    template<typename F>
    CArray(F f, DeleterType d) : fn_d(d) {
        auto [data, size] = f();
        this->d = data;
        this->nm = size;
    }
    /** 
     *  @brief Initialize a CArray from a function. The function should return a pointer the array and the number of 
     *  elements within the array.
     *
     *  @param f Any function whose return value can be destructured into [data, size]. Data and size are used to
     *  initialize the array pointer and array size respectively.
     */
    template<typename F>
    CArray(F f) : fn_d([](){}) {
        auto [data, size] = f();
        this->d = data;
        this->nm = size;
    }

    /** 
     *  @brief Initialize a CArray from a C-style array and and a size.
     *
     *  @param p Pointer to C-style array.
     *  @param s Number os elements in the array.
     *  initialize the array pointer and array size respectively.
     */
    CArray(Pointer p, SizeType s) : d(p), nm(s), fn_d([](){}) {}

    /** 
     *  @brief Initialize a CArray from a C-style array and and a size.
     *
     *  @param p Pointer to C-style array.
     *  @param s Number os elements in the array.
     *  @param d Deleter called on pointer at destruction.
     *  initialize the array pointer and array size respectively.
     */
    CArray(Pointer p, SizeType s, DeleterType d) : d(p), nm(s), fn_d(d) {}

    /** 
     *  @brief Call the deleter on the array.
     */
    ~CArray() {
        this->fn_d(this->d);
    }

    // size
    SizeType size() { return this->nm; }
    const SizeType size() const { return this->nm; }

    // element access
    Reference operator[](SizeType n) { return this->d[n]; }
    ConstReference operator[](SizeType n) const { return this->d[n]; }
    std::optional<Reference> at(SizeType n) {
        if (n < this->nm)
            return this->d[n]; 
        return std::nullopt;
    }
    Reference front() { return this->d[0]; }
    ConstReference front() const { return this->d[0]; }
    Reference back() { return this->d[this->nm - 1]; }
    ConstReference back() const { return this->nm[this->nm - 1]; }



    Pointer data() { return this->d; }
    ConstPointer data() const { return this->d; }

private:
    T* d; // data
    SizeType nm; // number of members
    DeleterType fn_d;
};
}
