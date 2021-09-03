/// \file cmd_history.hpp
/// \brief Created on 2021-09-01 by Ben

#pragma once
#include <cstring>
#include <string_view>
namespace hh::shell {

template<std::size_t NumLines, std::size_t LineLen>
class cmd_history;

// todo add pointer to member parameter for inc and dec functions
template<class T, class Container>
class circular_iter {
public:
    struct string_view_ptr: public std::string_view {
      string_view_ptr* operator->(){
          return this;
      }
    };

    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename std::remove_const<T>;
    using difference_type = std::ptrdiff_t;
    using reference = std::string_view;
    using pointer = string_view_ptr;

    circular_iter(T item, Container* container)
            :pos_(item), container_(container) { }

    template<class OtherT, class OtherContainer>
    requires std::is_convertible_v<T*, OtherT*>
    circular_iter(const circular_iter<OtherT, OtherContainer>& other)
            : pos_(other.pos_), container_(other.container_) { }

    reference operator*() { return pos_; }
    pointer operator->() { return pointer {pos_}; }

    /// \brief Increments to next string in buffer
    /// \return
    circular_iter& operator++()
    {
        container_->increment(pos_);
        if (pos_==container_->tail_) { pos_ = nullptr; }
        return *this;
    }

    circular_iter operator++(int)
    {
        auto old = *this;
        ++(*this);
        return old;
    }

    circular_iter& operator--()
    {
        if (pos_==nullptr) { pos_ = container_->tail_; }
        else { container_->decrement(pos_); }
        return *this;
    }

    circular_iter operator--(int)
    {
        auto old = *this;
        --(*this);
        return old;
    }

    bool operator==(const circular_iter& other) const { return pos_==other.pos_; }
private:
    Container* container_;
    T pos_;
};

template<std::size_t NumLines, std::size_t LineLen>
class cmd_history {
public:
    // todo pack strings in buffer
    // todo copy/move constructors
    using value_type = char*;
    using reference = char*;
    using const_reference = const char*;
    using const_iterator = circular_iter<const char*, const cmd_history>;
    using iterator = const_iterator;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    cmd_history() = default;

    const_iterator begin() const { return const_iterator{tail_, this}; }
    const_iterator end() const { return const_iterator{nullptr, this}; }

    reference back() { return head_; }
    [[nodiscard]] const_reference front() const { return tail_; }
    [[nodiscard]] const_reference back() const { return head_; }

    [[nodiscard]] size_type size() const { return num_cmds_; }
    [[nodiscard]] size_type max_size() const { return buffer_size_; }
    [[nodiscard]] bool empty() const { return num_cmds_==1 && count_==0; }

    void end_string()
    {
        head_ += LineLen;
        if (head_==buffer_end_) { head_ = buffer_; }
        if (head_==tail_) { tail_ += LineLen; }
        else { ++num_cmds_; }
        count_ = 0;
    }

    void push_back(char ch)
    {
        if (count_<LineLen) {
            head_[count_++] = ch;
        }
    }

    void push_back(const char* s)
    {
        while (*s!='\0') {
            push_back(*s++);
        }
    }

private:
    friend const_iterator;
    static constexpr std::ptrdiff_t line_len_ = LineLen;
    static constexpr std::ptrdiff_t buffer_size_ = NumLines*LineLen;
    char buffer_[buffer_size_]{};
    char* const buffer_end_{&buffer_[NumLines*LineLen]};
    char* head_{buffer_};
    char* tail_{buffer_};
    size_type count_{0};
    size_type num_cmds_{1};

    void increment(const char*& p) const
    {
        p += line_len_;
        if (p>=buffer_end_) { p -= buffer_size_; }
    }

    void decrement(const char*& p) const
    {
        p -= line_len_;
        if (p<buffer_) { p += buffer_size_; }
    }
};
}