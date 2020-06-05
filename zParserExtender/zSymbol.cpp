// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  zSTRING zCPar_Symbol::GetName() {
    return (Z name).GetWord( "." );
  }

  void zCPar_Symbol::Rename( const zSTRING& newName ) {
    zSTRING oldName = GetName();
    zCPar_Symbol* list = this;
    while( list ) {
      list->name.Replace( oldName, newName );
      list = list->next;
      if( list == this )
        break;
    }
  }
}