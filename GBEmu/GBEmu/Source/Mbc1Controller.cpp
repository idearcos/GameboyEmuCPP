#include "Mbc1Controller.h"
#include <sstream>

void Mbc1Controller::OnMemoryWrite(const Memory::Address &address, uint8_t value)
{
	Memory::Region region{ Memory::Region::ROM_BANK0 };
	std::tie(region, std::ignore) = address.GetRelativeAddress();
	if ((Memory::Region::ROM_BANK0 == region) || (Memory::Region::ROM_OTHER_BANKS == region))
	{
		if ((address.GetAbsoluteAddress() >= 0x0000) && (address.GetAbsoluteAddress() <= 0x1FFF))
		{
			throw std::logic_error("Enabling/disabling RAM banks is still not implemented");
		}
		else if ((address.GetAbsoluteAddress() >= 0x2000) && (address.GetAbsoluteAddress() <= 0x3FFF))
		{
			auto requested_rom_bank = static_cast<uint8_t>(value & 0x1F);
			// Banks #0 and multiples of 32 are ignored and treated as the next number
			if (requested_rom_bank == 0)
			{
				requested_rom_bank = 1;
			}

			requested_rom_bank += 32 * current_rom_bank_set;
			Notify(&IMbcControllerObserver::OnRomBankSwitchRequested, requested_rom_bank);
		}
		else if ((address.GetAbsoluteAddress() >= 0x4000) && (address.GetAbsoluteAddress() <= 0x5FFF))
		{
			switch (max_memory_mode_)
			{
			case MaxMemoryMode::Rom16Mbit_Ram8kByte:
				current_rom_bank_set = (value & 0x03);
				break;
			case MaxMemoryMode::Rom4Mbit_Ram32kByte:
				throw std::logic_error("RAM banking is still not implemented");
			default:
				std::stringstream msg;
				msg << "Memory mode " << static_cast<size_t>(max_memory_mode_) << " not implemented in MBC1 controller";
				throw std::logic_error(msg.str());
			}
		}
		else if ((address.GetAbsoluteAddress() >= 0x6000) && (address.GetAbsoluteAddress() <= 0x7FFF))
		{
			max_memory_mode_ = ((value & 0x01) != 0) ? MaxMemoryMode::Rom4Mbit_Ram32kByte : MaxMemoryMode::Rom16Mbit_Ram8kByte;
		}
	}
}
