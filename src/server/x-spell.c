/* File: x-spell.c */

/*
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research,
 * and not for profit purposes provided that this copyright and statement
 * are included in all such copies.  Other copyrights may also apply.
 */

#include "mangband.h"

/*
 * Maximum number of spells per realm
 */
#define BOOKS_PER_REALM 10


#define SPELL_MAGIC_MISSILE             0
#define SPELL_DETECT_MONSTERS           1
#define SPELL_PHASE_DOOR                2
#define SPELL_LIGHT_AREA                3
#define SPELL_FIND_TRAPS_DOORS          4
#define SPELL_CURE_LIGHT_WOUNDS         5
#define SPELL_TREASURE_DETECTION        6
#define SPELL_OBJECT_DETECTION          7
#define SPELL_IDENTIFY                  8
#define SPELL_DETECT_INVISIBLE          9
#define SPELL_DETECT_ENCHANTMENT        10
#define SPELL_STINKING_CLOUD            11
#define SPELL_LIGHTNING_BOLT            12
#define SPELL_CONFUSE_MONSTER           13
#define SPELL_SLEEP_MONSTER             14
#define SPELL_WONDER                    15
#define SPELL_FROST_BOLT                16
#define SPELL_ACID_BOLT                 17
#define SPELL_FIRE_BOLT                 18
#define SPELL_TRAP_DOOR_DESTRUCTION     19
#define SPELL_SPEAR_OF_LIGHT            20
#define SPELL_TURN_STONE_TO_MUD         21
#define SPELL_DOOR_CREATION             22
#define SPELL_EARTHQUAKE                23
#define SPELL_STAIR_CREATION            24
#define SPELL_CURE_POISON               25
#define SPELL_SATISFY_HUNGER            26
#define SPELL_HEROISM                   27
#define SPELL_BERSERKER                 28
#define SPELL_HASTE_SELF                29
#define SPELL_TELEPORT_SELF             30
#define SPELL_SLOW_MONSTER              31
#define SPELL_TELEPORT_OTHER            32
#define SPELL_TELEPORT_LEVEL            33
#define SPELL_WORD_OF_RECALL            34
#define SPELL_POLYMORPH_OTHER           35
#define SPELL_SHOCK_WAVE                36
#define SPELL_EXPLOSION                 37
#define SPELL_CLOUD_KILL                38
#define SPELL_MASS_SLEEP                39
#define SPELL_BEDLAM                    40
#define SPELL_REND_SOUL                 41
#define SPELL_WORD_OF_DESTRUCTION       42
#define SPELL_CHAOS_STRIKE              43
#define SPELL_RESIST_COLD               44
#define SPELL_RESIST_FIRE               45
#define SPELL_RESIST_POISON             46
#define SPELL_RESISTANCE                47
#define SPELL_SHIELD                    48
#define SPELL_RUNE_OF_PROTECTION        49
#define SPELL_RECHARGE_ITEM_I           50
#define SPELL_ENCHANT_ARMOR             51
#define SPELL_ENCHANT_WEAPON            52
#define SPELL_RECHARGE_ITEM_II          53
#define SPELL_ELEMENTAL_BRAND           54
#define SPELL_FROST_BALL                55
#define SPELL_ACID_BALL                 56
#define SPELL_FIRE_BALL                 57
#define SPELL_ICE_STORM                 58
#define SPELL_BANISHMENT                59
#define SPELL_METEOR_SWARM              60
#define SPELL_MASS_BANISHMENT           61
#define SPELL_RIFT                      62
#define SPELL_MANA_STORM                63

/* Beginners Handbook */
#define PRAYER_DETECT_EVIL              0
#define PRAYER_CURE_LIGHT_WOUNDS        1
#define PRAYER_BLESS                    2
#define PRAYER_REMOVE_FEAR              3
#define PRAYER_CALL_LIGHT               4
#define PRAYER_FIND_TRAPS               5
#define PRAYER_DETECT_DOORS_STAIRS      6
#define PRAYER_SLOW_POISON              7

/* Words of Wisdom */
#define PRAYER_SCARE_MONSTER            8
#define PRAYER_PORTAL                   9
#define PRAYER_CURE_SERIOUS_WOUNDS     10
#define PRAYER_CHANT                   11
#define PRAYER_SANCTUARY               12
#define PRAYER_SATISFY_HUNGER          13
#define PRAYER_REMOVE_CURSE            14
#define PRAYER_RESIST_HEAT_COLD        15

/* Chants and Blessings */
#define PRAYER_NEUTRALIZE_POISON       16
#define PRAYER_ORB_OF_DRAINING         17
#define PRAYER_CURE_CRITICAL_WOUNDS    18
#define PRAYER_SENSE_INVISIBLE         19
#define PRAYER_PROTECTION_FROM_EVIL    20
#define PRAYER_EARTHQUAKE              21
#define PRAYER_SENSE_SURROUNDINGS      22
#define PRAYER_CURE_MORTAL_WOUNDS      23
#define PRAYER_TURN_UNDEAD             24

/* Exorcism and Dispelling */
#define PRAYER_PRAYER                  25
#define PRAYER_DISPEL_UNDEAD           26
#define PRAYER_HEAL                    27
#define PRAYER_DISPEL_EVIL             28
#define PRAYER_GLYPH_OF_WARDING        29
#define PRAYER_HOLY_WORD               30

/* Godly Insights */
#define PRAYER_DETECT_MONSTERS         31
#define PRAYER_DETECTION               32
#define PRAYER_PERCEPTION              33
#define PRAYER_PROBING                 34
#define PRAYER_CLAIRVOYANCE            35

/* Purifications and Healing */
#define PRAYER_CURE_SERIOUS_WOUNDS2    36
#define PRAYER_CURE_MORTAL_WOUNDS2     37
#define PRAYER_HEALING                 38
#define PRAYER_RESTORATION             39
#define PRAYER_REMEMBRANCE             40

/* Wrath of God */
#define PRAYER_DISPEL_UNDEAD2          41
#define PRAYER_DISPEL_EVIL2            42
#define PRAYER_BANISH_EVIL             43
#define PRAYER_WORD_OF_DESTRUCTION     44
#define PRAYER_ANNIHILATION            45

/* Holy Infusions */
#define PRAYER_UNBARRING_WAYS          46
#define PRAYER_RECHARGING              47
#define PRAYER_DISPEL_CURSE            48
#define PRAYER_ENCHANT_WEAPON          49
#define PRAYER_ENCHANT_ARMOUR          50
#define PRAYER_ELEMENTAL_BRAND         51

/* Ethereal openings */
#define PRAYER_BLINK                   52
#define PRAYER_TELEPORT_SELF           53
#define PRAYER_TELEPORT_OTHER          54
#define PRAYER_TELEPORT_LEVEL          55
#define PRAYER_WORD_OF_RECALL          56
#define PRAYER_ALTER_REALITY           57

/* Undead powers (Ghost Spells) */
#define GHOSTLY_BLINK                   0
#define GHOSTLY_SCARE_MONSTER           1
#define GHOSTLY_CONFUSE_MONSTER         2
#define GHOSTLY_TELEPORT_SELF           3
#define GHOSTLY_NETHER_BOLT             4
#define GHOSTLY_NETHER_BALL             5
#define GHOSTLY_DARKNESS_STORM          6

#define SPELL_CASE_AIM \
	case SPELL_MAGIC_MISSILE: \
	case SPELL_STINKING_CLOUD: \
	case SPELL_CONFUSE_MONSTER: \
	case SPELL_LIGHTNING_BOLT: \
	case SPELL_SLEEP_MONSTER: \
	case SPELL_SPEAR_OF_LIGHT: \
	case SPELL_FROST_BOLT: \
	case SPELL_TURN_STONE_TO_MUD: \
	case SPELL_WONDER: \
	case SPELL_POLYMORPH_OTHER: \
	case SPELL_FIRE_BOLT: \
	case SPELL_SLOW_MONSTER: \
	case SPELL_FROST_BALL: \
	case SPELL_TELEPORT_OTHER: \
	case SPELL_BEDLAM: \
	case SPELL_FIRE_BALL: \
	case SPELL_ACID_BOLT: \
	case SPELL_CLOUD_KILL: \
	case SPELL_ACID_BALL: \
	case SPELL_ICE_STORM: \
	case SPELL_METEOR_SWARM: \
	case SPELL_MANA_STORM: \
	case SPELL_SHOCK_WAVE: \
	case SPELL_EXPLOSION: \
	case SPELL_RIFT: \
	case SPELL_REND_SOUL: \
	case SPELL_CHAOS_STRIKE:

