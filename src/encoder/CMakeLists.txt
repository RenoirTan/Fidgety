add_library(FidgetyEncoder STATIC encoder.cpp)
add_library(Fidgety::FidgetyEncoder ALIAS FidgetyEncoder)
target_link_libraries(FidgetyEncoder PRIVATE Fidgety::FidgetyHeaders)
target_link_libraries(FidgetyEncoder PRIVATE Fidgety::_FidgetyUtils)
target_link_libraries(FidgetyEncoder PRIVATE spdlog::spdlog)
install(TARGETS FidgetyEncoder EXPORT FidgetyEncoder LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(
    EXPORT FidgetyEncoder
    DESTINATION ${export_dest_dir}
    NAMESPACE Fidgety::
    FILE fidgety_encoder_config.cmake
)

add_library(FidgetyNormalConfEncoder STATIC normal_conf_encoder.cpp)
add_library(Fidgety::FidgetyNormalConfEncoder ALIAS FidgetyNormalConfEncoder)
target_link_libraries(FidgetyNormalConfEncoder PRIVATE Fidgety::FidgetyHeaders)
target_link_libraries(FidgetyNormalConfEncoder PRIVATE Fidgety::FidgetyEncoder)
target_link_libraries(FidgetyNormalConfEncoder PRIVATE Fidgety::_FidgetyUtils)
target_link_libraries(FidgetyNormalConfEncoder PRIVATE spdlog::spdlog)
install(
    TARGETS FidgetyNormalConfEncoder
    EXPORT FidgetyNormalConfEncoder
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(
    EXPORT FidgetyNormalConfEncoder
    DESTINATION ${export_dest_dir}
    NAMESPACE Fidgety::
    FILE fidgety_normal_conf_encoder_config.cmake
)
