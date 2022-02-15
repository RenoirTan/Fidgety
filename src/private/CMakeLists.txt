add_library(_fidgety_utils STATIC string.cpp)
target_link_libraries(_fidgety_utils PRIVATE Fidgety::FidgetyHeaders)