#define PRAYER_CASE_AIM \
	case PRAYER_TELEPORT_OTHER: \
	case PRAYER_SCARE_MONSTER: \
	case PRAYER_ORB_OF_DRAINING: \
	case PRAYER_ANNIHILATION:

#define SPELL_CASE_ITEM \
	case SPELL_ENCHANT_ARMOR: \
	case SPELL_ENCHANT_WEAPON: \
	case SPELL_ELEMENTAL_BRAND: \
	case SPELL_IDENTIFY: \
	case SPELL_RECHARGE_ITEM_I: \
	case SPELL_RECHARGE_ITEM_II:

#define SPELL_CASE_PROJ \
	case SPELL_DETECT_MONSTERS: \
	case SPELL_ELEMENTAL_BRAND: \
	case SPELL_SATISFY_HUNGER:

#define PRAYER_CASE_PROJ \
	case PRAYER_CURE_LIGHT_WOUNDS: \
	case PRAYER_CURE_SERIOUS_WOUNDS: \
	case PRAYER_CURE_CRITICAL_WOUNDS: \
	case PRAYER_CURE_MORTAL_WOUNDS: \
	case PRAYER_HEAL: \
	case PRAYER_CURE_SERIOUS_WOUNDS2: \
	case PRAYER_CURE_MORTAL_WOUNDS2: \
	case PRAYER_HEALING:

#define PRAYER_CASE_ITEM \
	case PRAYER_PERCEPTION: \
	case PRAYER_RECHARGING: \
	case PRAYER_ENCHANT_WEAPON: \
	case PRAYER_ENCHANT_ARMOUR: \
	case PRAYER_ELEMENTAL_BRAND:

#define GHOSTLY_CASE_AIM \
	case GHOSTLY_SCARE_MONSTER: \
	case GHOSTLY_CONFUSE_MONSTER: \
	case GHOSTLY_NETHER_BOLT: \
	case GHOSTLY_NETHER_BALL: \
	case GHOSTLY_DARKNESS_STORM:


/*
 * Spells in each book (mage spells then priest spells)
 */
static const s16b spell_list[2][BOOKS_PER_REALM][SPELLS_PER_BOOK] =
{
	/* Mage spells */
	{
		/* Magic for Beginners */
		{
			SPELL_MAGIC_MISSILE,
			SPELL_DETECT_MONSTERS,
			SPELL_PHASE_DOOR,
			SPELL_LIGHT_AREA,
			SPELL_TREASURE_DETECTION,
			SPELL_CURE_LIGHT_WOUNDS,
			SPELL_OBJECT_DETECTION,
			SPELL_FIND_TRAPS_DOORS,
			SPELL_STINKING_CLOUD,
		},

		/* Conjurings and Tricks */
		{
			SPELL_CONFUSE_MONSTER,
			SPELL_LIGHTNING_BOLT,
			SPELL_TRAP_DOOR_DESTRUCTION,
			SPELL_CURE_POISON,
			SPELL_SLEEP_MONSTER,
			SPELL_TELEPORT_SELF,
			SPELL_SPEAR_OF_LIGHT,
			SPELL_FROST_BOLT,
			SPELL_WONDER,
		},

		/* Incantations and Illusions */
		{
			SPELL_SATISFY_HUNGER,
			SPELL_RECHARGE_ITEM_I,
			SPELL_TURN_STONE_TO_MUD,
			SPELL_FIRE_BOLT,
			SPELL_POLYMORPH_OTHER,
			SPELL_IDENTIFY,
			SPELL_DETECT_INVISIBLE,
			SPELL_ACID_BOLT,
			SPELL_SLOW_MONSTER,
		},

		/* Sorcery and Evocations */
		{
			SPELL_FROST_BALL,
			SPELL_TELEPORT_OTHER,
			SPELL_HASTE_SELF,
			SPELL_MASS_SLEEP,
			SPELL_FIRE_BALL,
			SPELL_DETECT_ENCHANTMENT,
			-1,
			-1,
			-1,
		},

		/* Resistances of Scarabtarices */
		{
			SPELL_RESIST_COLD,
			SPELL_RESIST_FIRE,
			SPELL_RESIST_POISON,
			SPELL_RESISTANCE,
			SPELL_SHIELD,
			-1,
			-1,
			-1,
			-1,
		},

		/* Raal's Tome of Destruction */
		{
			SPELL_SHOCK_WAVE,
			SPELL_EXPLOSION,
			SPELL_CLOUD_KILL,
			SPELL_ACID_BALL,
			SPELL_ICE_STORM,
			SPELL_METEOR_SWARM,
			SPELL_RIFT,
			-1,
			-1,
		},

		/* Mordenkainen's Escapes */
		{
			SPELL_DOOR_CREATION,
			SPELL_STAIR_CREATION,
			SPELL_TELEPORT_LEVEL,
			SPELL_WORD_OF_RECALL,
			SPELL_RUNE_OF_PROTECTION,
			-1,
			-1,
			-1,
			-1,
		},

		/* Tenser's transformations */
		{
			SPELL_HEROISM,
			SPELL_BERSERKER,
			SPELL_ENCHANT_ARMOR,
			SPELL_ENCHANT_WEAPON,
			SPELL_RECHARGE_ITEM_II,
			SPELL_ELEMENTAL_BRAND,
			-1,
			-1,
			-1,
		},

		/* Kelek's Grimoire of Power */
		{
			SPELL_EARTHQUAKE,
			SPELL_BEDLAM,
			SPELL_REND_SOUL,
			SPELL_BANISHMENT,
			SPELL_WORD_OF_DESTRUCTION,
			SPELL_MASS_BANISHMENT,
			SPELL_CHAOS_STRIKE,
			SPELL_MANA_STORM,
			-1,
		},
	},

	/* Priest spells */
	{
		/* Beginners Handbook */
		{
			PRAYER_DETECT_EVIL,
			PRAYER_CURE_LIGHT_WOUNDS,
			PRAYER_BLESS,
			PRAYER_REMOVE_FEAR,
			PRAYER_CALL_LIGHT,
			PRAYER_FIND_TRAPS,
			PRAYER_DETECT_DOORS_STAIRS,
			PRAYER_SLOW_POISON,
			-1,
		},

		/* Words of Wisdom */
		{
			PRAYER_SCARE_MONSTER,
			PRAYER_PORTAL,
			PRAYER_CURE_SERIOUS_WOUNDS,
			PRAYER_CHANT,
			PRAYER_SANCTUARY,
			PRAYER_SATISFY_HUNGER,
			PRAYER_REMOVE_CURSE,
			PRAYER_RESIST_HEAT_COLD,
			-1,
		},

		/* Chants and Blessings */
		{
			PRAYER_NEUTRALIZE_POISON,
			PRAYER_ORB_OF_DRAINING,
			PRAYER_CURE_CRITICAL_WOUNDS,
			PRAYER_SENSE_INVISIBLE,
			PRAYER_PROTECTION_FROM_EVIL,
			PRAYER_EARTHQUAKE,
			PRAYER_SENSE_SURROUNDINGS,
			PRAYER_CURE_MORTAL_WOUNDS,
			PRAYER_TURN_UNDEAD,
		},

		/* Exorcism and Dispelling */
		{
			PRAYER_PRAYER,
			PRAYER_DISPEL_UNDEAD,
			PRAYER_HEAL,
			PRAYER_DISPEL_EVIL,
			PRAYER_GLYPH_OF_WARDING,
			PRAYER_HOLY_WORD,
			-1,
			-1,
			-1,
		},

		/* Ethereal openings */
		{
			PRAYER_BLINK,
			PRAYER_TELEPORT_SELF,
			PRAYER_TELEPORT_OTHER,
			PRAYER_TELEPORT_LEVEL,
			PRAYER_WORD_OF_RECALL,
			PRAYER_ALTER_REALITY,
			-1,
			-1,
			-1,
		},

		/* Godly Insights */
		{
			PRAYER_DETECT_MONSTERS,
			PRAYER_DETECTION,
			PRAYER_PERCEPTION,
			PRAYER_PROBING,
			PRAYER_CLAIRVOYANCE,
			-1,
			-1,
			-1,
			-1,
		},

		/* Purifications and Healing */
		{
			PRAYER_CURE_SERIOUS_WOUNDS2,
			PRAYER_CURE_MORTAL_WOUNDS2,
			PRAYER_HEALING,
			PRAYER_RESTORATION,
			PRAYER_REMEMBRANCE,
			-1,
			-1,
			-1,
			-1,
		},

		/* Holy Infusions */
		{
			PRAYER_UNBARRING_WAYS,
			PRAYER_RECHARGING,
			PRAYER_DISPEL_CURSE,
			PRAYER_ENCHANT_WEAPON,
			PRAYER_ENCHANT_ARMOUR,
			PRAYER_ELEMENTAL_BRAND,
			-1,
			-1,
			-1,
		},

		/* Wrath of God */
		{
			PRAYER_DISPEL_UNDEAD2,
			PRAYER_DISPEL_EVIL2,
			PRAYER_BANISH_EVIL,
			PRAYER_WORD_OF_DESTRUCTION,
			PRAYER_ANNIHILATION,
			-1,
			-1,
			-1,
			-1,
		}
	}
};

