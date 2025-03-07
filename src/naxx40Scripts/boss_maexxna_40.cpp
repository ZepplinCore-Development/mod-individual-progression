/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "CreatureScript.h"
#include "Player.h"
#include "PassiveAI.h"
#include "ScriptedCreature.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "SpellScriptLoader.h"
#include "naxxramas.h"

enum Spells
{
    SPELL_WEB_SPRAY                     = 29484,
    SPELL_POISON_SHOCK                  = 28741,
    SPELL_NECROTIC_POISON               = 54121,
    SPELL_FRENZY                        = 54123,
    SPELL_WEB_WRAP_STUN                 = 28622,
    SPELL_WEB_WRAP_SUMMON               = 28627,
    SPELL_WEB_WRAP_PACIFY_5             = 28618
};

enum Events
{
    EVENT_WEB_SPRAY                     =  1,
    EVENT_POISON_SHOCK                  =  2,
    EVENT_NECROTIC_POISON               =  3,
    EVENT_WEB_WRAP                      =  4,
    EVENT_HEALTH_CHECK                  =  5,
    EVENT_SUMMON_SPIDERLINGS            =  6,
    EVENT_WEB_WRAP_APPLY_STUN           =  7
};
enum Emotes
{
    EMOTE_SPIDERS = 0,
    EMOTE_WEB_WRAP = 1,
    EMOTE_WEB_SPRAY = 2
};

enum Misc
{
    NPC_WEB_WRAP = 351079,
    NPC_MAEXXNA_SPIDERLING = 351088
};

const Position PosWrap[7] =
{
    {3496.615f,  -3834.182f,  320.7863f},
    {3509.108f,  -3833.922f,  320.4750f},
    {3523.644f,  -3838.309f,  320.5775f},
    {3538.152f,  -3846.353f,  320.5188f},
    {3546.219f,  -3856.167f,  320.9324f},
    {3555.135f,  -3869.507f,  320.8307f},
    {3560.282f,  -3886.143f,  321.2827f}
};

struct WebTargetSelector
{
    WebTargetSelector(Unit* maexxna) : _maexxna(maexxna) {}
    bool operator()(Unit const* target) const
    {
        if (!target->IsPlayer()) // never web nonplayers (pets, guardians, etc.)
            return false;
        if (_maexxna->GetVictim() == target) // never target tank
            return false;
        if (target->HasAura(SPELL_WEB_WRAP_STUN)) // never target targets that are already webbed
            return false;
        return true;
    }

    private:
        Unit const* _maexxna;
};

