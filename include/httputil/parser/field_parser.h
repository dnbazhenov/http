#pragma once
#include <cstdlib>
#include <utility>

namespace httputil::parser
{
	struct field_parser
	{
		enum
		{
			s_name,
			s_value_start,
			s_value,
			s_cr,
			s_done,
			s_error
		};

		struct parse_results
		{
			using extent_t = std::pair<uint, uint>;
			extent_t name;
			extent_t value;
		};

		void init() { *this = field_parser(); }
		void finish()
		{
			char ch = '\0';
			parse(&ch, 1);
		}
		bool has_error() const { return _state == s_error; }
		bool has_finished() const { return _state == s_done; }
		bool can_proceed() const { return _state < s_done; }
		parse_results results() const;
		size_t parse(const char* data, size_t size);

	private:
		void set_state(int s);
		size_t set_error(size_t p);
		size_t set_done(size_t p);

	private:
		int _state = 0;
		size_t _name_sz = 0;
		size_t _vbegin = 0;
		size_t _vend = 0;
		size_t _pos = 0;
	};

}  // namespace httputil::parser