/*
 * Note that this is the same for all classes (for now).
 *
 * Such a hack....
 */
magic_type ghost_spells[8] =
{
	{  1,   1, 0, 0},
	{ 10,   2, 0, 0},
	{ 15,   3, 0, 0},
	{ 20,   5, 0, 0},
	{ 25,  10, 0, 0},
	{ 35,  60, 0, 0},
	{ 45, 100, 0, 0},

	{ 99,   0, 0, 0},
};

/*
 * Names of the spells (mage spells then priest spells)
 */
cptr spell_names[MAX_SPELL_REALMS][PY_MAX_SPELLS] =
{
	/*** Mage Spells ***/

	{
	  "Magic Missile",
	  "Detect Monsters",
	  "Phase Door",
	  "Light Area",
	  "Find Hidden Traps/Doors",
	  "Cure Light Wounds",
	  "Detect Treasure",
	  "Detect Objects",
	  "Identify",
	  "Detect Invisible",
	  "Detect Enchantment",
	  "Stinking Cloud",
	  "Lightning Bolt",
	  "Confuse Monster",
	  "Sleep Monster",
	  "Wonder",
	  "Frost Bolt",
	  "Acid Bolt",
	  "Fire Bolt",
	  "Trap/Door Destruction",
	  "Spear of Light",
	  "Turn Stone to Mud",
	  "Door Creation",
	  "Earthquake",
	  "Stair Creation",
	  "Cure Poison",
	  "Satisfy Hunger",
	  "Heroism",
	  "Berserker",
	  "Haste Self",
	  "Teleport Self",
	  "Slow Monster",
	  "Teleport Other",
	  "Teleport Level",
	  "Word of Recall",
	  "Polymorph Other",
	  "Shock Wave",
	  "Explosion",
	  "Cloudkill",
	  "Mass Sleep",
	  "Bedlam",
	  "Rend Soul",
	  "Word of Destruction",
	  "Chaos Strike",
	  "Resist Cold",
	  "Resist Fire",
	  "Resist Poison",
	  "Resistance",
	  "Shield",
	  "Rune of Protection",
	  "Lesser Recharging",
	  "Enchant Armor",
	  "Enchant Weapon",
	  "Greater Recharging",
	  "Elemental Brand",
	  "Frost Ball",
	  "Acid Ball",
	  "Fire Ball",
	  "Ice Storm",
	  "Banishment",
	  "Meteor Swarm",
	  "Mass Banishment",
	  "Rift",
	  "Mana Storm"
	},


	/*** Priest Spells ***/

	{
		/* Beginners Handbook (sval 0) */
		"Detect Evil",
		"Cure Light Wounds",
		"Bless",
		"Remove Fear",
		"Call Light",
		"Find Traps",
		"Detect Doors/Stairs",
		"Slow Poison",

		/* Words of Wisdom (sval 1) */
		"Scare Monster",
		"Portal",
		"Cure Serious Wounds",
		"Chant",
		"Sanctuary",
		"Satisfy Hunger",
		"Remove Curse",
		"Resist Heat and Cold",

		/* Chants and Blessings (sval 2) */
		"Neutralize Poison",
		"Orb of Draining",
		"Cure Critical Wounds",
		"Sense Invisible",
		"Protection from Evil",
		"Earthquake",
		"Sense Surroundings",
		"Cure Mortal Wounds",
		"Turn Undead",

		/* Exorcism and Dispelling (sval 3) */
		"Prayer",
		"Dispel Undead",
		"Heal",
		"Dispel Evil",
		"Glyph of Warding",
		"Holy Word",

		/* Godly Insights... (sval 5) */
		"Detect Monsters",
		"Detection",
		"Perception",
		"Probing",
		"Clairvoyance",

		/* Purifications and Healing (sval 6) */
		"Cure Serious Wounds",
		"Cure Mortal Wounds",
		"Healing",
		"Life Restoration",
		"Remembrance",

		/* Wrath of God (sval 8) */
		"Dispel Undead",
		"Dispel Evil",
		"Banish Evil",
		"Word of Destruction",
		"Annihilation",

		/* Holy Infusions (sval 7) */
		"Unbarring Ways",
		"Recharging",
		"Dispel Curse",
		"Enchant Weapon",
		"Enchant Armour",
		"Elemental Brand",

		/* Ethereal openings (sval 4) */
		"Blink",
		"Teleport Self",
		"Teleport Other",
		"Teleport Level",
		"Word of Recall",
		"Alter Reality",

		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)"
	},
	
	/*** Ghost abilities ***/
	{
		/* Standard set */
		"Blink",
		"Terrify",
		"Confuse",
		"Teleport",
		"Nether bolt",
		"Nether ball",
		"Darkness storm",

		/* XXX Unimplemented */
		
		/* Lich spell set (dead mage?) */
		"Teleport",
		"Paralyze",
		"Steal mana",
		"Cause wounds",
		"Steal experience",
		"Mind smash",
		"Mana storm",

		/* Death knight set (dead paladin?) */
		"Blind",
		"Terrify",
		"Summon zombie", /* XXX XXX */
		"Nether bolt",

		/* These may be used at some later date */
		/* Perhaps for different classes of ghosts */
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)",
		"(blank)"
	}
};

void spells_init()
{
	int i, j;
	for (j = 0; j < MAX_SPELL_REALMS; j++)
	{
		for (i = 0; i < PY_MAX_SPELLS; i++)
		{
			byte flag = 0;
			switch (j)
			{
				case 0:
				{
					switch(i) { SPELL_CASE_AIM flag |= PY_SPELL_AIM; 	break; default:	break; }
					switch(i) { SPELL_CASE_ITEM flag |= PY_SPELL_ITEM; 	break; default:	break; }
					switch(i) { SPELL_CASE_PROJ flag |= PY_SPELL_PROJECT; 	break; default:	break; }
					break;
				}
				case 1:
				{
					switch(i) { PRAYER_CASE_AIM flag |= PY_SPELL_AIM;	break; default:	break; }
					switch(i) { PRAYER_CASE_ITEM flag |= PY_SPELL_ITEM; 	break; default:	break; }
					switch(i) { PRAYER_CASE_PROJ flag |= PY_SPELL_PROJECT; 	break; default:	break; }
					break;
				}
				case 2:
				{
					switch(i) { GHOSTLY_CASE_AIM flag |= PY_SPELL_AIM;	break; default:	break; }
					break;
				}
			}
			spell_flags[j][i] = flag;
		}
	}
}
int get_spell_book(int Ind, int spell)
{
	player_type *p_ptr = Players[Ind];
	int i, j, index;
	
	/* Forget info about objects */
	for (i = 0; i < INVEN_TOTAL; i++)
	{
		object_type *o_ptr = &p_ptr->inventory[i];
		
		/* Skip non-objects */
		if (!o_ptr->k_idx) continue;
		
		/* Skip non-books */		
		if (o_ptr->tval != p_ptr->cp_ptr->spell_book) continue;
		
		/* Test each spell */
		for (j = 0; j < SPELLS_PER_BOOK; j++)
		{
			index = get_spell_index(Ind, o_ptr, j);
		
			/* Found nice spell */
 			if (index == spell) return i;
		}
	}
	return -1;
}

