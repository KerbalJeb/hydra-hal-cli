/// \file ansi_parser.cpp
/// \brief Created on 2021-08-30 by Ben

#include <hh/ansi_parser.hpp>

hh::ansi::parser::parser(const char* s, std::size_t count)
{
    for (std::size_t i = 0; i<count; ++i, ++s) {
        if (parse(*s)) { break; }
    }
    if (!done_parsing()){
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
            code_.control_char = ch;
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
            ++code_.num_params;
            currentState_ = state::digit;
            return;
        }
        else if (is_terminator(ch)) {
            ++code_.num_params;
            code_.control_char = ch;
            currentState_ = state::done;
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

bool hh::ansi::parser::parse(char ch)
{
    update_state(ch);
    return done_parsing();
}

bool hh::ansi::parser::is_terminator(char ch)
{
    return std::isalpha(ch);
}
void hh::ansi::parser::reset()
{
    status_ = true;
    currentState_ = state::start;
    code_ = {};
}
