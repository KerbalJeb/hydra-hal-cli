/// \file concepts.hpp
/// \brief Created on 2021-08-30 by Ben

#pragma once
#include <concepts>

namespace hh::cli {

template<typename T>
concept serial_output = requires(T so, const char ch, std::size_t count)
{
    so.write(ch, count);
    so.flush();
};

template<typename T> concept serial_input = requires(T si, char* s, std::size_t count)
{
    { si.readsome(s, count) } -> std::convertible_to<std::size_t>;
    si.read(s, count);
};

template<typename T> concept serial_io = requires
{
    serial_input<T>&& serial_output<T>;
};

template<typename T> concept basic_lockable = requires(T m){
    m.lock();
    m.unlock();
};
}