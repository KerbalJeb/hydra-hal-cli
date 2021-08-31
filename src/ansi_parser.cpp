/// \file ansi_parser.cpp
/// \brief Created on 2021-08-30 by Ben

#include <hh/ansi_parser.hpp>

hh::ansi::parser::parser(const char* s, std::size_t count)
{
    for (std::size_t i = 0; i<count; ++i, ++s) {
        update_state(*s);
        if (currentState_==state::done) {
            code_.control_char = *s;
            break;
        }
    }
    if (currentState_ != state::done){
        status_ = false;
    }
}

void hh::ansi::parser::update_state(char ch)
{
    switch (currentState_) {
    case state::start:
        if (ch=='[') {
            currentState_ = state::digit_terminator;
            return;
        }
        break;
    case state::digit_terminator:
        if (std::isdigit(ch)) {
            code_.params[0] = ch - '0';
            currentState_ = state::digit_terminator_sep;
            return;
        }
        else if (is_terminator(ch)) {
            currentState_ = state::done;
            return;
        }
        break;
    case state::digit_terminator_sep:
        if (std::isdigit(ch)) {
            auto& p =code_.params[code_.num_params];
            p *= 10;
            p += ch - '0';
            return;
        }
        else if (ch==';') {
            currentState_ = state::digit;
            ++code_.num_params;
            return;
        }
        else if (is_terminator(ch)) {
            currentState_ = state::done;
            ++code_.num_params;
            return;
        }
        break;
    case state::digit:
        if (std::isdigit(ch)) {
            code_.params[code_.num_params] = ch - '0';
            currentState_ = state::digit_terminator_sep;
            return;
        }
        break;
    default:
        break;
    }
    status_ = false;
}
