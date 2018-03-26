#include "MapSystem.h"
#include "IndexComponent.h"
#include <EntitasPP/Group.h>
#include <EntitasPP/Matcher.h>


namespace Chestnut {
namespace Ball {

int MapSystem::SystemType() {
	return EntitasPP::ST_BASE | EntitasPP::ST_SETPOOL | EntitasPP::ST_INIT;
}

void MapSystem::Initialize() {

	//hexmap_create_from_plist()
}

void MapSystem::FindPath(int index, struct vector3 start, struct vector3 dst) {
	pool->GetEntities(Matcher_AllOf(IndexComponent));

}


}
}



