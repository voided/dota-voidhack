
#include "fogmanager.h"

#include "convarhelper.h"


// fog enabled by default, as the usefulness is limited
ConVar vh_fog_enable( "vh_fog_enable", "1", FCVAR_NONE, "Enable or disable game fog" );

void FogManager_OnFogChanged( IConVar *var, const char *pOldValue, float flOldValue )
{
	if ( vh_fog_enable.GetBool() )
	{
		FogManager().EnableFog();
	}
	else
	{
		FogManager().DisableFog();
	}
}


CFogManager &FogManager()
{
	static CFogManager fogManager;
	return fogManager;
}


void CFogManager::Init()
{
	// install the change callback, and invoke it
	// this will perform the enable/disable fog logic
	vh_fog_enable.InstallChangeCallback( FogManager_OnFogChanged );
}

void CFogManager::Shutdown()
{
	// revert and enable fog on shutdown
	EnableFog();
}


void CFogManager::EnableFog()
{
	ConVarHelper().SetConVar( "fog_override", 0 );
	ConVarHelper().SetConVar( "fog_enable", 1 );
	ConVarHelper().SetConVar( "fow_client_visibility", 0 );
	ConVarHelper().SetConVar( "dota_minimap_draw_fow", 1 );
}

void CFogManager::DisableFog()
{
	ConVarHelper().SetConVar( "fog_override", 1 );
	ConVarHelper().SetConVar( "fog_enable", 0 );
	ConVarHelper().SetConVar( "fow_client_visibility", 1 );
	ConVarHelper().SetConVar( "dota_minimap_draw_fow", 0 );
}
