-----------------------------------
-- Spell: Sentinel's Scherzo
-- Mitigates the impact of severely damaging attacks for party members within an area of effect.
-----------------------------------
require("scripts/globals/status")
require("scripts/globals/msg")
-----------------------------------
local spell_object = {}

spell_object.onMagicCastingCheck = function(caster, target, spell)
    return 0
end

spell_object.onSpellCast = function(caster, target, spell)
    local sLvl = caster:getSkillLevel(xi.skill.SINGING) -- Gets skill level of Singing
    local iLvl = caster:getWeaponSkillLevel(xi.slot.RANGED)

    local power = math.floor((sLvl+iLvl-350) / 10)

    if (power >= 45) then
        power = 45
    end

    local iBoost = caster:getMod(xi.mod.SCHERZO_EFFECT) + caster:getMod(xi.mod.ALL_SONGS_EFFECT)
    power = power + iBoost

    local duration = 120

    duration = duration * ((iBoost * 0.1) + (caster:getMod(xi.mod.SONG_DURATION_BONUS)/100) + 1)

    if (caster:hasStatusEffect(xi.effect.SOUL_VOICE)) then
        duration = duration * 2
    elseif (caster:hasStatusEffect(xi.effect.MARCATO)) then
        duration = duration * 1.5
    end
    caster:delStatusEffect(xi.effect.MARCATO)

    if (caster:hasStatusEffect(xi.effect.TROUBADOUR)) then
        duration = duration * 2
    end

    if not (target:addBardSong(caster, xi.effect.SCHERZO, power, 0, duration, caster:getID(), 0, 1)) then
        spell:setMsg(xi.msg.basic.MAGIC_NO_EFFECT)
    end

    return xi.effect.SCHERZO
end

return spell_object
