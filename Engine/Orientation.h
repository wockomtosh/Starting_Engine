#pragma once

namespace Orientation {
	inline float clampOrientation(float i_orientation)
	{
		//I'm using degrees
		if (i_orientation < 0)
		{
			i_orientation += 360;
		}
		else if (i_orientation > 360)
		{
			i_orientation -= 360;
		}

		return i_orientation;
	}

	inline float degToRad(float i_orientationDeg)
	{
		return i_orientationDeg * (3.1415 / 180);
	}

	inline float radToDeg(float i_orientationRad)
	{
		return i_orientationRad * (180 / 3.1415);
	}
}