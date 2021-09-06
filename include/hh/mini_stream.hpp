/// \file mini_stream.hpp
/// \brief Lightweight implementation of c++ streams
/// \todo add format specifiers

#pragma once
#include <cmath>
#include <cstdint>
#include <cstring>
#include <hh/concepts.hpp>
#include <hh/hal_assert.hpp>
#include <limits>

namespace hh::shell {

    /// \brief Serves as the base class for all mini stream I/O classes, equivalent to `std::ios_base`
    class mini_ios_base {
    public:
        using fmtflags = std::uint32_t;
        using streamsize = std::size_t;

        mini_ios_base()
            : flags_(dec | left) {}
        explicit mini_ios_base(fmtflags flags)
            : flags_(flags) {}

        static constexpr fmtflags dec = 1 << 0;
        static constexpr fmtflags oct = 1 << 1;
        static constexpr fmtflags hex = 1 << 2;
        static constexpr fmtflags basefield = dec | oct | hex;

        static constexpr fmtflags left = 1 << 3;
        static constexpr fmtflags right = 1 << 4;
        static constexpr fmtflags internal = 1 << 5;
        static constexpr fmtflags adjustfield = left | right | internal;

        /// \brief Gets the current format settings
        /// \return the current format setting
        [[nodiscard]] fmtflags flags() const { return flags_; }
        /// \brief replaces the current format settings
        /// \param flags new format settings
        /// \return the formatting flags before the call to the function
        fmtflags flags(fmtflags flags);

        /// \brief Sets the given flags
        /// \param flags The flags to set
        /// \return the formatting flags before the call to the function
        fmtflags setf(fmtflags flags);
        /// \brief clears the formatting flags under mask, and sets the cleared flags to those specified by flags
        /// \param flags the flags to set
        /// \param mask the flags clear before setting
        /// \return the formatting flags before the call to the function
        fmtflags setf(fmtflags flags, fmtflags mask);

        /// \brief Clears the given flags
        /// \param flags the flags to clear
        /// \return the formatting flags before the call to the function
        fmtflags unsetf(fmtflags flags);

    private:
        fmtflags flags_{};
    };

    class mini_basic_ios : public mini_ios_base {
    public:
        using iostate = std::uint8_t;

        mini_basic_ios() = default;
        static constexpr iostate goodbit = 0;
        static constexpr iostate badbit = 1 << 0;
        static constexpr iostate failbit = 1 << 1;
        static constexpr iostate eofbit = 1 << 2;

        /// \brief Returns the current field width.
        ///
        /// Manages the minimum number of characters to generate
        /// on certain output operations and the maximum number of characters to generate on certain input operations.
        /// \return The field width before the call to the function
        streamsize width() const;

        /// \brief Sets the field width to the given one. Returns the previous field width.
        ///
        /// Manages the minimum number of characters to generate
        /// on certain output operations and the maximum number of characters to generate on certain input operations.
        /// \param new_width The new width
        /// \return The field width before the call to the function
        streamsize width(streamsize new_width);

        bool good() const;
        bool eof() const;
        bool fail() const;
        bool bad() const;
        bool operator!() const;
        operator bool() const;

        iostate rdstate() const;
        void setstate(iostate state);
        void clear(iostate state = goodbit);

    private:
        iostate state_{};
    };

    template<serial_out_device Out>
    class oserial_stream : public mini_basic_ios {
    public:
        explicit oserial_stream(Out &out)
            : output_{out} {}

        // Not movable or copyable
        oserial_stream(const oserial_stream &) = delete;
        oserial_stream(oserial_stream &&) = delete;
        oserial_stream &operator=(const oserial_stream &) = delete;
        oserial_stream &operator=(oserial_stream &&) = delete;

        /// \brief String stream inserter operator
        /// \param string a null terminated string
        /// \return `*this`
        oserial_stream &operator<<(const char *string) {
            write(string, std::strlen(string));
            return *this;
        }

        /// \brief Char stream inserter operator
        /// \param ch a charter
        /// \return `*this`
        oserial_stream &operator<<(char ch) {
            put(ch);
            return *this;
        }

        /// \brief Integer stream inserter operator
        /// \param value an integer value
        /// \return `*this`
        template<std::integral T>
        oserial_stream &operator<<(T value) {
            constexpr auto max_str_len = std::numeric_limits<T>::digits + 1;
            char s[max_str_len];
            int start_idx = itoa(value, s, max_str_len);
            write(&s[start_idx], max_str_len - start_idx);
            return *this;
        }

        /// \brief Writes a char to the output buffer
        /// \param ch The char
        /// \return `*this`
        oserial_stream &put(char ch) {
            output_.write(&ch, 1);
            return *this;
        }

        /// \brief Writes a char array to the output buffer
        /// \param s The char buffer
        /// \param count The size of the char buffer
        /// \return `*this`
        oserial_stream &write(const char *s, streamsize count) {
            output_.write(s, count);
            return *this;
        }

        /// \brief Flushes the output buffer
        ///
        /// Forces all data to be written to the SerialOutput device, blocks current thread until write is completed.
        /// \post `empty()` returns true
        /// \return `*this`
        oserial_stream &flush() {
            output_.flush();
            return *this;
        }

    private:
        Out &output_;
        static constexpr char ascii_digits[] = "0123456789abcdef";

        [[nodiscard]] std::size_t get_base() {
            // todo remove putcs from this function
            if (flags() & dec) {
                return 10;
            } else if (flags() & oct) {
                put('0');
                return 8;
            } else if (flags() & hex) {
                put('0');
                put('x');
                return 16;
            } else {
                return 0;
            }
        }

        template<std::unsigned_integral T>
        streamsize itoa(T value, char *s, streamsize count) {
            auto base = get_base();
            assert(base > 0);

            if (value == 0) {
                s[count - 1] = ascii_digits[0];
                return count - 1;
            }

            streamsize i;
            for (i = count - 1; value && i; --i, value /= base) {
                s[i] = ascii_digits[value % base];
            }
            return i + 1;
        }

        template<std::signed_integral T>
        int itoa(T value, char *s, streamsize count) {
            auto idx = itoa((std::make_unsigned_t<T>) std::abs(value), s, count);
            if (value < 0) {
                --idx;
                s[idx] = '-';
            }
            return idx;
        }
    };
}// namespace hh::shell