class boss_maexxna_40 : public CreatureScript
{
public:
    boss_maexxna_40() : CreatureScript("boss_maexxna_40") { }

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return GetNaxxramasAI<boss_maexxna_40AI>(pCreature);
    }

    struct boss_maexxna_40AI : public BossAI
    {
        explicit boss_maexxna_40AI(Creature* c) : BossAI(c, BOSS_MAEXXNA), summons(me)
        {
            pInstance = me->GetInstanceScript();
        }

        InstanceScript* pInstance;
        EventMap events;
        SummonList summons;
        GuidList wraps;

        bool IsInRoom()
        {
            if (me->GetExactDist(3486.6f, -3890.6f, 291.8f) > 100.0f)
            {
                EnterEvadeMode();
                return false;
            }
            return true;
        }

        void Reset() override
        {
            BossAI::Reset();
            events.Reset();
            summons.DespawnAll();
            if (pInstance)
            {
                if (GameObject* go = me->GetMap()->GetGameObject(pInstance->GetGuidData(DATA_MAEXXNA_GATE)))
                {
                    go->SetGoState(GO_STATE_ACTIVE);
                }
            }
        }

        void JustEngagedWith(Unit* who) override
        {
            BossAI::JustEngagedWith(who);
            me->SetInCombatWithZone();
            events.ScheduleEvent(EVENT_WEB_WRAP, 20s);
            events.ScheduleEvent(EVENT_WEB_SPRAY, 40s);
            events.ScheduleEvent(EVENT_POISON_SHOCK, 10s);
            events.ScheduleEvent(EVENT_NECROTIC_POISON, 5s);
            events.ScheduleEvent(EVENT_HEALTH_CHECK, 1s);
            events.ScheduleEvent(EVENT_SUMMON_SPIDERLINGS, 30s);
            if (pInstance)
            {
                if (GameObject* go = me->GetMap()->GetGameObject(pInstance->GetGuidData(DATA_MAEXXNA_GATE)))
                {
                    go->SetGoState(GO_STATE_READY);
                }
            }
        }

        void JustSummoned(Creature* cr) override
        {
            if (cr->GetEntry() == NPC_MAEXXNA_SPIDERLING)
            {
                cr->SetInCombatWithZone();
                if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0))
                {
                    cr->AI()->AttackStart(target);
                }
            }
            summons.Summon(cr);
        }

        void KilledUnit(Unit* who) override
        {
            if (who->GetTypeId() == TYPEID_PLAYER && pInstance)
            {
                pInstance->SetData(DATA_IMMORTAL_FAIL, 0);
            }
        }

        void JustDied(Unit* killer) override
        {
            BossAI::JustDied(killer);
        }

        void DoCastWebWrap()
        {
            std::list<Unit*> candidates;
            SelectTargetList(candidates, 2, SelectTargetMethod::Random, 0, WebTargetSelector(me));

            std::vector<uint32> positions {0, 1, 2, 3, 4, 5, 6};
            Acore::Containers::RandomShuffle(positions);

            if (candidates.empty())
                return;

            for (int i = 0; i < 2 ; i++)
            {
                if (candidates.empty())
                    break;
                const Position &randomPos = PosWrap[positions[i]];

                auto itr = candidates.begin();

                if (candidates.size() > 1)
                    std::advance(itr, urand(0, candidates.size() - 1));

                Unit *target = *itr;
                candidates.erase(itr);

                float dx = randomPos.GetPositionX() - target->GetPositionX();
                float dy = randomPos.GetPositionY() - target->GetPositionY();
                float distXY = std::hypotf(dx, dy);

                // smooth knockback arc that avoids the ceiling
                float horizontalSpeed = distXY / 1.5f;
                float verticalSpeed = 28.0f;
                if (distXY <= 10.0f)
                    verticalSpeed = 12.0f;
                else if (distXY <= 20.0f)
                    verticalSpeed = 16.0f;
                else if (distXY <= 30.0f)
                    verticalSpeed = 20.0f;
                else if (distXY <= 40.0f)
                    verticalSpeed = 24.0f;

                target->KnockbackFrom(randomPos.GetPositionX(), randomPos.GetPositionY(), -horizontalSpeed, verticalSpeed);
                me->CastSpell(target, SPELL_WEB_WRAP_PACIFY_5, true); // pacify silence for 5 seconds

                wraps.push_back(target->GetGUID());
            }
            events.ScheduleEvent(EVENT_WEB_WRAP_APPLY_STUN, 2s);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!IsInRoom())
                return;

            if (!UpdateVictim())
                return;

            events.Update(diff);
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case EVENT_WEB_SPRAY:
                Talk(EMOTE_WEB_SPRAY);
                me->CastCustomSpell(SPELL_WEB_SPRAY, SPELLVALUE_AURA_DURATION, 10000, nullptr, true);
                events.RepeatEvent(40000);
                break;
            case EVENT_POISON_SHOCK:
            {
                int32 bp0 = 1499;
                me->CastCustomSpell(me->GetVictim(), SPELL_POISON_SHOCK, &bp0, nullptr, nullptr, false, nullptr, nullptr, me->GetGUID());
                events.RepeatEvent(10000);
                break;
            }
            case EVENT_NECROTIC_POISON:
                me->CastSpell(me->GetVictim(), SPELL_NECROTIC_POISON, false);
                events.RepeatEvent(30000);
                break;
            case EVENT_SUMMON_SPIDERLINGS:
                Talk(EMOTE_SPIDERS);
                for (uint8 i = 0; i < 10; ++i)
                {
                    me->SummonCreature(NPC_MAEXXNA_SPIDERLING, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                }
                events.RepeatEvent(40000);
                break;
            case EVENT_HEALTH_CHECK:
                if (me->GetHealthPct() < 30)
                {
                    me->CastSpell(me, SPELL_FRENZY, true);
                    break;
                }
                events.RepeatEvent(1000);
                break;
            case EVENT_WEB_WRAP:
                Talk(EMOTE_WEB_WRAP);
                // reducing limit to 1 target for the web wrap
                // at some point this should be variable based on the number of players in the raid
                for (uint8 i = 0; i < 1; ++i)
                {
                    if (Unit* target = SelectTarget(SelectTargetMethod::Random, 1, 0, true, true, -SPELL_WEB_SPRAY))
                    {
                        target->RemoveAura(SPELL_WEB_SPRAY);
                        uint8 pos = urand(0, 2);
                        if (Creature* wrap = me->SummonCreature(NPC_WEB_WRAP, PosWrap[pos].GetPositionX(), PosWrap[pos].GetPositionY(), PosWrap[pos].GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, 60000))
                        {
                            wrap->AI()->SetGUID(target->GetGUID());
                            target->GetMotionMaster()->MoveJump(PosWrap[pos].GetPositionX(), PosWrap[pos].GetPositionY(), PosWrap[pos].GetPositionZ(), 20, 20);
                        }
                    }
                }
                DoCastWebWrap();
                events.Repeat(40s);
                break;
            case EVENT_WEB_WRAP_APPLY_STUN:
            {
                for (auto& p : wraps)
                {
                    if (Player* player = ObjectAccessor::GetPlayer(*me, p))
                    {
                        player->CastSpell(player, SPELL_WEB_WRAP_STUN, true);
                    }
                }
                wraps.clear();
                break;
            }
            }
            DoMeleeAttackIfReady();
        }
    };
};

