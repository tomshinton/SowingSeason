#pragma once

enum class EPolicyFailReason : uint8
{
	TooSteep,
	TooHigh,
	TooLow,
	Overlapping
};