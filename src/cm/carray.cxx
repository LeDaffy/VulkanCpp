module;

#include <optional>
#include <functional>

export module carray;

export {

class CFreeDeleter {
    public:
    void operator()(void* ptr){ std::free(ptr); }
};
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
template<typename T, typename SizeType=size_t, typename D=CFreeDeleter> class CArray {
    public:
    using ValueType = T;
    using DeleterType = D;
    using AllocatorType = std::allocator<T>;

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
    const Iterator begin()  const { return Iterator(m_data); }
          Iterator begin()        { return Iterator(m_data); }
    const Iterator end()    const { return Iterator(m_data + m_count); }
          Iterator end()          { return Iterator(m_data + m_count); }

    const Iterator cbegin() const { return ConstIterator(m_data); }
    const Iterator cend()   const { return ConstIterator(m_data + m_count); }

    const ReverseIterator rbegin() const { return ReverseIterator(end()); }
          ReverseIterator rbegin()       { return ReverseIterator(end()); }
    const ReverseIterator rend()   const { return ReverseIterator(begin()); }
          ReverseIterator rend()         { return ReverseIterator(begin()); }

    ConstReverseIterator crbegin() const { return ReverseIterator(end()); }
    ConstReverseIterator crend()   const { return ReverseIterator(begin()); }


    /** 
     *  @brief Initialize a CArray from a C-style array and and a size.
     *
     *  @param p Pointer to C-style array.
     *  @param s Number os elements in the array.
     *  initialize the array pointer and array size respectively.
     */
    CArray(Pointer p, SizeType s) : m_deleter(), m_data(p), m_count(s)  {}

    /** 
     *  @brief Initialize a CArray from a C-style array and and a size.
     *
     *  @param p Pointer to C-style array.
     *  @param s Number os elements in the array.
     *  initialize the array pointer and array size respectively.
     */
    CArray(SizeType s) : m_deleter(), m_data(), m_count(s)  {
        m_data = (ValueType*)std::malloc(sizeof(ValueType) * s);
    }


    /** 
     *  @brief Call the deleter on the array.
     */
    ~CArray() {
        m_deleter(m_data);
    }

    // size
    SizeType size() { return m_count; }
    const SizeType size() const { return m_count; }

    // element access
    Reference operator[](SizeType n) { return m_data[n]; }
    ConstReference operator[](SizeType n) const { return m_data[n]; }
    std::optional<Reference> at(SizeType n) {
        if (n < m_count)
            return m_data[n]; 
        return std::nullopt;
    }
    Reference front() { return m_data[0]; }
    ConstReference front() const { return m_data[0]; }
    Reference back() { return m_data[m_count - 1]; }
    ConstReference back() const { return m_count[m_count - 1]; }



    Pointer data() { return m_data; }
    ConstPointer data() const { return m_data; }

private:
    [[no_unique_address]] D m_deleter; //deleter
    T* m_data; // data
    SizeType m_count; // number of members
};
}
