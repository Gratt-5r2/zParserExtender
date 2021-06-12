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
}