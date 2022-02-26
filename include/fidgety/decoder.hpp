/**
 * @file include/fidgety/decoder.hpp
 * @author RenoirTan
 * @brief Main header library for the decoder in Fidgety. The decoder converts
 * a config file (such as /etc/tlp.conf) into an intermediate JSON that is
 * readable by the verifier and editor. It is one of the first few things to
 * be called up when a user wants to edit a config file through Fidgety.
 * @version 0.1
 * @date 2022-02-17
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef FIDGETY_DECODER_HPP
#   define FIDGETY_DECODER_HPP

#   include <fstream>
#   include <fidgety/exception.hpp>

namespace Fidgety {
    enum class DecoderStatus : int32_t {
        Ok = 0,
        FileNotFound = 1,
        CannotReadFile = 2,
        CannotWriteFile = 3,
        CannotCloseFile = 4,
        ResourceBusy = 5,
        CannotOpenMultipleFiles = 6,
        FilesNotOpen = 7,
        SyntaxError = 8
    };

    class DecoderException : public Exception {
        public:
            std::string codeAsErrorType(void) const;
        
        protected:
            const char *getSimpleWhat(void) const noexcept;
    };

    class Decoder {
        public:
            Decoder(void);
            ~Decoder(void);
            bool isConfOpened(void);
            DecoderStatus openConf(const std::string &inPath);
            DecoderStatus closeConf(void);
            DecoderStatus useNewConf(std::ifstream &&newConf);
            bool isIntermediateOpened(void);
            DecoderStatus openIntermediate(const std::string &outPath);
            DecoderStatus closeIntermediate(void);
            DecoderStatus useNewIntermediate(std::ofstream &&newIntermediate);
            virtual DecoderStatus dumpToIntermediate(void);
        
        protected:
            std::ifstream mConfFile;
            std::ofstream mIntermediateFile;
    };
}

#endif