int get_spell_index(int Ind, const object_type *o_ptr, int index)
{
	player_type *p_ptr = Players[Ind];

	/* Get the item's sval */
	int sval = o_ptr->sval;

	int spell_type;

	/* Check bounds */
	if ((index < 0) || (index >= SPELLS_PER_BOOK)) return (-1);
	if ((sval < 0) || (sval >= BOOKS_PER_REALM)) return (-1);

	/* Mage or priest spells? */
	if (p_ptr->cp_ptr->spell_book == TV_MAGIC_BOOK)
		spell_type = 0;
	else
		spell_type = 1;

	return spell_list[spell_type][sval][index];
}

byte get_spell_flag(int tval, int spell, byte player_flag)
{
	if (!(player_flag & PY_SPELL_LEARNED))
		return player_flag;

	if (tval == TV_MAGIC_BOOK)
		return (spell_flags[0][spell] | player_flag);
	else if (tval == TV_PRAYER_BOOK)
		return (spell_flags[1][spell] | player_flag);
	else
		return (spell_flags[GHOST_REALM][spell] | player_flag);
}

cptr get_spell_name(int tval, int spell)
{
	if (tval == TV_MAGIC_BOOK)
		return spell_names[0][spell];
	else
		return spell_names[1][spell];
}

cptr get_spell_info(int Ind, int spell)
{
	player_type *p_ptr = Players[Ind];
	static char p[80];
	int tval;
	
	tval = p_ptr->cp_ptr->spell_book;

	/* Default */
	strcpy(p, "");

	/* Mage spells */
	if (tval == TV_MAGIC_BOOK)
	{
		int plev = p_ptr->lev;

		/* Analyze the spell */
		switch (spell)
		{
		case SPELL_MAGIC_MISSILE:
			sprintf(p, " dam %dd4", 3 + ((plev - 1) / 5));
			break;
		case SPELL_PHASE_DOOR:
			sprintf(p, " range 10");
			break;
		case SPELL_CURE_LIGHT_WOUNDS:
			sprintf(p, " heal 2d8");
			break;
		case SPELL_STINKING_CLOUD:
			sprintf(p, " dam %d", 10 + (plev / 2));
			break;
		case SPELL_LIGHTNING_BOLT:
			sprintf(p, " dam %dd6", (3 + ((plev - 5) / 6)));
			break;
		case SPELL_FROST_BOLT:
			sprintf(p, " dam %dd8", (5 + ((plev - 5) / 4)));
			break;
		case SPELL_ACID_BOLT:
			sprintf(p, " dam %dd8", (8 + ((plev - 5) / 4)));
			break;
		case SPELL_FIRE_BOLT:
			sprintf(p, " dam %dd8", (6 + ((plev - 5) / 4)));
			break;
		case SPELL_SPEAR_OF_LIGHT:
			sprintf(p, " dam 6d8");
			break;
		case SPELL_HEROISM:
			sprintf(p, " dur 25+d25");
			break;
		case SPELL_BERSERKER:
			sprintf(p, " dur 25+d25");
			break;
		case SPELL_HASTE_SELF:
			sprintf(p, " dur %d+d20", plev);
			break;
		case SPELL_TELEPORT_SELF:
			sprintf(p, " range %d", plev * 5);
			break;
		case SPELL_SHOCK_WAVE:
			sprintf(p, " dam %d", 10 + plev);
			break;
		case SPELL_EXPLOSION:
			sprintf(p, " dam %d", 20 + plev * 2);
			break;
		case SPELL_CLOUD_KILL:
			sprintf(p, " dam %d", 40 + (plev / 2));
			break;
		case SPELL_REND_SOUL:
			sprintf(p, " dam 11d%d", plev);
			break;
		case SPELL_CHAOS_STRIKE:
			sprintf(p, " dam 13d%d", plev);
			break;
		case SPELL_RESIST_COLD:
			sprintf(p, " dur 20+d20");
			break;
		case SPELL_RESIST_FIRE:
			sprintf(p, " dur 20+d20");
			break;
		case SPELL_RESIST_POISON:
			sprintf(p, " dur 20+d20");
			break;
		case SPELL_RESISTANCE:
			sprintf(p, " dur 20+d20");
			break;
		case SPELL_SHIELD:
			sprintf(p, " dur 30+d20");
			break;
		case SPELL_FROST_BALL:
			sprintf(p, " dam %d", 30 + plev);
			break;
		case SPELL_ACID_BALL:
			sprintf(p, " dam %d", 40 + plev);
			break;
		case SPELL_FIRE_BALL:
			sprintf(p, " dam %d", 55 + plev);
			break;
		case SPELL_ICE_STORM:
			sprintf(p, " dam %d", 50 + (plev * 2));
			break;
		case SPELL_METEOR_SWARM:
			sprintf(p, " dam %dx%d", 30 + plev / 2, 2 + plev / 20);
			break;
		case SPELL_RIFT:
			sprintf(p, " dam 40+%dd7", plev);
			break;
		case SPELL_MANA_STORM:
			sprintf(p, " dam %d", 300 + plev * 2);
			break;
		}
	}

	/* Priest spells */
	if (tval == TV_PRAYER_BOOK)
	{
		int plev = p_ptr->lev;

		/* Analyze the spell */
		switch (spell)
		{
			case PRAYER_CURE_LIGHT_WOUNDS:
				strcpy(p, " heal 2d10");
				break;
			case PRAYER_BLESS:
				strcpy(p, " dur 12+d12");
				break;
			case PRAYER_PORTAL:
				sprintf(p, " range %d", 3 * plev);
				break;
			case PRAYER_CURE_SERIOUS_WOUNDS:
				strcpy(p, " heal 4d10");
				break;
			case PRAYER_CHANT:
				strcpy(p, " dur 24+d24");
				break;
			case PRAYER_RESIST_HEAT_COLD:
				strcpy(p, " dur 10+d10");
				break;
			case PRAYER_ORB_OF_DRAINING:
				sprintf(p, " %d+3d6", plev +
				        (plev / ((p_ptr->cp_ptr->flags & CF_BLESS_WEAPON) ? 2 : 4)));
				break;
			case PRAYER_CURE_CRITICAL_WOUNDS:
				strcpy(p, " heal 6d10");
				break;
			case PRAYER_SENSE_INVISIBLE:
				strcpy(p, " dur 24+d24");
				break;
			case PRAYER_PROTECTION_FROM_EVIL:
				sprintf(p, " dur %d+d25", 3 * plev);
				break;
			case PRAYER_CURE_MORTAL_WOUNDS:
				strcpy(p, " heal 8d10");
				break;
			case PRAYER_PRAYER:
				strcpy(p, " dur 48+d48");
				break;
			case PRAYER_DISPEL_UNDEAD:
				sprintf(p, " dam d%d", 3 * plev);
				break;
			case PRAYER_HEAL:
				strcpy(p, " heal 300");
				break;
			case PRAYER_DISPEL_EVIL:
				sprintf(p, " dam d%d", 3 * plev);
				break;
			case PRAYER_HOLY_WORD:
				strcpy(p, " heal 1000");
				break;
			case PRAYER_CURE_SERIOUS_WOUNDS2:
				strcpy(p, " heal 4d10");
				break;
			case PRAYER_CURE_MORTAL_WOUNDS2:
				strcpy(p, " heal 8d10");
				break;
			case PRAYER_HEALING:
				strcpy(p, " heal 2000");
				break;
			case PRAYER_DISPEL_UNDEAD2:
				sprintf(p, " dam d%d", 4 * plev);
				break;
			case PRAYER_DISPEL_EVIL2:
				sprintf(p, " dam d%d", 4 * plev);
				break;
			case PRAYER_ANNIHILATION:
				strcpy(p, " dam 200");
				break;
			case PRAYER_BLINK:
				strcpy(p, " range 10");
				break;
			case PRAYER_TELEPORT_SELF:
				sprintf(p, " range %d", 8 * plev);
				break;
		}
	}

	return (p);
}


