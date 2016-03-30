/*
Copyright (c) 2016 AscEmu Team <http://www.ascemu.org/>
This file is released under the MIT license. See README-MIT for more information.
*/

#include "Setup.h"
#include "Instance_TheVioletHold.h"

struct Location EventPreGuardSP[] = //PortalGuard spwns
{
    { 1888.046265f, 761.654053f, 47.667f, 2.2332f }, // [0] left
    { 1928.545532f, 803.849731f, 52.411f, 3.1223f }, // [1] center
    { 1878.080933f, 844.850281f, 43.334f, 4.2376f }  // [2] right
};

struct Location EventPreGuardWP[] = //PortalGuard WPs
{
    { 1858.386353f, 812.804993f, 42.9995f, 4.2376f }, // [0] left
    { 1861.916382f, 803.873230f, 43.6728f, 3.1223f }, // [1] center
    { 1858.678101f, 796.081970f, 43.1944f, 2.2332f }  // [2] right
};

enum DataIndex
{
    TVH_PHASE_1 = 0, // main event
    TVH_PHASE_2 = 1, // 1. portal
    TVH_PHASE_3 = 2, // 2. portal
    TVH_PHASE_4 = 3, // 3. portal
    TVH_PHASE_5 = 4, // 4. portal
    TVH_PHASE_6 = 5, // 5. portal
    TVH_PHASE_DONE = 6, // 6. portal

    TVH_END = 7
};

///////////////////////////////////////////////////////
//TheVioletHold Instance
class TheVioletHoldScript : public MoonInstanceScript
{
    friend class SinclariGossip; // Friendship forever ;-)

    private:

        int32 m_numBarrel;
        uint32 m_phaseData[TVH_END];
        int32 m_ticker;

    public:

        MOONSCRIPT_INSTANCE_FACTORY_FUNCTION(TheVioletHoldScript, MoonInstanceScript);
        TheVioletHoldScript(MapMgr* pMapMgr) : MoonInstanceScript(pMapMgr)
        {
            m_numBarrel = 0;

            for (uint8 i = 0; i < TVH_END; ++i)
                m_phaseData[i] = State_NotStarted;

            // Update every 250ms
            m_ticker = 0;
            this->SetUpdateEventFreq(250);
            this->RegisterScriptUpdateEvent();
        };

        void UpdateEvent()
        {
            ++m_ticker;
            switch (mInstance->GetWorldStatesHandler().GetWorldStateForZone(0, AREA_VIOLET_HOLD, WORLDSTATE_VH))
            {
                case State_NotStarted:
                case State_PreProgress:
                    S0_ReviveGuards();
                    break;
                case State_InProgress: printf("State: %s\n", "State_InProgress"); break;
                case State_Finished: printf("State: %s\n", "State_Finished"); break;
                case State_Performed: printf("State: %s\n", "State_Performed"); break;
            }
        }

        void S0_ReviveGuards()
        {
            auto guards = this->FindCreaturesOnMap(CN_VIOLET_HOLD_GUARD);
            for (auto guard : guards)
            {
                if (guard == nullptr || guard->isAlive())
                    continue;

                guard->Despawn(1500, 500);
            }
        }

        void SetData(uint32 pIndex, uint32 pData)
        {
            if (pIndex >= TVH_END)
                return;

            // If Data = MainEvent, set state "PreProgress". Gossip Sinclar 1 + 2
            if (pIndex == TVH_PHASE_1)
                mInstance->GetWorldStatesHandler().SetWorldStateForZone(0, AREA_VIOLET_HOLD, WORLDSTATE_VH, State_PreProgress);

            // If Data = second event, set state "InProgress". Gossip Sinclari Case 3
            if (pIndex == TVH_PHASE_2)
                mInstance->GetWorldStatesHandler().SetWorldStateForZone(0, AREA_VIOLET_HOLD, WORLDSTATE_VH, State_InProgress);

            m_phaseData[pIndex] = pData;
        };

        uint32 GetData(uint32 pIndex)
        {
            // If Phase = End/finishes, reset the Phases to 0
            if (pIndex >= TVH_END)
                return 0;

            return m_phaseData[pIndex];
        };

