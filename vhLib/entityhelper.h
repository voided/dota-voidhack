
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
		m_pEntInfo( NULL ),
		m_pResourceEntity( NULL )
	{
	}


public:
	void Init();
	void Shutdown();


	C_BasePlayer *GetLocalPlayer();
	C_BaseEntity *GetEntityFromIndex( int entIndex );

	C_BaseEntity *GetResourceEntity();


	bool	GetEntPropInt( C_BaseEntity *pEnt, EntPropType propType, const char *propName, int *pValue, int element = 0 );
	bool	GetEntPropHandle( C_BaseEntity *pEnt, EntPropType propType, const char *propName, CBaseHandle *pHandle, int element = 0 );
	bool	GetEntPropEnt( C_BaseEntity *pEnt, EntPropType propType, const char *propName, C_BaseEntity **pOutEnt, int element = 0 );
	int 	GetEntPropString( C_BaseEntity *pEnt, EntPropType propType, const char *propName, char **pOutString, int element = 0 );

	bool GetRecvPropInfo( IClientNetworkable *pNetworkable, const char *propName, RecvPropInfo_t *pInfo );
	bool GetDataMapInfo( C_BaseEntity *pEntity, const char *dataName, DataMapInfo_t *pInfo );


private:
	CEntInfo *m_pEntInfo; // CBaseEntityList entlist array

	C_BaseEntity *m_pResourceEntity;


};

CEntityHelper &EntityHelper();
