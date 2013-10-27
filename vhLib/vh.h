
#pragma once


#include "interface.h"

#include "utlvector.h"

#include "cbase.h"
#include "toolframework/ienginetool.h"

#include <sourcehook/sourcehook_impl.h>


class IVEngineClient;
class IBaseClientDLL;
class IEngineTool;
class IClientTools;
class IClientEntityList;
enum ClientFrameStage_t;


// a game manager that wishes to think per frame
class IFrameManager
{
public:
	virtual void Think() = 0;
};


struct FactoryInfo_t
{
	void Init();


	CreateInterfaceFn engineFactory;
	CreateInterfaceFn clientFactory;
	CreateInterfaceFn cvarFactory;
	CreateInterfaceFn fileSystemFactory;
	CreateInterfaceFn vguiFactory;
	CreateInterfaceFn scaleformFactory;
};

template <typename T>
T *GetInterface( CreateInterfaceFn factory, const char *version )
{
	T* iface = reinterpret_cast<T *>( factory( version, NULL ) );

	if ( !iface )
	{
		Error( "Unable to get interface %s!\n", version );
		return NULL;
	}

	return iface;
}


class CVH
{

public:
	CVH() :
		// engine
		m_pEngineClient( NULL ), 
		m_pEngineTool( NULL ),
		m_pGameEventManager( NULL ),
		m_pFileSystem( NULL ),

		// client
		m_pClientDLL( NULL ),
		m_pClientTools( NULL )
	{
	}


public:
	void Init();
	void Shutdown();

	// add and remove per-frame think hooks
	void AddFrameHook( const IFrameManager *manager );
	void RemoveFrameHook( const IFrameManager *manager );

	// factory accessors
	FactoryInfo_t &GetFactoryInfo() { return m_factoryInfo; }

	// engine interfaces
	IVEngineClient *EngineClient() { return m_pEngineClient; }
	IEngineTool *EngineTool() { return m_pEngineTool; }
	IGameEventManager2 *GameEventManager() { return m_pGameEventManager; }
	IFileSystem *FileSystem() { return m_pFileSystem; }

	// client interfaces
	IBaseClientDLL *ClientDLL() { return m_pClientDLL; }
	IClientTools *ClientTools() { return m_pClientTools; }


private:
	void FrameStageNotify( ClientFrameStage_t curStage ); // IBaseClientDLL::FrameStageNotify hook


private:
	FactoryInfo_t m_factoryInfo;

	// engine
	IVEngineClient *m_pEngineClient;
	IEngineTool *m_pEngineTool;
	IGameEventManager2 *m_pGameEventManager;
	IFileSystem *m_pFileSystem;

	// client
	IBaseClientDLL *m_pClientDLL;
	IClientTools *m_pClientTools;

	CUtlVector<IFrameManager *> m_FrameHooks;
	
};

CVH &VH();

extern SourceHook::ISourceHook *g_SHPtr;
extern int g_PLID;
