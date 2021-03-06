#include <algorithm>
#include <iostream>
#include "unit_tests.h"
#include "parser.h"
#include "interpolator.h"

namespace cnc {

	static bool test_parser(std::string code, std::string expected = "") {
		if (expected.empty()) {
			expected = code;
			std::transform(expected.begin(), expected.end(), expected.begin(), ::toupper);
		}
		parsing_result result = { 0 };
		instruction instr = parse_line(code, &result);
		if (result.error) {
			std::cerr << "Parsing error: " << result.message << '\n';
			return false;
		}
		std::string actual = instr.to_string();
		bool testing_result = actual == expected;
		if (!testing_result) {
			std::cerr << "Parsing failed. Expected result:\n" << expected
				<< "\nActual result:\n" << actual << '\n';
		}
		return testing_result;
	}

	static bool test_parser_simple_case() {
		return test_parser("g00 x1000 y500");
	}

	static bool test_parsing_negative_numbers() {
		return test_parser("g00 x-1000 y500");
	}

	static bool test_parsing_real_numbers() {
		return test_parser("g00 x1.001");
	}

	static bool test_parsing_negative_real_numbers() {
		return test_parser("g00 z-1.001");
	}

	static bool test_parsing_real_numbers_without_real_part() {
		return test_parser("g00 x1.", "G00 X1");
	}

	static bool test_parsing_real_numbers_without_int_part() {
		return test_parser("g00 x.1", "G00 X0.1");
	}

	static bool test_parsing_line_with_extra_spaces() {
		return test_parser(" g00   x1000", "G00 X1000");
	}

	static bool test_parsing_code() {
		std::string code = "G00 Z20\nG00 X500 Y250\nG01 Z-1\n";
		parsing_result result = { 0 };
		std::vector<instruction> parsed_code = parse_code(code, &result);
		if (result.error || parsed_code.size() != 3) {
			return false;
		}
		std::stringstream actual;
		for (instruction instr : parsed_code) {
			actual << instr.to_string() << '\n';
		}
		return code == actual.str();
	}

	static bool test_linear_stepper() {
		point begin = { 0 },
			end;
		linear_stepper stepper(begin, 2, 0, 0, 5);
		int count = 0;
		while (stepper.has_next()) {
			end = stepper.next();
			count++;
		}
		return count == 10 && end.X == 2 && end.Y == 0 && end.Z == 0 && end.percent == 1;
	}

	static bool test_creating_stepper_from_instruction() {
		point begin = { 0 },
			end;
		instruction i = { 0 };
		i.G.has_value = true;
		i.G.value = 1;
		i.X.has_value = true;
		i.X.value = 2.0;
		std::shared_ptr<stepper> stepper = i.create_stepper(begin, 5);
		int count = 0;
		while (stepper->has_next()) {
			end = stepper->next();
			count++;
		}
		return count == 10 && end.X == 2 && end.Y == 0 && end.Z == 0 && end.percent == 1;
	}


	typedef bool (*test_case)();

	test_case tests[] = {
		test_parser_simple_case,
		test_parsing_negative_numbers,
		test_parsing_real_numbers,
		test_parsing_negative_real_numbers,
		test_parsing_real_numbers_without_real_part,
		test_parsing_real_numbers_without_int_part,
		test_parsing_line_with_extra_spaces,
		test_parsing_code,
		test_linear_stepper,
		test_creating_stepper_from_instruction
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