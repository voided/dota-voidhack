
#pragma once

// windows why
#undef GetProp

#include "dt_recv.h"



struct typedescription_t;
class C_BaseEntity;
class C_BasePlayer;
class CBaseHandle;
class IClientNetworkable;
class CEntInfo;


struct RecvPropInfo_t
{
	RecvProp *prop;
	int actualOffset;
};

struct DataMapInfo_t
{
	typedescription_t *prop;
	int actualOffset;
};

enum EntPropType
{
	EntProp_RecvProp,
	EntProp_DataMap,
};


class CEntityHelper
{
	
public:
	CEntityHelper() :
		m_pEntInfo( NULL ), m_pGameRules( NULL ),
		m_RecvPropCache( CaselessStringLessThan )
	{
	}


public:
	void Init();
	void Shutdown();


	C_BasePlayer *GetLocalPlayer();
	C_BaseEntity *GetEntityFromIndex( int entIndex );
	IHandleEntity *LookupEntity( const CBaseHandle &handle );

	// returns the player resource entity
	C_BaseEntity *GetResourceEntity();
	// returns the gamerules proxy entity
	C_BaseEntity *GetGameRulesProxyEntity();

	// returns the real gamerules class
	C_GameRules *GetGameRules();


	template <class T>
	bool GetEntProp( C_BaseEntity *pEnt, EntPropType propType, const char *propName, T *pValue, int element = 0 );

	bool GetEntPropEnt( C_BaseEntity *pEnt, EntPropType propType, const char *propName, C_BaseEntity **pOutEnt, int element = 0 );
	int	GetEntPropString( C_BaseEntity *pEnt, EntPropType propType, const char *propName, char **pOutString, int element = 0 );


	bool GetRecvPropInfo( IClientNetworkable *pNetworkable, const char *propName, RecvPropInfo_t *pInfo );
	bool GetDataMapInfo( C_BaseEntity *pEntity, const char *dataName, DataMapInfo_t *pInfo );


private:
	C_BaseEntity *FindEntityByNetClass( const char *netClass );


private:
	CEntInfo *m_pEntInfo; // CBaseEntityList entlist array
	C_GameRules **m_pGameRules; // g_pGameRules

	CBaseHandle m_ResourceEntity;
	CBaseHandle m_GameRulesProxyEntity; // gamerules proxy


	CUtlMap<const char *, RecvPropInfo_t> m_RecvPropCache;
};

CEntityHelper &EntityHelper();

template <class T>
bool CEntityHelper::GetEntProp( C_BaseEntity *pEnt, EntPropType propType, const char *propName, T *pValue, int element )
{
	int offset = 0;

	switch ( propType )
	{
		case EntProp_RecvProp:
		{
			RecvPropInfo_t propInfo;

			if ( !GetRecvPropInfo( pEnt, propName, &propInfo ) )
				return false;

			SendPropType propType = propInfo.prop->GetType();

			if ( propType == DPT_DataTable )
			{
				// if the prop we're looking up is a datatable, it's an "array" prop with inner elements

				RecvTable *pTable = propInfo.prop->GetDataTable();
				Assert( pTable );

				if ( element < 0 || element >= pTable->GetNumProps() )
					return false;

				RecvProp *pProp = pTable->GetProp( element );
				Assert( pProp );

				propInfo.actualOffset += pProp->GetOffset();
			}

			offset = propInfo.actualOffset;
			break;
		}

		case EntProp_DataMap:
		{
			DataMapInfo_t mapInfo;

			if ( !GetDataMapInfo( pEnt, propName, &mapInfo ) )
				return false;

			if ( element < 0 || element >= mapInfo.prop->fieldSize )
				return false;

			offset = mapInfo.actualOffset + ( element * ( mapInfo.prop->fieldSizeInBytes / mapInfo.prop->fieldSize ) );
			break;
		}

	default:
		Assert( !"EntProp type not implemented!" );
		return false;
	}

	if ( pEnt == GetGameRulesProxyEntity() )
	{
		// if we're looking up a netprop on the gamerules proxy, we need to use the real gamerules class

		if ( GetGameRules() == NULL )
			return false;

		pEnt = reinterpret_cast<C_BaseEntity *>( GetGameRules() );
	}

	*pValue = *(T *)( (uint8 *)pEnt + offset );
	return true;
}

inline IHandleEntity* CBaseHandle::Get() const
{
	return EntityHelper().LookupEntity( *this );
}
