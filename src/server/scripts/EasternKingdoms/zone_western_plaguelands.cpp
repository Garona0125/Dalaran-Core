/*
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
SDName: Western_Plaguelands
SD%Complete: 90
SDComment: Quest support: 5097, 5098, 5216, 5219, 5222, 5225, 5229, 5231, 5233, 5235. To obtain Vitreous Focuser (could use more spesifics about gossip items)
SDCategory: Western Plaguelands
EndScriptData */

/* ContentData
npcs_dithers_and_arbington
npc_myranda_the_hag
npc_the_scourge_cauldron
npc_andorhal_tower
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "Player.h"
#include "WorldSession.h"

/*######
## npcs_dithers_and_arbington
######*/

#define GOSSIP_HDA1 "What does the Felstone Field Cauldron need?"
#define GOSSIP_HDA2 "What does the Dalson's Tears Cauldron need?"
#define GOSSIP_HDA3 "What does the Writhing Haunt Cauldron need?"
#define GOSSIP_HDA4 "What does the Gahrron's Withering Cauldron need?"

#define GOSSIP_SDA1 "Thanks, i need a Vitreous Focuser"

class npcs_dithers_and_arbington : public CreatureScript
{
public:
    npcs_dithers_and_arbington() : CreatureScript("npcs_dithers_and_arbington") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
            case GOSSIP_ACTION_TRADE:
                player->GetSession()->SendListInventory(creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+1:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SDA1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
                player->SEND_GOSSIP_MENU(3980, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SDA1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
                player->SEND_GOSSIP_MENU(3981, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SDA1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
                player->SEND_GOSSIP_MENU(3982, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+4:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SDA1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
                player->SEND_GOSSIP_MENU(3983, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+5:
                player->CLOSE_GOSSIP_MENU();
                creature->CastSpell(player, 17529, false);
                break;
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (creature->IsVendor())
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

        if (player->GetQuestRewardStatus(5237) || player->GetQuestRewardStatus(5238))
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HDA1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HDA2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HDA3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HDA4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
            player->SEND_GOSSIP_MENU(3985, creature->GetGUID());
        }
        else
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());

        return true;
    }
};

/*######
## npc_myranda_the_hag
######*/

enum Myranda
{
    ILLUSION_GOSSIP         = 4773,
    QUEST_SUBTERFUGE        = 5862,
    QUEST_IN_DREAMS         = 5944,
    SPELL_SCARLET_ILLUSION  = 17961
};

class npc_myranda_the_hag : public CreatureScript
{
public:
    npc_myranda_the_hag() : CreatureScript("npc_myranda_the_hag") { }

    bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            player->CLOSE_GOSSIP_MENU();
            player->CastSpell(player, SPELL_SCARLET_ILLUSION, false);
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (player->GetQuestStatus(QUEST_SUBTERFUGE) == QUEST_STATUS_COMPLETE &&
            player->GetQuestStatus(QUEST_IN_DREAMS) != QUEST_STATUS_COMPLETE &&
            !player->HasAura(SPELL_SCARLET_ILLUSION))
        {
            player->ADD_GOSSIP_ITEM_DB(Player::GetDefaultGossipMenuForSource(creature), 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(ILLUSION_GOSSIP, creature->GetGUID());
            return true;
        }
        else
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());

        return true;
    }
};

/*######
## npc_the_scourge_cauldron
######*/

class npc_the_scourge_cauldron : public CreatureScript
{
public:
    npc_the_scourge_cauldron() : CreatureScript("npc_the_scourge_cauldron") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_the_scourge_cauldronAI(creature);
    }

    struct npc_the_scourge_cauldronAI : public ScriptedAI
    {
        npc_the_scourge_cauldronAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override { }

        void EnterCombat(Unit* /*who*/) override { }

