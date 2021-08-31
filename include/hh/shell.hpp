/// \file shell.hpp
/// \brief Created on 2021-08-30 by Ben

#pragma once
#include <hh/mini_stream.hpp>
#include <hh/ansi_codes.hpp>
#include <cctype>

namespace hh::cli {

template<serial_io IO, std::size_t n>
class shell {
private:
    enum class parser_state {
      normal,
      ansi_start,
      ansi_parse,
      ansi_digit,
      ansi_done,
      error,
    };
public:
    using olstream = mini_ostream<IO>;
    struct command {
      using cmd_function = int(void*);
      cmd_function* execute;
      const char* cmd_name;
      const char* help_message;
    };

    olstream lout;

    shell(command* command_table, std::size_t num_cmds);

protected:
    /// \todo add task function
    void process_rx_char(char ch)
    {
        bool done = false;
        while (!done) {
            switch (state_) {
            case parser_state::normal:
                // normal char
                if (ch=='\x1b') { state_ = parser_state::ansi_start; }
                // todo parse normal char (handle backspace and delete)
                break;
            case parser_state::ansi_start:
                if (ch=='[') { state_ = parser_state::ansi_param; }
                else { state_ = parser_state::error; }
                ansi_code_ = {};
                break;
            case parser_state::ansi_parse:
                // valid digit
                if (std::isdigit(ch)) {
                    state_ = parser_state::ansi_digit;
                    ++ansi_code_.num_params;
                    continue;
                }
                else if (std::isalpha(ch)) {
                    // valid terminator
                    state_ = parser_state::ansi_done;
                    continue;
                }
                else {
                    // error
                    state_ = parser_state::error;
                }
            case parser_state::ansi_digit:
                // digit
                if (std::isdigit(ch)) {
                    auto& p = ansi_code_.params[ansi_code_.num_params-1];
                    p *= 10;
                    p += ch-'0';
                }
                else if (ch==';') {
                    state_ = parser_state::ansi_param;
                    continue;
                }
                else if (std::isalpha(ch)) {
                    state_ = parser_state::ansi_done;
                    continue;
                }
                else {
                    state_ = parser_state::error;
                    continue;
                }
                break;
            case parser_state::ansi_done:
                // done
                ansi_code_.control_char = ch;
                state_ = parser_state::normal;
                break;
            case parser_state::error:
                // error
                state_ = parser_state::normal;
                break;
            }
            done = true;
        }
    }

    void on_connection() { lout << endl << welcome_message_ << endl << prompt_char_; }
private:
    struct ansi_code {
      std::uint8_t num_params{};
      std::uint8_t params[3]{};
      char control_char{};
    };

    const char* prev_cmd();
    const char* next_cmd();
    void parse_cmd(const char* cmd);

    bool process_ansi(char ch)
    {
        static std::size_t idx{0};
        static int i;
        static char c;
        if (idx==0) { return ch!='['; }
        if (std::isalpha(c)) {
            idx = 0;
        }
        if (std::isdigit(c)) { }
        ++idx;
    }

    IO io_;
    parser_state state_{parser_state::normal};
    ansi_code ansi_code_;
    const char* prompt_char_{"$"};
    const char* welcome_message_{""};
    const char* endl{"\n\r"};
    char storage_[n];
    std::uint8_t digit_idx;

};
}