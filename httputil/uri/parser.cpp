#include <climits>
#include <cstdio>
#include <utility>

#include <httputil/uri/char_traits.h>
#include <httputil/uri/parser.h>

namespace httputil
{
	namespace uri
	{
		template<bool _Debug>
		struct logger_t
		{
			void log_type(uri_type t) {}
			void log_state(int) {}
			template<typename... _Args>
			void log(const char* fmt, _Args&&... args)
			{
			}
		};

		template<>
		struct logger_t<true>
		{
			void log_type(uri_type t)
			{
				const char* t_str[] = {
					"absolute", "origin", "authority", "authorityplus", "asteriks"
				};
				printf("type=%s\n", t_str[static_cast<int>(t)]);
			}

			void log_state(int s)
			{
				const char* s_str[] = {
					"S_start",    "S_scheme",       "S_scheme_colon", "S_scheme_colon_slash",
					"S_asterisk", "S_user_or_host", "S_user_or_port", "S_user",
					"S_host",     "S_port",         "S_path",         "S_query",
					"S_done",     "S_error"
				};
				printf("state=%s\n", s_str[s]);
			}

			template<typename... _Args>
			void log(const char* fmt, _Args&&... args)
			{
				printf(fmt, std::forward<_Args>(args)...);
			}
		};

		static logger_t<false> logger;

		inline void parser::set_state(state_t s)
		{
			logger.log_state(s);
			_state = s;
		}

		inline void parser::set_type(uri_type t)
		{
			logger.log_type(t);
			_type = t;
		}

		inline size_t parser::set_error(size_t p)
		{
			set_state(S_error);
			return p;
		}

		inline size_t parser::set_done(size_t p)
		{
			set_state(S_done);
			return p;
		}

