
#include "fogmanager.h"

#include "convarhelper.h"


CFogManager &FogManager()
{
	static CFogManager fogManager;
	return fogManager;
}


void CFogManager::Init()
{
	ConVar *fog_override = ConVarHelper().FindConVar( "fog_override" );
	if ( fog_override )
	{
		fog_override->SetValue( 1 );
	}

	ConVar *fog_enable = ConVarHelper().FindConVar( "fog_enable" );
	if ( fog_enable )
	{
		fog_enable->SetValue( 0 );
	}

	ConVar *fow_client_visibility = ConVarHelper().FindConVar( "fow_client_visibility" );
	if ( fow_client_visibility )
	{
		fow_client_visibility->SetValue( 1 );
	}

	ConVar *dota_minimap_draw_fow = ConVarHelper().FindConVar( "dota_minimap_draw_fow" );
	if ( dota_minimap_draw_fow )
	{
		dota_minimap_draw_fow->SetValue( 0 );
	}
}

void CFogManager::Shutdown()
{
}
