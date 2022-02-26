fidgety_add_my_library(FidgetyEncoder STATIC encoder.cpp)
set_target_properties(FidgetyEncoder PROPERTIES OUTPUT_NAME fidgety_encoder)
fidgety_link_common_libraries(FidgetyEncoder)
fidgety_link_exception(FidgetyEncoder)
fidgety_install_library(FidgetyEncoder fidgety_encoder_config.cmake)

if(FIDGETY_BUILD_EXTENSIONS)
    fidgety_add_my_library(FidgetyNormalConfEncoder SHARED normal_conf_encoder.cpp)
    set_target_properties(
        FidgetyNormalConfEncoder PROPERTIES
        OUTPUT_NAME fidgety_normal_conf_encoder
    )
    fidgety_link_common_libraries(FidgetyNormalConfEncoder)
    target_link_libraries(FidgetyNormalConfEncoder PUBLIC Fidgety::FidgetyEncoder)
    target_link_libraries(FidgetyNormalConfDecoder PRIVATE nlohmann_json)
    fidgety_install_library(FidgetyNormalConfEncoder fidgety_normal_conf_encoder_config.cmake)
endif()
