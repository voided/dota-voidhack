

#pragma once


#include "entprop.h"

#include "dota_consts.h"


#define DOTA_CLASS( className, baseClass ) \
	typedef baseClass BaseClass; \
	typedef className ThisClass;

#define DOTA_CLASS_NOBASE( className ) \
	typedef className ThisClass;


class C_BaseEntity;


// base wrapper for most dota related entities
class C_DOTABaseEntity
{
	DOTA_CLASS_NOBASE( C_DOTABaseEntity );

public:
	C_DOTABaseEntity( C_BaseEntity *pEnt );

	operator C_BaseEntity *() { return GetEntity(); }


	// return the underlying entity this class wraps
	// this is required to support entprop lookup!
	C_BaseEntity *GetEntity() const
	{
		IClientUnknown *pUnk = reinterpret_cast<IClientUnknown *>( m_Entity.Get() );

		if ( !pUnk )
			return NULL;

		return pUnk->GetBaseEntity();
	}

	// is the wrapped entity valid?
	bool IsValid() { return GetEntity() != NULL; }


private:
	CBaseHandle m_Entity;

};
