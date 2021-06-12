// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  // CAST
  int Cast_PointerToInstance() {
    zCParser* par = zCParser::GetParser();
    int address;
    par->GetParameter( address );
    par->SetReturn( (void*)address );
    return 0;
  }

  int Cast_InstanceToPointer() {
    zCParser* par = zCParser::GetParser();
    void* instance = par->GetInstance();
    par->SetReturn( (int)instance );
    return 0;
  }

  int Cast_PointerToNpc() {
    zCParser* par = zCParser::GetParser();
    void* instance = par->GetInstance();
    par->SetReturn( (int)instance );
    return 0;
  }

  int Cast_InstanceIsNpc() {
    zCParser* par = zCParser::GetParser();
    zCObject* instance = (zCObject*)par->GetInstance();
    par->SetReturn( (int)instance->CastTo<oCNpc>() );
    return 0;
  }

  int Cast_InstanceIsItem() {
    zCParser* par = zCParser::GetParser();
    zCObject* instance = (zCObject*)par->GetInstance();
    par->SetReturn( (int)instance->CastTo<oCItem>() );
    return 0;
  }

  int Cast_InstanceIsMob() {
    zCParser* par = zCParser::GetParser();
    zCObject* instance = (zCObject*)par->GetInstance();
    par->SetReturn( (int)instance->CastTo<oCMOB>() );
    return 0;
  }

  int Cast_GetInstanceIndex() {
    zCParser* par = zCParser::GetParser();
    zCObject* instance = (zCObject*)par->GetInstance();
    oCVob* vob = instance->CastTo<oCVob>();
    par->SetReturn( vob ? vob->GetInstance() : -1 );
    return 0;
  }

  int Cast_GetClassID() {
    zCParser* par = zCParser::GetParser();
    zSTRING className;
    par->GetParameter( className );
    int classID = (int)zCClassDef::GetClassDef( className );
    par->SetReturn( classID );
    return 0;
  }

  int Cast_GetVobClassID() {
    zCParser* par = zCParser::GetParser();
    zCObject* instance = (zCObject*)par->GetInstance();
    int classID = (int)instance->_GetClassDef();
    par->SetReturn( classID );
    return 0;
  }

  int Cast_CheckVobClassID() {
    zCParser* par = zCParser::GetParser();
    zCObject* instance = (zCObject*)par->GetInstance();
    int classID;
    par->GetParameter( classID );
    int inheritance = zCObject::CheckInheritance( (zCClassDef*)classID, instance->_GetClassDef() );
    par->SetReturn( inheritance );
    return 0;
  }

  // HLP
  int Hlp_HasFocusVob() {
    zCParser* par = zCParser::GetParser();
    oCNpc* instance = (oCNpc*)par->GetInstance();
    par->SetReturn( (int)instance->GetFocusVob() );
    return 0;
  }

  int Hlp_GetFocusVob() {
    zCParser* par = zCParser::GetParser();
    oCNpc* instance = (oCNpc*)par->GetInstance();
    par->SetReturn( instance->GetFocusVob() );
    return 0;
  }

  int Hlp_GetFocusVobName() {
    static zSTRING name = "";
    zCParser* par = zCParser::GetParser();
    oCNpc* instance = (oCNpc*)par->GetInstance();
    zCVob* vob = instance->GetFocusVob();
    name = vob ? (zSTRING&)vob->GetObjectName() : "";
    par->SetReturn( name );
    return 0;
  }

  int Hlp_GetStringLength() {
    zCParser* par = zCParser::GetParser();
    zSTRING str;
    par->GetParameter( str );
    par->SetReturn( str.Length() );
    return 0;
  }

  int Hlp_IsNAN() {
    zCParser* par = zCParser::GetParser();
    float value;
    par->GetParameter( value );
    par->SetReturn( ISNAN( value ) );
    return 0;
  }

  int Hlp_IsNull() {
    zCParser* par = zCParser::GetParser();
    void* value = par->GetInstance();
    par->SetReturn( value == Null );
    return 0;
  }

  int Hlp_KeyToggled() {
    zCParser* par = zCParser::GetParser();
    int key;
    par->GetParameter( key );
    par->SetReturn( zKeyToggled( key ) );
    return 0;
  }

  int Hlp_KeyPressed() {
    zCParser* par = zCParser::GetParser();
    int key;
    par->GetParameter( key );
    par->SetReturn( zKeyPressed( key ) );
    return 0;
  }

  int Hlp_LogicalKeyToggled() {
    zCParser* par = zCParser::GetParser();
    int logical;
    par->GetParameter( logical );
    par->SetReturn( zinput->GetToggled( logical ) );
    return 0;
  }

  int Hlp_GameOnPause() {
    zCParser* par = zCParser::GetParser();
    par->SetReturn( ogame->IsOnPause() );
    return 0;
  }

  int Hlp_MessageBox() {
    zCParser* par = zCParser::GetParser();
    zSTRING message;
    par->GetParameter( message );
    Message::Box( message, "Parser" );
    return 0;
  }

  int Hlp_PrintConsole() {
    zCParser* par = zCParser::GetParser();
    zSTRING message;
    par->GetParameter( message );
    message
      .Replace( "\\r", "\r" )
      .Replace( "\\n", "\n" )
      .Replace( "\\t", "\t" )
      .Replace( "\\0", "\0" );

    cmd << message << endl;
    return 0;
  }

  int Hlp_GetNull() {
    zCParser* par = zCParser::GetParser();
    par->SetReturn( Null );
    return 0;
  }

  int Hlp_DoEvent() {
    zCParser* par = zCParser::GetParser();
    zSTRING eventName;
    par->GetParameter( eventName );
    par->DoEvent( eventName );
    return 0;
  }

  int Hlp_ReadOptionInt() {
    zCParser* par = zCParser::GetParser();
    zSTRING optName;
    zSTRING section;
    zSTRING parameter;
    int     default;

    par->GetParameter( default   );
    par->GetParameter( parameter );
    par->GetParameter( section   );
    par->GetParameter( optName   );

    int Result = default;
    if( optName == "Gothic"     ) Result = zoptions->ReadInt( section, parameter, default );
    if( optName == "Mod"        ) Result = zgameoptions->ReadInt( section, parameter, default );
    if( optName == "SystemPack" ) Union.GetSysPackOption().Read( Result, A section, A parameter, default );

    par->SetReturn( Result );
    return True;
  }

  int Hlp_ReadOptionFloat() {
    zCParser* par = zCParser::GetParser();
    zSTRING optName;
    zSTRING section;
    zSTRING parameter;
    float   default;
    par->GetParameter( default   );
    par->GetParameter( parameter );
    par->GetParameter( section   );
    par->GetParameter( optName   );

    float Result = default;
    if( optName == "Gothic"     ) Result = zoptions->ReadReal( section, parameter, default );
    if( optName == "Mod"        ) Result = zgameoptions->ReadReal( section, parameter, default );
    if( optName == "SystemPack" ) Union.GetSysPackOption().Read( Result, A section, A parameter, default );

    par->SetReturn( Result );
    return True;
  }

  int Hlp_ReadOptionString() {
    static zSTRING result;
    zCParser* par = zCParser::GetParser();
    zSTRING optName;
    zSTRING section;
    zSTRING parameter;
    zSTRING default;

    par->GetParameter( default   );
    par->GetParameter( parameter );
    par->GetParameter( section   );
    par->GetParameter( optName   );

    string value = default;
    if( optName == "Gothic"     ) value = A zoptions->ReadString( section, parameter, default );
    if( optName == "Mod"        ) value = A zgameoptions->ReadString( section, parameter, default );
    if( optName == "SystemPack" ) Union.GetSysPackOption().Read( value, A section, A parameter, default );

    result = Z value;
    par->SetReturn( result );
    return True;
  }

  int Hlp_OptionIsExists() {
    zCParser* par = zCParser::GetParser();
    zSTRING optName;
    zSTRING section;
    zSTRING parameter;
    int     default;

    par->GetParameter( default   );
    par->GetParameter( parameter );
    par->GetParameter( section   );
    par->GetParameter( optName   );

    int Result = False;
    if( parameter.IsEmpty() ) {
      if( optName == "Gothic"     ) Result = zoptions->SectionExists( section );
      if( optName == "Mod"        ) Result = zgameoptions->SectionExists( section );
      if( optName == "SystemPack" ) Result = Union.GetSysPackOption().IsExists( section );
    }
    else {
      if( optName == "Gothic"     ) Result = zoptions->EntryExists( section, parameter );
      if( optName == "Mod"        ) Result = zgameoptions->EntryExists( section, parameter );
      if( optName == "SystemPack" ) Result = Union.GetSysPackOption().IsExists( section, parameter );
    }

    par->SetReturn( Result );
    return True;
  }

  int Hlp_WriteOptionInt() {
    zCParser* par = zCParser::GetParser();
    zSTRING optName;
    zSTRING section;
    zSTRING parameter;
    int     value;

    par->GetParameter( value );
    par->GetParameter( parameter );
    par->GetParameter( section   );
    par->GetParameter( optName   );

    if( optName == "Gothic"     ) zoptions->WriteInt( section, parameter, value, False );
    if( optName == "Mod"        ) zgameoptions->WriteInt( section, parameter, value, False );
    if( optName == "SystemPack" ) Union.GetSysPackOption().Write( value, section, parameter );

    return True;
  }

  int Hlp_WriteOptionFloat() {
    zCParser* par = zCParser::GetParser();
    zSTRING optName;
    zSTRING section;
    zSTRING parameter;
    float   value;

    par->GetParameter( value     );
    par->GetParameter( parameter );
    par->GetParameter( section   );
    par->GetParameter( optName   );

    if( optName == "Gothic"     ) zoptions->WriteReal( section, parameter, value, False );
    if( optName == "Mod"        ) zgameoptions->WriteReal( section, parameter, value, False );
    if( optName == "SystemPack" ) Union.GetSysPackOption().Write( value, section, parameter );

    return True;
  }

  int Hlp_WriteOptionString() {
    zCParser* par = zCParser::GetParser();
    zSTRING optName;
    zSTRING section;
    zSTRING parameter;
    zSTRING value;

    par->GetParameter( value     );
    par->GetParameter( parameter );
    par->GetParameter( section   );
    par->GetParameter( optName   );

    if( optName == "Gothic"     ) zoptions->WriteString( section, parameter, value, False );
    if( optName == "Mod"        ) zgameoptions->WriteString( section, parameter, value, False );
    if( optName == "SystemPack" ) Union.GetSysPackOption().Write( value, section, parameter );

    return True;
  }

  // WLD
  int Wld_ChangeLevel() {
    zCParser* par = zCParser::GetParser();
    zSTRING worldName;
    zSTRING waypointName;
    par->GetParameter( waypointName );
    par->GetParameter( worldName );
    ogame->ChangeLevel( worldName, waypointName );
    return 0;
  }

  int Wld_FindVob() {
    zCParser* par = zCParser::GetParser();
    zSTRING vobName;
    par->GetParameter( vobName );
    oCWorld* world = ogame->GetGameWorld();
    zCVob* vob = world->SearchVobByName( vobName );
    par->SetReturn( vob );
    return 0;
  }

  int Wld_PlayEffectVob() {
    zCParser* par = zCParser::GetParser();
    zSTRING effect;
    zCVob* pvob;
    int level;
    int damage;
    int damage_type;
    int damage_speed;

    par->GetParameter( damage_speed );
    par->GetParameter( damage_type );
    par->GetParameter( damage );
    par->GetParameter( level );
    pvob = (zCVob*)par->GetInstance();
    par->GetParameter( effect );

    oCVisualFX::CreateAndPlay( effect, pvob, Null, level, (float)damage, damage_type, damage_speed );
    return 0;
  }

  int Wld_PlayEffectAt() {
    zCParser* par = zCParser::GetParser();
    zSTRING effect;
    zVEC3 ppos;
    int level;
    int damage;
    int damage_type;
    int damage_speed;

    par->GetParameter( damage_speed );
    par->GetParameter( damage_type );
    par->GetParameter( damage );
    par->GetParameter( level );
    ppos = *(zVEC3*)par->GetInstance();
    par->GetParameter( effect );

    oCVisualFX::CreateAndPlay( effect, ppos, Null, level, (float)damage, damage_type, damage_speed );
    return 0;
  }

  int Wld_ToggleRain() {
    zCParser* par = zCParser::GetParser();
    oCWorld* world = ogame->GetGameWorld();
    float weight;
    float time;
    par->GetParameter( time );
    par->GetParameter( weight );
    zCSkyControler_Outdoor* controller = world->GetActiveSkyControler()->CastTo<zCSkyControler_Outdoor>();
    if( controller )
      controller->SetRainFXWeight( weight, time );

    return 0;
  }

  int Wld_SetWeatherType() {
#if ENGINE >= Engine_G2
    zCParser* par = zCParser::GetParser();
    oCWorld* world = ogame->GetGameWorld();
    int weather;
    par->GetParameter( weather );
    zCSkyControler_Outdoor* controller = world->GetActiveSkyControler()->CastTo<zCSkyControler_Outdoor>();
    if( controller )
      controller->SetWeatherType( (zTWeather&)weather );
#endif
    return 0;
  }

  int Wld_GetWeatherType() {
    zCParser* par = zCParser::GetParser();
#if ENGINE >= Engine_G2
    oCWorld* world = ogame->GetGameWorld();
    zCSkyControler_Outdoor* controller = world->GetActiveSkyControler()->CastTo<zCSkyControler_Outdoor>();
    par->SetReturn( controller ? controller->GetWeatherType() : -1 );
#else
    par->SetReturn( -1 );
#endif
    return 0;
  }

  // MDL
  int Mdl_GetAnimationIndex() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    zSTRING aniName;
    par->GetParameter( aniName );
    npc = (oCNpc*)par->GetInstance();
    int index = npc->GetModel()->GetAniIDFromAniName( aniName );
    par->SetReturn( index );
    return 0;
  }

  int Mdl_GetAnimationName() {
    static zSTRING result;
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    int index;
    par->GetParameter( index );
    npc = (oCNpc*)par->GetInstance();
    zCModelAni* ani = npc->GetModel()->GetAniFromAniID( index );
    result = ani ? ani->aniName : Z "";
    par->SetReturn( result );
    return 0;
  }

  int Mdl_AnimationIsExists() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    int index;
    par->GetParameter( index );
    npc = (oCNpc*)par->GetInstance();
    zCModelAni* ani = npc->GetModel()->GetAniFromAniID( index );
    par->SetReturn( ani ? True : False );
    return 0;
  }

  int Mdl_AnimationIsActive() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    int index;
    par->GetParameter( index );
    npc = (oCNpc*)par->GetInstance();
    zCModel* model = npc->GetModel();
    zCModelAni* ani = model->GetAniFromAniID( index );
    par->SetReturn( ani ? model->IsAniActive( ani ) : False );
    return 0;
  }

  int Mdl_SetAllAnimationsFPS() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    float fps;
    par->GetParameter( fps );
    npc = (oCNpc*)par->GetInstance();
    zCModel* model = npc->GetModel();

    auto& protos = model->modelProtoList;
    for( int protoID = 0; protoID < protos.GetNum(); protoID++ ) {
      auto& anis = protos[protoID]->protoAnis;
      for( int aniID = 0; aniID < anis.GetNum(); aniID++ ) {
        if( anis[aniID] )
          anis[aniID]->fpsRate = fps;
      }
    } 

    return 0;
  }

  int Mdl_ResetAllAnimationsFPS() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    npc = (oCNpc*)par->GetInstance();
    zCModel* model = npc->GetModel();

    auto& protos = model->modelProtoList;
    for( int protoID = 0; protoID < protos.GetNum(); protoID++ ) {
      auto& anis = protos[protoID]->protoAnis;
      for( int aniID = 0; aniID < anis.GetNum(); aniID++ )
        anis[aniID]->fpsRate = anis[aniID]->fpsRateSource;
    }

    return 0;
  }

  int Mdl_SetAnimationFPS() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    int index;
    float fps;
    par->GetParameter( fps );
    par->GetParameter( index );
    npc = (oCNpc*)par->GetInstance();
    zCModel* model = npc->GetModel();

    zCModelAni* ani = model->GetAniFromAniID( index );
    if( ani )
      ani->fpsRate = fps;

    return 0;
  }

  int Mdl_ResetAnimationFPS() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    int index;
    par->GetParameter( index );
    npc = (oCNpc*)par->GetInstance();
    zCModel* model = npc->GetModel();

    zCModelAni* ani = model->GetAniFromAniID( index );
    if( ani )
      ani->fpsRate = ani->fpsRateSource;

    return 0;
  }

  int Mdl_SetNpcSpeedMultiplier() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    float multiplier;
    par->GetParameter( multiplier );
    npc = (oCNpc*)par->GetInstance();
    zCModel* model = npc->GetModel();
    if( model )
      SetModelSpeed( model, multiplier );

    return 0;
  }

  int Mdl_ResetNpcSpeedMultiplier() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc = (oCNpc*)par->GetInstance();
    zCModel* model = npc->GetModel();
    if( model )
      ResetModelSpeed( model );

    return 0;
  }

  int Mdl_SetVisible() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    int visible;
    par->GetParameter( visible );
    npc = (oCNpc*)par->GetInstance();
    npc->GetModel()->isVisible = visible;
    return 0;
  }

  int Mdl_ApplyOverlayMds_AtFirst() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    zSTRING overlay;
    par->GetParameter( overlay );
    npc = (oCNpc*)par->GetInstance();
    
    if( npc ) {
      npc->ApplyOverlay_AtFirst( overlay );

      if( npc->GetAnictrl() )
        npc->GetAnictrl()->InitAnimations();
    }

    return 0;
  }

  int oCNpc::ApplyOverlay_AtFirst( const zSTRING& ov ) {
    zSTRING overlay = ov;
    overlay.Upper();

    int Ok = ApplyOverlay( overlay );
    int index = activeOverlays.Search( overlay );
    if( index != Invalid ) {
      zSTRING overlay_tmp = activeOverlays[0];
      activeOverlays[0] = activeOverlays[index];
      activeOverlays[index] = overlay_tmp;
    }

    return Ok;
  }

  // NPC
  int Npc_SetAsHero() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    npc = (oCNpc*)par->GetInstance();
    npc->SetAsPlayer();
    return 0;
  }

