add_library(httputil char_traits.cpp uri/parser.cpp uri/uri.cpp)
target_include_directories(httputil PUBLIC ../include)
