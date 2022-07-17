#pragma once

#include <httputil/uri.h>
#include <httputil/uri/parser.h>

namespace httputil
{
	namespace uri
	{
		struct builder
		{
			static http::uri::uri build(const char* uri, const httputil::uri::parse_results& pr)
			{
				std::string _uri{ uri };
				return http::uri::uri()
			}
		};
	}  // namespace uri
}  // namespace httputil
