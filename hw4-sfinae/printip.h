/**
 * @file printip.h
 * @brief File contains template function that prints IP address-like data structures.
 * @author dmitriirylko
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <tuple>
#include <stdint.h>

/**
 * @brief Template function that prints ip address represented as integral type.
 * @tparam T Integral type.
 * @tparam F Fake parameter to restrict overload only for integral types by SFINAE mechanism.
 * @param [in] t Integer to print.
 */
template<typename T,
         typename F = std::enable_if_t<std::is_integral_v<T>, int>>
void print_ip(T t)
{
    auto pt = reinterpret_cast<uint8_t*>(&t) + sizeof(T) - 1;
    for(size_t i = 0; i < sizeof(T); ++i)
    {
        if(i) std::cout << ".";
        std::cout << +(*pt);
        --pt;
    }
    std::cout << std::endl;
}

/**
 * @brief Template function that prints ip address represented as std::string.
 * @tparam T String type.
 * @tparam F Fake parameter to restrict overload only for std::string's by SFINAE mechanism.
 * @param [in] t String to print.
 */
template<typename T,
         typename F = std::enable_if_t<std::is_same_v<T, std::string>, int>>
void print_ip(const T& t)
{
    std::cout << t << std::endl;
}

/**
 * @brief Template function that prints ip address represented as container (std::vector or std::list).
 * @tparam Container Container (std::vector or std::list).
 * @tparam T Container data type.
 * @tparam Allocator Containers allocator.
 * @tparam F Fake parameter to restrict overload only for std::vector's and std::list's by SFINAE mechanism.
 * @param [in] t Container to print.
 */
template<
        template<typename, typename> typename Container,
        typename T,
        typename Allocator = std::allocator<T>,
        typename F = std::enable_if_t<std::is_same_v<Container<T, Allocator>, std::vector<T, Allocator>> ||
                                      std::is_same_v<Container<T, Allocator>, std::list<T, Allocator>>, int> >
void print_ip(const Container<T, Allocator>& t)
{
    size_t counter = 0;
    for(const auto& v : t)
    {
        if(counter) std::cout << ".";
        std::cout << v;
        ++counter;
    }
    std::cout << std::endl;
}

/**
 * @brief Template function that prints ip address represented as std::tuple.
 *          This overload ends recursion.
 * @tparam I Index of tuple element to print. Index should be zero.
 * @tparam Ts Tuple types.
 * @param [in] t Tuple to print.
 */
template<size_t I,
         typename... Ts,
         typename std::enable_if_t<I == sizeof...(Ts), int> = 0>
void print_ip(std::tuple<Ts...> t)
{
    std::cout << std::endl;
    return;
}

/**
 * @brief Template function that prints ip address represented as std::tuple recursively.
 * @tparam I Index of tuple element to print.
 * @tparam Ts Tuple types.
 * @param [in] t Tuple to print.
 */
template<size_t I = 0,
         typename... Ts,
         typename std::enable_if_t<I < sizeof...(Ts), int> = 0>
void print_ip(std::tuple<Ts...> t)
{
    using Tuple = std::tuple<Ts...>;
    static_assert(std::is_same_v<std::tuple_element_t<0, Tuple>, std::tuple_element_t<I, Tuple>>, "Type of tuple elements are not the same");
    if(I) std::cout << ".";
    std::cout << std::get<I>(t);
    print_ip<I + 1>(t);
}