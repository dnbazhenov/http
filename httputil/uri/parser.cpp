#include <stddef.h>
#include <utility>

#include <httputil/char_traits.h>
#include <httputil/uri/parser.h>

namespace httputil
{
	namespace uri
	{
		size_t parser::parse2(const char* data, size_t size)
		{
			if (_state == S_done || _state == S_error)
				return 0;

			size_t processed = 0;
			size_t retry = 0;
			char ch = 0;

			while (1)
			{
				if (retry == 0)
				{
					if (processed == size)
						break;

					ch = data[processed++];

					printf("%ld: '%c'(%d)\n", processed, ch, ch);

					if (!pct_check(ch))
						return set_error(processed);
				}

				retry = 0;

				if (ch == ' ' || ch == '\0')
				{
					return _pct ? set_error(processed) : set_done(processed);
				}

				switch (_state)
				{
					case S_start:
						// check for origin-form
						if (ch == '/')
						{
							set_state(S_path);
							set_type(type_t::origin);
							_path_sz = 1;
							continue;
						}

						// check for asteriks-form
						if (*data == '*')
						{
							set_state(S_done);
							set_type(uri_t::asteriks);
							_host_sz = 1;
							continue;
						}

						//
						// authority-form and absolute-form may not be distinguishable at start.
						// the absolute-form started with the scheme part is only recognized
						// if the URI starts with ALPHA,
						// contains only characters belonging to the scheme set, and ends
						// with "://".
						// if either of the conditions is not met, we treat the URI as a
						// authority form.
						//

						if (http::is_alpha(ch))
						{
							set_state(S_scheme);
							_host_sz = 1;
							continue;
						}

						set_state(S_user_or_host);
						set_type(uri_t::authority);
						retry = 1;
						continue;

					case S_scheme:
						if (http::is_schar(ch))
						{
							++_host_sz;
							continue;
						}

						if (ch == ':')
						{
							set_state(S_scheme_colon);
							continue;
						}

						set_state(S_user_or_host);
						retry = 1;
						continue;

					case S_scheme_colon:
						if (ch == '/')
						{
							set_state(S_scheme_colon_slash);
							set_type(uri_t::absolute);
							continue;
						}

						set_state(S_user_or_port);
						retry = 1;
						continue;

					case S_scheme_colon_slash:
						if (ch == '/')
						{
							set_state(S_user_or_host);
							std::swap(_host_sz, _scheme_sz);
							continue;
						}

						return set_error(processed);

					case S_user_or_host:
						if (http::is_uchar(ch))
						{
							++_host_sz;
							continue;
						}

						// next is either a port number or second user info word
						if (ch == ':')
						{
							set_state(S_user_or_port);
							continue;
						}

						// reinterpret as host
						if (ch == '/')
						{
							set_state(S_host);
							retry = 1;
							continue;
						}

						// reinterpret as user info
						set_state(S_user);
						std::swap(_user_sz, _host_sz);
						retry = 1;
						continue;

					case S_user_or_port:
						if (update_port(ch))
						{
							++_port_sz;
							continue;
						}

						// reinterpret as host
						if (ch == '/')
						{
							set_state(S_host);
							retry = 1;
							continue;
						}

						// reinterpret as user info
						set_state(S_host);
						std::swap(_user_sz, _host_sz);
						_user_sz += _port_sz + 1;
						_port = _port_sz = 0;
						retry = 1;
						continue;

					case S_user:
						if (http::is_uchar(ch))
						{
							++_user_sz;
							continue;
						}

						if (ch == '@')
						{
							set_state(S_host);
							continue;
						}

						return set_error(processed);

					case S_host:
						if (http::is_hchar(ch))
						{
							++_host_sz;
							continue;
						}

						if (ch == ':')
						{
							// next is port number
							set_state(S_port);
							continue;
						}

						if (ch == '/' && _type == type_t::absolute)
						{
							set_state(S_path);
							_path_sz = 1;
							continue;
						}

						return set_error(processed);

					case S_port:
						if (update_port(ch))
						{
							++_port_sz;
							continue;
						}

						if (ch == '/' && _type == type_t::absolute)
						{
							set_state(S_path);
							_path_sz = 1;
							continue;
						}

						return set_error(processed);

					case S_path:
						if (http::is_pchar(ch))
						{
							_path_sz++;
							continue;
						}

						if (ch == '&')
						{
							set_state(S_query);
							continue;
						}

						break;

					case S_query:
						if (http::is_qchar(ch))
						{
							_query_sz++;
							continue;
						}

						break;
				}

				return set_error(processed);
			}

			return processed;
		}

		size_t parser::set_done(size_t p)
		{
			_user_of = _scheme_sz ? _scheme_sz + 3 : 0;
			_host_of = _user_of + _user_sz ? _user_sz + 1 : 0;
			_port_of = _host_of + _host_sz + _port_sz ? 1 : 0;
			_path_of = _port_of + _port_sz;
			_query_of = _path_of + _path_sz + _query_sz ? 1 : 0;
			set_state(S_done);
			return p;
		}

		size_t parser::set_error(size_t p)
		{
			set_state(S_error);
			return p;
		}

		void parser::set_state(int s)
		{
			const char* st[] = {
				"S_start",        "S_scheme",       "S_scheme_colon", "S_scheme_colon_slash",
				"S_user_or_host", "S_user_or_port", "S_user",         "S_host",
				"S_port",         "S_path",         "S_query",        "S_done",
				"S_error"
			};
			_state = s;
			printf("state=%s\n", st[s]);
		}

		void parser::set_type(uri_t t)
		{
			const char* tp[] = { "invalid", "absolute", "origin", "authority", "asteriks" };
			_type = t;
			printf("type=%s\n", tp[static_cast<int>(t)]);
		}

		bool parser::update_port(char ch)
		{
			if (!http::is_digit(ch))
				return false;

			auto port = _port * 10 + ch - '0';
			if (port < _port)
				return false;

			_port = port;
			return true;
		}

		bool parser::pct_check(char ch)
		{
			if (_pct == pct_t::none && ch == '%')
			{
				_pct = pct_t::percent;
				printf("_pct=%d\n", _pct);
				return true;
			}

			if (_pct == pct_t::none)
				return true;

			if (!http::is_hexdig(ch))
				return false;

			if (_pct == pct_t::percent)
				_pct = pct_t::hex;
			else
				_pct = pct_t::none;

			printf("_pct=%d\n", _pct);

			return true;
		}
	};  // namespace uri
};      // namespace httputil
