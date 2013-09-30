
#include "vh.h"

#include "bytescanner.h"
#include "entityhelper.h"

#include "cbase.h"

// windows why
#undef GetProp



CEntityHelper g_EntityHelper;

CEntityHelper &EntityHelper()
{
	return g_EntityHelper;
}

void CEntityHelper::Init()
{ 
	CByteScanner byteScan( "client" );

	// we're sigscanning the cl_dota_showents concommand handler
	// which references CBaseEntityList's m_EntInfoArray

	void *pFunc = NULL;
	if ( byteScan.FindCodePattern( "\x55\x8B\xEC\x83\xEC\x10\xA1\x00\x00\x00\x00\x8B\x50\x18", "xxxxxxx????xxx", &pFunc ) )
	{
		// m_EntInfoArray's access is inlined 0x67 bytes into the func
		m_pEntInfo = *(CEntInfo **)( (uint8 *)pFunc + 0x67 );
	}
	else
	{
		Warning( "[EntityHelper] Unable to find cl_dota_showents!\n" );
	}


	// we find g_pGameRules relative to CDOTA_Modifier_Roshan_CandyBuff::GetActivityTranslationModifiers
	// cause why not
	
	if ( byteScan.FindCodePattern( "\x55\x8B\xEC\x8B\x49\x78\x83\xF9\xFF\x74\x51", "xxxxxxxxxxx", &pFunc ) )
	{
		m_pGameRules = *(C_GameRules ***)( (uint8 *)pFunc + 0x2A );
	}
	else
	{
		Warning( "[EntityHelper] Unable to find CDOTA_Modifier_Roshan_CandyBuff::GetActivityTranslationModifiers!\n" );
	}
}

void CEntityHelper::Shutdown()
{
	m_pEntInfo = NULL;
}

C_BasePlayer *CEntityHelper::GetLocalPlayer()
{
	// IClientTools::GetLocalPlayer returns the C_BasePlayer casted to a EntitySearchResult
	return reinterpret_cast<C_BasePlayer *>( VH().ClientTools()->GetLocalPlayer() );
}

C_BaseEntity *CEntityHelper::GetEntityFromIndex( int entIndex )
{
	if ( m_pEntInfo == NULL )
		return NULL; // can't get an entity if we don't have the entlist

	if ( entIndex < 0 || entIndex >= NUM_ENT_ENTRIES )
		return NULL;

	IClientUnknown *pEnt = reinterpret_cast<IClientUnknown *>( m_pEntInfo[ entIndex ].m_pEntity );

	if ( !pEnt )
		return NULL; // no ent in this index

	return reinterpret_cast<C_BaseEntity *>( pEnt->GetBaseEntity() );
}

C_BaseEntity *CEntityHelper::GetResourceEntity()
{
	if ( m_pResourceEntity == NULL )
		m_pResourceEntity = FindEntityByNetClass( "CDOTA_PlayerResource" );

	return m_pResourceEntity;
}

C_BaseEntity *CEntityHelper::GetGameRulesProxyEntity()
{
	if ( m_pGameRulesProxyEntity == NULL )
		m_pGameRulesProxyEntity = FindEntityByNetClass( "CDOTAGamerulesProxy" );

	return m_pGameRulesProxyEntity;
}

C_GameRules *CEntityHelper::GetGameRules()
{
	if ( m_pGameRules == NULL )
		return NULL;

	return *m_pGameRules;
}


bool CEntityHelper::GetEntPropInt( C_BaseEntity *pEnt, EntPropType propType, const char *propName, int *pValue, int element )
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

				if ( pProp->GetType() != DPT_Int )
					return false;

				propInfo.actualOffset += pProp->GetOffset();
			}
			else if ( propType != DPT_Int )
			{
				return false;
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

	if ( pEnt == GetGameRulesProxyEntity() && GetGameRules() )
	{
		// if we're looking up a netprop on the proxy, we use the real gamerules pointer instead
		pEnt = reinterpret_cast<C_BaseEntity *>( GetGameRules() );
	}

	*pValue = *(int32 *)( (uint8 *)pEnt + offset );
	return true;
}

bool CEntityHelper::GetEntPropHandle( C_BaseEntity *pEnt, EntPropType propType, const char *propName, CBaseHandle *pHandle, int element )
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

				if ( pProp->GetType() != DPT_Int )
					return false;

				propInfo.actualOffset += pProp->GetOffset();
			}
			else if ( propType != DPT_Int )
			{
				return false;
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

	if ( pEnt == GetGameRulesProxyEntity() && GetGameRules() )
	{
		// if we're looking up a netprop on the proxy, we use the real gamerules pointer instead
		pEnt = reinterpret_cast<C_BaseEntity *>( GetGameRules() );
	}

	*pHandle = *(CBaseHandle *)( (uint8 *)pEnt + offset );
	return true;
}

bool CEntityHelper::GetEntPropEnt( C_BaseEntity *pEnt, EntPropType propType, const char *propName, C_BaseEntity **pOutEnt, int element )
{
	CBaseHandle handle;

	if ( !GetEntPropHandle( pEnt, propType, propName, &handle, element ) )
		return false;

	if ( !handle.IsValid() )
		return false;

	int entIndex = handle.GetEntryIndex();
	C_BaseEntity *pEntFromHandle = GetEntityFromIndex( entIndex );

	*pOutEnt = pEntFromHandle;
	return pEntFromHandle != NULL;
}

