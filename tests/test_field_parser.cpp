#include <cstring>
#include <iostream>

#include <httputil/parser/field_parser.h>

struct test_case {
	const char *in;
	size_t parsed;
	bool error;
	size_t namesz;
	size_t vbegin;
	size_t vsz;
};

static const test_case test_cases[] = {
		// empty string
		{ "", 0, true },
		// bad token char #1
		{ "\001", 0, true },
		// bad token char #2
		{ "\100", 0, true },
		// bad whitespace #1
		{ " ", 0, true },
		// bad white space #2
		{ "ab :", 2, true },
		// bad white space #3
		{ "ab cd:", 2, true },
		// immediate colon
		{ ":", 0, true },
		// only token chars
		{ "!#$%&'*+-.^_`|~0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyx", 77, true },
		// bad empty value #1 (no CRLF)
		{ "name:", 5, true },
		// bad empty value #2 (bad LFCR)
		{ "name:\n\r", 5, true },
		// bad empty value #3 (CR no LF)
		{ "name:\r", 6, true },
		// bad empty value #4 (bad ch after CR)
		{ "name:\ra", 6, true },
		// empty value #1
		{ "name:\r\n", 7, false, 4 },
		// empty value #2
		{ "name: \r\n", 8, false, 4 },
		// empty value #3
		{ "name: \r\n ", 8, false, 4 },
		// non-empty value #1
		{ "name:a\r\n", 8, false, 4, 5, 1 },
		// non-empty value #2
		{ "name:a\r\n ", 8, false, 4, 5, 1 },
		// non-empty value #3
		{ "name:a \r\n ", 9, false, 4, 5, 1 },
		// non-empty value #4
		{ "name:a \t\r\n ", 10, false, 4, 5, 1 },
		// non-empty value #5
		{ "name:a \ta\r\n ", 11, false, 4, 5, 4 },
		// non-empty value #6
		{ "name: a \t\r\n ", 11, false, 4, 6, 1 },
		// non-empty value #6
		{ "name: \ta \t\r\n ", 12, false, 4, 7, 1 },
		// non-empty value #5
		{ "name: \ta \ta \r\n ", 14, false, 4, 7, 4 },
		// all token field name #1
		{ "!#$%&'*+-.^_`|~0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyx:\r\n", 80, false, 77 },
		// all token field name #1
		{ "!#$%&'*+-.^_`|~0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyx:!#$%&'*+-.^_`|~0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyx\r\n", 157, false, 77, 78, 77 },
};

int main(int argc, char *argv[])
{
	int n = 0;
	for (auto& tc: test_cases) {
		printf("%d: \"%s\"\n", ++n, tc.in);

		httputil::parser::field_parser p;

		auto parsed = p.parse(tc.in, std::strlen(tc.in));
		auto res = p.results();

		if (parsed != tc.parsed) {
			printf("Test case %d: invalid processed %ld/%ld\n", n, parsed, tc.parsed);
			continue;
		}

		auto error = !p.has_finished();

		if (error != tc.error) {
			printf("Test case %d: invalid error status %d/%d\n", n, error, tc.error);
			continue;
		}

		if (tc.error) {
			continue;
		}

		if (res.name.second != tc.namesz) {
			printf("Test case %d: invalid name size %d/%ld\n", n, res.name.second, tc.namesz);
		}

		if (res.value.first != tc.vbegin) {
			printf("Test case %d: invalid value offset %d/%ld\n", n, res.value.first , tc.vbegin);
		}

		if (res.value.second != tc.vsz) {
			printf("Test case %d: invalid value size %d/%ld\n", n, res.value.second , tc.vsz);
		}
	}

	return 0;
}
