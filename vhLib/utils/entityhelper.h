
#pragma once


#include <cstddef>


class RecvProp;
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
		m_pResourceEntity( NULL ), m_pGameRulesProxyEntity( NULL )
	{
	}


public:
	void Init();
	void Shutdown();


	C_BasePlayer *GetLocalPlayer();
	C_BaseEntity *GetEntityFromIndex( int entIndex );

	// returns the player resource entity
	C_BaseEntity *GetResourceEntity();
	// returns the gamerules proxy entity
	C_BaseEntity *GetGameRulesProxyEntity();

	// returns the real gamerules class
	C_GameRules *GetGameRules();


	bool	GetEntPropInt( C_BaseEntity *pEnt, EntPropType propType, const char *propName, int *pValue, int element = 0 );
	bool	GetEntPropHandle( C_BaseEntity *pEnt, EntPropType propType, const char *propName, CBaseHandle *pHandle, int element = 0 );
	bool	GetEntPropEnt( C_BaseEntity *pEnt, EntPropType propType, const char *propName, C_BaseEntity **pOutEnt, int element = 0 );
	int 	GetEntPropString( C_BaseEntity *pEnt, EntPropType propType, const char *propName, char **pOutString, int element = 0 );

	bool GetRecvPropInfo( IClientNetworkable *pNetworkable, const char *propName, RecvPropInfo_t *pInfo );
	bool GetDataMapInfo( C_BaseEntity *pEntity, const char *dataName, DataMapInfo_t *pInfo );


private:
	C_BaseEntity *FindEntityByNetClass( const char *netClass );

private:
	CEntInfo *m_pEntInfo; // CBaseEntityList entlist array
	C_GameRules **m_pGameRules; // g_pGameRules

	C_BaseEntity *m_pResourceEntity;
	C_BaseEntity *m_pGameRulesProxyEntity; // gamerules proxy


};

CEntityHelper &EntityHelper();
