// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  HOOK Hook_oCInfoManager_Unarchive PATCH_IF( &oCInfoManager::Unarchive, &oCInfoManager::Unarchive_Union, !NinjaInjected() );

  void oCInfoManager::Unarchive_Union( zCArchiver& arc ) {
    // Map list for quick access to initialized
    // oInfo objects and easy replacement 
    Map<string, oCInfo**> infoInstances;
    auto list = infoList.next;
    while( list ) {
      infoInstances.Insert( list->data->name, &list->data );
      list = list->next;
    }

    int NumOfEntries;
    arc.ReadInt( "NumOfEntries", NumOfEntries );

    for( int i = 0; i < NumOfEntries; i++ ) {
      oCInfo* entry = new oCInfo();
      entry->Unarchive( arc );

      auto& pair = infoInstances[entry->name];

      // Add inexists or lost dialogs
      // for support compatibility
      if( pair.IsNull() ) {
        infoList.InsertSort( entry );
        continue;
      }

      oCInfo*& info = *pair;
      delete info;
      info = entry;
    }
  }

  HOOK Hook_oCInfo_Info PATCH( &oCInfo::Info, &oCInfo::Info_Union );

  void oCInfo::Info_Union() {
    zCPar_Symbol* symbol1 = parser->GetSymbol( "DIA_CURRENTNAME" );
    if( symbol1 && symbol1->type == zPAR_TYPE_STRING ) {
      static zSTRING currentName;
      currentName = name;
      symbol1->SetValue( name, 0 );
    }

    zCPar_Symbol* symbol2 = parser->GetSymbol( "DIA_CURRENTCOUNTER" );
    if( symbol2 && symbol2->type == zPAR_TYPE_INT )
      symbol2->SetValue( 0, 0 );

    zCPar_Symbol* symbol3 = parser->GetSymbol( "DIA_CURRENTINSTANCE" );
    if( symbol3 && (symbol3->type == zPAR_TYPE_INT || symbol3->type == zPAR_TYPE_INSTANCE) )
      symbol3->SetValue( instance, 0 );

    THISCALL( Hook_oCInfo_Info )();
  }
}