/// \file concepts.hpp
/// \brief Created on 2021-08-30 by Ben

#pragma once
#include <concepts>

namespace hh::shell {

template<typename T>
concept serial_out_device =  std::movable<T> && requires(T so, const char* s, std::size_t count, T&& rv)
{
    so.write(s, count);
    so.flush();
};

template<typename T> concept serial_in_device = std::movable<T> && requires(T si, char* s, std::size_t count, char ch)
{
    { si.get() } -> std::convertible_to<int>;
};

template<typename T> concept serial_io_device = requires
{
    serial_in_device<T>&& serial_out_device<T>;
};

template<typename T> concept basic_lockable = requires(T m){
    m.lock();
    m.unlock();
};
}