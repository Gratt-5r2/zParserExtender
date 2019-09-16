// Supported with union (c) 2018 Union team

#ifndef __ZMACRO_H__VER0__
#define __ZMACRO_H__VER0__

namespace Gothic_I_Classic {

#define zRELEASE( obj ) { (obj)->Release(); obj = Null; }
#define zADDREF( obj )  (obj)->AddRef()
#define zNEW( obj ) new obj
#define zDELETE( obj ) delete obj
#ifndef SAFE_DELETE
#define SAFE_DELETE( obj ) if( obj ) { delete obj; obj = 0 }
#endif
#define zCLASS_DECLARATION( classname ) static zCClassDef* classDef;
#define zCall( address ) { XCALL( address ); }
#define zInit( call ) { if( Union.GetEngineVersion() == Engine_G1 ) call; }
#define zRTTI( type ) { return type; }
#ifndef __DONT_USE_VTABLE__
#define zPureCall = 0;
#else
#define zPureCall ;
#define virtual
#endif
#define group struct
#define RGBA(r,g,b,a)   ((zUINT32)(zUINT8)(r)|(zUINT32)(zUINT8)(g)<<8|(zUINT32)(zUINT8)(b)<<16|(zUINT32)(zUINT8)(a)<<24)
#define EXTRACT_R(rgba) (rgba		& 0xff)
#define EXTRACT_G(rgba) (rgba >> 8	& 0xff)
#define EXTRACT_B(rgba) (rgba >> 16 & 0xff)
#define EXTRACT_A(rgba) (rgba >> 24 & 0xff)
#define zMEMPOOL_VOLATILE_DECLARATION( className )


#define SafeDiv( a, b ) \
    ( b == 0.0 ? 0.0 : a / b )


#define sqr(a) (a * a)


#define CalcAngle(a, b)                 \
    float angle = atan (SafeDiv(a, b)); \
    if (a >= 0 && b < 0)                \
    angle = 180.0f / DEGREE + angle;    \
    else if (a < 0 && b <= 0)           \
    angle = 180.0f / DEGREE + angle;    \
    else if (a <= 0 && b > 0)           \
    angle = 360.0f / DEGREE + angle;    \
    return angle;


#define RotAngle(a, b, c)                     \
    float radAngle = GetAngle##c();           \
    float length   = sqrt(sqr(a) + sqr(b));   \
    radAngle      += rad;                     \
    a              = sin(radAngle) * length;  \
    b              = cos(radAngle) * length;  \
    return *this;
	

#define zOperatorNew( className, baseClassName )            \
  static void* operator new( unsigned int v ) {             \
    return operator new( v, #className, #baseClassName, 0); \
  }


#define zOperatorDelete( className, baseClassName )             \
  static void operator delete(void* v) {                        \
    return operator delete (v, #className, #baseClassName, 0);  \
  }


#define zMEMPOOL_DECLARATION_TEMPLATE( classname, address )                                         \
    void *operator new( size_t s ) {                                                                \
      return ((zCMemPoolBase*)address)->Alloc();                                                    \
    }                                                                                               \
    void operator delete( void *p ) {                                                               \
      ((zCMemPoolBase*)address)->Free( p );                                                         \
    }                                                                                               \
    void* operator new( unsigned int size, const char* classn, const char *file, int l ) {          \
      zCMemPoolBase::SetMemDebug( classn, file, l );                                                \
      return operator new( size );                                                                  \
    }                                                                                               \
    void operator delete( void* ptr, const char* classn, const char* file, int line ) {             \
      operator delete( ptr );                                                                       \
    }                                                                                               \
    static void PreAlloc( size_t num, zBOOL force_oneblock = False ){                               \
      ((zCMemPoolBase*)address)->PreAlloc( num, force_oneblock );                                   \
    }


#define zMEMPOOL_DECLARATION( classname, address )                                                  \
    void* operator new( size_t s ){                                                                 \
      return ((zCMemPoolBase*)address)->Alloc();                                                    \
    }                                                                                               \
    void operator delete( void* p ) {                                                               \
      ((zCMemPoolBase*)address)->Free( p );                                                         \
    }                                                                                               \
    void* operator new( unsigned int size, const char* classn, const char* file, int l ) {          \
      zCMemPoolBase::SetMemDebug( classn, file, l );                                                \
      return operator new(size);                                                                    \
    }                                                                                               \
    void  operator delete( void* ptr,const char *classn,const char *file,int line) {                \
      operator delete(ptr);                                                                         \
    }                                                                                               \
    static void PreAlloc( size_t num, bool32 force_oneblock = False ) {                             \
      ((zCMemPoolBase*)address)->PreAlloc( num, force_oneblock );                                   \
    }                                                                                               \
    static void PoolAdd( classname *mem, int num_objects, bool32 free = False ) {                   \
      ((zCMemPoolBase*)address)->PoolAdd( mem, num_objects, free );                                 \
    }

  
#define zCOLLISION_OBJECT_DECLARATION( className )                    \
    static zCCollisionObjectDef* className::S_GetCollObjClass(void) { \
      return className::s_oCollObjClass;                              \
    };                                                                \
    static zCCollisionObjectDef* className::s_oCollObjClass; 


#define zCLASS_UNION_DECLARATION( className )                \
  static  zCClassDef* className::classDef;                   \
  static  zCObject*   className::_CreateNewInstance( void ); \
  virtual zCClassDef* className::_GetClassDef( void ) const { return className::classDef; };


#define zCLASS_UNION_DEFINITION( className, baseClassName, classFlags, archiveVersion )                                                       \
  zCClassDef* className::classDef = new zCClassDef( #className, #baseClassName, className::_CreateNewInstance, sizeof(className), 0, 0, "" ); \
  zCObject*  className::_CreateNewInstance ( void ) { return new className; }


#define zCOLLISION_OBJECT_UNION_DECLARATION( className )                                                           \
  static  zCCollisionObjectDef* className::S_GetCollObjClass(void)      { return &(className::s_oCollObjClass); }; \
  virtual zCCollisionObjectDef* className::GetCollObjClass(void) const  { return &(className::s_oCollObjClass); }; \
  static  zCCollisionObjectDef  className::s_oCollObjClass;                                                        \
  static  zCCollisionObject*    className::_CreateNewInstance(void);


#define zCOLLISION_OBJECT_UNION_DEFINITION( className, isVolatile )                           \
  zCCollisionObjectDef className::s_oCollObjClass(isVolatile, className::_CreateNewInstance); \
  zCCollisionObject* className::_CreateNewInstance(void) {                                    \
    return new className;                                                                     \
  }

} // namespace Gothic_II_Addon

#endif // __ZMACRO_H__VER0__