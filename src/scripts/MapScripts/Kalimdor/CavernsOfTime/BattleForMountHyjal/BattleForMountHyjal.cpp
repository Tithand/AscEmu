/*
Copyright (c) 2014-2018 AscEmu Team <http://www.ascemu.org>
This file is released under the MIT license. See README-MIT for more information.
*/

#include "Setup.h"
#include "BattleForMountHyjal.h"


class BattleForMountHyjal : public InstanceScript
{
public:

    explicit BattleForMountHyjal(MapMgr* pMapMgr) : InstanceScript(pMapMgr)
    {
    }

    static InstanceScript* Create(MapMgr* pMapMgr) { return new BattleForMountHyjal(pMapMgr); }
};


void BattleForMountHyjalScripts(ScriptMgr* scriptMgr)
{
    scriptMgr->register_instance_script(534, &BattleForMountHyjal::Create);
}