static int beam_chance(int Ind)
{
	player_type *p_ptr = Players[Ind];
	int plev = p_ptr->lev;
	return ((p_ptr->cp_ptr->flags & CF_BEAM) ? plev : (plev / 2));
}


static void spell_wonder(int Ind, int dir)
{
/* This spell should become more useful (more
   controlled) as the player gains experience levels.
   Thus, add 1/5 of the player's level to the die roll.
   This eliminates the worst effects later on, while
   keeping the results quite random.  It also allows
   some potent effects only at high level. */

	player_type *p_ptr = Players[Ind];
	
	int py = p_ptr->py;
	int px = p_ptr->px;
	int Depth = p_ptr->dun_depth;
	int plev = p_ptr->lev;
	int die = randint(100) + plev / 5;
	int beam = beam_chance(Ind);

	if (die > 100)
		msg_print(Ind, "You feel a surge of power!");
	if (die < 8) clone_monster(Ind, dir);
	else if (die < 14) speed_monster(Ind, dir);
	else if (die < 26) heal_monster(Ind, dir);
	else if (die < 31) poly_monster(Ind, dir);
	else if (die < 36)
		fire_bolt_or_beam(Ind, beam - 10, GF_MISSILE, dir,
		                  damroll(3 + ((plev - 1) / 5), 4));
	else if (die < 41) confuse_monster(Ind, dir, plev);
	else if (die < 46) fire_ball(Ind, GF_POIS, dir, 20 + (plev / 2), 3);
	else if (die < 51) lite_line(Ind, dir);
	else if (die < 56)
		fire_beam(Ind, GF_ELEC, dir, damroll(3+((plev-5)/6), 6));
	else if (die < 61)
		fire_bolt_or_beam(Ind, beam-10, GF_COLD, dir,
		                  damroll(5+((plev-5)/4), 8));
	else if (die < 66)
		fire_bolt_or_beam(Ind, beam, GF_ACID, dir,
		                  damroll(6+((plev-5)/4), 8));
	else if (die < 71)
		fire_bolt_or_beam(Ind, beam, GF_FIRE, dir,
		                  damroll(8+((plev-5)/4), 8));
	else if (die < 76) drain_life(Ind, dir, 75);
	else if (die < 81) fire_ball(Ind, GF_ELEC, dir, 30 + plev / 2, 2);
	else if (die < 86) fire_ball(Ind, GF_ACID, dir, 40 + plev, 2);
	else if (die < 91) fire_ball(Ind, GF_ICE, dir, 70 + plev, 3);
	else if (die < 96) fire_ball(Ind, GF_FIRE, dir, 80 + plev, 3);
	else if (die < 101) drain_life(Ind, dir, 100 + plev);
	else if (die < 104) earthquake(Depth, py, px, 12);
	else if (die < 106) destroy_area(Depth, py, px, 15, TRUE);
	else if (die < 108) banishment(Ind);
	else if (die < 110) dispel_monsters(Ind, 120);
	else /* RARE */
	{
		dispel_monsters(Ind, 150);
		slow_monsters(Ind);
		sleep_monsters(Ind);
		hp_player(Ind, 300);
	}
}


