
#pragma once


#define DECLARE_MEMBER( type, name, offset ) \
	type Get##name() \
	{ \
		AssertMsg( IsValid(), "Attempted to get member on invalid entity!" ); \
		C_BaseEntity *pEnt = GetEntity(); \
		return (type)((uint8 *)pEnt + offset ); \
	}
