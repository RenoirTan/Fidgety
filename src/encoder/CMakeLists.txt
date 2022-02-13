add_library(fidgety_encoder STATIC encoder.cpp normal_conf_encoder.cpp)
target_link_libraries(fidgety_encoder PRIVATE _fidgety_utils)
target_link_libraries(fidgety_encoder PRIVATE spdlog::spdlog)
target_link_libraries(fidgety_encoder PRIVATE nlohmann_json)
install(
    TARGETS fidgety_encoder
    LIBRARY DESTINATION lib
)