		size_t parser::parse(const char* data, size_t size)
		{
			if (_state == S_done || _state == S_error)
				return 0;

			size_t processed = 0;

			while (processed < size)
			{
				auto ch = data[processed++];

				logger.log("%ld: '%c'(%d)\n", processed - 1, ch, ch);

				if (!pct_check(ch))
					return set_error(processed - 1);

				if (_state == S_start)
				{
					if (ch == '/')
					{
						_path_sz = 1;
						set_type(uri_type::origin);
						set_state(S_path);
						continue;
					}

					if (*data == '*')
					{
						_host_sz = 1;
						set_type(uri_type::asterisk);
						set_state(S_asterisk);
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

					if (is_alpha(ch))
					{
						set_state(S_scheme);
						_host_sz = 1;
						continue;
					}

					// reinterpret as user or host character
					set_type(uri_type::authority);
					set_state(S_user_or_host);
				}

				if (_state == S_scheme)
				{
					if (is_schar(ch))
					{
						++_host_sz;
						continue;
					}

					if (ch == ':')
					{
						set_state(S_scheme_colon);
						continue;
					}

					// reinterpret as port character
					set_state(S_user_or_host);
				}

				if (_state == S_scheme_colon)
				{
					if (ch == '/')
					{
						set_state(S_scheme_colon_slash);
						continue;
					}

					// reinterpret as port character
					_port_sz = 1;
					set_state(S_user_or_port);
				}

				if (_state == S_scheme_colon_slash)
				{
					if (ch == '/')
					{
						set_state(S_user_or_host);
						set_type(uri_type::absolute);
						std::swap(_host_sz, _scheme_sz);
						continue;
					}

					_port_sz = 1;  // colon would be a part of port string
					_path_sz = 1;  // slash would be a part of path string

					// this is an authority + path [query] form
					set_type(uri_type::authority_plus);

					// reinterpret as path character
					set_state(S_path);
				}

				if (_state == S_asterisk && ch != ' ' && ch != '\0')
				{
					set_type(uri_type::authority);

					// reinterpret as user or host character
					set_state(S_user_or_host);
				}

				if (_state == S_user_or_host)
				{
					if (ch == ':')
					{
						_port_sz = 1;
						set_state(S_user_or_port);
						continue;
					}

					if (is_uchar(ch))
					{
						++_host_sz;
						continue;
					}

					if (ch == '/' && _host_sz)
					{
						_path_sz = 1;
						if (_type == uri_type::authority)
							set_type(uri_type::authority_plus);
						set_state(S_path);
						continue;
					}

					if (ch == '@' && _host_sz)
					{
						std::swap(_user_sz, _host_sz);
						set_state(S_host);
						continue;
					}
				}

				if (_state == S_user_or_port)
				{
					if (update_port(ch))
					{
						++_port_sz;
						continue;
					}

					if (ch == '/')
					{
						_path_sz = 1;
						if (_type == uri_type::authority)
							set_type(uri_type::authority_plus);
						set_state(S_path);
						continue;
					}

					if (ch != ' ' && ch != '\0')
					{
						// reinterpret as user info
						std::swap(_user_sz, _host_sz);
						_user_sz += _port_sz;
						_port = _port_sz = 0;
						set_state(S_user);
					}
				}

				if (_state == S_user)
				{
					if (is_uchar(ch))
					{
						++_user_sz;
						continue;
					}

					if (ch == '@' && _user_sz)
					{
						set_state(S_host);
						continue;
					}

					return set_error(processed - 1);
				}

				if (_state == S_host)
				{
					if (is_hchar(ch))
					{
						++_host_sz;
						continue;
					}

					if (ch == ':')
					{
						_port_sz = 1;
						set_state(S_port);
						continue;
					}

					if (ch == '/' && _host_sz)
					{
						_path_sz = 1;
						if (_type == uri_type::authority)
							set_type(uri_type::authority_plus);
						set_state(S_path);
						continue;
					}
				}

				if (_state == S_port)
				{
					if (update_port(ch))
					{
						++_port_sz;
						continue;
					}

					if (ch == '/')
					{
						_path_sz = 1;
						if (_type == uri_type::authority)
							set_type(uri_type::authority_plus);
						set_state(S_path);
						continue;
					}
				}

				if (_state == S_path)
				{
					if (is_pchar(ch) || ch == '/')
					{
						_path_sz++;
						continue;
					}

					if (ch == '?')
					{
						set_state(S_query);
						continue;
					}
				}

				if (_state == S_query)
				{
					if (is_qchar(ch))
					{
						_query_sz++;
						continue;
					}
				}

				if ((ch == ' ' || ch == '\0') && _pct == pct_t::none && (_host_sz || _path_sz))
					return set_done(processed);

				return set_error(processed - 1);
			}

			return processed;
		}

		bool parser::update_port(char ch)
		{
			if (!is_digit(ch))
				return false;

			auto port = _port * 10 + ch - '0';
			logger.log("port=%d\n", port);
			if (port > USHRT_MAX)
				return false;

			_port = port;
			return true;
		}

		bool parser::pct_check(char ch)
		{
			if (_pct == pct_t::none && ch == '%')
			{
				_pct = pct_t::percent;
				logger.log("_pct=%d\n", _pct);
				return true;
			}

			if (_pct == pct_t::none)
				return true;

			if (!is_hexdig(ch))
				return false;

			_pct = _pct == pct_t::percent ? pct_t::hex : pct_t::none;

			logger.log("_pct=%d\n", _pct);

			return true;
		}

		parse_results parser::results() const
		{
			auto user_of = _scheme_sz ? _scheme_sz + 3 : 0;
			auto host_of = user_of + (_user_sz ? _user_sz + 1 : 0);
			auto port_of = host_of + _host_sz;
			auto path_of = port_of + _port_sz;
			auto query_of = path_of + _path_sz + 1;
			auto type = _state == S_error ? uri_type::invalid : _type;

			return parse_results{ { 0, _scheme_sz },
								  { user_of, _user_sz },
								  { host_of, _host_sz },
								  { port_of, _port_sz },
								  { path_of, _path_sz },
								  { query_of, _query_sz },
								  type,
								  _port };
		}

	};  // namespace uri
};      // namespace httputil
