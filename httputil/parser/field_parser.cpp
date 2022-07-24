#include <cstdio>
#include <httputil/char_traits/field_traits.h>
#include <httputil/parser/field_parser.h>

/*
 * rfc9110 field-related ABNF
 * header-field = field-name ":" OWS field-value OWS
 * field-name = token
 * field-value = *( field-content / obs-fold )
 * field-vchar = VCHAR / obs-text
 * obs-fold = CRLF 1*( SP / HTAB )
 * obs-text = %x80-FF
 *
 * OWS = *( SP / HTAB )
 *
 * token          = 1*tchar
 * tchar          = "!" / "#" / "$" / "%" / "&" / "'" / "*"
 *                   / "+" / "-" / "." / "^" / "_" / "`" / "|" / "~"
 *                   / DIGIT / ALPHA
 *                   ; any VCHAR, except delimiters
 * ALPHA          =  %x41-5A / %x61-7A   ; A-Z / a-z
 * CR             =  %x0D
 * CRLF           =  CR LF
 * DIGIT          =  %x30-39
 * HEXDIG         =  DIGIT / "A" / "B" / "C" / "D" / "E" / "F"
 * HTAB           =  %x09
 * LF             =  %x0A
 * SP             =  %x20
 * VCHAR          =  %x21-7E
 */

namespace httputil::parser
{
	using namespace httputil::char_traits;

	template<bool _Debug>
	struct logger_t
	{
		void log_state(int) {}
		template<typename... _Args>
		void log(const char* fmt, _Args&&... args)
		{
		}
	};

	template<>
	struct logger_t<true>
	{
		void log_state(int s)
		{
			const char* s_str[] = { "S_name", "S_svalue_start", "S_svalue",
									"S_cr",   "S_done",         "S_error" };
			printf("state=%s\n", s_str[s]);
		}

		template<typename... _Args>
		void log(const char* fmt, _Args&&... args)
		{
			printf(fmt, std::forward<_Args>(args)...);
		}
	};

	static logger_t<false> logger;

	inline void field_parser::set_state(int s)
	{
		logger.log_state(s);
		_state = s;
	}

	inline size_t field_parser::set_error(size_t p)
	{
		set_state(s_error);
		return p;
	}

	inline size_t field_parser::set_done(size_t p)
	{
		set_state(s_done);
		return p;
	}

	size_t field_parser::parse(const char* data, size_t size)
	{
		size_t processed = 0;

		if (_state == s_done || _state == s_error)
			return 0;

		while (processed < size)
		{
			char ch = data[processed++];

			logger.log("  %d: '%c' (%d)\n", processed - 1, ch, ch);
			_pos++;

			switch (_state)
			{
				case s_name:
					if (is_tchar(ch))
					{
						_name_sz = _pos;
						continue;
					}

					if (ch == ':' && _name_sz)
					{
						set_state(s_value_start);
						continue;
					}

					break;

				case s_value_start:
					if (ch == ' ' || ch == '\t')
						continue;

					if (is_field_vchar(ch))
					{
						_vend = _pos;
						_vbegin = _pos - 1;
						set_state(s_value);
						continue;
					}

					if (ch == '\r')
					{
						set_state(s_cr);
						continue;
					}

					break;

				case s_value:
					if (is_field_vchar(ch))
					{
						_vend = _pos;
						continue;
					}

					if (ch == ' ' || ch == '\t')
						continue;

					if (ch == '\r')
					{
						set_state(s_cr);
						continue;
					}

					break;

				case s_cr:
					if (ch == '\n')
						return set_done(processed);

					break;
			}

			return set_error(processed - 1);
		}

		return processed;
	}

	field_parser::parse_results field_parser::results() const
	{
		return parse_results{ { 0, _name_sz }, { _vbegin, _vbegin ? _vend - _vbegin : 0 } };
	}

}  // namespace httputil::parser
