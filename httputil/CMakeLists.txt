add_library(httputil
	char_traits/core_traits.cpp
	char_traits/uri_traits.cpp
	parser/uri_parser.cpp
	parser/field_parser.cpp
	uri/uri.cpp)
target_include_directories(httputil PUBLIC ../include)
