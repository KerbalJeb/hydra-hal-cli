/// \file cmd_history.hpp
/// \brief Created on 2021-09-01 by Ben

#pragma once
#include <cstring>
#include <string_view>
namespace hh::shell {

template<std::size_t NumLines, std::size_t LineLen>
class cmd_history;

template<class T, class Container>
class circular_iter {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename std::remove_const<T>;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using pointer = T*;

    circular_iter(value_type item, Container* container)
            :pos_(item), container_(container) { }

    template<class OtherT, class OtherContainer>
    requires std::is_convertible_v<T*, OtherT*>
    circular_iter(const circular_iter<OtherT, OtherContainer>& other)
            : pos_(other.pos_), container_(other.container_) { }

    reference operator*() { return pos_; }
    pointer operator->() { return &pos_; }

    /// \brief Increments to next string in buffer
    /// \return
    circular_iter& operator++()
    {
        pos_ += inc_amount;
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
        else { pos_ -= inc_amount; }
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
    static constexpr difference_type inc_amount = Container::line_len;
    Container* container_;
    value_type pos_;
    bool end = false;
};

template<std::size_t NumLines, std::size_t LineLen>
class cmd_history {
public:
    // todo convert char* to std string view
    // todo pack strings in buffer
    using value_type = char*;
    using reference = char*;
    using const_reference = const char*;
    using iterator = circular_iter<value_type, cmd_history>;
    using const_iterator = circular_iter<const value_type, cmd_history>;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    cmd_history() = default;

    reference back() { return head_; }
    [[nodiscard]] const_reference front() const { return tail_; }
    [[nodiscard]] const_reference back() const { return head_; }

    [[nodiscard]] size_type size() const { return num_cmds_; }
    [[nodiscard]] size_type max_size() const { return NumLines; }
    [[nodiscard]] bool empty() const { return num_cmds_==1 && count_==0; }

    void end_string();
    void push_back(char ch);
    void push_back(const char* s);

private:
    friend iterator;
    friend const_iterator;
    static constexpr std::ptrdiff_t line_len = LineLen;
    char buffer_[NumLines*LineLen]{};
    char* const buffer_end_{&buffer_[NumLines*LineLen]};
    char* head_{buffer_};
    char* tail_{buffer_};
    size_type count_{0};
    size_type num_cmds_{1};
};

// Defs
template<std::size_t NumLines, std::size_t LineLen>
void cmd_history<NumLines, LineLen>::end_string()
{
    head_ += LineLen;
    if (head_==buffer_end_) { head_ = buffer_; }
    if (head_==tail_) { tail_ += LineLen; }
    else { ++num_cmds_; }
    count_ = 0;
}

template<std::size_t NumLines, std::size_t LineLen>
void cmd_history<NumLines, LineLen>::push_back(char ch)
{
    if (count_<LineLen) { head_[count_++] = ch; }
}

template<std::size_t NumLines, std::size_t LineLen>
void cmd_history<NumLines, LineLen>::push_back(const char* s)
{
    while (*s!='\0') { push_back(*s++); }
}
}