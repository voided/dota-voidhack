
#pragma once


#include "cbase.h"

#include "entityhelper.h"


#define CEntPropInternal( entPropType, propType, propName, ret ) \
	class CEntProp_##propName  \
	{ \
	public: \
		operator propType##() { return Get(); } \
		propType Get() \
		{ \
			ThisClass *pOuter = GET_OUTER( ThisClass, propName ); \
			C_BaseEntity *pEnt = pOuter->GetEntity(); \
			AssertMsg1( pEnt, "Entprop '%s' lookup on invalid entity!", #propName ); \
			propType value; \
			if ( EntityHelper().GetEntProp<propType>( pEnt, entPropType, #propName, &value ) ) \
			{ \
				return value; \
			} \
			AssertMsg2( 0, "Unable to find entprop %s on %s", #propName, pOuter->GetEntityName() ); \
			return ret; \
		} \
	} propName;

#define CEntPropArrayInternal( entPropType, propType, propName, ret ) \
	class CEntPropArray_##propName  \
	{ \
	public: \
		propType operator[]( const int index ) { return Get( index ); } \
		propType Get( int element ) \
		{ \
			ThisClass *pOuter = GET_OUTER( ThisClass, propName ); \
			C_BaseEntity *pEnt = pOuter->GetEntity(); \
			AssertMsg1( pEnt, "Entprop '%s' lookup on invalid entity!", #propName ); \
			propType value; \
			if ( EntityHelper().GetEntProp<propType>( pEnt, entPropType, #propName, &value, element ) ) \
			{ \
				return value; \
			} \
			AssertMsg2( 0, "Unable to find entprop %s on %s", #propName, pOuter->GetEntityName() ); \
			return ret; \
		} \
		int Size() \
		{ \
			ThisClass *pOuter = GET_OUTER( ThisClass, propName ); \
			C_BaseEntity *pEnt = pOuter->GetEntity(); \
			AssertMsg1( pEnt, "Entprop '%s' lookup on invalid entity!", #propName ); \
			int size = 0; \
			if ( EntityHelper().GetEntPropArraySize( pEnt, entPropType, #propName, &size ) ) \
			{ \
				return size; \
			} \
			AssertMsg2( 0, "Unable to find entprop %s on %s", #propName, pOuter->GetEntityName() ); \
			return 0; \
		} \
	} propName;


#define CEntProp( propType, propName ) \
	CEntPropInternal( EntProp_RecvProp, propType, propName, (propType)0 )
#define CDataProp( propType, propName ) \
	CEntPropInternal( EntProp_DataMap, propType, propName, (propType)0 )

#define CEntPropVector( propName ) \
	CEntPropInternal( EntProp_RecvProp, Vector, propName, Vector() )
#define CDataPropVector( propName ) \
	CEntPropInternal( EntProp_DataMap, Vector, propName, Vector() )


#define CEntPropArray( propType, propName ) \
	CEntPropArrayInternal( EntProp_RecvProp, propType, propName, (propType)0 )
#define CDataPropArray( propType, propName ) \
	CEntPropArrayInternal( EntProp_DataMap, propType, propName, (propType)0 )

#define CEntPropVectorArray( propName ) \
	CEntPropArrayInternal( EntProp_RecvProp, propType, propName, Vector() )
#define CDataPropVectorArray( propName ) \
	CEntPropArrayInternal( EntProp_DataMap, propType, propName, Vector() )