#if ENGINE >= Engine_G2
#define INV_MODE INV_MODE_DEFAULT
#else
#define INV_MODE
#endif

  int Npc_OpenInventory() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    npc = (oCNpc*)par->GetInstance();
    npc->OpenInventory( INV_MODE );
    return 0;
  }

  int Npc_OpenInventorySteal() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    npc = (oCNpc*)par->GetInstance();
    npc->OpenSteal();
    return 0;
  }

  int Npc_OpenInventoryTrade() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    npc = (oCNpc*)par->GetInstance();
    npc->OpenTradeOffer( player );
    return 0;
  }

  int Npc_GetLeftHandItem() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    npc = (oCNpc*)par->GetInstance();
    oCItem* item = npc->GetLeftHand()->CastTo<oCItem>();
    par->SetReturn( item );
    return 0;
  }

  int Npc_GetRightHandItem() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    npc = (oCNpc*)par->GetInstance();
    oCItem* item = npc->GetRightHand()->CastTo<oCItem>();
    par->SetReturn( item );
    return 0;
  }

  int Npc_GetSlotItem() {
    zCParser* par = zCParser::GetParser();
    oCNpc* npc;
    zSTRING slotName;
    par->GetParameter( slotName );
    npc = (oCNpc*)par->GetInstance();
    oCItem* item = npc->GetSlotItem( slotName.Upper() )->CastTo<oCItem>();
    par->SetReturn( item );
    return 0;
  }

  int Mob_Destroy() {
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    mob = (oCMOB*)par->GetInstance();
    mob->Destroy();
    return 0;
  }

  // MOB
  int Mob_RemoveItem() {
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    int index;
    par->GetParameter( index );
    mob = (oCMOB*)par->GetInstance();
    
    oCMobContainer* mob_container = mob->CastTo<oCMobContainer>();
    if( mob_container ) {
      zCListSort<oCItem>* list = mob_container->items->contents->next;
      while( list ) {
        oCItem* item = list->GetData();
        if( item->instanz == index ) {
          mob_container->Remove( item );
          break;
        }

        list = list->next;
      }
    }

    return 0;
  }

  int Mob_RemoveItems() {
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    int index;
    int count;
    par->GetParameter( count );
    par->GetParameter( index );
    mob = (oCMOB*)par->GetInstance();
    
    oCMobContainer* mob_container = mob->CastTo<oCMobContainer>();
    if( mob_container ) {
      zCListSort<oCItem>* list = mob_container->items->contents->next;
      while( list ) {
        oCItem* item = list->GetData();
        if( item->instanz == index ) {
          if( item->amount > count )
            item->amount -= count;
          else
            mob_container->Remove( item );
          break;
        }

        list = list->next;
      }
    }

    return 0;
  }

  int Mob_InsertItem() {
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    int index;
    par->GetParameter( index );
    mob = (oCMOB*)par->GetInstance();

    oCMobContainer* mob_container = mob->CastTo<oCMobContainer>();
    if( mob_container ) {
      oCItem* item = new oCItem();
      item->InitByScript( index, 1 );
      mob_container->Insert( item );
    }

    return 0;
  }
  
  int Mob_InsertItems() {
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    int index;
    int count;
    par->GetParameter( count );
    par->GetParameter( index );
    mob = (oCMOB*)par->GetInstance();

    oCMobContainer* mob_container = mob->CastTo<oCMobContainer>();
    if( mob_container ) {
      oCItem* item = new oCItem();
      item->InitByScript( index, count );
      mob_container->Insert( item );
    }

    return 0;
  }

  int Mob_GetLockCombination() {
    static zSTRING result;
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    mob = (oCMOB*)par->GetInstance();

    oCMobLockable* mob_lockable = mob->CastTo<oCMobContainer>();
    if( mob_lockable ) {
      result = mob_lockable->pickLockStr;
      par->SetReturn( result );
    }

    return 0;
  }

  int Mob_SetLockCombination() {
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    zSTRING combination;
    par->GetParameter( combination );
    mob = (oCMOB*)par->GetInstance();

    oCMobLockable* mob_lockable = mob->CastTo<oCMobContainer>();
    if( mob_lockable )
      mob_lockable->pickLockStr = combination;

    return 0;
  }

  int Mob_IsLocked() {
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    mob = (oCMOB*)par->GetInstance();

    oCMobLockable* mob_lockable = mob->CastTo<oCMobContainer>();
    if( mob_lockable )
      par->SetReturn( mob_lockable->locked );

    return 0;
  }

  int Mob_SetLocked() {
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    int locked;
    par->GetParameter( locked );
    mob = (oCMOB*)par->GetInstance();

    oCMobLockable* mob_lockable = mob->CastTo<oCMobContainer>();
    if( mob_lockable )
      mob_lockable->locked = locked;

    return 0;
  }

  int Mob_GetKeyInstance() {
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    mob = (oCMOB*)par->GetInstance();

    oCMobLockable* mob_lockable = mob->CastTo<oCMobContainer>();
    if( mob_lockable ) {
      int index = par->GetIndex( mob_lockable->keyInstance );
      par->SetReturn( index );
    }

    return 0;
  }

  int Mob_SetKeyInstance() {
    zCParser* par = zCParser::GetParser();
    oCMOB* mob;
    int index;
    par->GetParameter( index );
    mob = (oCMOB*)par->GetInstance();

    oCMobLockable* mob_lockable = mob->CastTo<oCMobContainer>();
    if( mob_lockable ) {
      zCPar_Symbol* sym = par->GetSymbol( index );
      if( sym )
        mob_lockable->keyInstance = sym->name;
    }

    return 0;
  }

  // AI
  int AI_CallScript() {
    zSTRING scriptName;
    oCNpc* self;
    oCNpc* other;

    other = (oCNpc*)parser->GetInstance();
    self = (oCNpc*)parser->GetInstance();
    parser->GetParameter( scriptName );

    int function = parser->GetIndex( scriptName );
    if( function != Invalid )
      self->GetEM( False )->OnMessage( new oCMsgManipulate( oCMsgManipulate::EV_CALLSCRIPT, other, function ), other );

    return True;
  }

  HOOK Hook_oCNpc_EV_CallScript AS( &oCNpc::EV_CallScript, &oCNpc::EV_CallScript_Union );

  int oCNpc::EV_CallScript_Union( oCMsgManipulate* csg ) {
    if( csg->targetVob != Null ) {
      parser->SetInstance( "SELF", this );
      parser->SetInstance( "OTHER", csg->targetVob );
      parser->CallFunc( csg->targetState );
      return True;
    }

    return THISCALL( Hook_oCNpc_EV_CallScript )(csg);
  }

  int AI_StartTriggerScript() {
    zCParser* par = zCParser::GetParser();
    zSTRING funcName;
    int delay;
    par->GetParameter( delay );
    par->GetParameter( funcName );

    zTTriggerScript* trigger = zTTriggerScript::Create( funcName, delay );
    par->SetReturn( trigger );
    return True;
  }

  int AI_StartTriggerScriptEx() {
    zCParser* par = zCParser::GetParser();
    zSTRING funcName;
    int delay;

    oCNpc* Victim = (oCNpc*)par->GetInstance();
    oCNpc* Other  = (oCNpc*)par->GetInstance();
    oCNpc* Self   = (oCNpc*)par->GetInstance();

    par->GetParameter( delay );
    par->GetParameter( funcName );

    zTTriggerScript* trigger = zTTriggerScript::Create( funcName, delay );
    trigger->Self            = Self;
    trigger->Other           = Other;
    trigger->Victim          = Victim;

    par->SetReturn( trigger );
    return True;
  }

  int AI_GetFirstTrigger() {
    zCParser* par = zCParser::GetParser();
    par->SetReturn( zTTriggerScript::GetFirstTrigger() );
    return True;
  }

  int AI_GetTriggerByID() {
    zCParser* par = zCParser::GetParser();

    int ID;
    par->GetParameter( ID );
    zTTriggerScript* trigger = zTTriggerScript::TriggerScripts[ID];
    par->SetReturn( trigger );
    return True;
  }

  int AI_GetTriggersNum() {
    zCParser* par = zCParser::GetParser();
    par->SetReturn( (int)zTTriggerScript::TriggerScripts.GetNum() );
    return True;
  }

  int AI_GetTriggerNPC() {
    zCParser* par = zCParser::GetParser();

    zTTriggerScript* trigger;
    int ID;

    par->GetParameter( ID );
    trigger = (zTTriggerScript*)par->GetInstance();

    oCNpc* npc =
      ID == 0 ? trigger->Self   :
      ID == 1 ? trigger->Other  :
      ID == 2 ? trigger->Victim : Null;

    par->SetReturn( npc );
    return True;
  }

  int AI_GetTriggerFunc() {
    zCParser* par = zCParser::GetParser();
    zTTriggerScript* trigger = (zTTriggerScript*)par->GetInstance();
    par->SetReturn( trigger->Function );
    return True;
  }

  int AI_GetTriggerFuncName() {
    static zSTRING result;
    zCParser* par = zCParser::GetParser();
    zTTriggerScript* trigger = (zTTriggerScript*)par->GetInstance();
    result = trigger->FunctionName;
    par->SetReturn( result );
    return True;
  }

  int AI_GetNextTriggerByFunc() {
    zCParser* par = zCParser::GetParser();

    Array<zTTriggerScript*>& triggers = zTTriggerScript::TriggerScripts;
    zTTriggerScript* trigger;
    int func;

    par->GetParameter( func );
    trigger = (zTTriggerScript*)par->GetInstance();

    uint startIndex = Invalid;
    if( trigger )
      startIndex = triggers.SearchEqualSorted( trigger );

    for( uint i = startIndex + 1; i < triggers.GetNum(); i++ ) {
      if( triggers[i]->Function == func ) {
        par->SetReturn( triggers[i] );
        return True;
      }
    }

    par->SetReturn( Null );
    return True;
  }

  int AI_GetNextTriggerByFuncName() {
    zCParser* par = zCParser::GetParser();

    Array<zTTriggerScript*>& triggers = zTTriggerScript::TriggerScripts;
    zTTriggerScript* trigger;
    zSTRING funcName;

    par->GetParameter( funcName );
    trigger = (zTTriggerScript*)par->GetInstance();

    uint startIndex = Invalid;
    if( trigger )
      startIndex = triggers.SearchEqualSorted( trigger );

    for( uint i = startIndex + 1; i < triggers.GetNum(); i++ ) {
      if( triggers[i]->FunctionName == funcName ) {
        par->SetReturn( triggers[i] );
        return True;
      }
    }

    par->SetReturn( Null );
    return True;
  }

  int AI_GetNextTriggerBySelf() {
    zCParser* par = zCParser::GetParser();

    Array<zTTriggerScript*>& triggers = zTTriggerScript::TriggerScripts;
    zTTriggerScript* trigger;
    oCNpc* npc;

    npc     = (oCNpc*)par->GetInstance();
    trigger = (zTTriggerScript*)par->GetInstance();

    uint startIndex = Invalid;
    if( trigger )
      startIndex = triggers.SearchEqualSorted( trigger );

    for( uint i = startIndex + 1; i < triggers.GetNum(); i++ ) {
      if( triggers[i]->Self == npc ) {
        par->SetReturn( triggers[i] );
        return True;
      }
    }

    par->SetReturn( Null );
    return True;
  }

  int AI_GetNextTriggerByOther() {
    zCParser* par = zCParser::GetParser();

    Array<zTTriggerScript*>& triggers = zTTriggerScript::TriggerScripts;
    zTTriggerScript* trigger;
    oCNpc* npc;

    npc     = (oCNpc*)par->GetInstance();
    trigger = (zTTriggerScript*)par->GetInstance();

    uint startIndex = Invalid;
    if( trigger )
      startIndex = triggers.SearchEqualSorted( trigger );

    for( uint i = startIndex + 1; i < triggers.GetNum(); i++ ) {
      if( triggers[i]->Other == npc ) {
        par->SetReturn( triggers[i] );
        return True;
      }
    }

    par->SetReturn( Null );
    return True;
  }
 
  int AI_GetNextTriggerByVictim() {
    zCParser* par = zCParser::GetParser();

    Array<zTTriggerScript*>& triggers = zTTriggerScript::TriggerScripts;
    zTTriggerScript* trigger;
    oCNpc* npc;

    npc     = (oCNpc*)par->GetInstance();
    trigger = (zTTriggerScript*)par->GetInstance();

    uint startIndex = Invalid;
    if( trigger )
      startIndex = triggers.SearchEqualSorted( trigger );

    for( uint i = startIndex + 1; i < triggers.GetNum(); i++ ) {
      if( triggers[i]->Victim == npc ) {
        par->SetReturn( triggers[i] );
        return True;
      }
    }

    par->SetReturn( Null );
    return True;
  }
  
  int AI_GetNextTriggerByNPCs() {
    zCParser* par = zCParser::GetParser();

    Array<zTTriggerScript*>& triggers = zTTriggerScript::TriggerScripts;
    zTTriggerScript* trigger;
    oCNpc* self;
    oCNpc* other;
    oCNpc* victim;

    victim  = (oCNpc*)par->GetInstance();
    other   = (oCNpc*)par->GetInstance();
    self    = (oCNpc*)par->GetInstance();
    trigger = (zTTriggerScript*)par->GetInstance();

    uint startIndex = Invalid;
    if( trigger )
      startIndex = triggers.SearchEqualSorted( trigger );


    for( uint i = startIndex + 1; i < triggers.GetNum(); i++ ) {
      if( triggers[i]->Self   == self &&
          triggers[i]->Other  == other &&
          triggers[i]->Victim == victim ) {
        par->SetReturn( triggers[i] );
        return True;
      }
    }

    par->SetReturn( Null );
    return True;
  }

  // STR
  int Str_Format() {
    static zSTRING format;
    zSTRING parameter_s;
    int parameter_i;
    float parameter_f;

    parser->GetParameter( format );

    for( uint i = format.Length() - 1; i > 0; i-- ) {
      if( format[i - 1] == '%' ) {
        switch( format[i] ) {
          case 's':
            parser->GetParameter( parameter_s );
            format.Cut( i - 1, 2 );
            format.Put( parameter_s, i - 1 );
            break;
          case 'i':
            parser->GetParameter( parameter_i );
            parameter_s = parameter_i;
            format.Cut( i - 1, 2 );
            format.Put( parameter_s, i - 1 );
            break;
          case 'x':
            parser->GetParameter( parameter_i );
            parameter_s = Z AHEX32( parameter_i );
            format.Cut( i - 1, 2 );
            format.Put( parameter_s, i - 1 );
            break;
          case 'f':
            parser->GetParameter( parameter_f );
            parameter_s = parameter_f;
            format.Cut( i - 1, 2 );
            format.Put( parameter_s, i - 1 );
            break;
          case 'b':
            parser->GetParameter( parameter_i );
            parameter_s = parameter_i ? "True" : "False";
            format.Cut( i - 1, 2 );
            format.Put( parameter_s, i - 1 );
            break;
          case 'p':
            zCObject* object = (zCObject*)parser->GetInstance();
            parameter_s = Z AHEX32( object );
            format.Cut( i - 1, 2 );
            format.Put( parameter_s, i - 1 );
            break;
        }
      }
    }

    parser->SetReturn( format );
    return True;
  }

  astring UTF8_To_ANSI( byte* byteArray, uint strLength, uint codePage ) {
    UTF8StringConverter utf8_converter;
    Array<byte>& utf8 = *(Array<byte>*)&utf8_converter;
    ArrayLocator<byte>& allocator = const_cast<ArrayLocator<byte>&>(utf8.GetArrayLocator());
    allocator.PrepareToReserveArray( strLength + 1 );
    allocator.ActivateAllocatedMemory();
    memcpy( &utf8[0], byteArray, strLength + 1 );
    byteArray[strLength] = 0;
    astring ansi;
    utf8_converter.ToAnsi( ansi, codePage );
    return ansi;
  }

  wstring UTF8_To_UNICODE( byte* byteArray, uint strLength, uint codePage ) {
    UTF8StringConverter utf8_converter;
    Array<byte>& utf8 = *(Array<byte>*)&utf8_converter;
    ArrayLocator<byte>& allocator = const_cast<ArrayLocator<byte>&>(utf8.GetArrayLocator());
    allocator.PrepareToReserveArray( strLength + 1 );
    allocator.ActivateAllocatedMemory();
    memcpy( &utf8[0], byteArray, strLength + 1 );
    byteArray[strLength] = 0;
    wstring unicode;
    utf8_converter.ToUnicode( unicode );
    return unicode;
  }

  int Str_UTF8_to_ANSI() {
    static zSTRING result;
    zCParser* par = zCParser::GetParser();
    zSTRING utf8_data;
    int codePage;
    par->GetParameter( codePage );
    par->GetParameter( utf8_data );

    if( codePage == Invalid )
      codePage = ANSI_CODEPAGE_DEFAULT;

    string ansi = UTF8_To_ANSI( (byte*)utf8_data.ToChar(), utf8_data.Length(), codePage );

    result = Z ansi;
    par->SetReturn( result );

    return 0;
  }

  int Str_GetCurrentCP() {
    zCParser* par = zCParser::GetParser();
    par->SetReturn( (int)ANSI_CODEPAGE_DEFAULT );
    return 0;
  }

  int Str_GetLocalizedString() {
    static zSTRING result;
    zCParser* par = zCParser::GetParser();
    static uint cp[4] = {
      CP_RUSSIAN,
      CP_ENGLISH,
      CP_GERMAN,
      CP_POLISH
    };

    zSTRING src[4];
    par->GetParameter( src[3] );
    par->GetParameter( src[2] );
    par->GetParameter( src[1] );
    par->GetParameter( src[0] );

    uint lang = Union.GetSystemLanguage() - 1;
    if( lang >= 4 )
      lang = 2;

    string ansi = UTF8_To_ANSI( (byte*)src[lang].ToChar(), src[lang].Length(), cp[lang] );
    result = Z ansi;
    par->SetReturn( result );

    return 0;
  }



