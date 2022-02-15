add_library(FidgetyEncoder STATIC encoder.cpp normal_conf_encoder.cpp)
add_library(Fidgety::FidgetyEncoder ALIAS FidgetyEncoder)
target_link_libraries(FidgetyEncoder PRIVATE Fidgety::FidgetyHeaders)
target_link_libraries(FidgetyEncoder PRIVATE Fidgety::_FidgetyUtils)
target_link_libraries(FidgetyEncoder PRIVATE spdlog::spdlog)
target_link_libraries(FidgetyEncoder PRIVATE nlohmann_json)
install(TARGETS FidgetyEncoder EXPORT FidgetyEncoder LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(
    EXPORT FidgetyEncoder
    DESTINATION ${export_dest_dir}
    NAMESPACE Fidgety::
    FILE fidgety_encoder_config.cmake
)
