#include <algorithm>
#include <iostream>
#include "unit_tests.h"
#include "parser.h"

namespace cnc {

	bool test_parser(std::string sample) {
		std::transform(sample.begin(), sample.end(), sample.begin(), ::toupper);
		parsing_result result = { 0 };
		parse_line(sample, &result);
		if (result.error) {
			std::cerr << "Parsing error: " << result.message << '\n';
			return false;
		}
		bool testing_result = result.instr.to_string() == sample;
		if (!testing_result) {
			std::cerr << "Parsing failed. Expected result:\n" << sample
				<< "\nActual result:\n" << result.instr.to_string() << '\n';
		}
		return testing_result;
	}

	bool test_parser_simple_case() {
		return test_parser("g00 x1000 y500");
	}

	typedef bool (*test_case)();

	test_case tests[] = {
		test_parser_simple_case
	};

	void test_all() {
		const size_t size = sizeof(tests) / sizeof(test_case);
		size_t passed = 0,
			failed = 0;
		for (size_t index = 0; index < size; index++) {
			test_case tc = tests[index];
			if (tc()) {
				passed++;
			}
			else {
				failed++;
			}
		}
		std::cout << "Total tests: " << size << ", passed: " << passed << ", failed: " << failed << ".\n";
	}
}