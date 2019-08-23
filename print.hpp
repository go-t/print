#ifndef _PRINT_HPP_
#define _PRINT_HPP_

#include <type_traits>
#include <sstream>

namespace std {
extern std::ostream cout;     
}

namespace print_helper {

// -- _void_t(std::void_t)

template< class... > using _void_t = void;

// -- _not(std::negation) --

template<class B> struct _not : std::integral_constant<bool, !bool(B::value)> { };

// -- _and(std::conjunction) --

template<class...> struct _and : std::true_type { };
template<class B1> struct _and<B1> : B1 { };
template<class B1, class... Bn>
struct _and<B1, Bn...>: std::conditional_t<bool(B1::value), _and<Bn...>, B1> {};

// -- _is_pointer --

template <class T, class = _void_t<>> 
struct _is_pointer : std::is_pointer<T> {}; 


// -- _is_std_string --

template <class T, class = _void_t<>> 
struct _is_std_string : std::false_type {}; 

template <class T> 
struct _is_std_string<T, _void_t<decltype(std::declval<T &>().c_str())>> : std::true_type {};

// -- _is_std_container --

template <class T, class = _void_t<>> 
struct _is_std_container : std::false_type {}; 

template <class T> 
struct _is_std_container<T, _void_t<typename T::iterator> > : _not<_is_std_string<T>> {};

// -- _is_printable_helper --

template <class T, class OS, class = _void_t<>>
struct _is_printable_helper : std::false_type {};

template <class T, class OS>
struct _is_printable_helper<T, OS, _void_t<decltype(std::declval<OS &>() << std::declval<const T &>())>> : std::true_type {};

// -- _is_printable --

template <class T>
struct _is_printable: _and<_is_printable_helper<T, std::ostream>, _not<std::is_same<T, bool>>, _not<_is_pointer<T>>>{};



template <class T>
using _enable_if =
    typename std::enable_if<T::value, std::ostream>::type &;

// -- declare --
template <class K, class V>
std::ostream &_print(std::ostream &os, const std::pair<K, V> &v);

template <class C>
_enable_if<_is_std_container<C>> _print(std::ostream &os, const C &c);


std::ostream& _print(std::ostream &os, const char* v);

template <class T>
_enable_if<_is_printable<T>> _print(std::ostream &os, const T &v);

template <class T, size_t N>
std::ostream &_print(std::ostream &os, const T (&v)[N]);

template <class T>
_enable_if<_is_printable<T>> _print(std::ostream &os, const T &v);

std::ostream& _print(std::ostream &os, const char* v);

template <class T>
_enable_if<std::is_same<T, bool>> _print(std::ostream &os, const T &v);

// -- is_printable_pointer --

template <class T, class = _void_t<>>
struct is_printable_pointer : std::false_type {};

template <class T>
struct is_printable_pointer<T*, _void_t<decltype(_print(std::declval<std::ostream&>() , std::declval<const T&>()))>> : std::true_type {};

template <class T>
struct is_opaque_pointer: _and<_is_pointer<T>, _not<is_printable_pointer<T>>> {};


// -- define --

template <class T>
_enable_if<std::is_same<T, bool>> _print(std::ostream &os, const T &v) {
    return os << (v ? "true" : "false");
}

std::ostream& _print(std::ostream &os, const char* v) {
    return os << v;
}

template <class T>
_enable_if<_is_printable<T>> _print(std::ostream &os, const T &v) {
    return os << v;
}

template <class T, size_t N>
std::ostream &_print(std::ostream &os, const T (&v)[N]) {
    os << "[" << N << "]{";
    for (size_t i = 0; i < N; i++) {
        if (i > 0) {
            os << ", ";
        }
        _print(os, v[i]);
    }
    return os << "}";
}

template <class T>
_enable_if<is_printable_pointer<T>> _print(std::ostream &os, const T &v) {
    if (v == nullptr) {
        return os << "<nil>";
    } else {
        return _print(os << "*", *v);;
    }
}

// TODO: opaque pointer
template <class T>
_enable_if<is_opaque_pointer<T>> _print(std::ostream &os, const T &v) {
    if (v == nullptr) {
        return os << "<nil>";
    } else {
        return os << std::hex << v;
    }
}

template <class C>
_enable_if<_is_std_container<C>> _print(std::ostream &os, const C &c) {
    os << "{";
    auto it = c.begin();
    if (it != c.end()) {
        _print(os, *it++);
    }
    while (it != c.end()) {
        _print(os << ", ", *it++);
    }
    return os << "}";
}

template <class K, class V>
std::ostream &_print(std::ostream &os, const std::pair<K, V> &v) {
    return _print(_print(os, v.first) << ": ", v.second);
}

} // namespace print_helper

template <class T>
std::ostream &oprint(std::ostream &os, const T &v) {
    return print_helper::_print(os, v);
}

// oprint(const T&...)
template <class T0, class... Args>
std::ostream &oprint(std::ostream &os, const T0 &v0, Args &&... args) {
    return oprint(oprint(os, v0) << ", ", std::forward<Args>(args)...);
}

template <class... Args>
std::string sprint(Args &&... args) {
    std::ostringstream oss;
    oprint(oss, std::forward<Args>(args)...);
    return oss.str();
}

template <class... Args>
void print(Args &&... args) { oprint(std::cout, std::forward<Args>(args)...); }

template <class... Args>
void println(Args &&... args) { oprint(std::cout, std::forward<Args>(args)...) << std::endl; }


#endif // _PRINT_HPP_