class boss_maexxna_webwrap_40 : public CreatureScript
{
public:
    boss_maexxna_webwrap_40() : CreatureScript("boss_maexxna_webwrap_40") { }

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return GetNaxxramasAI<boss_maexxna_webwrap_40AI>(pCreature);
    }

    struct boss_maexxna_webwrap_40AI : public NullCreatureAI
    {
        explicit boss_maexxna_webwrap_40AI(Creature* c) : NullCreatureAI(c) { }

        ObjectGuid victimGUID;

        void IsSummonedBy(WorldObject* summoner) override
        {
            if (!summoner)
                return;
            victimGUID = summoner->GetGUID();
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (victimGUID)
            {
                if (Unit* victim = ObjectAccessor::GetUnit(*me, victimGUID))
                {
                    victim->RemoveAurasDueToSpell(SPELL_WEB_WRAP_SUMMON);
                    victim->RemoveAurasDueToSpell(SPELL_WEB_WRAP_STUN);
                }
            }
        }

        void UpdateAI(uint32 /*diff*/) override
        {
            if (victimGUID)
            {
                if (Unit* victim = ObjectAccessor::GetUnit(*me, victimGUID))
                {
                    if (!victim->IsAlive())
                    {
                        me->KillSelf();
                    }
                }
            }
        }
    };
};

class spell_web_wrap_damage : public AuraScript
{
public:
    PrepareAuraScript(spell_web_wrap_damage);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WEB_WRAP_SUMMON });
    }

    void OnPeriodic(AuraEffect const* aurEff)
    {
        if (aurEff->GetTickNumber() == 2)
        {
            GetTarget()->CastSpell(GetTarget(), SPELL_WEB_WRAP_SUMMON, true);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_web_wrap_damage::OnPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

void AddSC_boss_maexxna_40()
{
    new boss_maexxna_40();
    new boss_maexxna_webwrap_40();
    RegisterSpellScript(spell_web_wrap_damage);
}
