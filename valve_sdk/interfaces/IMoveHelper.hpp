#pragma once

class IClientEntity;

class IMoveHelper
{
private:
	virtual void unknown() = 0;
public:
	virtual void SetHost(IClientEntity* host) = 0;
private:
	virtual void unknown1() = 0;
	virtual void unknown2() = 0;
public:
	virtual bool unknown_func() = 0;
};
