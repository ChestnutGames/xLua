#pragma once

#include <EntitasPP/ISystem.h>
#include <EntitasPP/Pool.h>
#include <EntitasPP/Entity.h>
#include <unordered_map>

namespace Chestnut {
namespace Ball {

class IndexSystem :
	public  EntitasPP::ISystem, public EntitasPP::ISetPoolSystem, public EntitasPP::IInitializeSystem {
public:
	IndexSystem() = default;
	virtual ~IndexSystem() {}

	int SystemType();

	void SetPool(EntitasPP::Pool* pool);

	void Initialize();

	void OnEntityCreated(EntitasPP::Pool* pool, EntitasPP::EntityPtr entity);

protected:

private:
	Chestnut::EntitasPP::Pool *pool;
	std::unordered_map<int, EntitasPP::EntityPtr> entitas;

};

}
}