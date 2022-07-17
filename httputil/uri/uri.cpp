#include <httputil/parser/uri_parser.h>
#include <httputil/uri.h>

namespace httputil
{
	namespace uri
	{
		void uri::copy(const uri& u)
		{
			// calculate difference between pointers
			std::ptrdiff_t diff = _uri.data() - u._uri.data();

			auto copy_sv = [=](const std::string_view& sv) {
				return std::string_view(sv.data() + diff, sv.size());
			};

			_scheme = copy_sv(u._scheme);
			_user = copy_sv(u._user);
			_host = copy_sv(u._host);
			_path = copy_sv(u._path);
			_query = copy_sv(u._query);
			_type = u._type;
			_port = u._port;
		}

		void uri::split()
		{
			parser::uri_parser p;

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
