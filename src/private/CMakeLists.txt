add_library(_FidgetyUtils STATIC string.cpp)
add_library(Fidgety::_FidgetyUtils ALIAS _FidgetyUtils)
set_target_properties(_FidgetyUtils PROPERTIES OUTPUT_NAME __fidgety_utils)
target_link_libraries(_FidgetyUtils PRIVATE Fidgety::FidgetyHeaders)
install(
    TARGETS
    _FidgetyUtils
    EXPORT _FidgetyUtils
    DESTINATION ${FIDGETY_EXPORT_LIB_DIR}
)
install(
    EXPORT _FidgetyUtils
    DESTINATION ${FIDGETY_EXPORT_CMAKE_DIR}
    NAMESPACE Fidgety::
    FILE _fidgety_utils_config.cmake
)