#define msg_spell(A) msg_format_complex_near(Ind, Ind, MSG_PY_SPELL, (A), p_ptr->name)
static bool cast_mage_spell(int Ind, int spell)
{
	player_type 	*p_ptr = Players[Ind];
	object_type		*o_ptr;
	int py = p_ptr->py;
	int px = p_ptr->px;
	int Depth = p_ptr->dun_depth;

	int dir;

	int plev = p_ptr->lev;

	/* Hack -- chance of "beam" instead of "bolt" */
	int beam = beam_chance(Ind);

	/* MAngband-specific: Projected */
	if (spell >= SPELL_PROJECTED)
	{
		if (!get_aim_dir(Ind, &dir)) return (FALSE);
		(void)project_spell_ball(Ind, dir, spell - SPELL_PROJECTED);
		return (TRUE);
	}

	/* Spells. */
	switch (spell)
	{
		case SPELL_MAGIC_MISSILE:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s fires a magic missile.");
			fire_bolt_or_beam(Ind, beam-10, GF_MISSILE, dir,
			                  damroll(3 + ((plev - 1) / 5), 4));
			break;
		}

		case SPELL_DETECT_MONSTERS:
		{
			(void)detect_creatures(Ind, TRUE);
			break;
		}

		case SPELL_PHASE_DOOR:
		{
			msg_spell("%s blinks away!");
			teleport_player(Ind, 10);
			break;
		}

		case SPELL_LIGHT_AREA:
		{
			(void)lite_area(Ind, damroll(2, (plev / 2)), (plev / 10) + 1);
			break;
		}

		case SPELL_TREASURE_DETECTION:
		{
			(void)detect_treasure(Ind);
			//(void)detect_objects_gold(Ind);
			break;
		}

		case SPELL_CURE_LIGHT_WOUNDS:
		{

			(void)hp_player(Ind, damroll(2, 8));
			(void)set_cut(Ind, p_ptr->cut - 15);
			break;
		}

		case SPELL_OBJECT_DETECTION:
		{
			(void)detect_objects_normal(Ind);
			break;
		}

		case SPELL_FIND_TRAPS_DOORS:
		{
			(void)detect_trap(Ind);//detect_traps(Ind);
			(void)detect_sdoor(Ind);//detect_doors(Ind);
			//(void)detect_stairs(Ind);
			break;
		}

		case SPELL_STINKING_CLOUD:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s casts a stinking cloud.");
			fire_ball(Ind, GF_POIS, dir, 10 + (plev / 2), 2);
			break;
		}

		case SPELL_CONFUSE_MONSTER:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s makes a complicated gesture.");
			(void)confuse_monster(Ind, dir, plev);
			break;
		}

		case SPELL_LIGHTNING_BOLT:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s casts a lightning bolt.");
			fire_beam(Ind, GF_ELEC, dir,
			          damroll(3+((plev-5)/6), 6));
			break;
		}

		case SPELL_TRAP_DOOR_DESTRUCTION:
		{
			(void)destroy_doors_touch(Ind);
			break;
		}

		case SPELL_SLEEP_MONSTER:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s gestures and mumbles calmly.");
			(void)sleep_monster(Ind, dir);
			break;
		}

		case SPELL_CURE_POISON:
		{
			(void)set_poisoned(Ind, 0);
			break;
		}

		case SPELL_TELEPORT_SELF:
		{
			msg_spell("%s teleports away!");
			teleport_player(Ind, plev * 5);
			break;
		}

		case SPELL_SPEAR_OF_LIGHT: /* spear of light */
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_print(Ind, "A line of blue shimmering light appears.");
			msg_spell("A line of blue shimmering light appears out of %s's hands.");
			lite_line(Ind, dir);
			break;
		}

		case SPELL_FROST_BOLT:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s casts a frost bolt.");
			fire_bolt_or_beam(Ind, beam-10, GF_COLD, dir,
			                  damroll(5+((plev-5)/4), 8));
			break;
		}

		case SPELL_TURN_STONE_TO_MUD:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s makes a moving gesture.");
			(void)wall_to_mud(Ind, dir);
			break;
		}

		case SPELL_SATISFY_HUNGER:
		{
			(void)set_food(Ind, PY_FOOD_MAX - 1);
			break;
		}

		case SPELL_RECHARGE_ITEM_I:
		{
			return recharge(Ind, 2 + plev / 5);
		}

		case SPELL_WONDER: /* wonder */
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			(void)spell_wonder(Ind, dir);
			break;
		}

		case SPELL_POLYMORPH_OTHER:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s discharges an everchanging blast of energy.");
			(void)poly_monster(Ind, dir);
			break;
		}

		case SPELL_IDENTIFY:
		{
			return ident_spell(Ind);
		}

		case SPELL_MASS_SLEEP:
		{
			(void)sleep_monsters(Ind);
			break;
		}

		case SPELL_FIRE_BOLT:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s casts a fire bolt.");
			fire_bolt_or_beam(Ind, beam, GF_FIRE, dir,
			                  damroll(6+((plev-5)/4), 8));
			break;
		}

		case SPELL_SLOW_MONSTER:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s makes a lengthy gesture.");
			(void)slow_monster(Ind, dir);
			break;
		}

		case SPELL_FROST_BALL:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s casts a frost ball.");
			fire_ball(Ind, GF_COLD, dir, 30 + (plev), 2);
			break;
		}

		case SPELL_RECHARGE_ITEM_II: /* greater recharging */
		{
			return recharge(Ind, 50 + plev);
		}

		case SPELL_TELEPORT_OTHER:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s makes a rush gesture.");
			(void)teleport_monster(Ind, dir);
			break;
		}

		case SPELL_BEDLAM:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s creates confusion.");
			fire_ball(Ind, GF_OLD_CONF, dir, plev, 4);
			break;
		}

		case SPELL_FIRE_BALL:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s casts a fire ball.");
			fire_ball(Ind, GF_FIRE, dir, 55 + (plev), 2);
			break;
		}

		case SPELL_WORD_OF_DESTRUCTION:
		{
			msg_spell("%s unleashes great power!"); 
			destroy_area(Depth, py, px, 15, TRUE);
			break;
		}

		case SPELL_BANISHMENT:
		{
			return banishment(Ind);
			break;
		}

		case SPELL_DOOR_CREATION:
		{
			(void)door_creation(Ind);
			break;
		}

		case SPELL_STAIR_CREATION:
		{
			(void)stair_creation(Ind);
			break;
		}

		case SPELL_TELEPORT_LEVEL:
		{
			(void)teleport_player_level(Ind);
			break;
		}

		case SPELL_EARTHQUAKE:
		{
		  	msg_spell("%s casts a spell, and the ground shakes!"); 
			earthquake(Depth, py, px, 10);
			break;
		}

		case SPELL_WORD_OF_RECALL:
		{
			o_ptr = &p_ptr->inventory[get_spell_book(Ind, spell)];
			set_recall(Ind, o_ptr);
			break;
		}

		case SPELL_ACID_BOLT:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s casts an acid bolt.");
			fire_bolt_or_beam(Ind, beam, GF_ACID, dir, damroll(8+((plev-5)/4), 8));
			break;
		}

		case SPELL_CLOUD_KILL:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s casts a cloud of death.");
			fire_ball(Ind, GF_POIS, dir, 40 + (plev / 2), 3);
			break;
		}

		case SPELL_ACID_BALL:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s casts an acid ball.");
			fire_ball(Ind, GF_ACID, dir, 40 + (plev), 2);
			break;
		}

		case SPELL_ICE_STORM:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s invokes an ice storm.");
			fire_ball(Ind, GF_ICE, dir, 50 + (plev * 2), 3);
			break;
		}

		case SPELL_METEOR_SWARM:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s casts a meteor shower.");
			fire_swarm(Ind, 2 + plev / 20, GF_METEOR, dir, 30 + plev / 2, 1);
			break;
		}

		case SPELL_MANA_STORM:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s casts a mana ball.");
			fire_ball(Ind, GF_MANA, dir, 300 + (plev * 2), 3);
			break;
		}
		case SPELL_DETECT_INVISIBLE:
		{
			(void)detect_invisible(Ind, TRUE);
			break;
		}

		case SPELL_DETECT_ENCHANTMENT:
		{
			(void)detect_objects_magic(Ind);
			break;
		}

		case SPELL_SHOCK_WAVE:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s casts a shock wave.");
			fire_ball(Ind, GF_SOUND, dir, 10 + plev, 2);
			break;
		}

		case SPELL_EXPLOSION:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s casts an explosion.");
			fire_ball(Ind, GF_SHARDS, dir, 20 + (plev * 2), 2);
			break;
		}

		case SPELL_MASS_BANISHMENT:
		{
			(void)mass_banishment(Ind);
			break;
		}

		case SPELL_RESIST_FIRE:
		{
			(void)set_oppose_fire(Ind, p_ptr->oppose_fire + randint(20) + 20);
			break;
		}

		case SPELL_RESIST_COLD:
		{
			(void)set_oppose_cold(Ind, p_ptr->oppose_cold + randint(20) + 20);
			break;
		}

		case SPELL_ELEMENTAL_BRAND: /* elemental brand */
		{
			if (!get_item(Ind, &dir, item_test(AMMO))) return (FALSE);
			(void)brand_ammo(Ind, dir);
			break;
		}

		case SPELL_RESIST_POISON:
		{
			(void)set_oppose_pois(Ind, p_ptr->oppose_pois + randint(20) + 20);
			break;
		}

		case SPELL_RESISTANCE:
		{
			int time = randint(20) + 20;
			(void)set_oppose_acid(Ind, p_ptr->oppose_acid + time);
			(void)set_oppose_elec(Ind, p_ptr->oppose_elec + time);
			(void)set_oppose_fire(Ind, p_ptr->oppose_fire + time);
			(void)set_oppose_cold(Ind, p_ptr->oppose_cold + time);
			(void)set_oppose_pois(Ind, p_ptr->oppose_pois + time);
			break;
		}

		case SPELL_HEROISM:
		{
			(void)hp_player(Ind, 10);
			(void)set_hero(Ind, p_ptr->hero + randint(25) + 25);
			(void)set_afraid(Ind, 0);
			break;
		}

		case SPELL_SHIELD:
		{
			msg_spell("%s forms a mystic shield."); 
			(void)set_shield(Ind, p_ptr->shield + randint(20) + 30);
			break;
		}

		case SPELL_BERSERKER:
		{
			msg_spell("%s enters a battle rage!");
			(void)hp_player(Ind, 30);
			(void)set_shero(Ind, p_ptr->shero + randint(25) + 25);
			(void)set_afraid(Ind, 0);
			break;
		}

		case SPELL_HASTE_SELF:
		{
			msg_spell("%s starts moving faster."); 
			if (!p_ptr->fast)
			{
				(void)set_fast(Ind, randint(20) + plev);
			}
			else
			{
				(void)set_fast(Ind, p_ptr->fast + randint(5));
			}
			break;
		}

		case SPELL_RIFT:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("Space warps in a beam from %s.");
			fire_beam(Ind, GF_GRAVITY, dir,	40 + damroll(plev, 7));
			break;
		}

		case SPELL_REND_SOUL: /* rend soul */
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s casts a nether ball.");
			fire_bolt_or_beam(Ind, beam / 4, GF_NETHER, dir, damroll(11, plev));
			break;
		}

		case SPELL_CHAOS_STRIKE: /* chaos strike */
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_spell("%s casts a ball of chaos.");
			fire_bolt_or_beam(Ind, beam, GF_CHAOS, dir, damroll(13, plev));
			break;
		}

		case SPELL_RUNE_OF_PROTECTION: /* rune of protection */
		{
			if (warding_glyph(Ind))
			{
				msg_spell("%s lays down a rune of protection."); 
			}
			break;
		}

		case SPELL_ENCHANT_ARMOR: /* enchant armor */
		{
			return enchant_spell(Ind, 0, 0, rand_int(3) + plev / 20);
		}

		case SPELL_ENCHANT_WEAPON: /* enchant weapon */
		{
			return enchant_spell(Ind, rand_int(4) + plev / 20,
			                          rand_int(4) + plev / 20, 0);
		}
	}
	
	/* Success */
	return (TRUE);
}

