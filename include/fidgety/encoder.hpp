/**
 * @file include/fidgety/encoder.hpp
 * @author your name (you@domain.com)
 * @brief Main header library for the encoder in Fidgety. The encoder converts
 * the intermediate JSON used by the verifier and editor into the config file
 * the user wants to edit. The encoder is usually called when the user presses
 * the 'Save Changes' button in Fidgety (note that this does not mean that the
 * changes will take effect immediately, because a daemon has to be restarted
 * for example).
 * @version 0.1
 * @date 2022-02-17
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef FIDGETY_ENCODER_HPP
#   define FIDGETY_ENCODER_HPP

#   include <fstream>
#   include <fidgety/exception.hpp>

namespace Fidgety {
    enum class EncoderStatus : int32_t {
        Ok = 0,
        FileNotFound = 1,
        CannotReadFile = 2,
        CannotWriteFile = 3,
        CannotCloseFile = 4,
        ResourceBusy = 5,
        CannotOpenMultipleFiles = 6,
        FilesNotOpen = 7,
        SyntaxError = 8,
        InvalidDataType = 9,
        VerifierError = 10
    };

    class EncoderException : public Exception {
        public:
            using Exception::Exception;
            std::string codeAsErrorType(void) const;
        
        protected:
            const char *getSimpleWhat(void) const noexcept;
    };

    class Encoder {
        public:
            Encoder(void) noexcept;
            ~Encoder(void);
            bool isConfOpened(void) noexcept;
            void openConf(const std::string &inPath);
            void closeConf(void);
            void useNewConf(std::ofstream &&newConf);
            bool isIntermediateOpened(void) noexcept;
            void openIntermediate(const std::string &inPath);
            void closeIntermediate(void);
            void useNewIntermediate(std::ifstream &&newIntermediate);
            virtual void dumpToConf(void);

        protected:
            std::ofstream mConfFile;
            std::ifstream mIntermediateFile;
    };
}

#endif