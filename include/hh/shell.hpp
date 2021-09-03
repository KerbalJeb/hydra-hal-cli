/// \file shell.hpp
/// \brief Created on 2021-08-30 by Ben
/// \todo deal with sharing streams between classes

#pragma once
#include <hh/mini_stream.hpp>
#include <hh/ansi_parser.hpp>
#include <hh/ansi_codes.hpp>
#include <hh/cmd_history.hpp>
#include <cctype>
#include <string>

namespace hh::cli {

template<serial_io_device IO, std::size_t n>
class shell {
public:
    using olstream = oserial_stream<IO>;
    struct command {
      using cmd_function = int(void*);
      cmd_function* execute;
      const char* cmd_name;
      const char* help_message;
    };

    olstream lout;

    explicit shell(IO& io)
            :io_{io}, lout{io} { }

    shell(IO& io, command* command_table, std::size_t num_cmds)
            :shell(io) { }

    [[nodiscard]] const char* current_line() const{
        return storage_;
    }

protected:
    // todo decide if these should just be public
    void notify_rx(){
        auto ch = io_.get();

        while (ch != eof_){
            process_rx_char(ch);
            ch = io_.get();
        }
    }

    void notify_connected() { lout << endl << welcome_message_ << endl << prompt_char_; }
private:

    enum class parser_state {
      text,
      ansi_cmd,
    };

    void process_rx_char(char ch)
    {
        switch (state_) {
        case parser_state::text:
            parse_text_char(ch);
            break;
        case parser_state::ansi_cmd:
            if (parser_.parse(ch)) {
                state_ = parser_state::text;
            }
            break;
        }
    }

    void parse_text_char(char ch)
    {
        switch (ch) {
        case '\x1b':
            state_ = parser_state::ansi_cmd;
            parser_.reset();
            break;
        case '\n':
        case '\r':
            lout << endl << prompt_char_;
            storage_end_ = storage_;
            storage_[0] = '\0';
            break;
        case '\b':
            if (storage_end_>storage_) {
                *(--storage_end_) = '\0';
                lout << ansi::save_cursor << ansi::clear_line << prompt_char_ << storage_;
                lout << ansi::restore_cursor << ansi::move_left<1>;
            }
            break;
        default:
            lout << ch;
            *storage_end_++ = ch;
            break;
        }
    }

    IO& io_;
    using int_type = std::char_traits<char>::int_type;
    static constexpr int_type eof_ = std::char_traits<char>::eof();
    const char prompt_char_{'>'};
    const char* welcome_message_{""};
    const char* endl{"\n\r"};
    char storage_[n]{};
    char* storage_end_{storage_};
    parser_state state_{parser_state::text};
    ansi::parser parser_{};
};
}