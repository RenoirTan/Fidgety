add_library(fidgety_decoder STATIC decoder.cpp normal_conf_decoder.cpp)
target_link_libraries(fidgety_decoder PRIVATE spdlog::spdlog)
target_link_libraries(fidgety_decoder PRIVATE nlohmann_json)
