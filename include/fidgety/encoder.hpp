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