#pragma once
#include <string>
#include <string_view>

namespace http {
namespace uri {

enum class type_t
{
	invalid,
	absolute,
	origin,
	authority,
	asteriks
};

struct uri {
	uri() = default;

	const std::string_view& scheme() const { return _scheme; }
	const std::string_view& user() const { return _user; }
	const std::string_view& host() const { return _host; }
	const std::string_view& path() const { return _path; }

private:
	std::string _uri;
	std::string_view _scheme;
	std::string_view _user;
	std::string_view _host;
	std::string_view _path;
	std::string_view _query;
};

}; // namespace uri
}; // namespace http
