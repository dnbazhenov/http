#pragma once
#include <string_view>
#include <sys/types.h>
#include <httputil/uri.h>

namespace httputil {
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

	enum class type_t
	{
		absolute,
		origin,
		authority,
		authority_plus,
		asteriks
	};

	enum pct_t
	{
		none,
		percent,
		hex
	};

	struct parser
	{
		void init() { *this = parser(); }
		//void finish() { set_done(0); }

		size_t parse(const char* data, size_t size);
		size_t parse2(const char* data, size_t size);

		type_t type() const { return _type; }

		bool done() const { return _state == S_done; }
		bool has_error() const { return _state == S_error; }

		bool has_scheme() const { return _scheme_sz != 0; }
		size_t scheme_off() const { return 0; }
		size_t scheme_size() const { return _scheme_sz; }

		bool has_user() const { return _user_sz != 0; }
		size_t user_off() const { return _user_of; }
		size_t user_size() const { return _user_sz; }

		bool has_host() const { return _host_sz != 0; }
		size_t host_off() const { return _host_of; }
		size_t host_size() const { return _host_sz; }

		bool has_port() const { return _port_sz != 0; }
		size_t port_off() const { return _port_of; }
		size_t port_size() const { return _port_sz; }
		u_int16_t port() const { return _port; }

		bool has_path() const { return _path_sz != 0; }
		size_t path_off() const { return _path_of; }
		size_t path_size() const { return _path_sz; }

		bool has_query() const { return _query_sz != 0; }
		size_t query_off() const { return _query_of; }
		size_t query_size() const { return _query_sz; }

	private:
		enum State {
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

		using uri_t = uri::type_t;

		uri_t _type = uri_t::authority;
		int _state = 0;

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
		void set_state(int s);
		void set_type(uri_t t);
		size_t set_error(size_t p);
		size_t set_done(size_t p);

		bool pct_check(char ch);
		bool update_port(char ch);

		size_t parse_start(const char* data, size_t size);
		size_t parse_scheme(const char* data, size_t size);
		size_t parse_authority(const char* data, size_t size);
		size_t parse_path(const char* data, size_t size);
		size_t parse_query(const char* data, size_t size);
	};

};  // namespace uri
};  // namespace httputil