extern void MessageExternalDefined( const string & funcName );

#define EXTERNAL_READ_BEGIN( f ) else if( funcName == #f ) { MessageExternalDefined( #f ); parser->DefineExternal( #f, f,
#define EXTERNAL_READ_END , 0); return true; }
  bool PostLoadExternal( const string& funcName ) {
    zCParser* parser = zCParser::GetParser();
    if( funcName.IsEmpty() )
      return false;

    // CAST
    EXTERNAL_READ_BEGIN( Cast_PointerToInstance ) zPAR_TYPE_INSTANCE, zPAR_TYPE_INT                         EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Cast_PointerToInstance ) zPAR_TYPE_INSTANCE, zPAR_TYPE_INT                         EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Cast_PointerToInstance ) zPAR_TYPE_INSTANCE, zPAR_TYPE_INT                         EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Cast_PointerToInstance ) zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE                    EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Cast_PointerToInstance ) zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE                    EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Cast_PointerToInstance ) zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE                    EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Cast_PointerToInstance ) zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE                    EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Cast_PointerToInstance ) zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE                    EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Cast_PointerToInstance ) zPAR_TYPE_INT,      zPAR_TYPE_STRING                      EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Cast_PointerToInstance ) zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE                    EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Cast_PointerToInstance ) zPAR_TYPE_INT,      zPAR_TYPE_INT,     zPAR_TYPE_INSTANCE EXTERNAL_READ_END

    // HLP
    EXTERNAL_READ_BEGIN( Hlp_HasFocusVob       )    zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE                                                      EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Hlp_GetFocusVob       )    zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE                                                      EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Hlp_GetFocusVobName   )    zPAR_TYPE_STRING,   zPAR_TYPE_INSTANCE                                                      EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Hlp_GetStringLength   )    zPAR_TYPE_INT,      zPAR_TYPE_STRING                                                        EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Hlp_IsNAN             )    zPAR_TYPE_INT,      zPAR_TYPE_FLOAT                                                         EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Hlp_IsNull            )    zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE                                                      EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Hlp_KeyToggled        )    zPAR_TYPE_INT,      zPAR_TYPE_INT                                                           EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Hlp_KeyPressed        )    zPAR_TYPE_INT,      zPAR_TYPE_INT                                                           EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Hlp_LogicalKeyToggled )    zPAR_TYPE_INT,      zPAR_TYPE_INT                                                           EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Hlp_GameOnPause       )    zPAR_TYPE_INT                                                                               EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Hlp_MessageBox        )    zPAR_TYPE_VOID,     zPAR_TYPE_STRING                                                        EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Hlp_PrintConsole      )    zPAR_TYPE_VOID,     zPAR_TYPE_STRING                                                        EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Hlp_GetNull           )    zPAR_TYPE_INSTANCE                                                                          EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Hlp_DoEvent           )    zPAR_TYPE_VOID,     zPAR_TYPE_STRING                                                        EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Hlp_ReadOptionInt     )    zPAR_TYPE_INT,      zPAR_TYPE_STRING,  zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_INT    EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Hlp_ReadOptionFloat   )    zPAR_TYPE_FLOAT,    zPAR_TYPE_STRING,  zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_FLOAT  EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Hlp_ReadOptionString  )    zPAR_TYPE_STRING,   zPAR_TYPE_STRING,  zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Hlp_OptionIsExists    )    zPAR_TYPE_INT,      zPAR_TYPE_STRING,  zPAR_TYPE_STRING                                     EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Hlp_WriteOptionInt    )    zPAR_TYPE_VOID,     zPAR_TYPE_STRING,  zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_INT    EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Hlp_WriteOptionFloat  )    zPAR_TYPE_VOID,     zPAR_TYPE_STRING,  zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_FLOAT  EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Hlp_WriteOptionString )    zPAR_TYPE_VOID,     zPAR_TYPE_STRING,  zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING EXTERNAL_READ_END

    // WLD
    EXTERNAL_READ_BEGIN( Wld_ChangeLevel    ) zPAR_TYPE_VOID,     zPAR_TYPE_STRING, zPAR_TYPE_STRING                                                               EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Wld_FindVob        ) zPAR_TYPE_INSTANCE, zPAR_TYPE_STRING                                                                                 EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Wld_PlayEffectVob  ) zPAR_TYPE_VOID,     zPAR_TYPE_STRING, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Wld_PlayEffectAt   ) zPAR_TYPE_VOID,     zPAR_TYPE_STRING, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Wld_ToggleRain     ) zPAR_TYPE_VOID,     zPAR_TYPE_FLOAT,  zPAR_TYPE_FLOAT                                                                EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Wld_SetWeatherType ) zPAR_TYPE_VOID,     zPAR_TYPE_INT                                                                                    EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Wld_GetWeatherType ) zPAR_TYPE_INT                                                                                                        EXTERNAL_READ_END
    
    // MDL
    EXTERNAL_READ_BEGIN( Mdl_GetAnimationIndex       ) zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, zPAR_TYPE_STRING                 EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Mdl_GetAnimationName        ) zPAR_TYPE_STRING,   zPAR_TYPE_INSTANCE, zPAR_TYPE_INT                    EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Mdl_AnimationIsExists       ) zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, zPAR_TYPE_INT                    EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Mdl_AnimationIsActive       ) zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, zPAR_TYPE_INT                    EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Mdl_SetAllAnimationsFPS     ) zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_FLOAT                  EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Mdl_ResetAllAnimationsFPS   ) zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE                                   EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Mdl_SetAnimationFPS         ) zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,   zPAR_TYPE_FLOAT EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Mdl_ResetAnimationFPS       ) zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT                    EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Mdl_SetVisible              ) zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT                    EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Mdl_ApplyOverlayMds_AtFirst ) zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_STRING                 EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Mdl_SetNpcSpeedMultiplier   ) zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_FLOAT                  EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Mdl_ResetNpcSpeedMultiplier ) zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE                                   EXTERNAL_READ_END

    // NPC
    EXTERNAL_READ_BEGIN( Npc_SetAsHero          ) zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE                   EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Npc_OpenInventory      ) zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE                   EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Npc_OpenInventorySteal ) zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE                   EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Npc_OpenInventoryTrade ) zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE                   EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Npc_GetLeftHandItem    ) zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE                   EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Npc_GetRightHandItem   ) zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE                   EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Npc_GetSlotItem        ) zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_STRING EXTERNAL_READ_END
    
    // MOB
    EXTERNAL_READ_BEGIN( Mob_Destroy            ) zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE                                 EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Mob_RemoveItem         ) zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT                  EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Mob_RemoveItems        ) zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,   zPAR_TYPE_INT EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Mob_InsertItem         ) zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT                  EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Mob_InsertItems        ) zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,   zPAR_TYPE_INT EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Mob_GetLockCombination ) zPAR_TYPE_STRING,   zPAR_TYPE_INSTANCE                                 EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Mob_SetLockCombination ) zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_STRING               EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Mob_IsLocked           ) zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE                                 EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Mob_SetLocked          ) zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT                  EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Mob_GetKeyInstance     ) zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE                                 EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Mob_SetKeyInstance     ) zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT                  EXTERNAL_READ_END
    
    // AI
    EXTERNAL_READ_BEGIN( AI_CallScript           ) zPAR_TYPE_VOID,     zPAR_TYPE_STRING,   zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE                                         EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( AI_StartTriggerScript   ) zPAR_TYPE_INSTANCE, zPAR_TYPE_STRING,   zPAR_TYPE_INT                                                                  EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( AI_StartTriggerScriptEx ) zPAR_TYPE_INSTANCE, zPAR_TYPE_STRING,   zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( AI_GetTriggerByID       ) zPAR_TYPE_INSTANCE, zPAR_TYPE_INT                                                                                      EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( AI_GetTriggersNum       ) zPAR_TYPE_INT                                                                                                          EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( AI_GetTriggerNPC        ) zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT                                                                  EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( AI_GetTriggerFunc       ) zPAR_TYPE_FUNC,     zPAR_TYPE_INSTANCE                                                                                 EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( AI_GetTriggerFuncName   ) zPAR_TYPE_STRING,   zPAR_TYPE_INSTANCE                                                                                 EXTERNAL_READ_END
    
    EXTERNAL_READ_BEGIN( AI_GetNextTriggerByFunc     ) zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_FUNC                                             EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( AI_GetNextTriggerByFuncName ) zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_STRING                                           EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( AI_GetNextTriggerBySelf     ) zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE                                         EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( AI_GetNextTriggerByOther    ) zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE                                         EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( AI_GetNextTriggerByVictim   ) zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE                                         EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( AI_GetNextTriggerByNPCs     ) zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE EXTERNAL_READ_END
    
    // STR
    EXTERNAL_READ_BEGIN( Str_Format             ) zPAR_TYPE_STRING                                                                         EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Str_GetLocalizedString ) zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Str_UTF8_to_ANSI       ) zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_INT                                        EXTERNAL_READ_END
    EXTERNAL_READ_BEGIN( Str_GetCurrentCP       ) zPAR_TYPE_INT                                                                            EXTERNAL_READ_END
    else if( PostLoadExternal_Vobs( funcName ) )
      return true;

    return false;
  }
