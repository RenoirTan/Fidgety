if(FIDGETY_BUILD_EXTENSIONS)
    fidgety_create_test(encoder_encoding encoding.cpp)
    target_link_libraries(encoder_encoding PRIVATE Fidgety::FidgetyNormalConfEncoder)
endif()
