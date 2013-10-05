
#pragma once


#include "cbase.h"

#include "entityhelper.h"


#define CEntPropInternal( entPropType, propType, propName ) \
	class CEntProp_##propName  \
	{ \
	public: \
		operator propType##() { return Get(); } \
		propType Get() \
		{ \
			C_BaseEntity *pEnt = GET_OUTER( ThisClass, propName )->GetEntity(); \
			AssertMsg( pEnt, "Entprop lookup on invalid entity!" ); \
			propType value; \
			if ( EntityHelper().GetEntProp<propType>( pEnt, entPropType, #propName, &value ) ) \
			{ \
				return value; \
			} \
			return (propType)0; \
		} \
	} propName;

#define CEntPropArrayInternal( entPropType, propType, propName ) \
	class CEntPropArray_##propName  \
	{ \
	public: \
		propType operator[]( const int index ) { return Get( index ); } \
		propType Get( int element ) \
		{ \
			C_BaseEntity *pEnt = GET_OUTER( ThisClass, propName )->GetEntity(); \
			AssertMsg( pEnt, "Entprop lookup on invalid entity!" ); \
			propType value; \
			if ( EntityHelper().GetEntProp<propType>( pEnt, entPropType, #propName, &value, element ) ) \
			{ \
				return value; \
			} \
			return (propType)0; \
		} \
	} propName;


#define CEntProp( propType, propName ) \
	CEntPropInternal( EntProp_RecvProp, propType, propName )
#define CDataProp( propType, propName ) \
	CEntPropInternal( EntProp_DataMap, propType, propName )


#define CEntPropArray( propType, propName ) \
	CEntPropArrayInternal( EntProp_RecvProp, propType, propName )
#define CDataPropArray( propType, propName ) \
	CEntPropArrayInternal( EntProp_DataMap, propType, propName )
