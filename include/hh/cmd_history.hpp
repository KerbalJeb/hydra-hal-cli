/// \file cmd_history.hpp
/// \brief Created on 2021-09-01 by Ben
/// \todo Add documentation

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
    T pos_;
    Container* container_;
};

template<std::size_t NumLines, std::size_t LineLen>
class cmd_history {
public:
    // todo pack strings in buffer
    // todo copy/move constructors
    using value_type = char*;
    using reference = char*;
    using const_reference = std::string_view;
    using const_iterator = circular_iter<const char*, const cmd_history>;
    using iterator = const_iterator;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    cmd_history() = default;

    const_iterator begin() const { return const_iterator{tail_, this}; }
    const_iterator end() const { return const_iterator{nullptr, this}; }

    [[nodiscard]] const_reference front() const { return tail_; }
    [[nodiscard]] const_reference back() const {
        const char *pos = head_;
        decrement(pos);
        return pos;
    }

    [[nodiscard]] bool empty() const { return size() == 0; }
    [[nodiscard]] size_type size() const { return numCmds_; }
    [[nodiscard]] size_type max_size() const { return NumLines; }

    void erase(const_iterator pos) {
        auto p = const_cast<char *>(pos->data());

        if (p >= tail_) {
            std::copy_backward(tail_, p, p + lineLen_);
            increment(tail_);
        } else {
            std::copy(p+lineLen_, head_, p);
            decrement(head_);
        }
    }

    void push_back(const char *line) {

        for (auto it = begin(); it != end(); ++it) {
            if (*it == line) {
                erase(it);
                break;
            }
        }

        std::strcpy(head_, line);

        if (head_ == tail_ && !empty()) {
            increment(tail_);
        } else {
            ++numCmds_;
        }

        increment(head_);
    }

private:
    friend const_iterator;
    static constexpr difference_type lineLen_ = LineLen + 1;
    static constexpr difference_type bufferSize_ = NumLines * lineLen_;
    char buffer_[bufferSize_]{};
    char *const bufferEnd_{&buffer_[bufferSize_]};
    char *head_{buffer_};
    char *tail_{buffer_};
    size_type numCmds_{0};

    void increment(const char *&p) const {
        p += lineLen_;
        if (p >= bufferEnd_) { p -= bufferSize_; }
    }

    void increment(char *&p) const {
        p += lineLen_;
        if (p >= bufferEnd_) { p -= bufferSize_; }
    }

    void decrement(const char *&p) const {
        p -= lineLen_;
        if (p < buffer_) { p += bufferSize_; }
    }

    void decrement(char *&p) const {
        p -= lineLen_;
        if (p < buffer_) { p += bufferSize_; }
    }
};
}