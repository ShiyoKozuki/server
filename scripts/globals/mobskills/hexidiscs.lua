-----------------------------------
--  Hexidiscs
--
--  Description: A sixfold attack damages targets in a fan-shaped area of effect.
--  Type: Physical
--  Utsusemi/Blink absorb: 6 shadows
--  Range: Unknown cone
--  Notes: Only used in "ball" form.
-----------------------------------
require("scripts/globals/monstertpmoves")
require("scripts/settings/main")
require("scripts/globals/status")
-----------------------------------
local mobskill_object = {}

-----------------------------------
-- onMobSkillCheck
-- if not in Ball form, then ignore.
-----------------------------------
mobskill_object.onMobSkillCheck = function(target, mob, skill)
    if (mob:getAnimationSub() ~=0) then
        return 1
    else
        return 0
    end
end

mobskill_object.onMobWeaponSkill = function(target, mob, skill)
    local numhits = 6
    local accmod = 1
    local dmgmod = .7
    local info = MobPhysicalMove(mob, target, skill, numhits, accmod, dmgmod, TP_NO_EFFECT)
    local dmg = MobFinalAdjustments(info.dmg, mob, skill, target, xi.attackType.PHYSICAL, xi.damageType.BLUNT, info.hitslanded)
    target:takeDamage(dmg, mob, xi.attackType.PHYSICAL, xi.damageType.BLUNT)
    return dmg
end

return mobskill_object
