#pragma once
#include <EntitasPP/IComponent.h>
#include <fixedpt.h>

namespace Chestnut {
namespace Ball {

class PositionComponent :
	public Chestnut::EntitasPP::IComponent
{
public:
	void Reset(fixedpt px, fixedpt py, fixedpt pz) {
		x = px;
		y = py;
		z = pz;
	}

	fixedpt x;
	fixedpt y;
	fixedpt z;
};
}
}