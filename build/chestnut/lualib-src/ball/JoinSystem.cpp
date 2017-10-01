#include "JoinSystem.h"
#include "PositionComponent.h"
#include <fixedpt.h>

namespace Chestnut {
namespace Ball {

void JoinSystem::SetPool(Chestnut::EntitasPP::Pool* pool){
	this->pool = pool;
}

void JoinSystem::Join(int index) {
	auto e = pool->CreateEntity();
	e->Add<PositionComponent>(fixedpt_fromint(1), fixedpt_fromint(2), fixedpt_fromint(3));
}
		
}
}