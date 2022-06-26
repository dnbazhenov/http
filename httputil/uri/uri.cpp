#include <httputil/uri.h>
#include <httputil/uri/parser.h>

namespace httputil
{
	namespace uri
	{
		void uri::split()
		{
			parser p;

			p.parse(_uri.data(), _uri.size());
			p.finish();

			auto r = p.results();
			_type = r.type;

			if (_type == uri_type::invalid)
				return;

			_scheme = std::string_view(_uri.data() + r.scheme.first, r.scheme.second);
			_user = std::string_view(_uri.data() + r.user.first, r.user.second);
			_host = std::string_view(_uri.data() + r.host.first, r.host.second);
			_path = std::string_view(_uri.data() + r.path.first, r.path.second);
			_query = std::string_view(_uri.data() + r.query.first, r.query.second);
			_port = r.port_num;
		}

	}  // namespace uri
}  // namespace httputil
