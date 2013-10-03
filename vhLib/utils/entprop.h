
#pragma once


#include "cbase.h"

#include "entityhelper.h"


// single entprop accessors

#define CEntPropInternal( entPropType, propType, propName ) \
	class CEntProp_##propName  \
	{ \
	public: \
		operator propType##() { return Get(); } \
		propType Get() \
		{ \
			C_BaseEntity *pEnt = GET_OUTER( ThisClass, propName )->GetEntity(); \
			propType value; \
			if ( EntityHelper().GetEntProp<propType>( pEnt, entPropType, #propName, &value ) ) \
			{ \
				return value; \
			} \
			return (propType)0; \
		} \
	} propName;

#define CEntPropEntInternal( entPropType, propName ) \
	class CEntPropEnt_##propName \
	{ \
	public: \
		operator C_BaseEntity *() { return Get(); } \
		C_BaseEntity *Get() \
		{ \
			C_BaseEntity *pOuterEnt = GET_OUTER( ThisClass, propName )->GetEntity(); \
			C_BaseEntity *pEnt = NULL; \
			if ( EntityHelper().GetEntPropEnt( pOuterEnt, entPropType, #propName, &pEnt ) ) \
			{ \
				return pEnt; \
			} \
			return NULL; \
		} \
	} propName;


#define CEntProp( propType, propName ) \
	CEntPropInternal( EntProp_RecvProp, propType, propName )
#define CDataProp( propType, propName ) \
	CEntPropInternal( EntProp_DataMap, propType, propName )


#define CEntPropEnt( propName ) \
	CEntPropEntInternal( EntProp_RecvProp, propName )
#define CDataPropEnt( propName ) \
	CEntPropEntInternal( EntProp_DataMap, propName )


// array ent prop accessors

#define CEntPropArrayInternal( entPropType, propType, propName ) \
	class CEntPropArray_##propName  \
	{ \
	public: \
		propType operator[]( const int index ) { return Get( index ); } \
		propType Get( int element ) \
		{ \
			C_BaseEntity *pEnt = GET_OUTER( ThisClass, propName )->GetEntity(); \
			propType value; \
			if ( EntityHelper().GetEntProp<propType>( pEnt, entPropType, #propName, &value ) ) \
			{ \
				return value; \
			} \
			return (propType)0; \
		} \
	} propName;

#define CEntPropEntArrayInternal( entPropType, propName ) \
	class CEntPropArray_##propName \
	{ \
	public: \
		C_BaseEntity *operator[]( const int index ) { return Get( index ); } \
		C_BaseEntity *Get( int element ) \
		{ \
			C_BaseEntity *pEntOuter = GET_OUTER( ThisClass, propName )->GetEntity(); \
			C_BaseEntity *pEnt = NULL; \
			if ( EntityHelper().GetEntPropEnt( pEntOuter, entPropType, #propName, &pEnt, element ) ) \
			{ \
				return pEnt; \
			} \
			return NULL; \
		} \
	} propName;


#define CEntPropArray( propType, propName ) \
	CEntPropArrayInternal( EntProp_RecvProp, propType, propName )
#define CDataPropArray( propType, propName ) \
	CEntPropArrayInternal( EntProp_DataMap, propType, propName )

#define CEntPropEntArray( propName ) \
	CEntPropEntArrayInternal( EntProp_RecvProp, propName )
#define CDataPropEntArray( propName ) \
	CEntPropEntArrayInternal( EntProp_DataMap, propName )
