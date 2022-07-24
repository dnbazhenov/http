#include <httputil/char_traits/field_traits.h>

namespace httputil::char_traits::details
{
	const std::array<field_traits_t, 256> __field_traits = mktraits<256>(details::field_traits);
}
