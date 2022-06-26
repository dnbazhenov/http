#pragma once

namespace httputil
{
	namespace uri
	{
		enum class uri_type
		{
			absolute,
			origin,
			authority,
			authority_plus,
			asterisk,
			invalid
		};

	}
}  // namespace httputil
