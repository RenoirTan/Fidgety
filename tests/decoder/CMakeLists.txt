if(FIDGETY_BUILD_EXTENSIONS)
    fidgety_create_test(decoder_sanity_check sanity_check.cpp)
    target_link_libraries(decoder_sanity_check PRIVATE Fidgety::FidgetyNormalConfDecoder)

    fidgety_create_test(decoder_decoding decoding.cpp)
    target_link_libraries(decoder_decoding PRIVATE Fidgety::FidgetyNormalConfDecoder)
endif()
