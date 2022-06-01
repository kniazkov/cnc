#pragma once

#include <sstream>

namespace cnc {

	struct int_value {
		bool has_value;
		long value;
	};

	struct real_value {
		bool has_value;
		double value;
	};

	struct instruction {
		int_value G;
		real_value X, Y, Z;

		bool is_not_empty();
		std::string to_string();
	};
}
