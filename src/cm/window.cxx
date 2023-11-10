module;
import types;
#include <cstddef>
#include <expected>
#include <optional>
#include <cstdio>
#include <iostream>
#include <functional>
#include <format>
#include <tuple>
#include <memory>
#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>
#include <X11/Xlibint.h>

export module window;


export namespace cm {
    template<typename T> struct WindowVec2 {
        T x;
        T y;
        WindowVec2(u32 x, u32 y) : x(x), y(y) {}
    };
    struct WindowAttributes {
        WindowVec2<u32> dimensions;
        WindowVec2<u32> position;
        u32 border_width;

        WindowAttributes() : dimensions(640, 480), position(0, 0), border_width(0){}
    };
    struct Window {
        Display* display;
        Screen*  screen;
        ::Window xwindow;

        Window(Display* display, Screen* screen) : display(display), screen(screen) {}
        ~Window() {
            XCloseDisplay(display);
        }
    };
    struct WindowBuilder {
        WindowAttributes attributes;
        WindowBuilder() { 
            this->attributes.dimensions.x = 640;
            this->attributes.dimensions.y = 480;
        };
        auto dimensions(u32 x, u32 y) -> WindowBuilder {
            this->attributes.dimensions = WindowVec2<u32>(640, 480);
            return *this;
        }
        auto border_width(u32 border_width) -> WindowBuilder {
            this->attributes.border_width = border_width;
            return *this;
        }
        auto build() -> Window {
            auto display = XOpenDisplay(nullptr);
            auto screen = XDefaultScreenOfDisplay(display);
            auto screen_id = XDefaultScreen(display);

            auto x = &XFree;

            XArray<i32, i32> depths([display, screen_id]() -> std::tuple<i32*, i32> {
                    auto depths_size = 0;
                    auto depths_array = XListDepths(display, screen_id, &depths_size);
                    return std::make_tuple(depths_array, depths_size);
                    }, std::function<int(void*)>(XFree));
            //for (int i = 0; i < depths.size(); i++) {
            //    std::printf("%d\n", depths[i]);
            //}
            //std::array<int,3>::iterator asdf;

            return Window(display, screen);
        }
        private:
        template<typename T, typename SizeType=size_t, typename D=std::function<int(void*)>> struct XArray {
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
            const Iterator begin() const { return Iterator(this->d); }
            Iterator begin() { return Iterator(this->d); }
            const Iterator end() const { return Iterator(this->d + this->size); }
            Iterator end() { return Iterator(this->d + this->size); }
            const Iterator cbegin() const { return ConstIterator(this->d); }
            const Iterator cend() const { return ConstIterator(this->d + this->size); }
            const ReverseIterator rbegin() const { return ReverseIterator(this->end()); }
            ReverseIterator rbegin() { return ReverseIterator(this->end()); }
            const ReverseIterator rend() const { return ReverseIterator(this->begin()); }
            ReverseIterator rend() { return ReverseIterator(this->begin()); }
            ConstReverseIterator crbegin() const { return ReverseIterator(this->end()); }
            ConstReverseIterator crend() const { return ReverseIterator(this->begin()); }


            XArray(const std::function<std::tuple<T*, SizeType> ()>& f, DeleterType d) : fn_d(d) {
                auto [data, size] = f();
                this->d = data;
                this->nm = size;
            }
            XArray(const std::function<std::tuple<T*, SizeType> ()>& f) : fn_d(std::nullopt) {
                auto [data, size] = f();
                this->d = data;
                this->nm = size;
            }
            ~XArray() {
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
    };
}
