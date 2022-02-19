add_library(FidgetyEncoder STATIC encoder.cpp)
add_library(Fidgety::FidgetyEncoder ALIAS FidgetyEncoder)
set_target_properties(FidgetyEncoder PROPERTIES OUTPUT_NAME fidgety_encoder)
target_link_libraries(FidgetyEncoder PUBLIC Fidgety::FidgetyHeaders)
target_link_libraries(FidgetyEncoder PRIVATE Fidgety::_FidgetyUtils)
target_link_libraries(FidgetyEncoder PRIVATE spdlog::spdlog)
install(
    TARGETS FidgetyEncoder
    EXPORT FidgetyEncoder
    DESTINATION ${export_lib_dir}
)
install(
    EXPORT FidgetyEncoder
    DESTINATION ${export_cmake_dir}
    NAMESPACE Fidgety::
    FILE fidgety_encoder_config.cmake
)

add_library(FidgetyNormalConfEncoder STATIC normal_conf_encoder.cpp)
add_library(Fidgety::FidgetyNormalConfEncoder ALIAS FidgetyNormalConfEncoder)
set_target_properties(FidgetyNormalConfEncoder PROPERTIES OUTPUT_NAME fidgety_normal_conf_encoder)
target_link_libraries(FidgetyNormalConfEncoder PUBLIC Fidgety::FidgetyEncoder)
target_link_libraries(FidgetyNormalConfEncoder PRIVATE Fidgety::_FidgetyUtils)
target_link_libraries(FidgetyNormalConfEncoder PRIVATE spdlog::spdlog)
target_link_libraries(FidgetyNormalConfDecoder PRIVATE nlohmann_json)
install(
    TARGETS FidgetyNormalConfEncoder
    EXPORT FidgetyNormalConfEncoder
    DESTINATION ${export_lib_dir}
)
install(
    EXPORT FidgetyNormalConfEncoder
    DESTINATION ${export_cmake_dir}
    NAMESPACE Fidgety::
    FILE fidgety_normal_conf_encoder_config.cmake
)
