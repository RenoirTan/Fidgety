add_library(fidgety_decoder STATIC decoder.cpp normal_conf_decoder.cpp)
target_link_libraries(fidgety_decoder PRIVATE Fidgety::FidgetyHeaders)
target_link_libraries(fidgety_decoder PRIVATE _fidgety_utils)
target_link_libraries(fidgety_decoder PRIVATE spdlog::spdlog)
target_link_libraries(fidgety_decoder PRIVATE nlohmann_json)
install(
    TARGETS fidgety_decoder
    LIBRARY DESTINATION lib
)
