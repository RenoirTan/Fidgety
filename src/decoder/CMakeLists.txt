fidgety_add_my_library(FidgetyDecoder STATIC decoder.cpp)
set_target_properties(FidgetyDecoder PROPERTIES OUTPUT_NAME fidgety_decoder)
fidgety_link_common_libraries(FidgetyDecoder)
fidgety_install_library(FidgetyDecoder fidgety_decoder_config.cmake)

if(FIDGETY_BUILD_EXTENSIONS)
    fidgety_add_my_library(FidgetyNormalConfDecoder SHARED normal_conf_decoder.cpp)
    set_target_properties(
        FidgetyNormalConfDecoder PROPERTIES
        OUTPUT_NAME fidgety_normal_conf_decoder
    )
    fidgety_link_common_libraries(FidgetyNormalConfDecoder)
    target_link_libraries(FidgetyNormalConfDecoder PUBLIC Fidgety::FidgetyDecoder)
    target_link_libraries(FidgetyNormalConfDecoder PRIVATE nlohmann_json)
    fidgety_install_library(FidgetyNormalConfDecoder fidgety_normal_conf_decoder_config.cmake)
endif()