#define msg_prayer(A) msg_format_complex_near(Ind, Ind, MSG_PY_PRAYER, (A), p_ptr->name)
static bool cast_priest_spell(int Ind, int spell)
{
	player_type 	*p_ptr = Players[Ind];
	object_type		*o_ptr;
	int py = p_ptr->py;
	int px = p_ptr->px;
	int Depth = p_ptr->dun_depth;

	int dir;

	int plev = p_ptr->lev;
	
	switch (spell)
	{
		case PRAYER_DETECT_EVIL:
		{
			(void)detect_evil(Ind);
			break;
		}

		case PRAYER_CURE_LIGHT_WOUNDS:
		{
			(void)hp_player(Ind, damroll(2, 10));
			(void)set_cut(Ind, p_ptr->cut - 10);
			break;
		}
		/* MAngband-specific: Heal Projectile */
		case PRAYER_CURE_LIGHT_WOUNDS + SPELL_PROJECTED:
		{
			if (!get_aim_dir(Ind, &dir)) return FALSE;
			(void)heal_player_ball(Ind, dir, damroll(2, 8));
			break;
		}

		case PRAYER_BLESS:
		{
			(void)set_blessed(Ind, p_ptr->blessed + randint(12) + 12);
			break;
		}

		case PRAYER_REMOVE_FEAR:
		{
			(void)set_afraid(Ind, 0);
			break;
		}

		case PRAYER_CALL_LIGHT:
		{
			msg_prayer("%s calls light.");
			(void)lite_area(Ind, damroll(2, (plev / 2)), (plev / 10) + 1);
			break;
		}

		case PRAYER_FIND_TRAPS:
		{
			(void)detect_trap(Ind);//detect_traps(Ind)
			break;
		}

		case PRAYER_DETECT_DOORS_STAIRS:
		{
			(void)detect_sdoor(Ind);//detect_doors(Ind);
			//(void)detect_stairs();
			break;
		}

		case PRAYER_SLOW_POISON:
		{
			(void)set_poisoned(Ind, p_ptr->poisoned / 2);
			break;
		}

		case PRAYER_SCARE_MONSTER:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			(void)fear_monster(Ind, dir, plev);
			break;
		}

		case PRAYER_PORTAL:
		{
			msg_prayer("%s blinks away!");
			teleport_player(Ind, plev * 3);
			break;
		}

		case PRAYER_CURE_SERIOUS_WOUNDS:
		{
			(void)hp_player(Ind, damroll(4, 10));
			(void)set_cut(Ind, (p_ptr->cut / 2) - 20);
			break;
		}
		/* MAngband-specific: Heal Projectile */
		case PRAYER_CURE_SERIOUS_WOUNDS + SPELL_PROJECTED:
		{
			if (!get_aim_dir(Ind, &dir)) return FALSE;
			(void)heal_player_ball(Ind, dir, damroll(4, 10));
			break;
		}

		case PRAYER_CHANT:
		{
			(void)set_blessed(Ind, p_ptr->blessed + randint(24) + 24);
			break;
		}

		case PRAYER_SANCTUARY:
		{
			msg_prayer("For a brief moment, %s is enclosed by a deep blue aura.");
			(void)sleep_monsters_touch(Ind);
			break;
		}

		case PRAYER_SATISFY_HUNGER:
		{
			(void)set_food(Ind, PY_FOOD_MAX - 1);
			break;
		}

		case PRAYER_REMOVE_CURSE:
		{
			remove_curse(Ind);
			break;
		}

		case PRAYER_RESIST_HEAT_COLD:
		{
			(void)set_oppose_fire(Ind, p_ptr->oppose_fire + randint(10) + 10);
			(void)set_oppose_cold(Ind, p_ptr->oppose_cold + randint(10) + 10);
			break;
		}

		case PRAYER_NEUTRALIZE_POISON:
		{
			(void)set_poisoned(Ind, 0);
			break;
		}

		case PRAYER_ORB_OF_DRAINING:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_prayer("%s fires a holy orb!"); 
			fire_ball(Ind, GF_HOLY_ORB, dir,
			          (damroll(3, 6) + plev +
			           (plev / ((p_ptr->cp_ptr->flags & CF_BLESS_WEAPON) ? 2 : 4))),
			          ((plev < 30) ? 2 : 3));
			break;
		}

		case PRAYER_CURE_CRITICAL_WOUNDS:
		{
			(void)hp_player(Ind, damroll(6, 10));
			(void)set_cut(Ind, 0);
			break;
		}
		/* MAngband-specific: Heal Projectile */
		case PRAYER_CURE_CRITICAL_WOUNDS + SPELL_PROJECTED:
		{
			if (!get_aim_dir(Ind, &dir)) return FALSE;
			(void)heal_player_ball(Ind, dir, damroll(6, 10));
			break;
		}

		case PRAYER_SENSE_INVISIBLE:
		{
			(void)set_tim_invis(Ind, p_ptr->tim_invis + randint(24) + 24);
			break;
		}

		case PRAYER_PROTECTION_FROM_EVIL:
		{
			(void)set_protevil(Ind, p_ptr->protevil + randint(25) + 3 * p_ptr->lev);
			break;
		}

		case PRAYER_EARTHQUAKE:
		{
			msg_prayer("%s murmurs, and the ground shakes!");
			earthquake(Depth, py, px, 10);
			break;
		}

		case PRAYER_SENSE_SURROUNDINGS:
		{
			map_area(Ind);
			break;
		}

		case PRAYER_CURE_MORTAL_WOUNDS:
		{
			(void)hp_player(Ind, damroll(8, 10));
			(void)set_stun(Ind, 0);
			(void)set_cut(Ind, 0);
			break;
		}
		/* MAngband-specific: Heal Projectile */
		case PRAYER_CURE_MORTAL_WOUNDS + SPELL_PROJECTED:
		{
			if (!get_aim_dir(Ind, &dir)) return FALSE;
			(void)heal_player_ball(Ind, dir, damroll(8, 10));
			break;
		}

		case PRAYER_TURN_UNDEAD:
		{
			msg_prayer("%s tries to turn undead.");
			(void)turn_undead(Ind);
			break;
		}

		case PRAYER_PRAYER:
		{
			(void)set_blessed(Ind, p_ptr->blessed + randint(48) + 48);
			break;
		}

		case PRAYER_DISPEL_UNDEAD:
		{
			msg_prayer("%s dispells undead.");
			(void)dispel_undead(Ind, randint(plev * 3));
			break;
		}

		case PRAYER_HEAL:
		{
			(void)hp_player(Ind, 300);
			(void)set_stun(Ind, 0);
			(void)set_cut(Ind, 0);
			break;
		}
		/* MAngband-specific: Heal Projectile */
		case PRAYER_HEAL + SPELL_PROJECTED:
		{
			if (!get_aim_dir(Ind, &dir)) return FALSE;
			(void)heal_player_ball(Ind, dir, 300);
			break;
		}

		case PRAYER_DISPEL_EVIL:
		{
			msg_prayer("%s dispells evil.");
			(void)dispel_evil(Ind, randint(plev * 3));
			break;
		}

		case PRAYER_GLYPH_OF_WARDING:
		{
			if (warding_glyph(Ind))
			{
				msg_prayer("%s lays down a glyph of warding.");
			}
			break;
		}

		case PRAYER_HOLY_WORD:
		{
			msg_prayer("%s shouts the holy word.");
			(void)dispel_evil(Ind, randint(plev * 4));
			(void)hp_player(Ind, 1000);
			(void)set_afraid(Ind, 0);
			(void)set_poisoned(Ind, 0);
			(void)set_stun(Ind, 0);
			(void)set_cut(Ind, 0);
			break;
		}

		case PRAYER_DETECT_MONSTERS:
		{
			(void)detect_creatures(Ind, TRUE);
			break;
		}

		case PRAYER_DETECTION:
		{
			(void)detection(Ind);//detect_all(Ind)
			break;
		}

		case PRAYER_PERCEPTION:
		{
			return ident_spell(Ind);
		}

		case PRAYER_PROBING:
		{
			(void)probing(Ind);
			break;
		}

		case PRAYER_CLAIRVOYANCE:
		{
			msg_prayer("An image of your surroundings forms in your mind...");
			wiz_lite(Ind);
			break;
		}

		case PRAYER_CURE_SERIOUS_WOUNDS2:
		{
			(void)hp_player(Ind, damroll(4, 10));
			(void)set_cut(Ind, 0);
			break;
		}
		/* MAngband-specific: Heal Projectile */
		case PRAYER_CURE_SERIOUS_WOUNDS2 + SPELL_PROJECTED:
		{
			if (!get_aim_dir(Ind, &dir)) return FALSE;
			(void)heal_player_ball(Ind, dir, damroll(4, 10));
			break;
		}

		case PRAYER_CURE_MORTAL_WOUNDS2:
		{
			(void)hp_player(Ind, damroll(8, 10));
			(void)set_stun(Ind, 0);
			(void)set_cut(Ind, 0);
			break;
		}
		/* MAngband-specific: Heal Projectile */
		case PRAYER_CURE_MORTAL_WOUNDS2 + SPELL_PROJECTED:
		{
			if (!get_aim_dir(Ind, &dir)) return FALSE;
			(void)heal_player_ball(Ind, dir, damroll(8, 10));
			break;
		}

		case PRAYER_HEALING:
		{
			(void)hp_player(Ind, 2000);
			(void)set_stun(Ind, 0);
			(void)set_cut(Ind, 0);
			break;
		}
		/* MAngband-specific: Heal Projectile */
		case PRAYER_HEALING + SPELL_PROJECTED:
		{
			if (!get_aim_dir(Ind, &dir)) return FALSE;
			(void)heal_player_ball(Ind, dir, 2000);
			break;
		}
		/* With MAngband-specific addon: ressurect ghosts */
		case PRAYER_RESTORATION:
		{
			if (!do_scroll_life(Ind))
			{
				(void)do_res_stat(Ind, A_STR);
				(void)do_res_stat(Ind, A_INT);
				(void)do_res_stat(Ind, A_WIS);
				(void)do_res_stat(Ind, A_DEX);
				(void)do_res_stat(Ind, A_CON);
				(void)do_res_stat(Ind, A_CHR);
			}
			break;
		}
		/* With Mangband-sepcific addon: restore others */
		case PRAYER_REMEMBRANCE:
		{
			if (!do_restoreXP_other(Ind))
			{
				(void)restore_level(Ind);
			}
			break;
		}

		case PRAYER_DISPEL_UNDEAD2:
		{
			msg_prayer("%s dispells undead.");
			(void)dispel_undead(Ind, randint(plev * 4));
			break;
		}

		case PRAYER_DISPEL_EVIL2:
		{
			msg_prayer("%s dispells evil.");
			(void)dispel_evil(Ind, randint(plev * 4));
			break;
		}

		case PRAYER_BANISH_EVIL:
		{
			msg_prayer("%s speaks a holy curse on nearby evil!");
			if (banish_evil(Ind, 100))
			{
				msg_print(Ind, "The power of your god banishes evil!");
			}
			break;
		}

		case PRAYER_WORD_OF_DESTRUCTION:
		{
			msg_prayer("%s unleashes a spell of great power!");
			destroy_area(Depth, py, px, 15, TRUE);
			break;
		}

		case PRAYER_ANNIHILATION:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_prayer("%s fires a massive bolt filled with pure energy!");
			drain_life(Ind, dir, 200);
			break;
		}

		case PRAYER_UNBARRING_WAYS:
		{
			msg_prayer("%s sways his hands.");
			(void)destroy_doors_touch(Ind);
			break;
		}

		case PRAYER_RECHARGING:
		{
			return recharge(Ind, 15);
		}

		case PRAYER_DISPEL_CURSE:
		{
			(void)remove_all_curse(Ind);
			break;
		}

		case PRAYER_ENCHANT_WEAPON:
		{
			return enchant_spell(Ind, rand_int(4) + 1, rand_int(4) + 1, 0);
		}

		case PRAYER_ENCHANT_ARMOUR:
		{
			return enchant_spell(Ind, 0, 0, rand_int(3) + 2);
		}

		case PRAYER_ELEMENTAL_BRAND:
		{
			brand_weapon(Ind);
			break;
		}

		case PRAYER_BLINK:
		{
			msg_prayer("%s blinks away!");
			teleport_player(Ind, 10);
			break;
		}

		case PRAYER_TELEPORT_SELF:
		{
			msg_prayer("%s teleports away!");
			teleport_player(Ind, plev * 8);
			break;
		}

		case PRAYER_TELEPORT_OTHER:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			msg_prayer("%s prays for divine intervention.");
			(void)teleport_monster(Ind, dir);
			break;
		}

		case PRAYER_TELEPORT_LEVEL:
		{
			(void)teleport_player_level(Ind);
			break;
		}

		case PRAYER_WORD_OF_RECALL:
		{
			o_ptr = &p_ptr->inventory[get_spell_book(Ind, spell)];
			set_recall(Ind, o_ptr);
			break;
		}

		case PRAYER_ALTER_REALITY:
		{
			(void)alter_reality(Ind, FALSE);
			break;
		}

		/* Paranoia: shouldn't happen with safe clients */
		default:
		{
			msg_print(Ind, "You cannot project that spell on other players.");
			return (FALSE);
		}
	}

	/* Success */
	return (TRUE);
}


