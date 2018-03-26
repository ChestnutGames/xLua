#include "JoinSystem.h"
#include "PositionComponent.h"
#include "IndexComponent.h"
#include <fixedpt.h>

namespace Chestnut {
namespace Ball {

JoinSystem::JoinSystem() {

}

int JoinSystem::SystemType() {
	return EntitasPP::ST_BASE | EntitasPP::ST_SETPOOL | EntitasPP::ST_INIT;
}

void JoinSystem::SetPool(Chestnut::EntitasPP::Pool* pool) {
	this->pool = pool;
}

void JoinSystem::Join(int index) {
	auto e = pool->CreateEntity();
	e->Add<IndexComponent>(index);
	e->Add<PositionComponent>(FIXEDPT_ZERO, FIXEDPT_ZERO, FIXEDPT_ZERO);
}

void JoinSystem::Leave(int index) {

}

}
}