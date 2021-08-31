/// \file shell.hpp
/// \brief Created on 2021-08-30 by Ben

#pragma once
#include <hh/mini_stream.hpp>
#include <hh/ansi_parser.hpp>
#include <cctype>

namespace hh::cli {

template<serial_io IO, std::size_t n>
class shell {
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
    }

    void on_connection() { lout << endl << welcome_message_ << endl << prompt_char_; }
private:
    const char* prev_cmd();
    const char* next_cmd();
    void parse_cmd(const char* cmd);

    IO io_;
    const char* prompt_char_{"$"};
    const char* welcome_message_{""};
    const char* endl{"\n\r"};
    char storage_[n];

};
}