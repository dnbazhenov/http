#include <httputil/uri/char_traits.h>

namespace httputil::uri::impl
{
	const std::array<char_flags_t, 256> __char_traits = mkchartraits();
}

#if 0
constexpr auto b1 = http::is_alpha('A');
constexpr auto b2 = http::is_digit('A');
constexpr auto b3 = http::is_hexdig('A');
constexpr auto b4 = http::is_pct('A');
constexpr auto b5 = http::is_gen_delim('A');
constexpr auto b6 = http::is_sub_delim('A');
constexpr auto b7 = http::is_unreserved('A');
constexpr auto b8 = http::is_schar('A');
constexpr auto b9 = http::is_uchar('A');
constexpr auto b10 = http::is_pchar('A');
constexpr auto b11 = http::is_qchar('A');
constexpr auto b12 = http::is_reserved('A');
#endif
