#pragma once
#include "stdafx.h"

class SystemFrame
{
protected:
	mutex m_mutex;
public:
	virtual void CheckPacket(void*) { };
};