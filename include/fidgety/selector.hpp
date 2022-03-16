/**
 * @file include/fidgety/selector.hpp
 * @author RenoirTan
 * @brief Header file for the selector portion of Fidgety. The editor tells the
 * selector which config file is going to be edited by the user and the
 * selector chooses which validators, encoders and decoders have to be loaded.
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef FIDGETY_SELECTOR_HPP
#   define FIDGETY_SELECTOR_HPP

#   include <fidgety/appdata.hpp>
#   include <fidgety/dylib.hpp>
#   include <fidgety/decoder.hpp>
#   include <fidgety/encoder.hpp>
#   include <fidgety/exception.hpp>
#   include <fidgety/options.hpp>
#   include <fidgety/verifier.hpp>

namespace Fidgety {
    enum class SelectorStatus : int32_t {
        Ok = 0,
        InvalidInfo = 1
    };

    class SelectorException : public Exception {
        public:
            using Exception::Exception;
            std::string codeAsErrorType(void) const;
        
        protected:
            const char *getSimpleWhat(void) const noexcept;
    };

    class Selector {
        public:
            Selector(void) = delete;
            Selector(const Appdata &appdata);
            Selector(Appdata &&appdata);

            bool isValid(void) const;

            std::string getDecoderLocation(void);
            std::string getEncoderLocation(void);
            std::string getValidatorLocation(void);
            std::string getValidatorContextCreatorLocation(void);

        protected:
            Appdata mAppdata;
    };

    class Loader {
        public:
            Loader(void) = delete;
            Loader(const Selector &selector);
            Loader(Selector &&selector);
            Loader(
                const Selector &selector,
                DyclassLoader<Decoder> decoderLoader,
                DyclassLoader<Encoder> encoderLoader,
                DyclassLoader<Validator> validatorLoader,
                DyclassLoader<ValidatorContextCreator> vccLoader
            );
            Loader(
                Selector &&selector,
                DyclassLoader<Decoder> decoderLoader,
                DyclassLoader<Encoder> encoderLoader,
                DyclassLoader<Validator> validatorLoader,
                DyclassLoader<ValidatorContextCreator> vccLoader
            );
            ~Loader(void);

            Loader(Loader &&selector);
            Loader &operator=(Loader &&selector);

            bool isDecoderLoaded(void) const noexcept;
            bool isEncoderLoaded(void) const noexcept;
            bool isValidatorLoaded(void) const noexcept;
            bool isValidatorContextCreatorLoaded(void) const noexcept;

            DylibStatus openDecoder(const std::string &dylibPath = "");
            DylibStatus openEncoder(const std::string &dylibPath = "");
            DylibStatus openValidator(const std::string &dylibPath = "");
            DylibStatus openValidatorContextCreator(const std::string &dylibPath = "");

            DylibStatus closeDecoder(void);
            DylibStatus closeEncoder(void);
            DylibStatus closeValidator(void);
            DylibStatus closeValidatorContextCreator(void);

            DyclassBox<Decoder> getDecoder(
                const std::string &allocClassSymbol = "",
                const std::string &deleteClassSymbol = ""
            );
            DyclassBox<Encoder> getEncoder(
                const std::string &allocClassSymbol = "",
                const std::string &deleteClassSymbol = ""
            );
            DyclassBox<Validator> getValidator(
                const std::string &allocClassSymbol = "",
                const std::string &deleteClassSymbol = ""
            );
            DyclassBox<ValidatorContextCreator> getValidatorContextCreator(
                const std::string &allocClassSymbol = "",
                const std::string &deleteClassSymbol = ""
            );

        protected:
            Selector mSelector;
            DyclassLoader<Decoder> mDecoderLoader;
            DyclassLoader<Encoder> mEncoderLoader;
            DyclassLoader<Validator> mValidatorLoader;
            DyclassLoader<ValidatorContextCreator> mVccLoader;
    };
};

#endif
