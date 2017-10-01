#pragma once
#include <EntitasPP\Pool.h>
#include <EntitasPP\ISystem.h>
#include <Singleton\Singleton.h>

namespace Chestnut {
namespace Ball {
class JoinSystem : EntitasPP::ISystem, EntitasPP::ISetPoolSystem, Chestnut::Singleton::Singleton<JoinSystem>
{
public:

	virtual void SetPool(EntitasPP::Pool* pool) override;

	void Join(int index);

	EntitasPP::Pool *pool;

};
}
}
