#include "UnionAfx.h"

namespace Gothic_II_Addon {

#ifdef __ZPROTO_B_VER3__
  zCEngine*&             zengine          = *(zCEngine**)            0x008C295C;
  zCOption*&             zoptions         = *(zCOption**)            0x008CD988;
  zCOption*&             zgameoptions     = *(zCOption**)            0x008CD98C;
  zCTimer*               ztimer           =  (zCTimer*)              0x0099B3D4;
  oCGame*&               ogame            = *(oCGame**)              0x00AB0884;
  zCInput*&              zinput           = *(zCInput**)             0x008D1650;
  zCRenderer*&           zrenderer        = *(zCRenderer**)          0x00982F08;
  zCLineCache*           zlineCache       =  (zCLineCache*)          0x008D42F8;
  zERROR*                zerr             =  (zERROR*)               0x008CDCD0;
  zCSoundSystem*&        zsound           = *(zCSoundSystem**)       0x0099B03C;
  zCMusicSystem*&        zmusic           = *(zCMusicSystem**)       0x008D1F14;
  oCItem*&               offer            = *(oCItem**)              0x00AB04F0;
  oCDoc*&                document         = *(oCDoc**)               0x00AB05D0;
  zCMallocGeneric*       zmalloc          =  (zCMallocGeneric*)      0x008D8778;
  zTEngineStats*         zengineStats     =  (zTEngineStats*)        0x008D4344;
  zCScanDir*             dirScanner       =  (zCScanDir*)            0x008CDA34;
  oCObjectFactory*&      zfactory         = *(oCObjectFactory**)     0x008D8DF0;
  zCArchiverFactory*     zarcFactory      =  (zCArchiverFactory*)    0x008D472C;
  oCNpc*&                player           = *(oCNpc**)               0x00AB2684;
  oCNpc*&                stealnpc         = *(oCNpc**)               0x00AB27D4;
  zCVob*&                speaker          = *(zCVob**)               0x008CEE4C;
  zCVob*&                listener         = *(zCVob**)               0x008CEE50;
  zCView*&               screen           = *(zCView**)              0x00AB6468;
  zCView*&               messages         = *(zCView**)              0x008D11BC;
  zCConsole*             zcon             =  (zCConsole*)            0x00AB3860;
  zCConsole*&            game_species_con = *(zCConsole**)           0x00AB0898;
  zCConsole*&            game_fight_con   = *(zCConsole**)           0x00AB089C;
  zCConsole*&            edit_con         = *(zCConsole**)           0x00AB08A0;
  zCConsole*&            game_cam_con     = *(zCConsole**)           0x00AB08A4;
  zCConsole*&            game_aiConsole   = *(zCConsole**)           0x00AB08A8;
  zCNet*&                znet             = *(zCNet**)               0x008C8F90;
  zCNetManager*&         znetman          = *(zCNetManager**)        0x008CD250;
  CGameManager*&         gameMan          = *(CGameManager**)        0x008C2958;
  zCFontMan*&            zfontman         = *(zCFontMan**)           0x00AB39D4;
  oCRtnManager*          rtnMan           =  (oCRtnManager*)         0x00AB31C8;
  oCMissionManager*      misMan           =  (oCMissionManager*)     0x00AB1384;
  zCRenderManager*       zrenderMan       =  (zCRenderManager*)      0x0099AA44;
  zCResourceManager*&    zresMan          = *(zCResourceManager**)   0x0099AB30;
  zCSoundManager*&       zsndMan          = *(zCSoundManager**)      0x0099B0B0;
  zCVertexBufferManager* zvertexBufferMan =  (zCVertexBufferManager*)0x009A341C;
  zCParser*              parser           =  (zCParser* )            0x00AB40C0;
  zCParser*&             parserSoundFX    = *(zCParser**)            0x008D2A64;
  zCParser*&             parserParticleFX = *(zCParser**)            0x008D9234;
  zCParser*&             parserVisualFX   = *(zCParser**)            0x008CE6EC;
  zCParser*&             parserCamera     = *(zCParser**)            0x008CEAC8;
  zCParser*&             parserMenu       = *(zCParser**)            0x008D1E68;
  zCParser*&             parserMusic      = *(zCParser**)            0x008D2140;
  zCFPUControler*        zfpuControler    =  (zCFPUControler*)       0x0099B3FC;
  oCParticleControl*&    pfxc             = *(oCParticleControl**)   0x00AB088C;
  HINSTANCE&             hInstApp         = *(HINSTANCE*)            0x008D4220;
  HDC&                   dcScreen         = *(HDC*)                  0x008D4224;
  HICON&                 hIconApp         = *(HICON*)                0x008D4228;
  HWND&                  hWndApp          = *(HWND*)                 0x008D422C;


