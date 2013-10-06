
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
enum ClientFrameStage_t;


typedef void (*FrameFunction)();


class CVH
{

public:
	CVH();


public:
	void Init();
	void Shutdown();

	// add and remove per-frame think hooks
	void AddFrameHook( FrameFunction func );
	void RemoveFrameHook( FrameFunction func );

	// various engine/client interface accessors
	IVEngineClient *EngineClient() { return m_pEngineClient; }
	IBaseClientDLL *ClientDLL() { return m_pClientDLL; }
	IEngineTool *EngineTool() { return m_pEngineTool; }
	IClientTools *ClientTools() { return m_pClientTools; }
	IGameEventManager2 *GameEventManager() { return m_pGameEventManager; }


private:
	void FrameStageNotify( ClientFrameStage_t curStage );
	void Think();


private:
	CreateInterfaceFn m_fnEngineFactory;
	CreateInterfaceFn m_fnClientFactory;
	CreateInterfaceFn m_fnCvarFactory;

	IVEngineClient *m_pEngineClient;
	IBaseClientDLL *m_pClientDLL;
	IEngineTool *m_pEngineTool;
	IClientTools *m_pClientTools;
	IGameEventManager2 *m_pGameEventManager;

	CUtlVector<FrameFunction> m_FrameHooks;
};

CVH &VH();

extern SourceHook::ISourceHook *g_SHPtr;
extern int g_PLID;