int CEntityHelper::GetEntPropString( C_BaseEntity *pEnt, EntPropType propType, const char *propName, char **pOutString, int element )
{
	int offset = 0;
	bool isStringIndex = false;

	switch ( propType )
	{
		case EntProp_RecvProp:
		{
			RecvPropInfo_t propInfo;

			if ( !GetRecvPropInfo( pEnt, propName, &propInfo ) )
				return 0;

			if ( propInfo.prop->GetType() != DPT_String )
				return 0;

			offset = propInfo.actualOffset;
			break;
		}

		case EntProp_DataMap:
		{
			DataMapInfo_t mapInfo;

			if ( !GetDataMapInfo( pEnt, propName, &mapInfo ) )
				return 0;

			if ( mapInfo.prop->fieldType != FIELD_CHARACTER &&
				 mapInfo.prop->fieldType != FIELD_STRING &&
				 mapInfo.prop->fieldType != FIELD_MODELNAME &&
				 mapInfo.prop->fieldType != FIELD_SOUNDNAME )
				 return 0;
			
			isStringIndex = mapInfo.prop->fieldType != FIELD_CHARACTER;

			if ( isStringIndex && ( element < 0 || element >= mapInfo.prop->fieldSize ) )
				return 0;

			offset = mapInfo.actualOffset;

			if ( isStringIndex )
				offset += ( element * ( mapInfo.prop->fieldSizeInBytes / mapInfo.prop->fieldSize ) );

			break;
		}
	}

	if ( pEnt == GetGameRulesProxyEntity() && GetGameRules() )
	{
		// if we're looking up a netprop on the proxy, we use the real gamerules pointer instead
		pEnt = reinterpret_cast<C_BaseEntity *>( GetGameRules() );
	}

	if ( isStringIndex )
	{
		string_t index = *(string_t *)( (uint8 *)pEnt + offset );
		*pOutString = index == NULL_STRING ? "" : STRING( index );
	}
	else
	{
		*pOutString = (char *)( (uint8 *)pEnt + offset );
	}
	
	return V_strlen( *pOutString );
}


bool FindInRecvTable( RecvTable *pTable, const char *propName, RecvPropInfo_t *pInfo, int startOffset )
{
	int numProps = pTable->GetNumProps();

	for ( int x = 0 ; x < numProps ; ++x )
	{
		RecvProp *pProp = pTable->GetProp( x );
		const char *name = pProp->GetName();

		if ( name && V_strcmp( name, propName ) == 0 )
		{
			pInfo->actualOffset = startOffset + pProp->GetOffset();
			pInfo->prop = pProp;
			return true;
		}

		if ( pProp->GetDataTable() )
		{
			if ( FindInRecvTable( pProp->GetDataTable(), propName, pInfo, startOffset + pProp->GetOffset() ) )
				return true;
		}
	}

	return false;
}

bool FindInDataMap( datamap_t *pDataMap, const char *dataName, DataMapInfo_t *pInfo )
{
	while ( pDataMap )
	{
		for ( int i = 0 ; i < pDataMap->dataNumFields ; ++i )
		{
			typedescription_t *pDesc = &( pDataMap->dataDesc[ i ] );

			if ( pDesc->fieldName == NULL )
				continue;

			if ( V_strcmp( dataName, pDesc->fieldName ) == 0 )
			{
				pInfo->prop = pDesc;
				pInfo->actualOffset = pDesc->fieldOffset;
				return true;
			}

			if ( pDesc->td == NULL )
				continue;

			if ( !FindInDataMap( pDesc->td, dataName, pInfo ) )
				continue;

			pInfo->actualOffset += pDesc->fieldOffset;
			return true;
		}

		pDataMap = pDataMap->baseMap;
	}

	return false;
}

bool CEntityHelper::GetRecvPropInfo( IClientNetworkable *pNetworkable, const char *propName, RecvPropInfo_t *pInfo )
{
	if ( pNetworkable == NULL )
		return false;

	ClientClass *pClass = pNetworkable->GetClientClass();

	if ( pClass == NULL )
		return false;

	return FindInRecvTable( pClass->m_pRecvTable, propName, pInfo, 0 );
}

bool CEntityHelper::GetDataMapInfo( C_BaseEntity *pEntity, const char *dataName, DataMapInfo_t *pInfo )
{
	if ( pEntity == NULL )
		return false;

	datamap_t *pDataMap = pEntity->GetDataDescMap();

	if ( pDataMap == NULL )
		return false;

	return FindInDataMap( pDataMap, dataName, pInfo );
}

C_BaseEntity *CEntityHelper::FindEntityByNetClass( const char *netClass )
{
	for ( int i = 0 ; i < MAX_EDICTS ; ++i )
	{
		C_BaseEntity *pEnt = GetEntityFromIndex( i );

		if ( !pEnt )
			continue;

		ClientClass *pClass = pEnt->GetClientClass();

		if ( !pClass || !pClass->m_pNetworkName )
			continue;

		if ( V_strcmp( pClass->m_pNetworkName, netClass ) == 0 )
		{
			return pEnt;
		}
	}

	return NULL;
}
