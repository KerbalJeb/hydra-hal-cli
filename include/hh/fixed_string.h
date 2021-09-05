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

        constexpr fixed_string() = default;
        constexpr fixed_string(const char *s) { append(s); }
        constexpr fixed_string(const fixed_string &other) {
            std::copy(other.begin(), other.end(), begin());
            cursor_ = buffer_ + other.size();
        }
        constexpr fixed_string &operator=(const fixed_string &other) {
            if (this != &other) {
                std::copy(other.begin(), other.end(), begin());
                cursor_ = buffer_ + other.size();
            }
            return *this;
        }
        constexpr fixed_string(fixed_string &&);
        constexpr fixed_string &operator=(fixed_string &&);

        constexpr reference operator[](size_type pos);
        constexpr const_reference operator[](size_type pos) const;

        [[nodiscard]] constexpr reference front() { return *(cursor_ - 1); }
        [[nodiscard]] constexpr const_reference front() const { return *(cursor_ - 1); }
        [[nodiscard]] constexpr reference back() { return *buffer_; }
        [[nodiscard]] constexpr const_reference back() const { return *buffer_; }
        [[nodiscard]] constexpr const value_type *c_str() const { return buffer_; }

        [[nodiscard]] constexpr iterator begin() { return buffer_; }
        [[nodiscard]] constexpr const_iterator begin() const { return buffer_; }
        [[nodiscard]] constexpr const_iterator cbegin() const { return buffer_; }
        [[nodiscard]] constexpr iterator end() { return cursor_; }
        [[nodiscard]] constexpr const_iterator end() const { return cursor_; }
        [[nodiscard]] constexpr const_iterator cend() const { return cursor_; }

        [[nodiscard]] constexpr bool empty() const { return buffer_ == cursor_; }
        [[nodiscard]] constexpr size_type size() const { return cursor_ - buffer_; }
        [[nodiscard]] constexpr size_type length() const { return size(); }
        [[nodiscard]] constexpr size_type max_size() const { return N; }

        constexpr void clear() {
            cursor_ = buffer_;
            *cursor_ = 0;
        }
        constexpr iterator insert(const_iterator pos, value_type ch) {
            insert(pos, &ch, 1);
            return const_cast<iterator>(pos);
        }
        constexpr fixed_string &insert(const_iterator pos, const value_type *s) {
            return insert(pos, s, traits_type::length(s));
        }
        constexpr fixed_string &insert(const_iterator pos, const value_type *s, size_type len) {
            auto it = const_cast<iterator>(pos);

            if (it <= end() && cursor_ + len < bufferEnd_) {
                std::copy_backward(it, end(), end() + len);
                std::copy(s, s + len, it);
            }

            return *this;
        }
        constexpr iterator erase(const_iterator pos) {
            auto it = const_cast<iterator>(pos);
            if (it >= begin() && it < end()) {
                std::copy(it + 1, cursor_, it);
                *(--cursor_) = 0;
            }
            return it;
        }
        constexpr void push_back(value_type ch) {
            if (cursor_ == bufferEnd_) { return; }
            *(cursor_++) = ch;
        }
        constexpr void pop_back() {
            if (cursor_ > buffer_) {
                *(--cursor_) = 0;
            }
        }
        constexpr fixed_string &append(const value_type *s) {
            while (*s != 0 && cursor_ != bufferEnd_) { push_back(*s++); }
            return *this;
        }

        template<std::size_t M>
        constexpr int compare(const fixed_string<M> &str) const;
        constexpr int compare(const char *str) const;

    private:
        value_type buffer_[N + 1]{};
        value_type *cursor_{buffer_};
        const value_type *const bufferEnd_{&buffer_[N]};
    };

    template<std::size_t N, std::size_t M>
    bool operator==(const fixed_string<N> &lhs, const fixed_string<M> &rhs) {
        auto lhs_sz = lhs.size();
        auto rhs_sz = rhs.size();
        if (lhs_sz != rhs_sz) { return false; }
        return fixed_string<N>::traits_type::compare(lhs.c_str(), rhs.c_str(), lhs_sz) == 0;
    }
}// namespace hh::container

#endif//HYDRA_HAL_FIXED_STRING_H
