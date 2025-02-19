﻿/*
===========================================================================

  Copyright (c) 2010-2015 Darkstar Dev Teams

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see http://www.gnu.org/licenses/

===========================================================================
*/

#include "ability.h"
#include "lua/luautils.h"

CAbility::CAbility(uint16 id)
{
    m_ID = id;
}

bool CAbility::isPetAbility() const
{
    return getID() >= ABILITY_HEALING_RUBY;
}

bool CAbility::isAoE() const
{
    return m_aoe == 1;
}

bool CAbility::isConal()
{
    // no abilities are conal?
    return false;
}

void CAbility::setID(uint16 id)
{
    m_ID = id;
}

void CAbility::setMobSkillID(uint16 id)
{
    m_mobskillId = id;
}

uint16 CAbility::getID() const
{
    return m_ID;
}

uint16 CAbility::getMobSkillID() const
{
    return m_mobskillId;
}

void CAbility::setJob(JOBTYPE Job)
{
    m_Job = Job;
}

void CAbility::setMeritModID(uint16 value)
{
    m_meritModID = value;
}

void CAbility::setActionType(ACTIONTYPE type)
{
    m_actionType = type;
}

JOBTYPE CAbility::getJob()
{
    return m_Job;
}

void CAbility::setLevel(uint8 level)
{
    m_level = level;
}

uint8 CAbility::getLevel() const
{
    return m_level;
}

void CAbility::setRange(float range)
{
    m_range = range;
}

float CAbility::getRange() const
{
    return m_range;
}

void CAbility::setAOE(uint8 aoe)
{
    m_aoe = aoe;
}

uint8 CAbility::getAOE() const
{
    return m_aoe;
}

void CAbility::setAnimationID(uint16 animationID)
{
    m_animationID = animationID;
}

void CAbility::setAnimationTime(duration time)
{
    m_animationTime = time;
}

void CAbility::setCastTime(duration time)
{
    m_castTime = time;
}

uint16 CAbility::getAnimationID() const
{
    return m_animationID;
}

duration CAbility::getAnimationTime()
{
    return m_animationTime;
}

duration CAbility::getCastTime()
{
    return m_castTime;
}

void CAbility::setRecastTime(uint16 recastTime)
{
    m_recastTime = (uint16)(recastTime * map_config.ability_recast_multiplier);
}

uint16 CAbility::getRecastTime() const
{
    return m_recastTime;
}

uint16 CAbility::getMeritModID() const
{
    return m_meritModID;
}

ACTIONTYPE CAbility::getActionType()
{
    return m_actionType;
}

void CAbility::setValidTarget(uint16 validTarget)
{
    m_validTarget = validTarget;
}

uint16 CAbility::getValidTarget() const
{
    return m_validTarget;
}

uint16 CAbility::getAddType() const
{
    return m_addType;
}

void CAbility::setAddType(uint16 addType)
{
    m_addType = addType;
}

const int8* CAbility::getName()
{
    return (const int8*)m_name.c_str();
}

void CAbility::setName(int8* name)
{
    m_name.clear();
    m_name.insert(0, (const char*)name);
}

uint16 CAbility::getRecastId() const
{
    return m_recastId;
}

void CAbility::setRecastId(uint16 recastId)
{
    m_recastId = recastId;
}

void CAbility::setCE(uint16 CE)
{
    m_CE = CE;
}

uint16 CAbility::getCE() const
{
    return m_CE;
}

void CAbility::setVE(uint16 VE)
{
    m_VE = VE;
}

uint16 CAbility::getVE() const
{
    return m_VE;
}

/************************************************************************
 *                                                                       *
 *  Получаем/Устанавливаем сообщение способности                         *
 *                                                                       *
 ************************************************************************/

uint16 CAbility::getMessage() const
{
    return m_message;
}

void CAbility::setMessage(uint16 message)
{
    m_message = message;
}

uint16 CAbility::getAoEMsg() const
{
    switch (m_message)
    {
        case 185:
            return 264;
        case 186:
            return 266;
        case 187:
            return 281;
        case 188:
            return 282;
        case 189:
            return 283;
        case 225:
            return 366;
        case 226:
            return 226; // no message for this... I guess there is no aoe TP drain move
        case 103:       // recover hp
        case 102:       // recover hp
        case 238:       // recover hp
        case 306:       // recover hp
        case 318:       // recover hp
            return 24;
        case 242:
            return 277;
        case 243:
            return 278;
        case 284:
            return 284; // already the aoe message
        case 370:
            return 404;
        case 362:
            return 363;
        case 378:
            return 343;
        case 224: // recovers mp
            return 276;
        case 420:
        case 424:
            return 421;
        case 422:
        case 425:
            return 423;
        case 426:
            return 427;
        case 435:
        case 437:
        case 439:
            return m_message + 1;

        default:
            return m_message;
    }
}

/************************************************************************
 *                                                                       *
 *  Реализация namespase для работы со способностями                     *
 *                                                                       *
 ************************************************************************/