        void SetInstanceData(uint32 pType, uint32 pIndex, uint32 pData)
        {
            if (pType != Data_EncounterState || pIndex == 0)
                return;

            EncounterMap::iterator Iter = mEncounters.find(pIndex);
            if (Iter == mEncounters.end())
                return;

            (*Iter).second.mState = (EncounterState)pData;
        };

        uint32 GetInstanceData(uint32 pType, uint32 pIndex)
        {
            if (pType != Data_EncounterState || pIndex == 0)
                return 0;

            EncounterMap::iterator Iter = mEncounters.find(pIndex);
            if (Iter == mEncounters.end())
                return 0;

            return (*Iter).second.mState;
        };

        void OnGameObjectActivate(GameObject* pGameObject, Player* pPlayer)
        {};

        void OnPlayerEnter(Player* pPlayer)
        {
            TheVioletHoldScript* pInstance = (TheVioletHoldScript*)pPlayer->GetMapMgr()->GetScript();
            if (!pInstance)
                return;

            if (pInstance->GetInstanceData(Data_EncounterState, MAP_VIOLET_HOLD) == State_NotStarted)
            {
                mEncounters.insert(EncounterMap::value_type(MAP_VIOLET_HOLD, State_NotStarted));
            }

        }
};

#define SINCLARI_SAY_1 "Prison guards, we are leaving! These adventurers are taking over! Go go go!"
#define SINCLARY_SAY_2 "I'm locking the door. Good luck, and thank you for doing this."

///////////////////////////////////////////////////////
//Lieutnant Sinclari StartEvent
class SinclariAI : public MoonScriptCreatureAI
{
    public:

        MOONSCRIPT_FACTORY_FUNCTION(SinclariAI, MoonScriptCreatureAI);
        SinclariAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
        {
            _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
        }

        void OnReachWP(uint32 iWaypointId, bool bForwards)
        {
            switch (iWaypointId)
            {
                case 2:
                {
                    this->OnRescuePrisonGuards();
                }break;
                case 4:
                {
                    _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, SINCLARY_SAY_2);
                    _unit->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }break;
                case 5:
                {
                    TheVioletHoldScript* pInstance = (TheVioletHoldScript*)_unit->GetMapMgr()->GetScript();
                    pInstance->SetInstanceData(Data_EncounterState, MAP_VIOLET_HOLD, State_InProgress);
                    GameObject* pVioletHoldDoor = pInstance->FindClosestGameObjectOnMap(GO_PRISON_SEAL, 1822.59f, 803.93f, 44.36f);
                    if (pVioletHoldDoor != NULL)
                        pVioletHoldDoor->SetState(GO_STATE_CLOSED);
                }break;
            }
        }

        void OnRescuePrisonGuards()
        {
            _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, SINCLARI_SAY_1);
            _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHENSTOP);
        }
};


///////////////////////////////////////////////////////
//Lieutnant Sinclari Gossip and init events
//Sinclari Gossip
class SinclariGossip : public GossipScript
{
    public:

        void GossipHello(Object* pObject, Player* pPlayer)
        {
            TheVioletHoldScript* pInstance = (TheVioletHoldScript*)pPlayer->GetMapMgr()->GetScript();
            if (!pInstance)
                return;

            GossipMenu* menu;

            //Page 1: Textid and first menu item
            if (pInstance->GetInstanceData(Data_EncounterState, MAP_VIOLET_HOLD) == State_NotStarted)
            {
                objmgr.CreateGossipMenuForPlayer(&menu, pObject->GetGUID(), SINCLARI_ON_HELLO, pPlayer);
                menu->AddItem(GOSSIP_ICON_CHAT, pPlayer->GetSession()->LocalizedGossipOption(SINCLARI_ACTIVATE), 1);

                menu->SendTo(pPlayer);
            }

            //If VioletHold is started, Sinclari has this item for people who aould join.
            if (pInstance->GetInstanceData(Data_EncounterState, MAP_VIOLET_HOLD) == State_InProgress)
            {
                objmgr.CreateGossipMenuForPlayer(&menu, pObject->GetGUID(), SINCLARI_OUTSIDE, pPlayer);
                menu->AddItem(GOSSIP_ICON_CHAT, pPlayer->GetSession()->LocalizedGossipOption(SINCLARI_SEND_ME_IN), 3);

                menu->SendTo(pPlayer);
            }
        }

