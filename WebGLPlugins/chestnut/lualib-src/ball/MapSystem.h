#pragma once
#include "hexmap.h"

#include <EntitasPP/ISystem.h>
#include <EntitasPP/Pool.h>
#include <fixedpt.h>
#include <Singleton\Singleton.h>

namespace Chestnut {
namespace Ball {

class MapSystem :
	public  EntitasPP::ISystem, public EntitasPP::IInitializeSystem {

public:
	MapSystem() = default;
	virtual ~MapSystem() {}

	int SystemType();

	void Initialize();

	void FindPath(int index, struct vector3 start, struct vector3 dst);

protected:

private:
	Chestnut::EntitasPP::Pool *pool;
	struct HexMap *map;

};

}
}