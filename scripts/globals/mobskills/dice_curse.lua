-----------------------------------
-- Goblin Dice
-- Description: AoE curse.
-----------------------------------
require("scripts/globals/monstertpmoves")
require("scripts/settings/main")
require("scripts/globals/status")
-----------------------------------
local mobskill_object = {}

mobskill_object.onMobSkillCheck = function(target, mob, skill)
    return 0
end

mobskill_object.onMobWeaponSkill = function(target, mob, skill)
    local typeEffect = xi.effect.CURSE_I

    skill:setMsg(MobStatusEffectMove(mob, target, typeEffect, 30, 0, 300))

    return typeEffect
end

return mobskill_object
