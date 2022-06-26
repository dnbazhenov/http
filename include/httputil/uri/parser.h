#pragma once
#include <sys/types.h>
#include <utility>

#include <httputil/uri/defs.h>

namespace httputil
{
	namespace uri
	{
		/*
		 * RFC7230: Appendix B
		 *
		 * request-target = origin-form
		 *                / absolute-form
		 *                / authority-form
		 *                / asterisk-form
		 *
		 * origin-form    = absolute-path [ "?" query ]
		 *
		 * absolute-form  = absolute-URI
		 *
		 * authority-form = authority
		 *
		 * asterisk-form  = "*"
		 *
		 * absolute-path = 1*( "/" segment )
		 *
		 */

		enum pct_t
		{
			none,
			percent,
			hex
		};

		struct parse_results
		{
			using extent_t = std::pair<uint, uint>;

			extent_t scheme;
			extent_t user;
			extent_t host;
			extent_t port;
			extent_t path;
			extent_t query;
			uri_type type;
			ushort port_num = 0;

			bool has_scheme() const { return scheme.second != 0; }
			bool has_user() const { return user.second != 0; }
			bool has_host() const { return host.second != 0; }
			bool has_port() const { return port_num != 0; }
			bool has_path() const { return path.second != 0; }
			bool has_query() const { return query.second != 0; }
		};

		struct parser
		{
			enum state_t
			{
				S_start,
				S_scheme,
				S_scheme_colon,
				S_scheme_colon_slash,
				S_asterisk,
				S_user_or_host,
				S_user_or_port,
				S_user,
				S_host,
				S_port,
				S_path,
				S_query,
				S_done,
				S_error
			};

			void init() { *this = parser(); }
			void finish()
			{
				char ch = '\0';
				parse(&ch, 1);
			}

			size_t parse(const char* data, size_t size);

			parse_results results() const;

		private:
			uri_type _type = uri_type::authority;
			state_t _state = S_start;

			size_t _scheme_sz = 0;
			size_t _user_of = 0;
			size_t _user_sz = 0;
			size_t _host_of = 0;
			size_t _host_sz = 0;
			size_t _port_of = 0;
			size_t _port_sz = 0;
			size_t _path_of = 0;
			size_t _path_sz = 0;
			size_t _query_of = 0;
			size_t _query_sz = 0;
			u_int16_t _port = 0;

			pct_t _pct = pct_t::none;

		private:
			void set_state(state_t s);
			void set_type(uri_type t);
			size_t set_error(size_t p);
			size_t set_done(size_t p);
			bool pct_check(char ch);
			bool update_port(char ch);
		};

	};  // namespace uri
};      // namespace httputil
