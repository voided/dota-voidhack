

#pragma once


#include "entprop.h"

#include "dota_consts.h"


#define DOTA_CLASS( className, baseClass ) \
	typedef baseClass BaseClass; \
	typedef className ThisClass;

#define DOTA_CLASS_NOBASE( className ) \
	typedef className ThisClass;


class C_BaseEntity;


class C_DOTABaseEntity
{
	DOTA_CLASS_NOBASE( C_DOTABaseEntity );

public:
	C_DOTABaseEntity( C_BaseEntity *pEnt );


	C_BaseEntity *GetEntity() { return m_pEntity; }
	bool IsValid() { return m_pEntity != NULL; }


protected:
	C_BaseEntity *m_pEntity;

};
