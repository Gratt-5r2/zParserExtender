// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  static Map<zCModel*, float> s_ModelSpeedList;
  static Map<oCNpc*, float> s_NpcSpeedListTemp;

  HOOK Hook_zCModel_AdvanceAnis PATCH( &zCModel::AdvanceAnis, &zCModel::AdvanceAnis_Union );

  void zCModel::AdvanceAnis_Union() {
    auto& multiplier = s_ModelSpeedList[this];
    if( multiplier.IsNull() )
      return THISCALL( Hook_zCModel_AdvanceAnis )();

    float timeScaleOld = timeScale;
    timeScale *= multiplier;
    THISCALL( Hook_zCModel_AdvanceAnis )();
    timeScale = timeScaleOld;
  }

  void SetModelSpeed( zCModel* model, const float& multiplier ) {
    if( !s_ModelSpeedList[model].IsNull() )
      return;

    s_ModelSpeedList.Insert( model, multiplier );
    model->AddRef();
  }

  void ResetModelSpeed( zCModel* model ) {
    auto& pair = s_ModelSpeedList[model];
    if( pair.IsNull() )
      return;

    pair.GetKey()->Release();
    s_ModelSpeedList.Remove( model );
  }

  void ResetModelSpeedList() {
    auto& array = s_ModelSpeedList.GetArray();
    for each( auto& pair in array ) {
      zCModel* model = pair.GetKey();
      model->Release();
    }

    s_ModelSpeedList.Clear();
    s_NpcSpeedListTemp.Clear();
  }


  HOOK Hook_oCNpc_UnShrink PATCH( &oCNpc::UnShrink, &oCNpc::UnShrink_Union );

  void oCNpc::UnShrink_Union() {
    THISCALL( Hook_oCNpc_UnShrink )();
    zCModel* model = GetModel();
    if( !model )
      return;

    auto& pair = s_NpcSpeedListTemp[this];
    if( pair.IsNull() )
      return;

    float value = pair;
    s_NpcSpeedListTemp.Remove( this );
    SetModelSpeed( model, value );
  }
}