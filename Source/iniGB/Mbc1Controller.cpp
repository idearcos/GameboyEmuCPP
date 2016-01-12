#include "Mbc1Controller.h"
#include <sstream>
#include <iostream>

void Mbc1Controller::OnMemoryWrite(const Memory::Address &address, uint8_t value)
{
	Memory::Region region{ Memory::Region::ROM_BANK0 };
	std::tie(region, std::ignore) = address.GetRelativeAddress();
	if ((Memory::Region::ROM_BANK0 == region) || (Memory::Region::ROM_OTHER_BANKS == region))
	{
		if ((address.GetAbsoluteAddress() >= 0x0000) && (address.GetAbsoluteAddress() <= 0x1FFF))
		{
			Notify(&IMbcControllerObserver::OnExternalRamEnabled, (value & 0x0F) == 0x0A);
		}
		else if ((address.GetAbsoluteAddress() >= 0x2000) && (address.GetAbsoluteAddress() <= 0x3FFF))
		{
			rom_bank_lower_5_bits_ = value & 0x1F;
			// Banks #0 and multiples of 32 are ignored and treated as the next number
			if (rom_bank_lower_5_bits_ == 0)
			{
				rom_bank_lower_5_bits_ = 1;
			}

			const auto requested_rom_bank = (current_rom_bank_set_ << 5) + rom_bank_lower_5_bits_;
			Notify(&IMbcControllerObserver::OnRomBankSwitchRequested, requested_rom_bank);
		}
		else if ((address.GetAbsoluteAddress() >= 0x4000) && (address.GetAbsoluteAddress() <= 0x5FFF))
		{
			switch (banking_mode_)
			{
			case BankingMode::Rom:
				current_rom_bank_set_ = (value & 0x03);
				const auto requested_rom_bank = (current_rom_bank_set_ << 5) + rom_bank_lower_5_bits_;
				Notify(&IMbcControllerObserver::OnRomBankSwitchRequested, requested_rom_bank);
				break;
			case BankingMode::Ram:
				Notify(&IMbcControllerObserver::OnRamBankSwitchRequested, value & 0x03);
				break;
			default:
				std::stringstream msg;
				msg << "Memory mode " << static_cast<size_t>(banking_mode_) << " not implemented in MBC1 controller";
				throw std::logic_error(msg.str());
			}
		}
		else if ((address.GetAbsoluteAddress() >= 0x6000) && (address.GetAbsoluteAddress() <= 0x7FFF))
		{
			banking_mode_ = ((value & 0x01) != 0) ? BankingMode::Ram : BankingMode::Rom;
		}
	}
}
