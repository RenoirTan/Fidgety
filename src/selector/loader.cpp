/**
 * @file src/selector/loader.cpp
 * @author RenoirTan
 * @brief Source file for loading the different modules in Fidgety.
 * @version 0.1
 * @date 2022-03-17
 * 
 * @copyright Copyright (c) 2022
 */

#include <spdlog/spdlog.h>
#include <fidgety/selector.hpp>

using namespace Fidgety;

Loader::Loader(const Selector &selector) : mSelector(selector) {
    spdlog::trace("creating Fidgety::Loader by copying Fidgety::Selector");
}

Loader::Loader(Selector &&selector) : mSelector(std::move(selector)) {
    spdlog::trace("creating Fidgety::Loader by moving Fidgety::Selector");
}

Loader::Loader(
    const Selector &selector,
    DyclassLoader<Decoder> &&decoderLoader,
    DyclassLoader<Encoder> &&encoderLoader,
    DyclassLoader<Validator> &&validatorLoader,
    DyclassLoader<ValidatorContextCreator> &&vccLoader
) :
    mSelector(selector),
    mDecoderLoader(std::move(decoderLoader)),
    mEncoderLoader(std::move(encoderLoader)),
    mValidatorLoader(std::move(validatorLoader)),
    mVccLoader(std::move(vccLoader))
{
    spdlog::trace(
        "creating Fidgety::Loader by copying Fidgety::Selector and moving Fidgety::DyclassLoaders"
    );
}

Loader::Loader(
    Selector &&selector,
    DyclassLoader<Decoder> &&decoderLoader,
    DyclassLoader<Encoder> &&encoderLoader,
    DyclassLoader<Validator> &&validatorLoader,
    DyclassLoader<ValidatorContextCreator> &&vccLoader
) :
    mSelector(std::move(selector)),
    mDecoderLoader(std::move(decoderLoader)),
    mEncoderLoader(std::move(encoderLoader)),
    mValidatorLoader(std::move(validatorLoader)),
    mVccLoader(std::move(vccLoader))
{
    spdlog::trace(
        "creating Fidgety::Loader by moving Fidgety::Selector and Fidgety::DyclassLoaders"
    );
}

Loader::Loader(Loader &&loader) :
    mSelector(std::move(loader.mSelector)),
    mDecoderLoader(std::move(loader.mDecoderLoader)),
    mEncoderLoader(std::move(loader.mEncoderLoader)),
    mValidatorLoader(std::move(loader.mValidatorLoader)),
    mVccLoader(std::move(loader.mVccLoader))
{
    spdlog::trace("creating Fidgety::Loader using move constructor");
}

Loader &Loader::operator=(Loader &&loader) {
    spdlog::trace("reassigning Fidgety::Loader using move assignment");
    mSelector = std::move(loader.mSelector);
    mDecoderLoader = std::move(loader.mDecoderLoader);
    mEncoderLoader = std::move(loader.mEncoderLoader);
    mValidatorLoader = std::move(loader.mValidatorLoader);
    mVccLoader = std::move(loader.mVccLoader);

    return *this;
}

bool Loader::isDecoderLoaded(void) const noexcept {
    return mDecoderLoader.isOpened();
}

bool Loader::isEncoderLoaded(void) const noexcept {
    return mEncoderLoader.isOpened();
}

bool Loader::isValidatorLoaded(void) const noexcept {
    return mValidatorLoader.isOpened();
}

bool Loader::isValidatorContextCreatorLoaded(void) const noexcept {
    return mVccLoader.isOpened();
}

DylibStatus Loader::openDecoder(const std::string &dylibPath) {
    if (!dylibPath.empty()) {
        DylibStatus status = mDecoderLoader.changePath(dylibPath);
        if (status != DylibStatus::Ok) {
            return status;
        }
    }
    return mDecoderLoader.openLibrary();
}

DylibStatus Loader::openEncoder(const std::string &dylibPath) {
    if (!dylibPath.empty()) {
        DylibStatus status = mEncoderLoader.changePath(dylibPath);
        if (status != DylibStatus::Ok) {
            return status;
        }
    }
    return mEncoderLoader.openLibrary();
}

DylibStatus Loader::openValidator(const std::string &dylibPath) {
    if (!dylibPath.empty()) {
        DylibStatus status = mValidatorLoader.changePath(dylibPath);
        if (status != DylibStatus::Ok) {
            return status;
        }
    }
    return mValidatorLoader.openLibrary();
}

DylibStatus Loader::openValidatorContextCreator(const std::string &dylibPath) {
    if (!dylibPath.empty()) {
        DylibStatus status = mVccLoader.changePath(dylibPath);
        if (status != DylibStatus::Ok) {
            return status;
        }
    }
    return mVccLoader.openLibrary();
}

DylibStatus Loader::closeDecoder(void) {
    return mDecoderLoader.closeLibrary();
}

DylibStatus Loader::closeEncoder(void) {
    return mEncoderLoader.closeLibrary();
}

DylibStatus Loader::closeValidator(void) {
    return mValidatorLoader.closeLibrary();
}

DylibStatus Loader::closeValidatorContextCreator(void) {
    return mVccLoader.closeLibrary();
}

DyclassBox<Decoder> Loader::getDecoder(
    const std::string &allocClassSymbol,
    const std::string &deleteClassSymbol
) {
    if (!allocClassSymbol.empty()) mDecoderLoader.changeAllocClassSymbol(allocClassSymbol);
    if (!deleteClassSymbol.empty()) mDecoderLoader.changeDeleteClassSymbol(deleteClassSymbol);

    return mDecoderLoader.getInstance();
}

DyclassBox<Encoder> Loader::getEncoder(
    const std::string &allocClassSymbol,
    const std::string &deleteClassSymbol
) {
    if (!allocClassSymbol.empty()) mEncoderLoader.changeAllocClassSymbol(allocClassSymbol);
    if (!deleteClassSymbol.empty()) mEncoderLoader.changeDeleteClassSymbol(deleteClassSymbol);

    return mEncoderLoader.getInstance();
}

DyclassBox<Validator> Loader::getValidator(
    const std::string &allocClassSymbol,
    const std::string &deleteClassSymbol
) {
    if (!allocClassSymbol.empty()) mValidatorLoader.changeAllocClassSymbol(allocClassSymbol);
    if (!deleteClassSymbol.empty()) mValidatorLoader.changeDeleteClassSymbol(deleteClassSymbol);

    return mValidatorLoader.getInstance();
}

DyclassBox<ValidatorContextCreator> Loader::getValidatorContextCreator(
    const std::string &allocClassSymbol,
    const std::string &deleteClassSymbol
) {
    if (!allocClassSymbol.empty()) mVccLoader.changeAllocClassSymbol(allocClassSymbol);
    if (!deleteClassSymbol.empty()) mVccLoader.changeDeleteClassSymbol(deleteClassSymbol);

    return mVccLoader.getInstance();
}
