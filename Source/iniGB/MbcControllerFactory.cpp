#include "MbcControllerFactory.h"
#include <sstream>

namespace MbcControllerFactory
{
	std::unique_ptr<IMbcController> Create(CartridgeType cartridge_type)
	{
		switch (cartridge_type)
		{
		case CartridgeType::ROM_Only:
			// No need for an MBC controller in this case
			return nullptr;
		case CartridgeType::MBC1:
			return std::make_unique<Mbc1Controller>();
		default:
			std::stringstream msg;
			msg << "MbcControllerFactory does not support cartridge type " << static_cast<size_t>(cartridge_type);
			throw std::logic_error(msg.str());
		}
	}
}
