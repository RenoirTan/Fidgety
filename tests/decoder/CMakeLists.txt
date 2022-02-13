add_executable(decoder_sanity_check sanity_check.cpp)
target_link_libraries(decoder_sanity_check PRIVATE fidgety_decoder ${GTEST} ${GTEST_MAIN})
add_test(NAME decoder_sanity_check_test COMMAND decoder_sanity_check)

add_executable(decoder_decoding decoding.cpp)
target_link_libraries(
    decoder_decoding PRIVATE
    fidgety_decoder nlohmann_json spdlog::spdlog ${GTEST} ${GTEST_MAIN}
)
add_test(NAME decoder_decoding_test COMMAND decoder_decoding)
