fidgety_add_my_library(FidgetyDecoder STATIC decoder.cpp)
set_target_properties(FidgetyDecoder PROPERTIES OUTPUT_NAME fidgety_decoder)
fidgety_set_output_directory(FidgetyDecoder)
fidgety_link_common_libraries(FidgetyDecoder)
fidgety_link_exception(FidgetyDecoder)
target_link_libraries(FidgetyDecoder PUBLIC nlohmann_json::nlohmann_json)
fidgety_install_library(FidgetyDecoder fidgety_decoder_config.cmake)

if(FIDGETY_BUILD_EXTENSIONS)
    fidgety_add_my_library(FidgetyNormalConfDecoder SHARED normal_conf_decoder.cpp)
    set_target_properties(
        FidgetyNormalConfDecoder PROPERTIES
        OUTPUT_NAME fidgety_normal_conf_decoder
    )
    fidgety_set_output_directory(FidgetyNormalConfDecoder)
    fidgety_link_common_libraries(FidgetyNormalConfDecoder)
    target_link_libraries(FidgetyNormalConfDecoder PUBLIC Fidgety::FidgetyDecoder)
    fidgety_install_extension(FidgetyNormalConfDecoder fidgety_normal_conf_decoder_config.cmake)
endif()
