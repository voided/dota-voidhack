
#pragma once


class VFuncEmptyClass {};


template <typename Return>
union VFuncUnion0
{
	Return ( VFuncEmptyClass::*mfpnew )();
	void *addr;
};
union VFuncUnion0_void
{
	void ( VFuncEmptyClass::*mfpnew )();
	void *addr;
};

template <typename Return, typename T1>
union VFuncUnion1
{
	Return ( VFuncEmptyClass::*mfpnew )( T1 );
	void *addr;
};
template <typename T1>
union VFuncUnion1_void
{
	void ( VFuncEmptyClass::*mfpnew )( T1 );
	void *addr;
};


#define VFUNC_INTERNAL( offset ) \
	AssertMsg( IsValid(), "VFunc call on invalid entity!" ); \
	C_BaseEntity *pEnt = GetEntity(); \
	void **vtable = *(void ***)pEnt; \
	void *func = vtable[ offset ];


#define DECLARE_VFUNC0( returnType, name, offset ) \
	returnType name##() \
	{ \
		VFUNC_INTERNAL( offset ); \
		VFuncUnion0<returnType> u; u.addr = func; \
		return ( reinterpret_cast<VFuncEmptyClass *>( pEnt )->*u.mfpnew )(); \
	}
#define DECLARE_VFUNC0_void( name, offset ) \
	void name##() \
	{ \
		VFUNC_INTERNAL( offset ); \
		VFuncUnion0<returnType> u; u.addr = func; \
		( reinterpret_cast<VFuncEmptyClass *>( pEnt )->*u.mfpnew )(); \
	}

#define DECLARE_VFUNC1( returnType, name, offset, param1 ) \
	returnType name##( param1 p1 ) \
	{ \
		VFUNC_INTERNAL( offset ); \
		VFuncUnion1<returnType, param1> u; u.addr = func; \
		return ( reinterpret_cast<VFuncEmptyClass *>( pEnt )->*u.mfpnew )( p1 ); \
	}
#define DECLARE_VFUNC1_void( name, offset, param1 ) \
	void name##( param1 p1 ) \
	{ \
		VFUNC_INTERNAL( offset ); \
		VFuncUnion1<returnType, param1> u; u.addr = func; \
		( reinterpret_cast<VFuncEmptyClass *>( pEnt )->*u.mfpnew )( p1 ); \
	}