namespace ability
{
    CAbility*              PAbilityList[MAX_ABILITY_ID]; // Complete Abilities List
    std::vector<CAbility*> PAbilitiesList[MAX_JOBTYPE];  // Abilities List By Job Type
    std::vector<Charge_t*> PChargesList;                 // Abilities with charges

    /************************************************************************
     *                                                                       *
     *  Load Abilities from Database                                         *
     *                                                                       *
     ************************************************************************/

    void LoadAbilitiesList()
    {
        // TODO: добавить поле message в таблицу

        memset(PAbilityList, 0, sizeof(PAbilityList));

        const char* Query = "SELECT "
                            "abilityId,"
                            "IFNULL(min_id,0),"
                            "name,"
                            "job,"
                            "level,"
                            "validTarget,"
                            "recastTime,"
                            "message1, "
                            "message2, "
                            "animation,"
                            "animationTime,"
                            "castTime,"
                            "actionType,"
                            "`range`,"
                            "isAOE,"
                            "recastId,"
                            "CE,"
                            "VE, "
                            "meritModID, "
                            "addType, "
                            "content_tag "
                            "FROM abilities LEFT JOIN (SELECT mob_skill_name, MIN(mob_skill_id) AS min_id "
                            "FROM mob_skills GROUP BY mob_skill_name) mob_skills_1 ON "
                            "abilities.name = mob_skills_1.mob_skill_name "
                            "WHERE job < %u AND abilityId < %u "
                            "ORDER BY job, level ASC";

        int32 ret = Sql_Query(SqlHandle, Query, MAX_JOBTYPE, MAX_ABILITY_ID);

        if (ret != SQL_ERROR && Sql_NumRows(SqlHandle) != 0)
        {
            while (Sql_NextRow(SqlHandle) == SQL_SUCCESS)
            {
                char* contentTag = nullptr;
                Sql_GetData(SqlHandle, 20, &contentTag, nullptr);

                if (!luautils::IsContentEnabled(contentTag))
                {
                    continue;
                }

                CAbility* PAbility = new CAbility(Sql_GetIntData(SqlHandle, 0));

                PAbility->setMobSkillID(Sql_GetIntData(SqlHandle, 1));
                PAbility->setName(Sql_GetData(SqlHandle, 2));
                PAbility->setJob((JOBTYPE)Sql_GetIntData(SqlHandle, 3));
                PAbility->setLevel(Sql_GetIntData(SqlHandle, 4));
                PAbility->setValidTarget(Sql_GetIntData(SqlHandle, 5));
                PAbility->setRecastTime(Sql_GetIntData(SqlHandle, 6));
                PAbility->setMessage(Sql_GetIntData(SqlHandle, 7));
                // PAbility->setMessage(Sql_GetIntData(SqlHandle,8));
                PAbility->setAnimationID(Sql_GetIntData(SqlHandle, 9));
                PAbility->setAnimationTime(std::chrono::milliseconds(Sql_GetIntData(SqlHandle, 10)));
                PAbility->setCastTime(std::chrono::milliseconds(Sql_GetIntData(SqlHandle, 11)));
                PAbility->setActionType(static_cast<ACTIONTYPE>(Sql_GetUIntData(SqlHandle, 12)));
                PAbility->setRange(Sql_GetFloatData(SqlHandle, 13));
                PAbility->setAOE(Sql_GetIntData(SqlHandle, 14));
                PAbility->setRecastId(Sql_GetIntData(SqlHandle, 15));
                PAbility->setCE(Sql_GetIntData(SqlHandle, 16));
                PAbility->setVE(Sql_GetIntData(SqlHandle, 17));
                PAbility->setMeritModID(Sql_GetIntData(SqlHandle, 18));
                PAbility->setAddType(Sql_GetUIntData(SqlHandle, 19));

                PAbilityList[PAbility->getID()] = PAbility;
                PAbilitiesList[PAbility->getJob()].push_back(PAbility);

                auto filename = fmt::format("./scripts/globals/abilities/{}.lua", PAbility->getName());
                if (PAbility->isPetAbility())
                {
                    filename = fmt::format("./scripts/globals/abilities/pets/{}.lua", PAbility->getName());
                }
                luautils::CacheLuaObjectFromFile(filename);
            }
        }

        const char* Query2 = "SELECT recastId, job, level, maxCharges, chargeTime, meritModId FROM abilities_charges ORDER BY job, level ASC;";

        ret = Sql_Query(SqlHandle, Query2);

        if (ret != SQL_ERROR && Sql_NumRows(SqlHandle) != 0)
        {
            while (Sql_NextRow(SqlHandle) == SQL_SUCCESS)
            {
                Charge_t* PCharge   = new Charge_t;
                PCharge->ID         = Sql_GetUIntData(SqlHandle, 0);
                PCharge->job        = (JOBTYPE)Sql_GetUIntData(SqlHandle, 1);
                PCharge->level      = Sql_GetUIntData(SqlHandle, 2);
                PCharge->maxCharges = Sql_GetUIntData(SqlHandle, 3);
                PCharge->chargeTime = Sql_GetUIntData(SqlHandle, 4);
                PCharge->merit      = Sql_GetUIntData(SqlHandle, 5);

                PChargesList.push_back(PCharge);
            }
        }
    }