#undef EXTERNAL_READ_BEGIN                        
#undef EXTERNAL_READ_END                                   
                                             

  void DefineExternals() {
#if 0
    // CAST
    parser->DefineExternal( "Cast_PointerToInstance",    Cast_PointerToInstance,    zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Cast_PointerToNpc",         Cast_PointerToInstance,    zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Cast_PointerToItem",        Cast_PointerToInstance,    zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Cast_InstanceToPointer",    Cast_InstanceToPointer,    zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Cast_InstanceIsNpc",        Cast_InstanceIsNpc,        zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Cast_InstanceIsItem",       Cast_InstanceIsItem,       zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Cast_InstanceIsMob",        Cast_InstanceIsMob,        zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Cast_GetInstanceIndex",     Cast_GetInstanceIndex,     zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, 0 );
    
    parser->DefineExternal( "Cast_GetClassID",           Cast_GetClassID,           zPAR_TYPE_INT,      zPAR_TYPE_STRING,   0 );
    parser->DefineExternal( "Cast_GetVobClassID",        Cast_GetVobClassID,        zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Cast_CheckVobClassID",      Cast_CheckVobClassID,      zPAR_TYPE_INT,      zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, 0 );

    // HLP
    parser->DefineExternal( "Hlp_HasFocusVob",           Hlp_HasFocusVob,           zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Hlp_GetFocusVob",           Hlp_GetFocusVob,           zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Hlp_GetFocusVobName",       Hlp_GetFocusVobName,       zPAR_TYPE_STRING,   zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Hlp_GetStringLength",       Hlp_GetStringLength,       zPAR_TYPE_INT,      zPAR_TYPE_STRING,   0 );
    parser->DefineExternal( "Hlp_IsNAN",                 Hlp_IsNAN,                 zPAR_TYPE_INT,      zPAR_TYPE_FLOAT,    0 );
    parser->DefineExternal( "Hlp_IsNull",                Hlp_IsNull,                zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Hlp_KeyToggled",            Hlp_KeyToggled,            zPAR_TYPE_INT,      zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Hlp_KeyPressed",            Hlp_KeyPressed,            zPAR_TYPE_INT,      zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Hlp_LogicalKeyToggled",     Hlp_LogicalKeyToggled,     zPAR_TYPE_INT,      zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Hlp_GameOnPause",           Hlp_GameOnPause,           zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Hlp_MessageBox",            Hlp_MessageBox,            zPAR_TYPE_VOID,     zPAR_TYPE_STRING,   0 );
    parser->DefineExternal( "Hlp_PrintConsole",          Hlp_PrintConsole,          zPAR_TYPE_VOID,     zPAR_TYPE_STRING,   0 );
    parser->DefineExternal( "Hlp_GetNull",               Hlp_GetNull,               zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Hlp_DoEvent",               Hlp_DoEvent,               zPAR_TYPE_VOID,     zPAR_TYPE_STRING,   0 );
    
    parser->DefineExternal( "Hlp_ReadOptionInt",         Hlp_ReadOptionInt,         zPAR_TYPE_INT,      zPAR_TYPE_STRING,   zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_INT,    0 );
    parser->DefineExternal( "Hlp_ReadOptionFloat",       Hlp_ReadOptionFloat,       zPAR_TYPE_FLOAT,    zPAR_TYPE_STRING,   zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_FLOAT,  0 );
    parser->DefineExternal( "Hlp_ReadOptionString",      Hlp_ReadOptionString,      zPAR_TYPE_STRING,   zPAR_TYPE_STRING,   zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, 0 );
    parser->DefineExternal( "Hlp_OptionIsExists",        Hlp_OptionIsExists,        zPAR_TYPE_INT,      zPAR_TYPE_STRING,   zPAR_TYPE_STRING, 0 );
    parser->DefineExternal( "Hlp_WriteOptionInt",        Hlp_WriteOptionInt,        zPAR_TYPE_VOID,     zPAR_TYPE_STRING,   zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_INT,    0 );
    parser->DefineExternal( "Hlp_WriteOptionFloat",      Hlp_WriteOptionFloat,      zPAR_TYPE_VOID,     zPAR_TYPE_STRING,   zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_FLOAT,  0 );
    parser->DefineExternal( "Hlp_WriteOptionString",     Hlp_WriteOptionString,     zPAR_TYPE_VOID,     zPAR_TYPE_STRING,   zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, 0 );

    // WLD
    parser->DefineExternal( "Wld_ChangeLevel",           Wld_ChangeLevel,           zPAR_TYPE_VOID,     zPAR_TYPE_STRING,   zPAR_TYPE_STRING,   0 );
    parser->DefineExternal( "Wld_FindVob",               Wld_FindVob,               zPAR_TYPE_INSTANCE, zPAR_TYPE_STRING,   0 );
    parser->DefineExternal( "Wld_PlayEffectVob",         Wld_PlayEffectVob,         zPAR_TYPE_VOID,     zPAR_TYPE_STRING,   zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, 0 );
    parser->DefineExternal( "Wld_PlayEffectAt",          Wld_PlayEffectAt,          zPAR_TYPE_VOID,     zPAR_TYPE_STRING,   zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, 0 );
    parser->DefineExternal( "Wld_ToggleRain",            Wld_ToggleRain,            zPAR_TYPE_VOID,     zPAR_TYPE_FLOAT,    zPAR_TYPE_FLOAT,    0 );
    parser->DefineExternal( "Wld_SetWeatherType",        Wld_SetWeatherType,        zPAR_TYPE_VOID,     zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Wld_GetWeatherType",        Wld_GetWeatherType,        zPAR_TYPE_INT,      0 );
    
    // MDL
    parser->DefineExternal( "Mdl_GetAnimationIndex",       Mdl_GetAnimationIndex,       zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, zPAR_TYPE_STRING,   0 );
    parser->DefineExternal( "Mdl_GetAnimationName",        Mdl_GetAnimationName,        zPAR_TYPE_STRING,   zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Mdl_AnimationIsExists",       Mdl_AnimationIsExists,       zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Mdl_AnimationIsActive",       Mdl_AnimationIsActive,       zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Mdl_SetAllAnimationsFPS",     Mdl_SetAllAnimationsFPS,     zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_FLOAT,    0 );
    parser->DefineExternal( "Mdl_ResetAllAnimationsFPS",   Mdl_ResetAllAnimationsFPS,   zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Mdl_SetAnimationFPS",         Mdl_SetAnimationFPS,         zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      zPAR_TYPE_FLOAT,    0 );
    parser->DefineExternal( "Mdl_ResetAnimationFPS",       Mdl_ResetAnimationFPS,       zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Mdl_SetVisible",              Mdl_SetVisible,              zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "Mdl_ApplyOverlayMds_AtFirst", Mdl_ApplyOverlayMds_AtFirst, zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_STRING,   0 );
    parser->DefineExternal( "Mdl_SetNpcSpeedMultiplier",   Mdl_SetNpcSpeedMultiplier,   zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_FLOAT,    0 );
    parser->DefineExternal( "Mdl_ResetNpcSpeedMultiplier", Mdl_ResetNpcSpeedMultiplier, zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, 0 );

    // NPC
    parser->DefineExternal( "Npc_SetAsHero",             Npc_SetAsHero,             zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Npc_OpenInventory",         Npc_OpenInventory,         zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Npc_OpenInventorySteal",    Npc_OpenInventorySteal,    zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Npc_OpenInventoryTrade",    Npc_OpenInventoryTrade,    zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Npc_GetLeftHandItem",       Npc_GetLeftHandItem,       zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Npc_GetRightHandItem",      Npc_GetRightHandItem,      zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Npc_GetSlotItem",           Npc_GetSlotItem,           zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_STRING,   0 );
    
    // MOB
    parser->DefineExternal( "Mob_Destroy",               Mob_Destroy,               zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Mob_RemoveItem",            Mob_RemoveItem,            zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,    0 );
    parser->DefineExternal( "Mob_RemoveItems",           Mob_RemoveItems,           zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,    zPAR_TYPE_INT, 0 );
    parser->DefineExternal( "Mob_InsertItem",            Mob_InsertItem,            zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,    0 );
    parser->DefineExternal( "Mob_InsertItems",           Mob_InsertItems,           zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,    zPAR_TYPE_INT, 0 );
    parser->DefineExternal( "Mob_GetLockCombination",    Mob_GetLockCombination,    zPAR_TYPE_STRING,   zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Mob_SetLockCombination",    Mob_SetLockCombination,    zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_STRING, 0 );
    parser->DefineExternal( "Mob_IsLocked",              Mob_IsLocked,              zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Mob_SetLocked",             Mob_SetLocked,             zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,    0 );
    parser->DefineExternal( "Mob_GetKeyInstance",        Mob_GetKeyInstance,        zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "Mob_SetKeyInstance",        Mob_SetKeyInstance,        zPAR_TYPE_VOID,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,    0 );
    
    // AI
    parser->DefineExternal( "AI_CallScript",             AI_CallScript,             zPAR_TYPE_VOID,     zPAR_TYPE_STRING,   zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "AI_StartTriggerScript",     AI_StartTriggerScript,     zPAR_TYPE_INSTANCE, zPAR_TYPE_STRING,   zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "AI_StartTriggerScriptEx",   AI_StartTriggerScriptEx,   zPAR_TYPE_INSTANCE, zPAR_TYPE_STRING,   zPAR_TYPE_INT,      zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "AI_GetTriggerByID",         AI_GetTriggerByID,         zPAR_TYPE_INSTANCE, zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "AI_GetTriggersNum",         AI_GetTriggersNum,         zPAR_TYPE_INT,      0 );
    parser->DefineExternal( "AI_GetTriggerNPC",          AI_GetTriggerNPC,          zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, 0 );
    parser->DefineExternal( "AI_GetTriggerFunc",         AI_GetTriggerFunc,         zPAR_TYPE_FUNC,     zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "AI_GetTriggerFuncName",     AI_GetTriggerFuncName,     zPAR_TYPE_STRING,   zPAR_TYPE_INSTANCE, 0 );
    
    parser->DefineExternal( "AI_GetNextTriggerByFunc",     AI_GetNextTriggerByFunc,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_FUNC,     0 );
    parser->DefineExternal( "AI_GetNextTriggerByFuncName", AI_GetNextTriggerByFuncName, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_STRING,   0 );
    parser->DefineExternal( "AI_GetNextTriggerBySelf",     AI_GetNextTriggerBySelf,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "AI_GetNextTriggerByOther",    AI_GetNextTriggerByOther,    zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "AI_GetNextTriggerByVictim",   AI_GetNextTriggerByVictim,   zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, 0 );
    parser->DefineExternal( "AI_GetNextTriggerByNPCs",     AI_GetNextTriggerByNPCs,     zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, 0 );
    
    // STR
    parser->DefineExternal( "Str_Format",             Str_Format,             zPAR_TYPE_STRING, 0 );
    parser->DefineExternal( "Str_GetLocalizedString", Str_GetLocalizedString, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, 0 );
    parser->DefineExternal( "Str_UTF8_to_ANSI",       Str_UTF8_to_ANSI,       zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_INT, 0 );
    parser->DefineExternal( "Str_GetCurrentCP",       Str_GetCurrentCP,       zPAR_TYPE_INT,    0 );

    DefineExternals_Vobs();

    // OTHER
    static float fNan  = NAN;
    static void* pNull = Null;
    parser->DefineExternalVar( "NAN",  &fNan,  zPAR_TYPE_FLOAT,    1 );
    parser->DefineExternalVar( "NULL", &pNull, zPAR_TYPE_INSTANCE, 1 );
#endif
  }
}