/*
Copyright (c) 2014-2017 AscEmu Team <http://www.ascemu.org/>
This file is released under the MIT license. See README-MIT for more information.
*/

#include "Setup.h"
#include "ShatteredHalls.h"

class ShatteredHalls : public InstanceScript
{
public:

    ShatteredHalls(MapMgr* pMapMgr) : InstanceScript(pMapMgr)
    {
        generateBossDataState();
    }

    static InstanceScript* Create(MapMgr* pMapMgr) { return new ShatteredHalls(pMapMgr); }

    void OnCreatureDeath(Creature* pCreature, Unit* pUnit)
    {
        setData(pCreature->GetEntry(), Finished);
    }
};


void ShatteredHallsScripts(ScriptMgr* scriptMgr)
{
    scriptMgr->register_instance_script(540, &ShatteredHalls::Create);
}