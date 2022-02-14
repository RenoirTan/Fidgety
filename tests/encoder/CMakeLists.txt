add_executable(encoder_encoding encoding.cpp)
target_link_libraries(
    encoder_encoding PRIVATE
    fidgety_encoder nlohmann_json spdlog::spdlog ${GTEST} ${GTEST_MAIN}
)
add_test(NAME encoder_encoding_test COMMAND encoder_encoding)
