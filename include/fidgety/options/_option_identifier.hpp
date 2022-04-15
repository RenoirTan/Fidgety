/**
 * @file include/fidgety/options/_option_identifier.hpp
 * @author RenoirTan
 * @brief Declaration for option 
 * @version 0.1
 * @date 2022-04-15
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef _FIDGETY_OPTIONS_OPTION_IDENTIFIER_HPP
#   define _FIDGETY_OPTIONS_OPTION_IDENTIFIER_HPP

#   include "_fwd.hpp"

namespace Fidgety {
    // Always use char array just in case
    constexpr char OPTION_NAME_DELIMITER[] = ".";

    class OptionIdentifier {
        public:

            struct Iterator {
                using iterator_category = std::random_access_iterator_tag;
                using difference_type = std::ptrdiff_t;
                using value_type = const OptionName;
                using pointer = value_type*;
                using reference = value_type&;

                static const int32_t VALID = 0;
                static const int32_t OUT_OF_BOUNDS = 1;

                // dereference

                reference operator*(void);
                pointer operator->(void);
                reference operator[](difference_type steps);

                // traversal

                Iterator &operator+=(difference_type steps);
                Iterator &operator-=(difference_type steps);

                Iterator operator+(difference_type steps) const;
                Iterator operator-(difference_type steps) const;

                Iterator &operator++(void); // ++it
                Iterator operator++(int); // it++
                Iterator &operator--(void); // --it
                Iterator operator--(int); // it--

                // comparison

#   define _FIDGETY_OIIT_CMPEX(cmpOp) \
    ( \
        a.identifier == b.identifier && \
        ( \
            (a.state == b.state) && \
            ( \
                (a.state == OptionIdentifier::Iterator::VALID && a.index cmpOp b.index) || \
                a.state == OptionIdentifier::Iterator::OUT_OF_BOUNDS \
            ) \
        ) \
    ) \

                friend bool operator==(const Iterator &a, const Iterator &b) {
                    return _FIDGETY_OIIT_CMPEX(==);
                }

                friend bool operator!=(const Iterator &a, const Iterator &b) {
                    return !(a == b);
                }

                friend bool operator<(const Iterator &a, const Iterator &b) {
                    return _FIDGETY_OIIT_CMPEX(<);
                }

                friend bool operator>(const Iterator &a, const Iterator &b) {
                    return _FIDGETY_OIIT_CMPEX(>);
                }

                friend bool operator<=(const Iterator &a, const Iterator &b) {
                    return _FIDGETY_OIIT_CMPEX(<=);
                }

                friend bool operator>=(const Iterator &a, const Iterator &b) {
                    return _FIDGETY_OIIT_CMPEX(>=);
                }

#   undef _FIDGETY_OIIT_CMPEX

                const OptionIdentifier *identifier;
                size_t index;
                OptionName name;
                int32_t state;
            };

            OptionIdentifier(const std::string &path);
            OptionIdentifier(std::string &&path);
            OptionIdentifier(const char path[]);

            OptionIdentifier &operator=(const std::string &path);
            OptionIdentifier &operator=(std::string &&path);

            bool isValid(void) const noexcept;

#   define _FIDGETY_OI_CMP(cmpOp) \
    friend bool operator cmpOp(const OptionIdentifier &a, const OptionIdentifier &b) { \
        return a.mPath cmpOp b.mPath; \
    } \
    \
    friend bool operator cmpOp(const OptionIdentifier &a, const std::string &b) { \
        return a.mPath cmpOp b; \
    } \
    friend bool operator cmpOp(const std::string &a, const OptionIdentifier &b) { \
        return a cmpOp b.mPath; \
    } \
    friend bool operator cmpOp(const OptionIdentifier &a, const char b[]) {\
        return a.mPath cmpOp b; \
    } \
    friend bool operator cmpOp(const char a[], const OptionIdentifier &b) { \
        return b.mPath cmpOp a; \
    } \

            _FIDGETY_OI_CMP(==)
            _FIDGETY_OI_CMP(!=)
            _FIDGETY_OI_CMP(<)
            _FIDGETY_OI_CMP(<=)
            _FIDGETY_OI_CMP(>)
            _FIDGETY_OI_CMP(>=)

#   undef _FIDGETY_OI_CMP

            operator const std::string &(void) const;
            const std::string &getPath(void) const;

            OptionIdentifier &operator+=(const std::string &addon);
            OptionIdentifier operator+(const std::string &addon) const;

            static const size_t npos;
            size_t findSubset(const OptionIdentifier &identifier) const;

            size_t depth(void) const;
            std::vector<OptionName> split(void) const;
            Iterator at(size_t index) const;
            Iterator begin(void) const;
            Iterator end(void) const;

        protected:
            std::string mPath;
    };
}

#endif
