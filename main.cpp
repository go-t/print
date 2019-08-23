#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <list>

#include "print.hpp"

using namespace print_helper;

static_assert(_not<std::false_type>::value, "");
static_assert(_not<_not<std::true_type>>::value, "");


static_assert(_and<std::true_type, std::true_type>::value, "");
static_assert(!_and<std::false_type, std::true_type>::value, "");
static_assert(!_and<std::false_type, std::false_type>::value, "");

static_assert(!_is_pointer<int>::value, "");
static_assert(_is_pointer<int*>::value, "");
static_assert(_is_pointer<void*>::value, "");

static_assert(!_is_std_string<int>::value, "");
static_assert(!_is_std_string<const char*>::value, "");
static_assert(_is_std_string<std::string>::value, "");
static_assert(_is_std_string<std::wstring>::value, "");
static_assert(!_is_std_string<std::vector<int>>::value, "");

static_assert(!_is_std_container<int>::value, "");
static_assert(!_is_std_container<const char*>::value, "");
static_assert(!_is_std_container<std::string>::value, "");
static_assert(_is_std_container<std::vector<int>>::value, "");

static_assert(_is_printable_helper<int, std::ostream>::value, "");
static_assert(_is_printable_helper<const char*, std::ostream>::value, "");
static_assert(_is_printable_helper<std::string, std::ostream>::value, "");
static_assert(!_is_printable_helper<std::vector<int>, std::ostream>::value, "");


static_assert(!is_printable_pointer<int>::value, "");
static_assert(is_printable_pointer<const int*>::value, "");
static_assert(!is_printable_pointer<const void*>::value, "");
static_assert(is_opaque_pointer<const void*>::value, "");
static_assert(!is_opaque_pointer<const int*>::value, "");


static_assert(_is_printable<int>::value, "");
static_assert(_is_printable<std::string>::value, "");
static_assert(!_is_printable<bool>::value, "");
static_assert(!_is_printable<int*>::value, "");
static_assert(!_is_printable<std::vector<int>>::value, "");

int main()
{
    int a = 456;
    int* pa = &a;
    int** ppa = &pa;
    
    std::map<std::string, double> dict{{"A", 1.1}, {"B", 2.2}};
    
    println(2.2);
    println(true, 123, 'A', "hello", &ppa);
    println(std::make_pair(1,2.0), std::make_pair("PI", 3.14159));
    println(std::vector<int>{1, 2, 3}, std::list<std::string>{"alpha", "beta"});
    println(dict, &dict, std::set<char>{'A', 'B'}); 
    return 0;
}
