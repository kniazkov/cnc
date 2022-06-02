#pragma once

#include <sstream>
#include <memory>
#include "point.h"
#include "interpolator.h"

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
		std::shared_ptr<stepper> create_stepper(point &begin, double points_per_millimeter);
	};
}