static bool cast_undead_spell(int Ind, int spell)
{
	player_type 	*p_ptr = Players[Ind];
	
	int dir;
	
	int plev = p_ptr->lev;

	switch (spell)
	{
		case GHOSTLY_BLINK:
		{
			teleport_player(Ind, 10);
			break;
		}
		case GHOSTLY_TELEPORT_SELF:
		{
			teleport_player(Ind, plev * 8);
			break;
		}
		case GHOSTLY_SCARE_MONSTER:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			(void)fear_monster(Ind, dir, plev);
			break;
		}
		case GHOSTLY_CONFUSE_MONSTER:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			confuse_monster(Ind, dir, plev);
			break;
		}
		case GHOSTLY_NETHER_BOLT:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			fire_bolt_or_beam(Ind, plev * 2, GF_NETHER, dir, 50 + damroll(5, 5) + plev);
			break;
		}
		case GHOSTLY_NETHER_BALL:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			fire_ball(Ind, GF_NETHER, dir, 100 + 2 * plev, 2);
			break;
		}
		case GHOSTLY_DARKNESS_STORM:
		{
			if (!get_aim_dir(Ind, &dir)) return (FALSE);
			fire_ball(Ind, GF_DARK, dir, plev * 5 + damroll(10, 10), 3);
			break;
		}
	}

	/* Success */
	return (TRUE);
}


bool cast_spell(int Ind, int tval, int index)
{
	player_type *p_ptr = Players[Ind];
	bool done;
	
	/* START */
	p_ptr->current_spell = index;

	if (tval == TV_MAGIC_BOOK)
	{
		done = cast_mage_spell(Ind, index);
		if (done) do_cmd_cast_fin(Ind, TRUE);
	}
	else if (tval == TV_PRAYER_BOOK)
	{
		done = cast_priest_spell(Ind, index);
		if (done) do_cmd_cast_fin(Ind, TRUE);
	}
	else
	{
		done = cast_undead_spell(Ind, index);
		if (done) do_cmd_ghost_power_fin(Ind);
	}
	
	/* DONE */
	if (done) p_ptr->current_spell = -1;

	return done;
}

/* Copy of "cast_spell", but with no "finalization" */
bool cast_spell_hack(int Ind, int tval, int index)
{
	player_type *p_ptr = Players[Ind];
	bool done;

	/* START */
	p_ptr->current_spell = index;

	if (tval == TV_MAGIC_BOOK)
	{
		done = cast_mage_spell(Ind, index);
	}
	else if (tval == TV_PRAYER_BOOK)
	{
		done = cast_priest_spell(Ind, index);
	}
	else
	{
		done = cast_undead_spell(Ind, index);
	}

	/* DONE */
	if (done) p_ptr->current_spell = -1;

	return done;
}
