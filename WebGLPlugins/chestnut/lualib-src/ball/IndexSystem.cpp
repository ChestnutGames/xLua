#include "IndexSystem.h"
#include "IndexComponent.h"
#include <EntitasPP/Group.h>
#include <EntitasPP/Matcher.h>


namespace Chestnut {
namespace Ball {

int IndexSystem::SystemType() {
	return EntitasPP::ST_BASE | EntitasPP::ST_SETPOOL | EntitasPP::ST_INIT;
}

void IndexSystem::SetPool(EntitasPP::Pool* pool) {
	this->pool = pool;
}

void IndexSystem::Initialize() {
}

void IndexSystem::OnEntityCreated(EntitasPP::Pool* pool, EntitasPP::EntityPtr entity) {
	/*int index = entity->Get<IndexComponent>()->index;
	entitas.emplace(std::make_pair<int, EntitasPP::EntityPtr>(index, entity));*/
}

}
}



