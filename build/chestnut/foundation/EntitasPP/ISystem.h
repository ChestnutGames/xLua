// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: MIT License
// MIT License web page: https://opensource.org/licenses/MIT

#pragma once

#include "Entity.h"
#include "Matcher.h"
#include "TriggerOnEvent.h"
#include <vector>

namespace Chestnut {
namespace EntitasPP {
class Pool;

enum SystemType {
	ST_BASE = 1 << 0,
	ST_SETPOOL = 1 << 1,
	ST_INIT = 1 << 2,
	ST_EXEC = 1 << 3,
	ST_FIXEDEXE = 1 << 4,

};

class ISystem {
	friend class SystemContainer;
	static int _systemid;
protected:
	ISystem() { _systemid++; }

public:
	virtual ~ISystem() = default;

	virtual int SystemType();
};

class ISetPoolSystem {
protected:
	ISetPoolSystem() = default;

public:
	virtual ~ISetPoolSystem() = default;

	virtual void SetPool(Pool* pool) = 0;
};

class IInitializeSystem {
protected:
	IInitializeSystem() = default;

public:
	virtual ~IInitializeSystem() = default;

	virtual void Initialize() = 0;
};

class IExecuteSystem : public ISystem {
protected:
	IExecuteSystem() = default;

public:
	virtual ~IExecuteSystem() = default;

	virtual void Execute() = 0;
};

class IFixedExecuteSystem : public ISystem {
protected:
	IFixedExecuteSystem() = default;

public:
	virtual ~IFixedExecuteSystem() = default;

	virtual void FixedExecute() = 0;
};

class IReactiveExecuteSystem : public ISystem {
protected:
	IReactiveExecuteSystem() = default;

public:
	virtual ~IReactiveExecuteSystem() = default;

	virtual void Execute(std::vector<EntityPtr> entities) = 0;
};

class IReactiveSystem : public IReactiveExecuteSystem {
public:
	virtual ~IReactiveSystem() = default;

	TriggerOnEvent trigger;
};

class IMultiReactiveSystem : public IReactiveExecuteSystem {
public:
	virtual ~IMultiReactiveSystem() = default;

	std::vector<TriggerOnEvent> triggers;
};

class IEnsureComponents {
protected:
	IEnsureComponents() = default;

public:
	Matcher ensureComponents;
};

class IExcludeComponents {
protected:
	IExcludeComponents() = default;

public:
	Matcher excludeComponents;
};

class IClearReactiveSystem {
protected:
	IClearReactiveSystem() = default;
};

}
}