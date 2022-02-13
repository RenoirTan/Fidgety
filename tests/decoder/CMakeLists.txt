add_executable(decoder_sanity_check sanity_check.cpp)
target_link_libraries(decoder_sanity_check fidgety_decoder ${GTEST} ${GTEST_MAIN})
add_test(NAME decoder_sanity_check_test COMMAND decoder_sanity_check)
