#include <cmath>
#include "interpolator.h"

namespace cnc {
	linear_stepper::linear_stepper(point& begin, double X, double Y, double Z, double points_per_millimeters) {
		p = begin;
		tX = X;
		tY = Y;
		tZ = Z;

		double dX = X - begin.X;
		double dY = Y - begin.Y;
		double dZ = Z - begin.Z;
		
		double distance = std::sqrt(dX * dX + dY * dY + dZ * dZ);
		count = (int)(distance * points_per_millimeters);
		if (count == 0) {
			count = 1;
		}
		total = count;
		
		iX = dX / count;
		iY = dY / count;
		iZ = dZ / count;
	}

	bool linear_stepper::has_next() {
		return count > 0;
	}

	point linear_stepper::next() {
		if (count == 1) {
			p.percent = 1;
			p.X = tX;
			p.Y = tY;
			p.Z = tZ;
			count = 0;
		}
		else if (count > 1) {
			p.percent = (double)(total - count) / total;
			p.X += iX;
			p.Y += iY;
			p.Z += iZ;
			count--;
		}
		return p;
	}
}