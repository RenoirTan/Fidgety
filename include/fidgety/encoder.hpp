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

    class Encoder {
        public:
            Encoder(void);
            ~Encoder(void);
            bool isConfOpened(void);
            EncoderStatus openConf(const std::string &inPath);
            EncoderStatus closeConf(void);
            EncoderStatus useNewConf(std::ofstream &&newConf);
            bool isIntermediateOpened(void);
            EncoderStatus openIntermediate(const std::string &inPath);
            EncoderStatus closeIntermediate(void);
            EncoderStatus useNewIntermediate(std::ifstream &&newIntermediate);
            virtual EncoderStatus dumpToConf(void) = 0;

        protected:
            std::ofstream mConfFile;
            std::ifstream mIntermediateFile;
    };

    class NormalConfEncoder : public Encoder {
        public:
            EncoderStatus dumpToConf(void);
    };
}

#endif