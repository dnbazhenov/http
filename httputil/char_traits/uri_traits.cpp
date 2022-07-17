#include <httputil/char_traits/uri_traits.h>

namespace httputil::char_traits::details
{
	const std::array<uri_traits_t, 128> __uri_traits = mktraits(details::uri_traits);
}
