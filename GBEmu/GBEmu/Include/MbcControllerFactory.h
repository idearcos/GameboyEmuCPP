#pragma once

#include <memory>
#include "RomInfo.h"
#include "Mbc1Controller.h"

namespace MbcControllerFactory
{
	std::unique_ptr<IMbcController> Create(CartridgeType cartridge_type);
}
