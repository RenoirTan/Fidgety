/**
 * @file include/fidgety/dylib.hpp
 * @author RenoirTan
 * @brief Header file for managing dynamic libraries by Fidgety.
 * Adapted from `https://blog.theopnv.com/posts/cpp-dynamic-loading/`.
 * @version 0.1
 * @date 2022-03-16
 *
 * @copyright Copyright (c) 2022
 */

#ifndef FIDGETY_DYLIB_HPP
#   define FIDGETY_DYLIB_HPP

#   include <dlfcn.h>
#   include <memory>
#   include <string>
#   include "spdlog/spdlog.h"
#   include <fidgety/exception.hpp>
#   include <fidgety/_utils.hpp>

namespace Fidgety {
    /*
     * Pointer to a function that returns an instance of the class.
     */
    template <typename T>
    using AllocClass = T *(*)(void);

    /*
     * Pointer to a function that deletes an instance of the class.
     */
    template <typename T>
    using DeleteClass = void (*)(T *);

    template <typename T>
    using DyclassBox = std::unique_ptr<T, DeleteClass<T>>;

    enum class DylibStatus : int32_t {
        Ok = 0,
        FileNotFound = 1,
        CannotReadFile = 2,
        DylibNotLoaded = 3,
        DylibAlreadyLoaded = 4,
        DylibNotClosed = 5,
        CannotOpenDylib = 6,
        CannotCloseDylib = 7,
        BadDylibInternals = 8
    };

    class DylibException : public Exception {
        public:
            using Exception::Exception;
            std::string codeAsErrorType(void) const {
                switch (mCode) {
                    case 0: return "Ok";
                    case 1: return "FileNotFound";
                    case 2: return "CannotReadFile";
                    case 3: return "DylibNotLoaded";
                    case 4: return "DylibAlreadyLoaded";
                    case 5: return "DylibNotClosed";
                    case 6: return "CannotOpenDylib";
                    case 7: return "CannotCloseDylib";
                    case 8: return "BadDylibInternals";
                    default: return "Other";
                }
            }
        
        protected:
            const char *getSimpleWhat(void) const noexcept {
                return "A Fidgety::DylibException occurred.";
            }
    };

    template <typename T>
    class DyclassLoader {
        public:
            DyclassLoader(
                const std::string &dylibPath,
                const std::string &allocClassSymbol = "allocator",
                const std::string &deleteClassSymbol = "deleter"
            ) :
                mHandle(nullptr),
                mDylibPath(dylibPath),
                mAllocClassSymbol(allocClassSymbol),
                mDeleteClassSymbol(deleteClassSymbol)
            { }

            ~DyclassLoader() = default;

            DyclassLoader(const DyclassLoader &loader) = delete;
            DyclassLoader &operator=(const DyclassLoader &loader) = delete;

            DylibStatus changePath(const std::string &dylibPath) {
                if (mHandle) {
                    FIDGETY_ERROR(
                        DylibException,
                        DylibStatus::DylibNotClosed,
                        "{0} must be closed before changing mDylibPath",
                        mDylibPath
                    );
                } else {
                    mDylibPath = dylibPath;
                    return DylibStatus::Ok;
                }
            }

            DylibStatus changeAllocClassSymbol(const std::string &allocClassSymbol) {
                mAllocClassSymbol = allocClassSymbol;
                return DylibStatus::Ok;
            }

            DylibStatus changeDeleteClassSymbol(const std::string &deleteClassSymbol) {
                mDeleteClassSymbol = deleteClassSymbol;
                return DylibStatus::Ok;
            }

            bool isOpened(void) const noexcept {
                return mHandle != nullptr;
            }

            DylibStatus openLibrary(void) {
                if (mHandle) {
                    FIDGETY_ERROR(
                        DylibException,
                        DylibStatus::DylibAlreadyLoaded,
                        "DyclassLoader::mHandle is already being used"
                    );
                }
                mHandle = dlopen(mDylibPath.c_str(), RTLD_NOW | RTLD_LAZY);
                if (mHandle) {
                    return DylibStatus::Ok;
                } else {
                    FIDGETY_ERROR(
                        DylibException,
                        DylibStatus::CannotOpenDylib,
                        "could not open dylib at '{0}'. dlerror: {1}",
                        mDylibPath,
                        dlerror()
                    );
                }
            }

            DylibStatus closeLibrary(void) {
                if (mHandle) {
                    if (dlclose(mHandle) != 0) {
                        FIDGETY_ERROR(
                            DylibException,
                            DylibStatus::CannotCloseDylib,
                            "could not close dylib at '{0}'. dlerror {1}",
                            mDylibPath,
                            dlerror()
                        );
                    } else {
                        mHandle = nullptr;
                    }
                } else {
                    spdlog::warn("DyclassLoader::mHandle is not open");
                }
                return DylibStatus::Ok;
            }

            DyclassBox<T> getInstance(void) {
                if (!mHandle) {
                    FIDGETY_CRITICAL(
                        DylibException,
                        DylibStatus::DylibNotLoaded,
                        "DyclassLoader::mHandle doesn't have an active dylib loaded yet"
                    );
                }

                // Look for a symbol named "allocator" (or whatever) and assign to allocFunc
                auto allocFunc = (AllocClass<T>) dlsym(mHandle, mAllocClassSymbol.c_str());
                if (!allocFunc) {
                    FIDGETY_CRITICAL(
                        DylibException,
                        DylibStatus::BadDylibInternals,
                        "could not load {0} function from {1} in DyclassLoader::getInstance",
                        mAllocClassSymbol,
                        mDylibPath
                    );
                }

                // Look for a symbol named "deleter" (or whatever) and assign to deleteFunc
                auto deleteFunc = (DeleteClass<T>) dlsym(mHandle, mDeleteClassSymbol.c_str());
                if (!deleteFunc) {
                    FIDGETY_CRITICAL(
                        DylibException,
                        DylibStatus::BadDylibInternals,
                        "could not load {0} function from {1} in DyclassLoader::getInstance",
                        mDeleteClassSymbol,
                        mDylibPath
                    );
                }

                // create the class from allocFunc and set the destructor to deleteFunc.
                return DyclassBox<T>(allocFunc(), deleteFunc);
            }

        protected:
            void *mHandle;
            std::string mDylibPath;
            std::string mAllocClassSymbol;
            std::string mDeleteClassSymbol;
    };
}

#endif
