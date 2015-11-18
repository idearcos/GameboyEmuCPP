#pragma once

#include "IMMUObserver.h"
#include "IMbcControllerObserver.h"
#include "Subject.h"

class IMbcController : public IMMUObserver, public Subject<IMbcControllerObserver>
{
public:
	virtual ~IMbcController() {}
};
