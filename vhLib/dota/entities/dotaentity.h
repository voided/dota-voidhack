

#pragma once


#include "entprop.h"
#include "entfuncs.h"

#include "dota_consts.h"

#include "networkvar.h"

#include "const.h"


class C_BaseEntity;


class CCollision
{
	DECLARE_CLASS_NOBASE( CCollision );

public:
	CCollision( C_BaseEntity *pEnt )
		: m_pOuter( pEnt )
	{
	}


public:
	CEntPropVector( m_vecMins );
	CEntPropVector( m_vecMaxs );

	CEntProp( uint8, m_nSolidType );
	CEntProp( uint16, m_usSolidFlags ); 


private:
	// to support ent prop lookup
	C_BaseEntity *GetEntity() { return m_pOuter; }


private:
	C_BaseEntity *m_pOuter;

};


// base wrapper for most dota related entities
class C_DOTABaseEntity
{
	DECLARE_CLASS_NOBASE( C_DOTABaseEntity );

public:
	C_DOTABaseEntity( C_BaseEntity *pEnt );

	operator C_BaseEntity *() { return GetEntity(); }


	// classname
	const char *GetEntityName() { return m_iName; }


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


	// is this entity outside of the client's PVS?
	bool IsDormant()
	{
		Assert( IsValid() );
		return GetEntity()->IsDormant();
	}

	const Vector &GetAbsOrigin();
	const QAngle &GetAbsAngles();


	// is this entity on the local player's team?
	bool IsOnLocalTeam();


	CCollision CollisionProp()
	{
		Assert( IsValid() );
		return CCollision( GetEntity() );
	}
	

public:
	CEntProp( char *, m_iName );

	CEntProp( int, m_iTeamNum );

	CEntPropVector( m_vecOrigin );

	CEntProp( ICollideable *, m_Collision );


private:
	EHANDLE m_Entity;
};
