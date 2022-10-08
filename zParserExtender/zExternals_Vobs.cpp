// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
#pragma warning(push)
#pragma warning(disable: 4244)

#pragma push_macro("A")
#pragma push_macro("Z")
#undef A
#undef Z
  // zCOLOR
  struct C_COLOR {
    int R;
    int G;
    int B;
    int A;
  };

  void InitializeColorVariables() {
    zCPar_Symbol* classSym = parser->GetSymbol( "ZPARSEREXTENDER:C_COLOR" );
    if( !classSym )
      return;

    auto& symtable = parser->symtab.table;
    for( int i = 0; i < symtable.GetNum(); i++ ) {
      zCPar_Symbol* sym = symtable[i];
      if( sym->parent == classSym && sym->type == zPAR_TYPE_INSTANCE )
        sym->offset = (int)new C_COLOR();
    }
  }

  // zVEC3
  struct C_POSITION {
    int X;
    int Y;
    int Z;
  };

  void InitializePositionVariables() {
    zCPar_Symbol* classSym = parser->GetSymbol( "ZPARSEREXTENDER:C_POSITION" );
    if( !classSym )
      return;

    auto& symtable = parser->symtab.table;
    for( int i = 0; i < symtable.GetNum(); i++ ) {
      zCPar_Symbol* sym = symtable[i];
      if( sym->parent == classSym && sym->type == zPAR_TYPE_INSTANCE )
        sym->offset = (int)new C_POSITION();
    }
  }

  int Vob_GetVobPosition() {
    static C_POSITION vobPosition;
    zCParser* par = zCParser::GetParser();
    zCVob* vob    = (zCVob*)par->GetInstance();
    zVEC3 pos     = vob->GetPositionWorld();

    vobPosition.X = (int)pos[VX];
    vobPosition.Y = (int)pos[VY];
    vobPosition.Z = (int)pos[VZ];
    par->SetReturn( &vobPosition );
    return 0;
  }

  int Vob_SetVobPosition() {
    zCParser* par = zCParser::GetParser();
    C_POSITION* vobPosition = (C_POSITION*)par->GetInstance();
    zCVob* vob = (zCVob*)par->GetInstance();
    cmd << string::Combine( "%x %x", vobPosition, vob ) << endl;

    vob->SetPositionWorld( zVEC3(
      (float)vobPosition->X,
      (float)vobPosition->Y,
      (float)vobPosition->Z ) );

    return 0;
  }

  // zCVob
  struct C_VOB_DATA {
    zSTRING Name;
    float VisualAlpha;
    int ShowVisual;
    int DrawBBox3D;
    int VisualAlphaEnabled;
    int PhysicsEnabled;
    int IgnoredByTraceRay;
    int CollDetectionStatic;
    int CollDetectionDynamic;
    int CastDynShadow;
    int LightColorStatDirty;
    int LightColorDynDirty;
    int SleepingMode;
    int DontWriteIntoArchive;
    C_POSITION Position;
  };

  int Vob_GetVobData() {
    static C_VOB_DATA vobData;
    zCParser* par                = zCParser::GetParser();
    zCVob* vob                   = (zCVob*)par->GetInstance();

    vobData.Name                 = vob->objectName;
    vobData.VisualAlpha          = vob->visualAlpha;
    vobData.ShowVisual           = vob->showVisual;
    vobData.DrawBBox3D           = vob->drawBBox3D;
    vobData.VisualAlphaEnabled   = vob->visualAlphaEnabled;
    vobData.PhysicsEnabled       = vob->physicsEnabled;
    vobData.IgnoredByTraceRay    = vob->ignoredByTraceRay;
    vobData.CollDetectionStatic  = vob->collDetectionStatic;
    vobData.CollDetectionDynamic = vob->collDetectionDynamic;
    vobData.CastDynShadow        = vob->castDynShadow;
    vobData.LightColorStatDirty  = vob->lightColorStatDirty;
    vobData.LightColorDynDirty   = vob->lightColorDynDirty;
    vobData.SleepingMode         = vob->sleepingMode;
    vobData.DontWriteIntoArchive = vob->dontWriteIntoArchive;

    par->SetReturn( &vobData );
    return 0;
  }

  int Vob_SetVobData() {
    zCParser* par             = zCParser::GetParser();
    C_VOB_DATA& vobData       = *(C_VOB_DATA*)par->GetInstance();
    zCVob* vob                = (zCVob*)par->GetInstance();

    vob->objectName           = vobData.Name;
    vob->visualAlpha          = vobData.VisualAlpha;
    vob->showVisual           = vobData.ShowVisual;
    vob->drawBBox3D           = vobData.DrawBBox3D;
    vob->visualAlphaEnabled   = vobData.VisualAlphaEnabled;
    vob->physicsEnabled       = vobData.PhysicsEnabled;
    vob->ignoredByTraceRay    = vobData.IgnoredByTraceRay;
    vob->collDetectionStatic  = vobData.CollDetectionStatic;
    vob->collDetectionDynamic = vobData.CollDetectionDynamic;
    vob->castDynShadow        = vobData.CastDynShadow;
    vob->lightColorStatDirty  = vobData.LightColorStatDirty;
    vob->lightColorDynDirty   = vobData.LightColorDynDirty;
    vob->sleepingMode         = vobData.SleepingMode;
    vob->dontWriteIntoArchive = vobData.DontWriteIntoArchive;

    return 0;
  }

  // zCVobLight
  struct C_LIGHT_DATA {
    int R;
    int G;
    int B;
    int Range;
    int RangeInv;
    int RangeBackup;
    int RangeAniActFrame;
    int RangeAniFPS;
    int ColorAniActFrame;
    int ColorAniFPS;
    int SpotConeAngleDeg;
    int IsStatic;
    int RangeAniSmooth;
    int RangeAniLoop;
    int ColorAniSmooth;
    int ColorAniLoop;
    int IsTurnedOn;
    int LightQuality;
    int LightType;
  };

  int Vob_GetLightData() {
    static C_LIGHT_DATA vobData;
    zCParser* par            = zCParser::GetParser();
    zCVob* vob               = (zCVob*)par->GetInstance();

    zCVobLight* light = vob->CastTo<zCVobLight>();
    if( !light ) {
      par->SetReturn( &vobData );
      return 0;
    }
    
    vobData.R                = light->lightData.lightColor.r;
    vobData.G                = light->lightData.lightColor.g;
    vobData.B                = light->lightData.lightColor.b;
    vobData.Range            = light->lightData.range;
    vobData.RangeInv         = light->lightData.rangeInv;
    vobData.RangeBackup      = light->lightData.rangeBackup;
    vobData.RangeAniActFrame = light->lightData.rangeAniActFrame;
    vobData.RangeAniFPS      = light->lightData.rangeAniFPS;
    vobData.ColorAniActFrame = light->lightData.colorAniActFrame;
    vobData.ColorAniFPS      = light->lightData.colorAniFPS;
    vobData.SpotConeAngleDeg = light->lightData.spotConeAngleDeg;
    vobData.IsStatic         = light->lightData.isStatic;
    vobData.RangeAniSmooth   = light->lightData.rangeAniSmooth;
    vobData.RangeAniLoop     = light->lightData.rangeAniLoop;
    vobData.ColorAniSmooth   = light->lightData.colorAniSmooth;
    vobData.ColorAniLoop     = light->lightData.colorAniLoop;
    vobData.IsTurnedOn       = light->lightData.isTurnedOn;
    vobData.LightQuality     = light->lightData.lightQuality;
    vobData.LightType        = light->lightData.lightType;

    par->SetReturn( &vobData );
    return 0;
  }

  int Vob_SetLightData() {
    zCParser* par                     = zCParser::GetParser();
    C_LIGHT_DATA& vobData             = *(C_LIGHT_DATA*)par->GetInstance();
    zCVob* vob                        = (zCVob*)par->GetInstance();

    zCVobLight* light = vob->CastTo<zCVobLight>();
    if( !light )
      return 0;

    light->lightData.lightColor.r     = vobData.R;
    light->lightData.lightColor.g     = vobData.G;
    light->lightData.lightColor.b     = vobData.B;
    light->lightData.range            = vobData.Range;
    light->lightData.rangeInv         = vobData.RangeInv;
    light->lightData.rangeBackup      = vobData.RangeBackup;
    light->lightData.rangeAniActFrame = vobData.RangeAniActFrame;
    light->lightData.rangeAniFPS      = vobData.RangeAniFPS;
    light->lightData.colorAniActFrame = vobData.ColorAniActFrame;
    light->lightData.colorAniFPS      = vobData.ColorAniFPS;
    light->lightData.spotConeAngleDeg = vobData.SpotConeAngleDeg;
    light->lightData.isStatic         = vobData.IsStatic;
    light->lightData.rangeAniSmooth   = vobData.RangeAniSmooth;
    light->lightData.rangeAniLoop     = vobData.RangeAniLoop;
    light->lightData.colorAniSmooth   = vobData.ColorAniSmooth;
    light->lightData.colorAniLoop     = vobData.ColorAniLoop;
    light->lightData.isTurnedOn       = vobData.IsTurnedOn;
    light->lightData.lightQuality     = vobData.LightQuality;
    light->lightData.lightType        = vobData.LightType;

    return 0;
  }

  int Vob_ClearLightAniList() {
    zCParser* par = zCParser::GetParser();
    zCVob* vob    = (zCVob*)par->GetInstance();

    zCVobLight* light = vob->CastTo<zCVobLight>();
    if( !light )
      return 0;

    light->lightData.colorAniList.DeleteList();
  }

  int Vob_AddLightAniColor() {
    zCParser* par  = zCParser::GetParser();
    C_COLOR& color = *(C_COLOR*)par->GetInstance();
    zCVob* vob     = (zCVob*)par->GetInstance();

    zCVobLight* light = vob->CastTo<zCVobLight>();
    if( !light )
      return 0;

    light->lightData.colorAniList.Insert( zCOLOR( color.R, color.G, color.B ) );
  }

  int Vob_AddLightAniColorRGB() {
    zCParser* par = zCParser::GetParser();
    int r, g, b;
    par->GetParameter( b );
    par->GetParameter( g );
    par->GetParameter( r );
    zCVob* vob = (zCVob*)par->GetInstance();

    zCVobLight* light = vob->CastTo<zCVobLight>();
    if( !light )
      return 0;

    light->lightData.colorAniList.Insert( zCOLOR( r, g, b ) );
    return 0;
  }

  // oCMob
  struct C_MOB_DATA {
    zSTRING VisibleName;
    int Hitpoints;
    int Damage;
    int IsDestroyed;
    int Moveable;
    int Takeable;
    int FocusOverride;
    int SndMat;
    zSTRING VisualDestroyed;
    zSTRING OwnerStr;
    zSTRING OwnerGuildStr;
    int Owner;
    int OwnerGuild;
    int FocusNameIndex;
  };

  int Vob_GetMobData() {
    static C_MOB_DATA vobData;
    zCParser* par = zCParser::GetParser();
    zCVob* vob = (zCVob*)par->GetInstance();

    oCMOB* mob = vob->CastTo<oCMOB>();
    if( !mob ) {
      par->SetReturn( &vobData );
      return 0;
    }

    vobData.VisibleName      = mob->name;
    vobData.Hitpoints        = mob->hitp;
    vobData.Damage           = mob->damage;
    vobData.IsDestroyed      = mob->isDestroyed;
    vobData.Moveable         = mob->moveable;
    vobData.Takeable         = mob->takeable;
    vobData.FocusOverride    = mob->focusOverride;
    vobData.SndMat           = mob->sndMat;
    vobData.VisualDestroyed  = mob->visualDestroyed;
    vobData.OwnerStr         = mob->ownerStr;
    vobData.OwnerGuildStr    = mob->ownerGuildStr;
    vobData.Owner            = mob->owner;
    vobData.OwnerGuild       = mob->ownerGuild;
    vobData.FocusNameIndex   = mob->focusNameIndex;

    par->SetReturn( &vobData );
    return 0;
  }

  int Vob_SetMobData() {
    zCParser* par             = zCParser::GetParser();
    C_MOB_DATA& vobData       = *(C_MOB_DATA*)par->GetInstance();
    zCVob* vob                = (zCVob*)par->GetInstance();

    oCMOB* mob = vob->CastTo<oCMOB>();
    if( !mob )
      return 0;

    mob->name             = vobData.VisibleName;
    mob->hitp             = vobData.Hitpoints;
    mob->damage           = vobData.Damage;
    mob->isDestroyed      = vobData.IsDestroyed;
    mob->moveable         = vobData.Moveable;
    mob->takeable         = vobData.Takeable;
    mob->focusOverride    = vobData.FocusOverride;
    mob->sndMat           = (oTSndMaterial&)vobData.SndMat;
    mob->visualDestroyed  = vobData.VisualDestroyed;
    mob->ownerStr         = vobData.OwnerStr;
    mob->ownerGuildStr    = vobData.OwnerGuildStr;
    mob->owner            = vobData.Owner;
    mob->ownerGuild       = vobData.OwnerGuild;
    mob->focusNameIndex   = vobData.FocusNameIndex;

    return 0;
  }

  // oCMobInter
  struct C_MOBINTER_DATA {
    zSTRING TriggerTarget;
    zSTRING UseWithItem;
    zSTRING Sceme;
    zSTRING ConditionFunc;
    zSTRING OnStateFuncName;
    int State;
    int State_num;
    int State_target;
    int Rewind;
    int MobStateAni;
    int NpcStateAni;
  };

  int Vob_GetMobInterData() {
    static C_MOBINTER_DATA vobData;
    zCParser* par = zCParser::GetParser();
    zCVob* vob    = (zCVob*)par->GetInstance();

    oCMobInter* mob = vob->CastTo<oCMobInter>();
    if( !mob ) {
      par->SetReturn( &vobData );
      return 0;
    }

    vobData.TriggerTarget   = mob->triggerTarget;
    vobData.UseWithItem     = mob->useWithItem;
    vobData.Sceme           = mob->sceme;
    vobData.ConditionFunc   = mob->conditionFunc;
    vobData.OnStateFuncName = mob->onStateFuncName;
    vobData.State           = mob->state;
    vobData.State_num       = mob->state_num;
    vobData.State_target    = mob->state_target;
    vobData.Rewind          = mob->rewind;
    vobData.MobStateAni     = mob->mobStateAni;
    vobData.NpcStateAni     = mob->npcStateAni;

    par->SetReturn( &vobData );
    return 0;
  }

  int Vob_SetMobInterData() {
    zCParser* par             = zCParser::GetParser();
    C_MOBINTER_DATA& vobData  = *(C_MOBINTER_DATA*)par->GetInstance();
    zCVob* vob                = (zCVob*)par->GetInstance();

    oCMobInter* mob = vob->CastTo<oCMobInter>();
    if( !mob )
      return 0;

    mob->triggerTarget   = vobData.TriggerTarget;
    mob->useWithItem     = vobData.UseWithItem;
    mob->sceme           = vobData.Sceme;
    mob->conditionFunc   = vobData.ConditionFunc;
    mob->onStateFuncName = vobData.OnStateFuncName;
    mob->state           = vobData.State;
    mob->state_num       = vobData.State_num;
    mob->state_target    = vobData.State_target;
    mob->rewind          = vobData.Rewind;
    mob->mobStateAni     = vobData.MobStateAni;
    mob->npcStateAni     = vobData.NpcStateAni;

    return 0;
  }

  // oCMobLockable
  struct C_MOBLOCKABLE_DATA {
    int Locked;
    int AutoOpen;
    int PickLockNr;
    zSTRING KeyInstance;
    zSTRING PickLockStr;
  };

  int Vob_GetMobLockableData() {
    static C_MOBLOCKABLE_DATA vobData;
    zCParser* par = zCParser::GetParser();
    zCVob* vob    = (zCVob*)par->GetInstance();

    oCMobLockable* mob = vob->CastTo<oCMobLockable>();
    if( !mob ) {
      par->SetReturn( &vobData );
      return 0;
    }

    vobData.Locked      = mob->locked;
    vobData.AutoOpen    = mob->autoOpen;
    vobData.PickLockNr  = mob->pickLockNr;
    vobData.KeyInstance = mob->keyInstance;
    vobData.PickLockStr = mob->pickLockStr;

    par->SetReturn( &vobData );
    return 0;
  }

  int Vob_SetMobLockableData() {
    zCParser* par               = zCParser::GetParser();
    C_MOBLOCKABLE_DATA& vobData = *(C_MOBLOCKABLE_DATA*)par->GetInstance();
    zCVob* vob                  = (zCVob*)par->GetInstance();

    oCMobLockable* mob = vob->CastTo<oCMobLockable>();
    if( !mob )
      return 0;
    
    mob->locked      = vobData.Locked;
    mob->autoOpen    = vobData.AutoOpen;
    mob->pickLockNr  = vobData.PickLockNr;
    mob->keyInstance = vobData.KeyInstance;
    mob->pickLockStr = vobData.PickLockStr;

    return 0;
  }

  bool ActivateDynamicExternal_Vobs( const zSTRING& funcName, bool createFuncList ) {
    zCParser* par = Null;
    if( !createFuncList ) {
      par = zCParser::GetParser();
      if( par->GetIndex( funcName ) != Invalid )
        return true;

      if( funcName.IsEmpty() )
        return false;
    }

    EXTERNAL_DEFINE_BEGIN( Vob_GetVobPosition      ) zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE                                          EXTERNAL_DEFINE_END
    EXTERNAL_DEFINE_BEGIN( Vob_SetVobPosition      ) zPAR_TYPE_VOID, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE                          EXTERNAL_DEFINE_END
    EXTERNAL_DEFINE_BEGIN( Vob_GetVobData          ) zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE                                          EXTERNAL_DEFINE_END
    EXTERNAL_DEFINE_BEGIN( Vob_SetVobData          ) zPAR_TYPE_VOID, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE                          EXTERNAL_DEFINE_END
    EXTERNAL_DEFINE_BEGIN( Vob_GetLightData        ) zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE                                          EXTERNAL_DEFINE_END
    EXTERNAL_DEFINE_BEGIN( Vob_SetLightData        ) zPAR_TYPE_VOID, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE                          EXTERNAL_DEFINE_END
    EXTERNAL_DEFINE_BEGIN( Vob_ClearLightAniList   ) zPAR_TYPE_VOID, zPAR_TYPE_INSTANCE                                              EXTERNAL_DEFINE_END
    EXTERNAL_DEFINE_BEGIN( Vob_AddLightAniColor    ) zPAR_TYPE_VOID, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE                          EXTERNAL_DEFINE_END
    EXTERNAL_DEFINE_BEGIN( Vob_AddLightAniColorRGB ) zPAR_TYPE_VOID, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT EXTERNAL_DEFINE_END
    EXTERNAL_DEFINE_BEGIN( Vob_GetMobData          ) zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE                                          EXTERNAL_DEFINE_END
    EXTERNAL_DEFINE_BEGIN( Vob_SetMobData          ) zPAR_TYPE_VOID, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE                          EXTERNAL_DEFINE_END
    EXTERNAL_DEFINE_BEGIN( Vob_GetMobInterData     ) zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE                                          EXTERNAL_DEFINE_END
    EXTERNAL_DEFINE_BEGIN( Vob_SetMobInterData     ) zPAR_TYPE_VOID, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE                          EXTERNAL_DEFINE_END
    EXTERNAL_DEFINE_BEGIN( Vob_GetMobLockableData  ) zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE                                          EXTERNAL_DEFINE_END
    EXTERNAL_DEFINE_BEGIN( Vob_SetMobLockableData  ) zPAR_TYPE_VOID, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE                          EXTERNAL_DEFINE_END
    return false;
  }
  
  void DefineExternals_Vobs() {
  }

#pragma pop_macro("A")
#pragma pop_macro("Z")
#pragma warning(pop)
}