#include <httputil/char_traits/core_traits.h>

namespace httputil::char_traits::details
{
	const std::array<core_traits_t, 128> __core_traits = mktraits<128>(details::core_traits);
}
