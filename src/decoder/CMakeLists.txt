add_library(FidgetyDecoder STATIC decoder.cpp)
add_library(Fidgety::FidgetyDecoder ALIAS FidgetyDecoder)
target_link_libraries(FidgetyDecoder PUBLIC Fidgety::FidgetyHeaders)
target_link_libraries(FidgetyDecoder PRIVATE Fidgety::_FidgetyUtils)
target_link_libraries(FidgetyDecoder PRIVATE spdlog::spdlog)
install(TARGETS FidgetyDecoder EXPORT FidgetyDecoder LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(
    EXPORT FidgetyDecoder
    DESTINATION ${export_dest_dir}
    NAMESPACE Fidgety::
    FILE fidgety_decoder_config.cmake
)

add_library(FidgetyNormalConfDecoder STATIC normal_conf_decoder.cpp)
add_library(Fidgety::FidgetyNormalConfDecoder ALIAS FidgetyNormalConfDecoder)
target_link_libraries(FidgetyNormalConfDecoder PUBLIC Fidgety::FidgetyDecoder)
target_link_libraries(FidgetyNormalConfDecoder PRIVATE Fidgety::_FidgetyUtils)
target_link_libraries(FidgetyNormalConfDecoder PRIVATE spdlog::spdlog)
target_link_libraries(FidgetyNormalConfDecoder PRIVATE nlohmann_json)
install(
    TARGETS FidgetyNormalConfDecoder
    EXPORT FidgetyNormalConfDecoder
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(
    EXPORT FidgetyNormalConfDecoder
    DESTINATION ${export_dest_dir}
    NAMESPACE Fidgety::
    FILE fidgety_normal_conf_decoder_config.cmake
)
