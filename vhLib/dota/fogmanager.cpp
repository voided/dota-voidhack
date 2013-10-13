
#include "fogmanager.h"

#include "convar.h"


CFogManager &FogManager()
{
	static CFogManager fogManager;
	return fogManager;
}


void CFogManager::Init()
{
	ConVarRef fog_override( "fog_override" );
	if ( fog_override.IsValid() )
	{
		fog_override.SetValue( 1 );
	}

	ConVarRef fog_enable( "fog_enable" );
	if ( fog_enable.IsValid() )
	{
		fog_enable.SetValue( 0 );
	}

	ConVarRef fow_client_visibility( "fow_client_visibility" );
	if ( fow_client_visibility.IsValid() )
	{
		fow_client_visibility.SetValue( 1 );
	}
}

void CFogManager::Shutdown()
{
}
