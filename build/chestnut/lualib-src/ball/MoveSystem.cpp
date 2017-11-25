#include "MoveSystem.h"

namespace Chestnut {
namespace Ball {

int MoveSystem::SystemType() {
	return EntitasPP::ST_BASE | EntitasPP::ST_SETPOOL | EntitasPP::ST_INIT;
}

void MoveSystem::Execute() {

}

}
}