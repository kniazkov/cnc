#pragma once

#include "point.h"

namespace cnc {
	class stepper {
	public:
		virtual ~stepper() {};
		virtual bool has_next() = 0;
		virtual point next() = 0;
	};

	class linear_stepper : public stepper {
		double tX,
			tY,
			tZ,
			iX,
			iY,
			iZ;
		int count, total;
		point p;
	public:
		linear_stepper(point& begin, double X, double Y, double Z, double points_per_millimeters);
		bool has_next() override;
		point next() override;
	};
}