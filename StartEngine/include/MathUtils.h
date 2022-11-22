#pragma once

template <typename T>
T clip(T val, T lower, T upper)
{
	if (val > upper) return upper;
	if (val < lower) return lower;

	return val;
}

template <typename T>
T wrapAround(T val, T wrval)
{
	if (val > wrval) return val - 2 * wrval;
	if (val < -wrval) return val + 2 * wrval;

	return val;
}

