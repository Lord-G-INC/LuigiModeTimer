#include "Game/NPC/CometEventKeeper.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/System/ScenarioDataParser.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "kamek/hooks.h"

#ifdef KOR
#define sTimeLimitInfoTable __kAutoMap_80539848
#else
#define sTimeLimitInfoTable __kAutoMap_805382C0
#endif

extern GalaxyTimeLimitInfo sTimeLimitInfoTable[];

void CometEventKeeper::init() {
    initCometStatus();

    u32 timer = 0;
    if (!MR::isPlayerLuigi()) {
        if (isStartEvent("Red") || isStartEvent("Black"))
            timer = getTimeLimitFromTable(sTimeLimitInfoTable, 9) / 60;
    } else {
        GalaxyStatusAccessor gsa = MR::makeCurrentGalaxyStatusAccessor();
        if (!gsa.mScenarioData->getValueU32("LuigiModeTimer", MR::getCurrentScenarioNo(), &timer))
            timer = 0;
    }

    if (timer != 0) {
        _0 = new CometEventExecutorTimeLimit(timer);
        _0->initWithoutIter();
        _0->kill();
    }

    if (_8) {
        _4 = new GalaxyCometScreenFilter();
        _4->initWithoutIter();
        _4->_20 = 1;
        _4->setCometType(_8);
    }
}

void CometEventKeeper::endCometEvent() {
    if (_0) {
        _0->kill();
        if (_4)
            _4->_20 = 0;
    }
}

extern "C" {
    extern void init__16CometEventKeeperFv();
    extern void endCometEvent__16CometEventKeeperFv();

#if defined(USA) || defined(JPN)
    kmBranch(0x80299AE4, init__16CometEventKeeperFv);
    kmBranch(0x80299C28, endCometEvent__16CometEventKeeperFv);
#elif defined(PAL)
    kmBranch(0x80299B00, init__16CometEventKeeperFv);
    kmBranch(0x80299C44, endCometEvent__16CometEventKeeperFv);
#else // KOR
    kmBranch(0x8029A094, init__16CometEventKeeperFv);
    kmBranch(0x8029A1D8, endCometEvent__16CometEventKeeperFv);
#endif
}
