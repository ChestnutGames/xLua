#pragma once
#include <EntitasPP\Pool.h>
#include <EntitasPP\ISystem.h>
#include <Singleton\Singleton.h>

namespace Chestnut {
namespace Ball {
class JoinSystem : public EntitasPP::ISystem, public EntitasPP::ISetPoolSystem {
public:
	JoinSystem();

	int SystemType();

	void SetPool(EntitasPP::Pool* pool);

	void Join(int index);

	void Leave(int index);

protected:
	EntitasPP::Pool *pool;

};
}
}