        void GossipSelectOption(Object* pObject, Player* pPlayer, uint32 Id, uint32 IntId, const char* Code)
        {
            TheVioletHoldScript* pInstance = (TheVioletHoldScript*)pPlayer->GetMapMgr()->GetScript();

            if (!pInstance)
                return;

            if (!pObject->IsCreature())
                return;

            Creature* pCreature = static_cast<Creature*>(pObject);

            switch (IntId)
            {
                case 0:
                    GossipHello(pObject, pPlayer);
                    break;

                case 1:
                {
                    GossipMenu* menu;
                    objmgr.CreateGossipMenuForPlayer(&menu, pObject->GetGUID(), SINCLARI_ON_FINISH, pPlayer);
                    menu->AddItem(GOSSIP_ICON_CHAT, pPlayer->GetSession()->LocalizedGossipOption(SINCLARI_GET_SAFETY), 2);
                    menu->SendTo(pPlayer);

                    // New Encounter State included
                    pInstance->SetInstanceData(Data_EncounterState, MAP_VIOLET_HOLD, State_PreProgress);
                }break;

                case 2:
                {
                    static_cast<Creature*>(pObject)->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
                    pCreature->GetAIInterface()->setMoveType(MOVEMENTTYPE_NONE);
                    //pCreature->MoveToWaypoint(1);
                    pCreature->GetAIInterface()->StopMovement(10);

                }break;

                case 3:
                {
                    Arcemu::Gossip::Menu::Complete(pPlayer);
                    pPlayer->SafeTeleport(pPlayer->GetInstanceID(), MAP_VIOLET_HOLD, 1830.531006f, 803.939758f, 44.340508f, 6.281611f);
                }break;
            }
        }
};

///////////////////////////////////////////////////////
//VH Guards
class VHGuardsAI : public MoonScriptCreatureAI
{
    public:

        MOONSCRIPT_FACTORY_FUNCTION(VHGuardsAI, MoonScriptCreatureAI);
        VHGuardsAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
        {
            _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
        }

        //WPs inserted in db.

};

///////////////////////////////////////////////////////
//Boss: Erekem
//class ErekemAI : public CreatureAIScript

///////////////////////////////////////////////////////
//Boss: Moragg
//class MoraggAI : public CreatureAIScript

///////////////////////////////////////////////////////
//Boss: Ichoron
//class IchoronAI : public CreatureAIScript

///////////////////////////////////////////////////////
//Boss: Xevozz
//class XevozzAI : public CreatureAIScript

///////////////////////////////////////////////////////
//Boss: Lavanthos
//class LavanthosAI : public CreatureAIScript

///////////////////////////////////////////////////////
//Boss: Zuramat the Obliterator
//class ZuramatTheObliteratorAI : public CreatureAIScript

///////////////////////////////////////////////////////
//Final Boss: Cyanigosa
//class CyanigosaAI : public CreatureAIScript


void SetupTheVioletHold(ScriptMgr* mgr)
{
    //Instance
    mgr->register_instance_script(MAP_VIOLET_HOLD, &TheVioletHoldScript::Create);

    //Sinclari and Guards
    mgr->register_creature_script(CN_LIEUTNANT_SINCLARI, &SinclariAI::Create);
    mgr->register_creature_script(CN_VIOLET_HOLD_GUARD, &VHGuardsAI::Create);

    //Bosses
    //mgr->register_creature_script(CN_EREKEM, &ErekemAI::Create);
    //mgr->register_creature_script(CN_MORAGG, &MoraggAI::Create);
    //mgr->register_creature_script(CN_ICHORON, &IchoronAI::Create);
    //mgr->register_creature_script(CN_XEVOZZ, &XevozzAI::Create);
    //mgr->register_creature_script(CN_LAVANTHOR, &LavanthorAI::Create);
    //mgr->register_creature_script(CN_TURAMAT_THE_OBLITERATOR, &ZuramatTheObliteratorAI::Create);
    //mgr->register_creature_script(CN_CYANIGOSA, &CyanigosaAI::Create);

    GossipScript* GSinclari = new SinclariGossip;
    mgr->register_gossip_script(CN_LIEUTNANT_SINCLARI, GSinclari);


}