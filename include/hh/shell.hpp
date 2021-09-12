/// \file shell.hpp
/// \brief Created on 2021-08-30 by Ben
/// \todo Run commands


#pragma once
#include <cctype>
#include <hh/ansi_codes.hpp>
#include <hh/ansi_parser.hpp>
#include <hh/cmd_history.hpp>
#include <hh/fixed_string.h>
#include <hh/mini_stream.hpp>
#include <string>

namespace hh::shell {

    template<serial_io_device IO, std::size_t NumLines, std::size_t LineLen>
    class shell {
    public:
        using olstream = oserial_stream<IO>;
        struct command {
            using cmd_function = int(void *);
            cmd_function *execute;
            const char *cmd_name;
            const char *help_message;
        };

        olstream lout;

        explicit shell(IO &io)
            : lout{io}, io_{io} {}

        shell(IO &io, const command *command_table, std::size_t num_cmds)
            : io_{io}, lout{io}, commandTable_{command_table}, numCommands_{num_cmds} {}

        [[nodiscard]] std::string_view current_line() const {
            return currentLine_.c_str();
        }

        void notify_rx() {
            auto ch = io_.get();

            while (ch != eof_) {
                process_rx_char(ch);
                ch = io_.get();
            }
        }

        void notify_connected() { lout << endl
                                       << welcome_message_ << endl
                                       << prompt_char_; }

    private:
        using history = cmd_history<NumLines, LineLen>;
        using int_type = std::char_traits<char>::int_type;
        using cmd_iter_type = typename history::const_iterator;
        using string = container::fixed_string<LineLen>;

        enum class parser_state {
            text,
            ansi_cmd,
        };

        void process_rx_char(char ch) {
            switch (state_) {
                case parser_state::text:
                    parse_text_char(ch);
                    break;
                case parser_state::ansi_cmd:
                    if (parser_.parse(ch)) {
                        state_ = parser_state::text;
                        // handle ansi command
                        handle_ansi_cmd(parser_.code());
                    }
                    break;
            }
        }

        void handle_ansi_cmd(const ansi::parser::ansi_code &code) {
            switch (code.control_char) {
                case 'A':
                    // up
                    if (!onFirstCmd_) {
                        currentLine_.clear();
                        currentLine_.append(prevCommand_->data());
                        lout << ansi::clear_line << prompt_char_;
                        lout << prevCommand_->data();
                        if (prevCommand_ == history_.begin()) {
                            onFirstCmd_ = true;
                        } else {
                            onFirstCmd_ = false;
                            --prevCommand_;
                        }
                    }

                    break;
                case 'B':
                    // down
                    {
                        auto it = prevCommand_;
                        ++it;
                        if (!onFirstCmd_ && it != history_.end()) {
                            ++it;
                        }
                        if (it != history_.end()) {
                            currentLine_.clear();
                            currentLine_.append(it->data());
                            lout << ansi::clear_line << prompt_char_;
                            lout << it->data();
                            prevCommand_ = it;
                        }
                    }
                    onFirstCmd_ = false;
                    break;
                case 'C':
                    // right
                    cursor_ += code.params[0];
                    lout << "\x1b[" << code.params[0] << 'C';
                    break;
                case 'D':
                    cursor_ -= code.params[0];
                    lout << "\x1b[" << code.params[0] << 'D';
                    // left
                    break;
                default:
                    break;
            }
        }

        void parse_text_char(char ch) {
            switch (ch) {
                case '\x1b':
                    state_ = parser_state::ansi_cmd;
                    parser_.reset();
                    break;
                case '\n':
                case '\r':
                    lout << endl
                         << prompt_char_;
                    history_.push_back(currentLine_.c_str());
                    prevCommand_ = history_.end();
                    --prevCommand_;
                    currentLine_.clear();
                    cursor_ = currentLine_.begin();
                    // todo: run command from command table
                    break;
                case '\b':
                    if (!currentLine_.empty()) {
                        currentLine_.erase(--cursor_);
                        lout << ansi::save_cursor << ansi::clear_line << prompt_char_ << currentLine_.c_str();
                        lout << ansi::restore_cursor << ansi::move_left<1>;
                    }
                    break;
                default:
                    lout << ch;
                    currentLine_.insert(cursor_, ch);
                    ++cursor_;
                    break;
            }
        }

        IO &io_;
        static constexpr int_type eof_ = std::char_traits<char>::eof();
        const char prompt_char_{'>'};
        const char *welcome_message_{""};
        const char *endl{"\n\r"};
        history history_;
        string currentLine_;
        cmd_iter_type prevCommand_{history_.begin()};
        bool onFirstCmd_ = false;
        const char *cursor_{currentLine_.begin()};
        parser_state state_{parser_state::text};
        ansi::parser parser_{};
        const command *commandTable_{nullptr};
        std::size_t numCommands_{0};
    };
}// namespace hh::shell