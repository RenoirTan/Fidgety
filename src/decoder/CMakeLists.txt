add_library(FidgetyDecoder STATIC decoder.cpp normal_conf_decoder.cpp)
add_library(Fidgety::FidgetyDecoder ALIAS FidgetyDecoder)
target_link_libraries(FidgetyDecoder PRIVATE Fidgety::FidgetyHeaders)
target_link_libraries(FidgetyDecoder PRIVATE Fidgety::_FidgetyUtils)
target_link_libraries(FidgetyDecoder PRIVATE spdlog::spdlog)
target_link_libraries(FidgetyDecoder PRIVATE nlohmann_json)
install(TARGETS FidgetyDecoder EXPORT FidgetyDecoder LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(
    EXPORT FidgetyDecoder
    DESTINATION ${export_dest_dir}
    NAMESPACE Fidgety::
    FILE fidgety_decoder_config.cmake
)