  namespace Gothic {
    namespace Managers {
      CGameManager*&          Game         = gameMan;
      zCFontMan*&             Font         = zfontman;
      oCRtnManager*&          Routine      = rtnMan;
      oCMissionManager*&      Mission      = misMan;
      zCRenderManager*&       Render       = zrenderMan;
      zCResourceManager*&     Resource     = zresMan;
      zCSoundManager*&        Sound        = zsndMan;
      zCVertexBufferManager*& VertexBuffer = zvertexBufferMan;
    }
    namespace Parsers {
      zCParser*& Game   = parser;
      zCParser*& SFX    = parserSoundFX;
      zCParser*& PFX    = parserParticleFX;
      zCParser*& VFX    = parserVisualFX;
      zCParser*& Camera = parserCamera;
      zCParser*& Menu   = parserMenu;
      zCParser*& Music  = parserMusic;
    }
    namespace Network {
      zCNet*&        Net     = znet;
      zCNetManager*& Manager = znetman;
    }
    namespace Entities {
      oCNpc*& Player      = player;
      oCNpc*& StealNpc    = stealnpc;
      zCVob*& VobSpeaker  = speaker;
      zCVob*& VobListener = listener;
    }
    namespace Consoles {
      zCConsole*& Main      = zcon;
      zCConsole*& Species   = game_species_con;
      zCConsole*& Fight     = game_fight_con;
      zCConsole*& Abilities = edit_con;
      zCConsole*& Camera    = game_cam_con;
      zCConsole*& AI        = game_aiConsole;
    }
    namespace Factories {
      oCObjectFactory*&   Objects  = zfactory;
      zCArchiverFactory*& Archives = zarcFactory;
    }
    namespace Views {
      zCView*& Screen   = screen;
      zCView*& Messages = messages;
    }
    namespace Options {
      zCOption*& Gothic = zoptions;
      zCOption*& Game   = zgameoptions;
    }
    namespace Game {
      zCTimer*&       Timer        = ztimer;
      oCGame*&        Session      = ogame;
      zCInput*&       Input        = zinput;
      zCRenderer*&    Renderer     = zrenderer;
      zCLineCache*&   LineCache    = zlineCache;
      zERROR*&        Error        = zerr;
      zCSoundSystem*& Sound        = zsound;
      zCMusicSystem*& Music        = zmusic;
      zTEngineStats*& EngineStatus = zengineStats;
    }
    namespace Other {
      zCEngine*&          EnginePointer     = zengine;
      oCItem*&            OfferItem         = offer;
      oCDoc*&             Document          = document;
      zCMallocGeneric*&   Malloc            = zmalloc;
      zCScanDir*&         DirectoryExplorer = dirScanner;
      zCFPUControler*&    FPUController     = zfpuControler;
      oCParticleControl*& PartivleFXControl = pfxc;
    }
    namespace Application {
      HINSTANCE& Instance = hInstApp;
      HDC&       ScreenDC = dcScreen;
      HICON&     Icon     = hIconApp;
      HWND&      Window   = hWndApp;
    }
  }

#endif // __ZPROTO_B_VER3__

#ifdef __OCS_MANAGER_H__VER3__
  zCClassDef* oCCSManager::classDef                 = (zCClassDef*)0x008C1DA8;
#endif
#ifdef __OCS_PLAYER_H__VER3__
  zCClassDef* oCCSPlayer::classDef                  = (zCClassDef*)0x008C1F90;
#endif
#ifdef __OCS_PROPS_H__VER3__
  zCClassDef* oCCSProps::classDef                   = (zCClassDef*)0x008C2030;
#endif
#ifdef __OCS_TRIGGER_H__VER3__
  zCClassDef* oCCSTrigger::classDef                 = (zCClassDef*)0x008C20A8;
#endif
#ifdef __ZCCS_CONTEXT_H__VER3__
  zCClassDef* zCCSCutsceneContext::classDef         = (zCClassDef*)0x008C2120;
#endif
#ifdef __ZCCS_CUTSCENE_H__VER3__
  zCClassDef* zCEvMsgCutscene::classDef             = (zCClassDef*)0x008C2190;
#endif
#ifdef __ZCCS_CUTSCENE_H__VER3__
  zCClassDef* zCCSBlock::classDef                   = (zCClassDef*)0x008C2208;
#endif
#ifdef __ZCCS_CUTSCENE_H__VER3__
  zCClassDef* zCCSSyncBlock::classDef               = (zCClassDef*)0x008C2278;
#endif
#ifdef __ZCCS_CUTSCENE_H__VER3__
  zCClassDef* zCCSAtomicBlock::classDef             = (zCClassDef*)0x008C22E8;
#endif
#ifdef __ZCCS_CUTSCENE_H__VER3__
  zCClassDef* zCCutscene::classDef                  = (zCClassDef*)0x008C2358;
#endif
#ifdef __ZCCS_CUTSCENE_H__VER3__
  zCClassDef* zCCSBlockBase::classDef               = (zCClassDef*)0x008C2418;
#endif
#ifdef __ZCCS_CUTSCENE_H__VER3__
  zCClassDef* zCCSRole::classDef                    = (zCClassDef*)0x008C2488;
#endif
#ifdef __ZCCS_LIB_H__VER3__
  zCClassDef* zCCSLib::classDef                     = (zCClassDef*)0x008C2500;
#endif
#ifdef __ZCCS_MANAGER_H__VER3__
  zCClassDef* zCCSManager::classDef                 = (zCClassDef*)0x008C2588;
#endif
#ifdef __ZCCS_PLAYER_H__VER3__
  zCClassDef* zCCSPlayer::classDef                  = (zCClassDef*)0x008C2608;
#endif
#ifdef __ZCCS_POOL_H__VER3__
  zCClassDef* zCCSPoolItem::classDef                = (zCClassDef*)0x008C2680;
#endif
#ifdef __ZCCS_PROPS_H__VER3__
  zCClassDef* zCCSProps::classDef                   = (zCClassDef*)0x008C26F8;
#endif
#ifdef __OSAVEGAME_H__VER3__
  zCClassDef* oCSavegameInfo::classDef              = (zCClassDef*)0x008C2D48;
#endif
#ifdef __OTRIGGER_H__VER3__
  zCClassDef* oCTriggerScript::classDef             = (zCClassDef*)0x008C2E48;
#endif
#ifdef __OTRIGGER_H__VER3__
  zCClassDef* oCTriggerChangeLevel::classDef        = (zCClassDef*)0x008C2F40;
#endif
#ifdef __ZNET_EVENT_MAN_H__VER3__
  zCClassDef* zCNetEventManager::classDef           = (zCClassDef*)0x008CD0E0;
#endif
#ifdef __ZNET_MANAGER_H__VER3__
  zCClassDef* zCNetManager::classDef                = (zCClassDef*)0x008CD1D0;
#endif
#ifdef __ZNET_VOB_CONTROL_H__VER3__
  zCClassDef* zCNetVobControl::classDef             = (zCClassDef*)0x008CD2E8;
#endif
#ifdef __OSPELL_H__VER3__
  zCClassDef* oCSpell::classDef                     = (zCClassDef*)0x008CE520;
#endif
#ifdef __OVIS_FX_H__VER3__
  zCClassDef* oCVisualFX::classDef                  = (zCClassDef*)0x008CE658;
#endif
#ifdef __OVIS_FX__MULTI_TARGET_H__VER3__
  zCClassDef* oCVisFX_MultiTarget::classDef         = (zCClassDef*)0x008CE7F8;
#endif
#ifdef __ZAI_CAMERA_H__VER3__
  zCClassDef* zCAICamera::classDef                  = (zCClassDef*)0x008CE950;
#endif
#ifdef __ZCS_CAMERA_H__VER3__
  zCClassDef* zCCSCamera_EventMsg::classDef         = (zCClassDef*)0x008D0F38;
#endif
#ifdef __ZCS_CAMERA_H__VER3__
  zCClassDef* zCCSCamera::classDef                  = (zCClassDef*)0x008D0FB8;
#endif
#ifdef __ZCS_CAMERA_H__VER3__
  zCClassDef* zCCamTrj_KeyFrame::classDef           = (zCClassDef*)0x008D1028;
#endif
#ifdef __ZCS_CAMERA_H__VER3__
  zCClassDef* zCCSCamera_EventMsgActivate::classDef = (zCClassDef*)0x008D1098;
#endif
#ifdef __ZAI_H__VER3__
  zCClassDef* zCAIBase::classDef                    = (zCClassDef*)0x008D43C8;
#endif
#ifdef __ZAI_H__VER3__
  zCClassDef* zCAIBaseSound::classDef               = (zCClassDef*)0x008D4438;
#endif
#ifdef __ZAI_PLAYER_H__VER3__
  zCClassDef* zCAIPlayer::classDef                  = (zCClassDef*)0x008D4538;
#endif
#ifdef __ZARCHIVER_H__VER3__
  zCClassDef* zCArchiver::classDef                  = (zCClassDef*)0x008D4658;
#endif
#ifdef __ZARCHIVER2_H__VER3__
  zCClassDef* zCArchiverBinSafe::classDef           = (zCClassDef*)0x008D47B8;
#endif
#ifdef __ZARCHIVER_GENERIC_H__VER3__
  zCClassDef* zCArchiverGeneric::classDef           = (zCClassDef*)0x008D48B0;
#endif
#ifdef __ZVISUAL_H__VER3__
  zCClassDef* zCDecal::classDef                     = (zCClassDef*)0x008D84B0;
#endif
#ifdef __ZLENSFLARE_H__VER3__
  zCClassDef* zCLensFlareFX::classDef               = (zCClassDef*)0x008D85F8;
#endif
#ifdef __ZMATERIAL_H__VER3__
  zCClassDef* zCMaterial::classDef                  = (zCClassDef*)0x008D8708;
#endif
#ifdef __ZVISUAL_H__VER3__
  zCClassDef* zCMesh::classDef                      = (zCClassDef*)0x008D87C8;
#endif
#ifdef __ZMODEL_H__VER3__
  zCClassDef* zCModelAni::classDef                  = (zCClassDef*)0x008D8898;
#endif
#ifdef __ZMODEL_H__VER3__
  zCClassDef* zCModel::classDef                     = (zCClassDef*)0x008D8920;
#endif
#ifdef __ZMODEL_H__VER3__
  zCClassDef* zCModelMeshLib::classDef              = (zCClassDef*)0x008D8B38;
#endif
#ifdef __ZMORPH_MESH_H__VER3__
  zCClassDef* zCMorphMesh::classDef                 = (zCClassDef*)0x008D8C00;
#endif
#ifdef __ZOBJECT_H__VER3__
  zCClassDef* zCObject::classDef                    = (zCClassDef*)0x008D8CD8;
#endif
#ifdef __ZOBJECT_H__VER3__
  zCClassDef* zCObjectFactory::classDef             = (zCClassDef*)0x008D8D48;
#endif
#ifdef __ZPARTICLE_H__VER3__
  zCClassDef* zCParticleFX::classDef                = (zCClassDef*)0x008D91A8;
#endif
#ifdef __ZPOLY_STRIP_H__VER3__
  zCClassDef* zCPolyStrip::classDef                 = (zCClassDef*)0x009594F0;
#endif
#ifdef __ZPROG_MESH_H__VER3__
  zCClassDef* zCMeshSoftSkin::classDef              = (zCClassDef*)0x00970AA0;
#endif
#ifdef __ZPROG_MESH_H__VER3__
  zCClassDef* zCProgMeshProto::classDef             = (zCClassDef*)0x00982B48;
#endif
#ifdef __ZPOLY_STRIP_H__VER3__
  zCClassDef* zCQuadMark::classDef                  = (zCClassDef*)0x00982C28;
#endif
#ifdef __ZRESOURCE_H__VER3__
  zCClassDef* zCResource::classDef                  = (zCClassDef*)0x0099AAC0;
#endif
#ifdef __ZSKY_H__VER3__
  zCClassDef* zCSkyControler::classDef              = (zCClassDef*)0x0099AB40;
#endif
#ifdef __ZSKY_H__VER3__
  zCClassDef* zCSkyControler_Mid::classDef          = (zCClassDef*)0x0099ABB0;
#endif
#ifdef __ZSKY_H__VER3__
  zCClassDef* zCSkyControler_Indoor::classDef       = (zCClassDef*)0x0099AC20;
#endif
#ifdef __ZSKY__OUTDOOR_H__VER3__
  zCClassDef* zCSkyControler_Outdoor::classDef      = (zCClassDef*)0x0099ACD8;
#endif
#ifdef __ZSOUND_H__VER3__
  zCClassDef* zCSoundFX::classDef                   = (zCClassDef*)0x0099ADC0;
#endif
#ifdef __ZTEXTURE_H__VER3__
  zCClassDef* zCTextureFileFormat::classDef         = (zCClassDef*)0x0099B0E8;
#endif
#ifdef __ZTEXTURE_H__VER3__
  zCClassDef* zCTextureFileFormatTGA::classDef      = (zCClassDef*)0x0099B158;
#endif
#ifdef __ZTEXTURE_H__VER3__
  zCClassDef* zCTextureFileFormatInternal::classDef = (zCClassDef*)0x0099B1D8;
#endif
#ifdef __ZTEXTURE_H__VER3__
  zCClassDef* zCLightMap::classDef                  = (zCClassDef*)0x0099B250;
#endif
#ifdef __ZTEXTURE_H__VER3__
  zCClassDef* zCTexture::classDef                   = (zCClassDef*)0x0099B2F8;
#endif
#ifdef __ZVERTEX_BUFFER_H__VER3__
  zCClassDef* zCVertexBuffer::classDef              = (zCClassDef*)0x009A3438;
#endif
#ifdef __ZVOB_H__VER3__
  zCClassDef* zCVob::classDef                       = (zCClassDef*)0x009A34D8;
#endif
#ifdef __ZVOB_H__VER3__
  zCClassDef* zCEventCore::classDef                 = (zCClassDef*)0x009A3548;
#endif
#ifdef __ZVOB_H__VER3__
  zCClassDef* zCVobLevelCompo::classDef             = (zCClassDef*)0x009A35B8;
#endif
#ifdef __ZVOB_H__VER3__
  zCClassDef* zCVobLightPreset::classDef            = (zCClassDef*)0x009A3628;
#endif
#ifdef __ZVOB_H__VER3__
  zCClassDef* zCEventMessage::classDef              = (zCClassDef*)0x009A3698;
#endif
#ifdef __ZVISUAL_H__VER3__
  zCClassDef* zCVisual::classDef                    = (zCClassDef*)0x009A3708;
#endif
#ifdef __ZVISUAL_H__VER3__
  zCClassDef* zCVisualAnimate::classDef             = (zCClassDef*)0x009A3778;
#endif
#ifdef __ZVOB_H__VER3__
  zCClassDef* zCVobLight::classDef                  = (zCClassDef*)0x009A3810;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCEventMover::classDef                = (zCClassDef*)0x009A3890;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCVobStair::classDef                  = (zCClassDef*)0x009A3900;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCTouchAnimate::classDef              = (zCClassDef*)0x009A3978;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCMoverControler::classDef            = (zCClassDef*)0x009A39E8;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCMover::classDef                     = (zCClassDef*)0x009A3A60;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCEarthquake::classDef                = (zCClassDef*)0x009A3AD0;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCPFXControler::classDef              = (zCClassDef*)0x009A3B40;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCVobLensFlare::classDef              = (zCClassDef*)0x009A3BB0;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCEventScreenFX::classDef             = (zCClassDef*)0x009A3C20;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCTriggerBase::classDef               = (zCClassDef*)0x009A3C90;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCTouchDamage::classDef               = (zCClassDef*)0x009A3D00;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCTrigger::classDef                   = (zCClassDef*)0x009A3D70;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCEffect::classDef                    = (zCClassDef*)0x009A3DE0;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCVobAnimate::classDef                = (zCClassDef*)0x009A3E50;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCMessageFilter::classDef             = (zCClassDef*)0x009A3EC0;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCVobScreenFX::classDef               = (zCClassDef*)0x009A3F30;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCEventCommon::classDef               = (zCClassDef*)0x009A3FA0;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCTriggerUntouch::classDef            = (zCClassDef*)0x009A4010;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCTriggerTeleport::classDef           = (zCClassDef*)0x009A4080;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCCodeMaster::classDef                = (zCClassDef*)0x009A40F0;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCTriggerList::classDef               = (zCClassDef*)0x009A4160;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCTouchAnimateSound::classDef         = (zCClassDef*)0x009A41D0;
#endif
#ifdef __ZVOB_MISC_H__VER3__
  zCClassDef* zCTriggerWorldStart::classDef         = (zCClassDef*)0x009A4240;
#endif
#ifdef __ZWORLD_H__VER3__
  zCClassDef* zCWorld::classDef                     = (zCClassDef*)0x009A43A0;
#endif
#ifdef __ZZONE_H__VER3__
  zCClassDef* zCVobSound::classDef                  = (zCClassDef*)0x009A44C8;
#endif
#ifdef __ZZONE_H__VER3__
  zCClassDef* zCZoneZFog::classDef                  = (zCClassDef*)0x009A4538;
#endif
#ifdef __ZZONE_H__VER3__
  zCClassDef* zCZone::classDef                      = (zCClassDef*)0x009A45A8;
#endif
#ifdef __ZZONE_H__VER3__
  zCClassDef* zCZoneZFogDefault::classDef           = (zCClassDef*)0x009A4618;
#endif
#ifdef __ZZONE_H__VER3__
  zCClassDef* zCVobSoundDaytime::classDef           = (zCClassDef*)0x009A4688;
#endif
#ifdef __ZZONE_H__VER3__
  zCClassDef* zCZoneReverbDefault::classDef         = (zCClassDef*)0x009A46F8;
#endif
#ifdef __ZZONE_H__VER3__
  zCClassDef* zCZoneVobFarPlaneDefault::classDef    = (zCClassDef*)0x009A4768;
#endif
#ifdef __ZZONE_H__VER3__
  zCClassDef* zCZoneReverb::classDef                = (zCClassDef*)0x009A47D8;
#endif
#ifdef __ZZONE_H__VER3__
  zCClassDef* zCZoneMusic::classDef                 = (zCClassDef*)0x009A4848;
#endif
#ifdef __ZZONE_H__VER3__
  zCClassDef* zCZoneVobFarPlane::classDef           = (zCClassDef*)0x009A48B8;
#endif
#ifdef __OMUSIC_ZONE_H__VER3__
  zCClassDef* oCZoneMusicDefault::classDef          = (zCClassDef*)0x009A4938;
#endif
#ifdef __OMUSIC_ZONE_H__VER3__
  zCClassDef* oCZoneMusic::classDef                 = (zCClassDef*)0x009A49B0;
#endif
#ifdef __ZMUSIC_CTRL_H__VER3__
  zCClassDef* zCEventMusicControler::classDef       = (zCClassDef*)0x009A4A38;
#endif
#ifdef __ZMUSIC_CTRL_H__VER3__
  zCClassDef* zCMusicControler::classDef            = (zCClassDef*)0x009A4AA8;
#endif
#ifdef __OVIEW_DIALOG_INVENTORY_H__VER3__
  zCClassDef* oCViewDialogInventory::classDef       = (zCClassDef*)0x00AACB98;
#endif
#ifdef __OVIEW_DIALOG_ITEM_H__VER3__
  zCClassDef* oCViewDialogItem::classDef            = (zCClassDef*)0x00AACC10;
#endif
#ifdef __OVIEW_DIALOG_ITEM_CONTAINER_H__VER3__
  zCClassDef* oCViewDialogItemContainer::classDef   = (zCClassDef*)0x00AACC88;
#endif
#ifdef __OVIEW_DIALOG_STEAL_CONTAINER_H__VER3__
  zCClassDef* oCViewDialogStealContainer::classDef  = (zCClassDef*)0x00AACD90;
#endif
#ifdef __OVIEW_DIALOG_TRADE_H__VER3__
  zCClassDef* oCViewDialogTrade::classDef           = (zCClassDef*)0x00AACE80;
#endif
#ifdef __ZVIEW_DIALOG_H__VER3__
  zCClassDef* zCViewDialog::classDef                = (zCClassDef*)0x00AAD138;
#endif
#ifdef __ZVIEW_DIALOG_CHOICE_H__VER3__
  zCClassDef* zCViewDialogChoice::classDef          = (zCClassDef*)0x00AAD1B0;
#endif
#ifdef __ZVIEW_DRAW_H__VER3__
  zCClassDef* zCViewDraw::classDef                  = (zCClassDef*)0x00AAD228;
#endif
#ifdef __ZVIEW_FX_H__VER3__
  zCClassDef* zCViewFX::classDef                    = (zCClassDef*)0x00AAD2A8;
#endif
#ifdef __ZVIEW_OBJECT_H__VER3__
  zCClassDef* zCViewObject::classDef                = (zCClassDef*)0x00AAD320;
#endif
#ifdef __ZVIEW_PRINT_H__VER3__
  zCClassDef* zCViewPrint::classDef                 = (zCClassDef*)0x00AAD398;
#endif
#ifdef __OAI_HUMAN_H__VER3__
  zCClassDef* oCAIHuman::classDef                   = (zCClassDef*)0x00AAD468;
#endif
#ifdef __OAI_HUMAN_H__VER3__
  zCClassDef* oCAIHuman_Stand::classDef             = (zCClassDef*)0x00AAD4D8;
#endif
#ifdef __OAI_HUMAN_H__VER3__
  zCClassDef* oCAICamera::classDef                  = (zCClassDef*)0x00AAD578;
#endif
#ifdef __OAI_SHOOT_H__VER3__
  zCClassDef* oCAIVobMove::classDef                 = (zCClassDef*)0x00AAD720;
#endif
#ifdef __OAI_SHOOT_H__VER3__
  zCClassDef* oCAIArrowBase::classDef               = (zCClassDef*)0x00AAD790;
#endif
#ifdef __OAI_SHOOT_H__VER3__
  zCClassDef* oCAIDrop::classDef                    = (zCClassDef*)0x00AAD840;
#endif
#ifdef __OAI_SHOOT_H__VER3__
  zCClassDef* oCAIArrow::classDef                   = (zCClassDef*)0x00AAD8C0;
#endif
#ifdef __OAI_SHOOT_H__VER3__
  zCClassDef* oCAISound::classDef                   = (zCClassDef*)0x00AAD940;
#endif
#ifdef __OAI_SHOOT_H__VER3__
  zCClassDef* oCAIVobMoveTorch::classDef            = (zCClassDef*)0x00AAD9D8;
#endif
#ifdef __OANI_CTRL_H__VER3__
  zCClassDef* oCAniCtrl_Human::classDef             = (zCClassDef*)0x00AADB38;
#endif
#ifdef __OITEM_H__VER3__
  zCClassDef* oCItem::classDef                      = (zCClassDef*)0x00AB1168;
#endif
#ifdef __OMOB_INTER_H__VER3__
  zCClassDef* oCMobWheel::classDef                  = (zCClassDef*)0x00AB1430;
#endif
#ifdef __OMOB_INTER_H__VER3__
  zCClassDef* oCMobBed::classDef                    = (zCClassDef*)0x00AB14A0;
#endif
#ifdef __OMOB_INTER_H__VER3__
  zCClassDef* oCMobDoor::classDef                   = (zCClassDef*)0x00AB1518;
#endif
#ifdef __OMOB_INTER_H__VER3__
  zCClassDef* oCMobLockable::classDef               = (zCClassDef*)0x00AB1598;
#endif
#ifdef __OMOB_INTER_H__VER3__
  zCClassDef* oCMobMsg::classDef                    = (zCClassDef*)0x00AB1618;
#endif
#ifdef __OMOB_INTER_H__VER3__
  zCClassDef* oCMobFire::classDef                   = (zCClassDef*)0x00AB1698;
#endif
#ifdef __OMOB_INTER_H__VER3__
  zCClassDef* oCMobItemSlot::classDef               = (zCClassDef*)0x00AB1730;
#endif
#ifdef __OMOB_INTER_H__VER3__
  zCClassDef* oCMobSwitch::classDef                 = (zCClassDef*)0x00AB17C0;
#endif
#ifdef __OMOB_INTER_H__VER3__
  zCClassDef* oCMobLadder::classDef                 = (zCClassDef*)0x00AB1840;
#endif
#ifdef __OMOB_INTER_H__VER3__
  zCClassDef* oCMobContainer::classDef              = (zCClassDef*)0x00AB18B0;
#endif
#ifdef __OMOB_INTER_H__VER3__
  zCClassDef* oCDummyVobGenerator::classDef         = (zCClassDef*)0x00AB1930;
#endif
#ifdef __OMOB_INTER_H__VER3__
  zCClassDef* oCMobInter::classDef                  = (zCClassDef*)0x00AB19A0;
#endif
#ifdef __OMOB_INTER_H__VER3__
  zCClassDef* oCMOB::classDef                       = (zCClassDef*)0x00AB1A10;
#endif
#ifdef __ONPC_H__VER3__
  zCClassDef* oCNpc::classDef                       = (zCClassDef*)0x00AB1E20;
#endif
#ifdef __ONPC_H__VER3__
  zCClassDef* oCNpcTalent::classDef                 = (zCClassDef*)0x00AB2578;
#endif
#ifdef __ONPC_MESSAGES_H__VER3__
  zCClassDef* oCMsgManipulate::classDef             = (zCClassDef*)0x00AB2860;
#endif
#ifdef __ONPC_MESSAGES_H__VER3__
  zCClassDef* oCMsgUseItem::classDef                = (zCClassDef*)0x00AB28E8;
#endif
#ifdef __ONPC_MESSAGES_H__VER3__
  zCClassDef* oCMsgConversation::classDef           = (zCClassDef*)0x00AB2980;
#endif
#ifdef __ONPC_MESSAGES_H__VER3__
  zCClassDef* oCMsgDamage::classDef                 = (zCClassDef*)0x00AB29F0;
#endif
#ifdef __ONPC_MESSAGES_H__VER3__
  zCClassDef* oCMsgAttack::classDef                 = (zCClassDef*)0x00AB2A70;
#endif
#ifdef __ONPC_MESSAGES_H__VER3__
  zCClassDef* oCMsgState::classDef                  = (zCClassDef*)0x00AB2AE0;
#endif
#ifdef __ONPC_MESSAGES_H__VER3__
  zCClassDef* oCNpcMessage::classDef                = (zCClassDef*)0x00AB2B60;
#endif
#ifdef __ONPC_MESSAGES_H__VER3__
  zCClassDef* oCMsgMagic::classDef                  = (zCClassDef*)0x00AB2BE0;
#endif
#ifdef __ONPC_MESSAGES_H__VER3__
  zCClassDef* oCMsgMovement::classDef               = (zCClassDef*)0x00AB2C50;
#endif
#ifdef __ONPC_MESSAGES_H__VER3__
  zCClassDef* oCMsgWeapon::classDef                 = (zCClassDef*)0x00AB2CC0;
#endif
#ifdef __OOBJ_FACTORY_H__VER3__
  zCClassDef* oCObjectFactory::classDef             = (zCClassDef*)0x00AB2FB0;
#endif
#ifdef __OVOB_H__VER3__
  zCClassDef* oCVob::classDef                       = (zCClassDef*)0x00AB3510;
#endif
#ifdef __OVOB_H__VER3__
  zCClassDef* oCTouchDamage::classDef               = (zCClassDef*)0x00AB3590;
#endif
#ifdef __OWORLD_H__VER3__
  zCClassDef* oCWorld::classDef                     = (zCClassDef*)0x00AB37A8;
#endif
#ifdef __ZEVENT_MAN_H__VER3__
  zCClassDef* zCEventManager::classDef              = (zCClassDef*)0x00AB3948;
#endif
#ifdef __ZWAYNET_H__VER3__
  zCClassDef* zCWayNet::classDef                    = (zCClassDef*)0x00AB6488;
#endif
#ifdef __ZWAYNET_H__VER3__
  zCClassDef* zCVobWaypoint::classDef               = (zCClassDef*)0x00AB64F8;
#endif
#ifdef __ZWAYNET_H__VER3__
  zCClassDef* zCVobStartpoint::classDef             = (zCClassDef*)0x00AB6568;
#endif
#ifdef __ZWAYNET_H__VER3__
  zCClassDef* zCWaypoint::classDef                  = (zCClassDef*)0x00AB65D8;
#endif
#ifdef __ZWAYNET_H__VER3__
  zCClassDef* zCVobSpot::classDef                   = (zCClassDef*)0x00AB6648;
#endif
}