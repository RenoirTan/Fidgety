add_library(_FidgetyUtils STATIC string.cpp)
add_library(Fidgety::_FidgetyUtils ALIAS _FidgetyUtils)
target_link_libraries(_FidgetyUtils PRIVATE Fidgety::FidgetyHeaders)
install(TARGETS _FidgetyUtils EXPORT _FidgetyUtils LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(
    EXPORT _FidgetyUtils
    DESTINATION ${export_dest_dir}
    NAMESPACE Fidgety::
    FILE _fidgety_utils_config.cmake
)
