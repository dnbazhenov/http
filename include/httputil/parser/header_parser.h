#pragma once

namespace httputil::parser
{
	struct header_parser
	{
		int _state;
		size_t _name_sz;
		size_t _value_begin;
		size_t _value_end;
	};

}  // namespace httputil::parser
