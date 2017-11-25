#pragma once
#include <EntitasPP/ISystem.h>
#include <EntitasPP/Group.h>
#include <fixedpt.h>

namespace Chestnut {
namespace Ball {

class MoveSystem :
	public EntitasPP::ISystem, public EntitasPP::IExecuteSystem
{
public:
	int SystemType();

	virtual void Execute();

};

}
}