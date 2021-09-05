#ifndef HYDRA_HAL_FIXED_STRING_H
#define HYDRA_HAL_FIXED_STRING_H

#include <string>

namespace hh::container {

    template<std::size_t N>
    class fixed_string {
    public:
        using traits_type = std::char_traits<char>;
        using value_type = char;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type &;
        using const_reference = const value_type &;
        using iterator = char *;
        using const_iterator = const char *;

        constexpr fixed_string();
        constexpr fixed_string(const fixed_string&);
        constexpr fixed_string& operator=(const fixed_string&);
        constexpr fixed_string(fixed_string&&);
        constexpr fixed_string& operator=(fixed_string&&);

        constexpr reference operator[](size_type pos);
        constexpr const_reference operator[](size_type pos) const;

        constexpr reference front();
        constexpr const_reference front() const;
        constexpr reference back();
        constexpr const_reference back() const;
        constexpr const value_type* c_str() const;

        constexpr iterator begin();
        constexpr const_iterator begin() const;
        constexpr const_iterator cbegin() const;
        constexpr iterator end();
        constexpr const_iterator end() const;
        constexpr const_iterator cend() const;

        constexpr bool empty() const;
        constexpr size_type size() const;
        constexpr size_type length() const;
        constexpr size_type max_size() const;

        constexpr void clear();
        constexpr iterator insert(const_iterator pos, value_type ch);
        constexpr fixed_string& insert(const_iterator pos, const value_type* s);
        constexpr iterator erase(const_iterator pos);
        constexpr void push_back(value_type ch);
        constexpr void pop_back();
        constexpr fixed_string& append(value_type* s);

        template<std::size_t M>
        constexpr int compare(const fixed_string<M>& str) const;
        constexpr int compare(const char* str) const;

    private:
        value_type buffer_[N+1]{};
        value_type* cursor_{buffer_};
    };

    template<std::size_t N, std::size_t M>
    bool operator <=>(const fixed_string<N>& lsh, const fixed_string<M>& rhs);

    template<std::size_t N>
    bool operator <=>(const fixed_string<N>& lsh, const char* rhs);
}

#endif //HYDRA_HAL_FIXED_STRING_H
