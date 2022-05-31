#pragma once

#include <sstream>

namespace cnc {

	struct int_value {
		bool has_value;
		int value;
	};

	struct real_value {
		bool has_value;
		float value;
	};

	struct instruction {
		int_value G;
		real_value X, Y, Z;

		std::string to_string();
	};
}
