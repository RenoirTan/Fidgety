/**
 * @file include/fidgety/_utils.hpp
 * @author RenoirTan
 * @brief Private (not really) header for utility functions used internally by
 * the component libraries in Fidgety.
 * @version 0.1
 * @date 2022-02-17
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef _FIDGETY_UTILS_HPP
#   define _FIDGETY_UTILS_HPP

#   include <string>

namespace Fidgety {
    class StringEditor {
        public:
            virtual void mutateString(std::string &s);
            virtual std::string convertString(const std::string &s);
    };

    class StringIndenter : public StringEditor {
        public:
            StringIndenter(void);
            StringIndenter(bool useTabs);
            StringIndenter(size_t size);
            StringIndenter(bool useTabs, size_t size);

            void mutateString(std::string &s);
            std::string convertString(const std::string &s);

            static StringIndenter tabs(void);
            static StringIndenter spaces(void);

        protected:
            std::string mHeader;
    };

    void ltrim(std::string &s);
    void rtrim(std::string &s);
    void trim(std::string &s);
    bool isEffectivelyEmpty(const std::string &s);
    std::string sed(const std::string &s, StringEditor &m);
    std::string tabIndentSed(const std::string &s, uint32_t tabs=1);
    std::string spaceIndentSed(const std::string &s, uint32_t spaces=4);
}

#endif