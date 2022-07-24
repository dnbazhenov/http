#include(ExternalData)
include_directories(../include)
link_libraries(httputil)
add_executable(test_uri_parser test_uri_parser.cpp)
add_test(NAME test_uri_parser COMMAND test_uri_parser)

add_executable(test_field_parser test_field_parser.cpp)
add_test(NAME test_field_parser COMMAND test_field_parser)

add_executable(test_char_traits test_char_traits.cpp)
add_test(NAME test_char_traits COMMAND test_char_traits)

file(COPY test_uri_parser.csv DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#install(DIRECTORY data DESTINATION tests/data)

#ExternalData_Add_Target(
#	uri_test_data DATA{tests/test_uri_parser.csv}
#	)