#ifndef FIDGETY_DECODER_HPP
#   define FIDGETY_DECODER_HPP

#   include <fstream>

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
            virtual DecoderStatus dumpToIntermediate(void) = 0;
        
        protected:
            std::ifstream mConfFile;
            std::ofstream mIntermediateFile;
    };

    class NormalConfDecoder : public Decoder {
        public:
            DecoderStatus dumpToIntermediate(void);
    };
}

#endif