        void DoDie()
        {
            //summoner dies here
            me->DealDamage(me, me->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            //override any database `spawntimesecs` to prevent duplicated summons
            uint32 rTime = me->GetRespawnDelay();
            if (rTime<600)
                me->SetRespawnDelay(600);
        }

        void MoveInLineOfSight(Unit* who) override
        {
            if (!who)
                return;

            Player* player = who->ToPlayer();
            if (!player)
                return;

            switch (me->GetAreaId())
            {
                case 199:                                   //felstone
                    if (player->GetQuestStatus(5216) == QUEST_STATUS_INCOMPLETE ||
                        player->GetQuestStatus(5229) == QUEST_STATUS_INCOMPLETE)
                    {
                        me->SummonCreature(11075, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000);
                        DoDie();
                    }
                    break;
                case 200:                                   //dalson
                    if (player->GetQuestStatus(5219) == QUEST_STATUS_INCOMPLETE ||
                        player->GetQuestStatus(5231) == QUEST_STATUS_INCOMPLETE)
                    {
                        me->SummonCreature(11077, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000);
                        DoDie();
                    }
                    break;
                case 201:                                   //gahrron
                    if (player->GetQuestStatus(5225) == QUEST_STATUS_INCOMPLETE ||
                        player->GetQuestStatus(5235) == QUEST_STATUS_INCOMPLETE)
                    {
                        me->SummonCreature(11078, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000);
                        DoDie();
                    }
                    break;
                case 202:                                   //writhing
                    if (player->GetQuestStatus(5222) == QUEST_STATUS_INCOMPLETE ||
                        player->GetQuestStatus(5233) == QUEST_STATUS_INCOMPLETE)
                    {
                        me->SummonCreature(11076, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000);
                        DoDie();
                    }
                    break;
            }
        }
    };
};

/*######
##    npcs_andorhal_tower
######*/

enum AndorhalTower
{
    GO_BEACON_TORCH                             = 176093
};

class npc_andorhal_tower : public CreatureScript
{
public:
    npc_andorhal_tower() : CreatureScript("npc_andorhal_tower") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_andorhal_towerAI(creature);
    }

    struct npc_andorhal_towerAI : public ScriptedAI
    {
        npc_andorhal_towerAI(Creature* creature) : ScriptedAI(creature)
        {
            SetCombatMovement(false);
        }

        void MoveInLineOfSight(Unit* who) override

        {
            if (!who || who->GetTypeId() != TYPEID_PLAYER)
                return;

            if (me->FindNearestGameObject(GO_BEACON_TORCH, 10.0f))
                if (Player* player = who->ToPlayer())
                    player->KilledMonsterCredit(me->GetEntry(), me->GetGUID());
        }
    };
};

/*####
## Quest 5096: Scarlet Diversions
## npc_scarlet_diversions_trigger
####*/

enum QuestScarletDiversions
{
	GO_COMMAND_TENT = 176210,
	GO_SCARLET_CRUSADE_FORWARD_CAMP = 176088
};

class npc_scarlet_diversions_trigger : public CreatureScript
{
public:
	npc_scarlet_diversions_trigger() : CreatureScript("npc_scarlet_diversions_trigger") { }

	struct npc_scarlet_diversions_triggerAI : public ScriptedAI
	{
		npc_scarlet_diversions_triggerAI(Creature* creature) : ScriptedAI(creature) { }

		void UpdateAI(uint32 /*diff*/) override
		{
			if (GameObject* tent = me->FindNearestGameObject(GO_COMMAND_TENT, 20.0f))
			{
				if (tent->GetGoState() == GO_STATE_ACTIVE) // Tent is burned down
				{
					if (!me->FindNearestGameObject(GO_SCARLET_CRUSADE_FORWARD_CAMP, 20.0f))
						me->SummonGameObject(GO_SCARLET_CRUSADE_FORWARD_CAMP, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
				}
				else
				{
					if (GameObject* spellfocus = me->FindNearestGameObject(GO_SCARLET_CRUSADE_FORWARD_CAMP, 20.0f))
						spellfocus->Delete();
				}
			}
		}
	};

	CreatureAI* GetAI(Creature* creature) const override
	{
		return new npc_scarlet_diversions_triggerAI(creature);
	}
};

void AddSC_western_plaguelands()
{
    new npcs_dithers_and_arbington();
    new npc_myranda_the_hag();
    new npc_the_scourge_cauldron();
    new npc_andorhal_tower();
	new npc_scarlet_diversions_trigger();
}
