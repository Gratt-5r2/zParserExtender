
// This file added in headers queue
// File: "Headers.h"

namespace GOTHIC_ENGINE {
  string SymbolTypeToString( const int& type ) {
    switch( type ) {
      case zPAR_TYPE_VOID:      return "VOID";
      case zPAR_TYPE_FLOAT:     return "FLOAT";
      case zPAR_TYPE_INT:       return "INT";
      case zPAR_TYPE_STRING:    return "STRING";
      case zPAR_TYPE_CLASS:     return "CLASS";
      case zPAR_TYPE_FUNC:      return "FUNC";
      case zPAR_TYPE_PROTOTYPE: return "PROTOTYPE";
      case zPAR_TYPE_INSTANCE:  return "INSTANCE";
    }

    return "UNKNOWN";
  }

#define EXTERNAL_DEFINE_BEGIN( f )                            \
  if( createFuncList ) {                                      \
    AddDynamicExternalFuncToList( #f );                       \
  }                                                           \
  else if( funcName == #f ) {                                 \
    MessageExternalDefined( #f ); par->DefineExternal( #f, f, \

#define EXTERNAL_DEFINE_BEGIN_EX( n, f )                      \
  if( createFuncList ) {                                      \
    AddDynamicExternalFuncToList( n );                        \
  }                                                           \
  else if( funcName == n ) {                                  \
    MessageExternalDefined( n );  par->DefineExternal( n, f,  \

#define EXTERNAL_DEFINE_END \
    , 0);                   \
    return true;            \
  }

  static void MessageExternalDefined( const string& funcName ) {
    if( zCParserExtender::MessagesLevel <= 0 )
      return;

    Col16 cl1;
    Col16 cl2( CMD_CYAN );
    Col16 cl3( CMD_CYAN | CMD_INT );

    cmd << cl3 << "zParserExtender: "
        << cl2 << "new external defined - "
        << cl3 << funcName
        << cl1 << endl;
  }

  static void AddDynamicExternalFuncToList( const string& funcName, ... ) {
    DynamicExternalFuncList.Insert( funcName );
  }

  template<class T, typename O>
  T* CreateSharedPatchSym( const string& name, const O& default ) {
    if( !CHECK_THIS_ENGINE )
      return Null;

    T* shared = new T();
    shared->SetObjectName( name );
    shared->DontRemove();
    shared->Init();
    shared->SetValue( default );
    return shared;
  }
}