    /************************************************************************
     *                                                                       *
     *  Get Ability By ID                                                    *
     *                                                                       *
     ************************************************************************/

    CAbility* GetAbility(uint16 AbilityID)
    {
        if (AbilityID < MAX_ABILITY_ID)
        {
            return PAbilityList[AbilityID];
        }
        ShowFatalError("AbilityID <%u> is out of range", AbilityID);
        return nullptr;
    }

    /************************************************************************
     *                                                                       *
     *  Получаем основную способность профессии                              *
     *                                                                       *
     ************************************************************************/

    CAbility* GetTwoHourAbility(JOBTYPE JobID)
    {
        XI_DEBUG_BREAK_IF(JobID < JOB_WAR || JobID >= MAX_JOBTYPE);

        switch (JobID)
        {
            case JOB_WAR:
                return PAbilityList[ABILITY_MIGHTY_STRIKES];
                break;
            case JOB_MNK:
                return PAbilityList[ABILITY_HUNDRED_FISTS];
                break;
            case JOB_WHM:
                return PAbilityList[ABILITY_BENEDICTION];
                break;
            case JOB_BLM:
                return PAbilityList[ABILITY_MANAFONT];
                break;
            case JOB_RDM:
                return PAbilityList[ABILITY_CHAINSPELL];
                break;
            case JOB_THF:
                return PAbilityList[ABILITY_PERFECT_DODGE];
                break;
            case JOB_PLD:
                return PAbilityList[ABILITY_INVINCIBLE];
                break;
            case JOB_DRK:
                return PAbilityList[ABILITY_BLOOD_WEAPON];
                break;
            case JOB_BST:
                return PAbilityList[ABILITY_FAMILIAR];
                break;
            case JOB_BRD:
                return PAbilityList[ABILITY_SOUL_VOICE];
                break;
            case JOB_RNG:
                return PAbilityList[ABILITY_EAGLE_EYE_SHOT];
                break;
            case JOB_SAM:
                return PAbilityList[ABILITY_MEIKYO_SHISUI];
                break;
            case JOB_NIN:
                return PAbilityList[ABILITY_MIJIN_GAKURE];
                break;
            case JOB_DRG:
                return PAbilityList[ABILITY_SPIRIT_SURGE];
                break;
            case JOB_SMN:
                return PAbilityList[ABILITY_ASTRAL_FLOW];
                break;
            case JOB_BLU:
                return PAbilityList[ABILITY_AZURE_LORE];
                break;
            case JOB_COR:
                return PAbilityList[ABILITY_WILD_CARD];
                break;
            case JOB_PUP:
                return PAbilityList[ABILITY_OVERDRIVE];
                break;
            case JOB_DNC:
                return PAbilityList[ABILITY_TRANCE];
                break;
            case JOB_SCH:
                return PAbilityList[ABILITY_TABULA_RASA];
                break;
            case JOB_GEO:
                return PAbilityList[ABILITY_BOLSTER];
                break;
            case JOB_RUN:
                return PAbilityList[ABILITY_ELEMENTAL_SFORZO];
                break;
            default:
                break;
        }
        return nullptr;
    }

    bool CanLearnAbility(CBattleEntity* PUser, uint16 AbilityID)
    {
        if (GetAbility(AbilityID) != nullptr)
        {
            uint8 Job    = PAbilityList[AbilityID]->getJob();
            uint8 JobLvl = PAbilityList[AbilityID]->getLevel();

            return ((PUser->GetMJob() == Job && PUser->GetMLevel() >= JobLvl) || (PUser->GetSJob() == Job && PUser->GetSLevel() >= JobLvl));
        }
        return false;
    }

    /************************************************************************
     *                                                                       *
     *  Get Abilities By JobID                                               *
     *                                                                       *
     ************************************************************************/

    std::vector<CAbility*> GetAbilities(JOBTYPE JobID)
    {
        return PAbilitiesList[JobID];
    }

    Charge_t* GetCharge(CBattleEntity* PUser, uint16 chargeID)
    {
        Charge_t* charge = nullptr;
        for (auto PCharge : PChargesList)
        {
            if (PCharge->ID == chargeID)
            {
                if (PUser->GetMJob() == PCharge->job)
                {
                    if (PUser->GetMLevel() >= PCharge->level)
                    {
                        charge = PCharge;
                    }
                    else
                    {
                        break;
                    }
                }
                else if (PUser->GetSJob() == PCharge->job)
                {
                    if (PUser->GetSLevel() >= PCharge->level)
                    {
                        charge = PCharge;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        return charge;
    }

    uint32 GetAbsorbMessage(uint32 msg)
    {
        if (msg == 110)
        {
            return 102;
        }
        else if (msg == 264)
        {
            return 263;
        }
        return msg;
    }
}; // namespace ability
