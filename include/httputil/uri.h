#pragma once
#include <string>
#include <string_view>

#include <httputil/uri/defs.h>

namespace httputil
{
	namespace uri
	{
		struct uri
		{
			uri() = default;
			uri(uri&& u)
				: _uri(std::move(u._uri))
			{
				copy(u);
			}
			uri(const uri& u)
				: _uri(u._uri)
			{
				copy(u);
			}

			uri& operator=(uri&& u)
			{
				_uri = std::move(u._uri);
				copy(u);
				return *this;
			}
			uri& operator=(const uri& u)
			{
				_uri = u._uri;
				copy(u);
				return *this;
			}

			const std::string_view& scheme() const { return _scheme; }

			const std::string_view& user() const { return _user; }

			const std::string_view& host() const { return _host; }

			const std::string_view& path() const { return _path; }

			const std::string_view& query() const { return _query; }

			ushort port() const { return _port; }

			uri_type type() const { return _type; }

			bool has_scheme() const { return _scheme.size() != 0; }

			bool has_user() const { return _user.size() != 0; }

			bool has_host() const { return _host.size() != 0; }

			bool has_port() const { return _port != 0; }

			bool has_path() const { return _path.size() != 0; }

			bool has_query() const { return _query.size() != 0; }

			bool is_bad() const { return _type == uri_type::invalid; }

		private:
			template<typename _Tp>
			explicit uri(_Tp&& s)
				: _uri(std::forward<_Tp>(s))
			{
				split();
			}

			template<typename _Tp>
			friend uri parse(_Tp&& s);

			friend class parser;
			void copy(const uri& u);
			void split();

		private:
			std::string _uri;
			std::string_view _scheme;
			std::string_view _user;
			std::string_view _host;
			std::string_view _path;
			std::string_view _query;
			uri_type _type;
			ushort _port;
		};

		template<typename _Tp>
		static inline uri parse(_Tp&& s)
		{
			return uri(std::forward<_Tp>(s));
		}

	};  // namespace uri
};      // namespace httputil
