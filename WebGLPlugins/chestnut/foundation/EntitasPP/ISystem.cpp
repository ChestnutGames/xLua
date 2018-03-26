#include "ISystem.h"

namespace Chestnut {
namespace EntitasPP {

int ISystem::_systemid = 1000;

int ISystem::SystemType() {
	return ST_BASE;
}

}
}