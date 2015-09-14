#include "Z80.h"

Z80::Z80()
{
	//operations_[] = 
}

// DAA
// (Z80 p.171)
Clock Z80::DAA()
{
	bool bAdjustLowNibble{ false }, bAdjustHighNibble{ false };
	const auto high_nibble = (registers.Read(Register8bit::A) & 0xF0) >> 4;
	const auto low_nibble = registers.Read(Register8bit::A) & 0x0F;
	// N == 0
	if (!registers.IsFlagSet(Flags::Subtract))
	{
		// N == 0, C == 0
		if (!registers.IsFlagSet(Flags::Carry))
		{
			// Check if +0x06 is needed
			if ((registers.IsFlagSet(Flags::HalfCarry) && (low_nibble >= 0) && (low_nibble <= 3))
				|| !registers.IsFlagSet(Flags::HalfCarry) && (low_nibble >= 0xA) && (low_nibble <= 0xF))
			{
				bAdjustLowNibble = true;
			}

			// Check if +0x60 is needed
			if ((high_nibble >= 0xA) && (high_nibble <= 0xF))
			{
				if (!registers.IsFlagSet(Flags::HalfCarry)
					|| registers.IsFlagSet(Flags::HalfCarry) && (low_nibble >= 0) && (low_nibble <= 3))
				{
					bAdjustHighNibble = true;
				}
			}
			else if (high_nibble == 9)
			{
				if (!registers.IsFlagSet(Flags::HalfCarry) && (low_nibble >= 0xA) && (low_nibble <= 0xF))
				{
					bAdjustHighNibble = true;
				}
			}
		}
		//  N == 0, C == 1
		else
		{
			// Check if +0x60 is needed
			if (!registers.IsFlagSet(Flags::HalfCarry))
			{
				if ((high_nibble >= 0) && (high_nibble <= 2))
				{
					bAdjustHighNibble = true;
				}
			}
			else
			{
				if ((high_nibble >= 0) && (high_nibble <= 3) && (low_nibble >= 0) && (low_nibble <= 3))
				{
					bAdjustHighNibble = true;
				}
			}

			// Check if +0x06 is needed
			if (!registers.IsFlagSet(Flags::HalfCarry))
			{
				if ((high_nibble >= 0) && (high_nibble <= 2) && (low_nibble >= 0xA) && (low_nibble <= 0xF))
				{
					bAdjustLowNibble = true;
				}
			}
			else
			{
				if ((high_nibble >= 0) && (high_nibble <= 3) && (low_nibble >= 0) && (low_nibble <= 3))
				{
					bAdjustLowNibble = true;
				}
			}
		}
	}
	// N == 1
	else
	{
		// N == 1, C == 0
		if (!registers.IsFlagSet(Flags::Carry))
		{
			if (registers.IsFlagSet(Flags::HalfCarry))
			{

			}
			else
			{

			}
		}
		// N == 1, C == 1
		else
		{
			if (registers.IsFlagSet(Flags::HalfCarry))
			{

			}
			else
			{

			}
		}
	}

	return Clock(1, 4);
}
