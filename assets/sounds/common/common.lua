return {
    -- ArcaneRaySound = {
    --     loop = false,
    --     gain = 0.68,
    --     source_group = "SPECIALS",
    --     files = {"Sound_RayArcane.ogg"}
    -- },
    -- ArcherMusketeerShrapnelTaunt = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Muskateer_Event1.ogg"}
    -- },
    -- ArcherMusketeerSniperTaunt = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Muskateer_Snipe.ogg"}
    -- },
    -- ArcherMusketeerTaunt = {
    --     loop = false,
    --     mode = "sequence",
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Muskateer_Ready.ogg", "Muskateer_Event1.ogg", "Muskateer_Event2.ogg"}
    -- },
    -- ArcherRangerPoisonTaunt = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Ranger_Taunt1.ogg"}
    -- },
    -- ArcherRangerTaunt = {
    --     loop = false,
    --     mode = "sequence",
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Ranger_Ready.ogg", "Ranger_Taunt1.ogg", "Ranger_Taunt2.ogg"}
    -- },
    -- ArcherRangerThornTaunt = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Ranger_Taunt2.ogg"}
    -- },
    ArcherTaunt = {
        loop = false,
        gain = 0.6,
        ignore = 1.5,
        mode = "sequence",
        source_group = "TAUNTS",
        files = {"Archer_Ready.ogg", "Archer_Taunt1.ogg", "Archer_Taunt2.ogg"}
    },
    AreaAttack = {
        loop = false,
        gain = 1,
        source_group = "BULLETS",
        files = {"Sound_CommonAreaHit.ogg"}
    },
    ArrowSound = {
        loop = false,
        source_group = "BULLETS",
        files = {"Sound_ArrowRelease2.ogg", "Sound_ArrowRelease3.ogg"}
    },
    -- AxeSound = {
    --     loop = false,
    --     source_group = "BULLETS",
    --     files = {"Sound_BattleAxe.ogg"}
    -- },
    -- BarrackBarbarianDoubleAxesTaunt = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Barbarian_Move.ogg"}
    -- },
    -- BarrackBarbarianTaunt = {
    --     loop = false,
    --     mode = "sequence",
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Barbarian_Ready.ogg", "Barbarian_Taunt1.ogg", "Barbarian_Taunt2.ogg", "Barbarian_Move.ogg"}
    -- },
    -- BarrackBarbarianThrowingAxesTaunt = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Barbarian_Ready.ogg"}
    -- },
    -- BarrackBarbarianTwisterTaunt = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Barbarian_Taunt1.ogg"}
    -- },
    -- BarrackPaladinHealingTaunt = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Paladin_Ready.ogg"}
    -- },
    -- BarrackPaladinHolyStrikeTaunt = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Paladin_Taunt1.ogg"}
    -- },
    -- BarrackPaladinShieldTaunt = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Paladin_Taunt2.ogg"}
    -- },
    -- BarrackPaladinTaunt = {
    --     loop = false,
    --     mode = "sequence",
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Paladin_Ready.ogg", "Paladin_Taunt1.ogg", "Paladin_Taunt2.ogg", "Paladin_Move.ogg"}
    -- },
    BarrackTaunt = {
        loop = false,
        gain = 0.6,
        ignore = 1.5,
        mode = "sequence",
        source_group = "TAUNTS",
        files = {"Barrack_Ready.ogg", "Barrack_Taunt1.ogg", "Barrack_Taunt2.ogg", "Barrack_Move.ogg"}
    },
    -- BlackburnAmbienceBlackburn = {
    --     loop = false,
    --     gain = 0.8,
    --     source_group = "SFX",
    --     files = {"kr_ambience_cb_aquelarre.ogg", "halloween_werewolf_minormoans.ogg"}
    -- },
    -- BlackburnGhosts = {
    --     loop = false,
    --     gain = 0.8,
    --     source_group = "SFX",
    --     files = {"kr_ambience_cb_ghosts_1.ogg"}
    -- },
    -- BoltSorcererSound = {
    --     loop = false,
    --     gain = 0.68,
    --     source_group = "BULLETS",
    --     files = {"Sound_Sorcerer.ogg"}
    -- },
    BoltSound = {
        loop = false,
        gain = 0.68,
        source_group = "BULLETS",
        files = {"Sound_MageShot.ogg"}
    },
    BombExplosionSound = {
        loop = false,
        gain = 0.8,
        source_group = "EXPLOSIONS",
        files = {"Sound_Bomb1.ogg"}
    },
    BombShootSound = {
        loop = false,
        gain = 0.75,
        source_group = "EXPLOSIONS",
        files = {"Sound_EngineerShot.ogg"}
    },
    DeathBig = {
        loop = false,
        source_group = "DEATH",
        files = {"Sound_EnemyBigDead.ogg"}
    },
    DeathEplosion = {
        loop = false,
        gain = 0.4,
        source_group = "DEATH",
        files = {"Sound_EnemyExplode1.ogg"}
    },
    DeathGoblin = {
        loop = false,
        source_group = "DEATH",
        files = {"Sound_EnemyGoblinDead.ogg"}
    },
    DeathHuman = {
        loop = false,
        mode = "random",
        source_group = "DEATH",
        files = {"Sound_HumanDead1.ogg", "Sound_HumanDead2.ogg", "Sound_HumanDead3.ogg", "Sound_HumanDead4.ogg"}
    },
    -- DeathJuggernaut = {
    --     loop = false,
    --     gain = 0.9,
    --     source_group = "DEATH",
    --     files = {"Sound_JuggernautDeath.ogg"}
    -- },
    DeathOrc = {
        loop = false,
        source_group = "DEATH",
        files = {"Sound_EnemyOrcDead.ogg"}
    },
    DeathPuff = {
        loop = false,
        gain = 0.8,
        source_group = "DEATH",
        files = {"Sound_EnemyPuffDead.ogg"}
    },
    -- DeathSkeleton = {
    --     loop = false,
    --     source_group = "DEATH",
    --     files = {"Sound_EnemySkeletonBreak2.ogg"}
    -- },
    -- DeathTroll = {
    --     loop = false,
    --     source_group = "DEATH",
    --     files = {"Sound_EnemyTrollDead.ogg"}
    -- },
    -- DesintegrateSound = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "SPECIALS",
    --     files = {"Sound_ArcaneDesintegrate.ogg"}
    -- },
    -- ElfTaunt = {
    --     loop = false,
    --     mode = "sequence",
    --     gain = 0.8,
    --     source_group = "TAUNTS",
    --     files = {"Elf_Taun1.ogg", "Elf_Taun2.ogg"}
    -- },
    -- ElfBleed = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "TAUNTS",
    --     files = {"Elf_bleed.ogg"}
    -- },
    -- ElfCripple = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "TAUNTS",
    --     files = {"Elf_cripple.ogg"}
    -- },
    -- EnemyBlackburnBossDeath = {
    --     loop = false,
    --     gain = 0.5,
    --     source_group = "SFX",
    --     files = {"kr_sfx_cb_lordblackburn_death.ogg"}
    -- },
    -- EnemyBlackburnBossSpecialStomp = {
    --     loop = false,
    --     gain = 0.5,
    --     source_group = "SFX",
    --     files = {"kr_sfx_cb_lordblackburn_specialstomp.ogg"}
    -- },
    -- EnemyBlackburnBossSwing = {
    --     loop = false,
    --     gain = 0.5,
    --     source_group = "SFX",
    --     files = {"kr_sfx_cb_lordblackburn_swing.ogg"}
    -- },
    -- EnemyBlackburnGiantRat = {
    --     loop = false,
    --     gain = 0.5,
    --     source_group = "SFX",
    --     files = {"kr_sfx_cb_giantrats1.ogg", "kr_sfx_cb_giantrats2.ogg"}
    -- },
    -- EnemyBlackburnWitch = {
    --     loop = false,
    --     gain = 0.5,
    --     source_group = "SFX",
    --     files = {"kr_sfx_cb_witch_fast.ogg", "kr_sfx_cb_witch_slow.ogg"}
    -- },
    -- EnemyBlackburnWitchDeath = {
    --     loop = false,
    --     gain = 0.5,
    --     source_group = "SFX",
    --     files = {"kr_sfx_cb_witchdeath.ogg"}
    -- },
    -- EnemyChieftain = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "SFX",
    --     files = {"Sound_Chieftain.ogg"}
    -- },
    -- EnemyHealing = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "SFX",
    --     files = {"Sound_EnemyHealing.ogg"}
    -- },
    -- EnemyInfernoBossDeath = {
    --     loop = false,
    --     gain = 0.9,
    --     source_group = "SFX",
    --     files = {"inferno_boss_death.ogg"}
    -- },
    -- EnemyInfernoHorns = {
    --     loop = false,
    --     gain = 0.9,
    --     source_group = "SFX",
    --     files = {"inferno_boss_horns.ogg"}
    -- },
    -- EnemyInfernoStomp = {
    --     loop = false,
    --     gain = 0.9,
    --     source_group = "SFX",
    --     files = {"inferno_boss_stomp.ogg"}
    -- },
    -- EnemyMushroomBorn = {
    --     loop = false,
    --     gain = 0.5,
    --     source_group = "SFX",
    --     files = {"KR_sfx_mushroomcreep_nacimiento.ogg"}
    -- },
    -- EnemyMushroomBossDeath = {
    --     loop = false,
    --     gain = 0.5,
    --     source_group = "SFX",
    --     files = {"KR_sfx_mushroomboss_muerte.ogg"}
    -- },
    -- EnemyMushroomDeath = {
    --     loop = false,
    --     gain = 0.5,
    --     source_group = "SFX",
    --     files = {"KR_sfx_mushroomcreep_muerte_edit_vanzen.ogg"}
    -- },
    -- EnemyMushroomGas = {
    --     loop = false,
    --     gain = 0.5,
    --     source_group = "SFX",
    --     files = {"KR_sfx_mushroomboss_gas_op1.ogg"}
    -- },
    -- EnemyRocketeer = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "SFX",
    --     files = {"Sound_EnemyRocketeer.ogg"}
    -- },
    -- EngineerBfgClusterTaunt = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"BFG_Taunt1.ogg"}
    -- },
    -- EngineerBfgMissileTaunt = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"BFG_Taunt2.ogg"}
    -- },
    -- EngineerBfgTaunt = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"BFG_Ready.ogg"}
    -- },
    EngineerTaunt = {
        loop = false,
        gain = 0.6,
        ignore = 1.5,
        mode = "sequence",
        source_group = "TAUNTS",
        files = {"Artillery_Ready.ogg", "Artillery_Taunt1.ogg", "Artillery_Taunt2.ogg"}
    },
    -- EngineerTeslaChargedBoltTaunt = {
    --     loop = false,
    --     mode = "sequence",
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Tesla_Taunt2a.ogg", "Tesla_Taunt2b.ogg", "Tesla_Taunt2c.ogg"}
    -- },
    -- EngineerTeslaOverchargeTaunt = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Tesla_Taunt1.ogg"}
    -- },
    -- EngineerTeslaTaunt = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Tesla_Ready.ogg"}
    -- },
    -- ExtraBlackburnCrow = {
    --     loop = false,
    --     gain = 0.5,
    --     source_group = "SFX",
    --     files = {"kr_sfx_cb_crow.ogg"}
    -- },
    -- ExtraBlackburnNessie = {
    --     loop = false,
    --     gain = 0.5,
    --     source_group = "SFX",
    --     files = {"kr_sfx_cb_nessie.ogg"}
    -- },
    -- FireballHit = {
    --     loop = false,
    --     gain = 0.5,
    --     source_group = "SPECIALS",
    --     files = {"Sound_FireballHit.ogg"}
    -- },
    -- FireballRelease = {
    --     loop = false,
    --     gain = 0.5,
    --     source_group = "SPECIALS",
    --     files = {"Sound_FireballUnleash.ogg"}
    -- },
    -- GUIAchievementWin = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "GUI",
    --     files = {"Sound_AchievementWin.ogg"}
    -- },
    GUIButtonCommon = {
        loop = false,
        gain = 1,
        source_group = "GUI",
        files = {"Sound_GUIButtonCommon.ogg"}
    },
    GUIBuyUpgrade = {
        loop = false,
        gain = 0.6,
        source_group = "GUI",
        files = {"Sound_GUIBuyUpgrade.ogg"}
    },
    GUICoins = {
        loop = false,
        gain = 1,
        source_group = "GUI",
        files = {"Sound_Coins.ogg"}
    },
    GUILooseLife = {
        loop = false,
        gain = 1,
        source_group = "GUI",
        files = {"Sound_LooseLife.ogg"}
    },
    -- GUIMapNewFlah = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "GUI",
    --     files = {"Sound_MapNewFlag.ogg"}
    -- },
    GUINextWaveIncoming = {
        loop = false,
        gain = 1,
        source_group = "GUI",
        files = {"Sound_WaveIncoming.ogg"}
    },
    GUINextWaveReady = {
        loop = false,
        gain = 1,
        source_group = "GUI",
        files = {"Sound_NextWaveReady.ogg"}
    },
    -- GUINotificationClose = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "GUI",
    --     files = {"Sound_NotificationClose.ogg"}
    -- },
    -- GUINotificationOpen = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "GUI",
    --     files = {"Sound_NotificationOpen.ogg"}
    -- },
    -- GUINotificationPaperOver = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "GUI",
    --     files = {"Sound_NotificationPaperOver.ogg"}
    -- },
    -- GUINotificationSecondLevel = {
    --     loop = false,
    --     gain = 0.8,
    --     source_group = "GUI",
    --     files = {"Sound_NotificationSecondLevel.ogg"}
    -- },
    GUIPlaceRallyPoint = {
        loop = false,
        gain = 0.8,
        source_group = "GUI",
        files = {"Sound_RallyPointPlaced.ogg"}
    },
    -- GUIQuestCompleted = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "GUI",
    --     files = {"Sound_QuestCompleted.ogg"}
    -- },
    -- GUIQuestFailed = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "GUI",
    --     files = {"Sound_QuestFailed.ogg"}
    -- },
    -- GUIQuickMenuOpen = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "GUI",
    --     files = {"Sound_GUIOpenTowerMenu.ogg"}
    -- },
    -- GUIQuickMenuOver = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "GUI",
    --     files = {"Sound_GUIMouseOverTowerIcon.ogg"}
    -- },
    -- GUISpellRefresh = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "GUI",
    --     files = {"Sound_SpellRefresh.ogg"}
    -- },
    -- GUISpellSelect = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "GUI",
    --     files = {"Sound_SpellSelect.ogg"}
    -- },
    GUITowerBuilding = {
        loop = false,
        gain = 1,
        source_group = "GUI",
        files = {"Sound_TowerBuilding.ogg"}
    },
    GUITowerOpenDoor = {
        loop = false,
        gain = 1,
        source_group = "GUI",
        files = {"Sound_TowerOpenDoor.ogg"}
    },
    GUITowerSell = {
        loop = false,
        gain = 1,
        source_group = "GUI",
        files = {"Sound_TowerSell.ogg"}
    },
    -- GUITransitionClose = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "GUI",
    --     files = {"GUITransitionClose.ogg"}
    -- },
    -- GUITransitionOpen = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "GUI",
    --     files = {"GUITransitionOpen.ogg"}
    -- },
    GUIWinStars = {
        loop = false,
        gain = 1,
        source_group = "GUI",
        files = {"Sound_WinStars.ogg"}
    },
    -- GuimapNewRoad = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "GUI",
    --     files = {"Sound_MapRoad.ogg"}
    -- },
    -- HWAbominationExplosion = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "SFX",
    --     files = {"halloween_abominationexplosion.ogg"}
    -- },
    -- HWAlphaWolf = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "SFX",
    --     files = {"halloween_werewolf_alfawolf.ogg"}
    -- },
    -- HWGhosts = {
    --     loop = false,
    --     gain = 0.8,
    --     ignore = 10,
    --     source_group = "SFX",
    --     files = {"halloween_werewolf_minormoans.ogg"}
    -- },
    -- HWWerewolfTransformation = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "SFX",
    --     files = {"halloween_werewolf_transformation.ogg"}
    -- },
    -- HealingSound = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "SPECIALS",
    --     files = {"Sound_PaladinHeal.ogg"}
    -- },
    HitSound = {
        loop = false,
        gain = 0.15,
        source_group = "BULLETS",
        files = {"Sound_ArrowHit2.ogg", "Sound_ArrowHit3.ogg"}
    },
    -- JtAttack = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "BULLETS",
    --     files = {"Sound_EnemyJtAttack.ogg"}
    -- },
    -- JtDeath = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "BULLETS",
    --     files = {"Sound_EnemyJtDeath.ogg"}
    -- },
    -- JtEat = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "BULLETS",
    --     files = {"Sound_EnemyJtEat.ogg"}
    -- },
    -- JtExplode = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "BULLETS",
    --     files = {"Sound_EnemyJtExplode.ogg"}
    -- },
    -- JtHitIce = {
    --     loop = false,
    --     mode = "sequence",
    --     gain = 1,
    --     source_group = "SFX",
    --     files = {"Sound_HitIce1.ogg", "Sound_HitIce2.ogg", "Sound_HitIce3.ogg"}
    -- },
    -- JtRest = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "BULLETS",
    --     files = {"Sound_EnemyJtRest.ogg"}
    -- },
    -- MageArcaneDesintegrateTaunt = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Arcane_Taunt2.ogg"}
    -- },
    -- MageArcaneTaunt = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Arcane_Ready.ogg"}
    -- },
    -- MageArcaneTeleporthTaunt = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Arcane_Taunt1.ogg"}
    -- },
    -- MageSorcererAshesToAshesTaunt = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Sorcerer_Taunt2.ogg"}
    -- },
    -- MageSorcererTaunt = {
    --     loop = false,
    --     mode = "sequence",
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Sorcerer_Ready.ogg", "Sorcerer_Taunt1.ogg", "Sorcerer_Taunt2.ogg"}
    -- },
    MageTaunt = {
        loop = false,
        gain = 0.6,
        ignore = 1.5,
        mode = "sequence",
        source_group = "TAUNTS",
        files = {"Mage_Ready.ogg", "Mage_Taunt1.ogg", "Mage_Taunt2.ogg"}
    },
    -- PirateBombShootSound = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "EXPLOSIONS",
    --     files = {"Sound_EngineerShot.ogg"}
    -- },
    -- PolymorphSound = {
    --     loop = false,
    --     gain = 0.9,
    --     source_group = "SPECIALS",
    --     files = {"Sound_Polimorph.ogg"}
    -- },
    -- ReinforcementTaunt = {
    --     loop = false,
    --     mode = "sequence",
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Reinforcements_Event1.ogg", "Reinforcements_Event2.ogg", "Reinforcements_Event3.ogg",
    --              "Reinforcements_Event4.ogg"}
    -- },
    -- RockElementalDeath = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Sound_RockElementalDeath.ogg"}
    -- },
    -- RockElementalRally = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "TAUNTS",
    --     files = {"Sound_RockElementalRally.ogg"}
    -- },
    -- RocketLaunchSound = {
    --     loop = false,
    --     gain = 0.8,
    --     source_group = "BULLETS",
    --     files = {"Sound_RocketLaunt.ogg"}
    -- },
    -- SasquashRally = {
    --     loop = false,
    --     gain = 0.8,
    --     source_group = "TAUNTS",
    --     files = {"Sound_TowerSoldierSasquashReady.ogg"}
    -- },
    -- SasquashReady = {
    --     loop = false,
    --     gain = 0.8,
    --     source_group = "TAUNTS",
    --     files = {"Sound_TowerSoldierSasquashRally.ogg"}
    -- },
    -- Sheep = {
    --     loop = false,
    --     gain = 0.6,
    --     ignore = 1.5,
    --     source_group = "TAUNTS",
    --     files = {"Sound_Sheep.ogg"}
    -- },
    -- ShotgunSound = {
    --     loop = false,
    --     source_group = "BULLETS",
    --     files = {"Sound_Shootgun.ogg"}
    -- },
    -- ShrapnelSound = {
    --     loop = false,
    --     source_group = "BULLETS",
    --     files = {"Sound_Shrapnel.ogg"}
    -- },
    -- SniperSound = {
    --     loop = false,
    --     source_group = "BULLETS",
    --     files = {"Sound_Sniper.ogg"}
    -- },
    -- SpiderAttack = {
    --     loop = false,
    --     gain = 0.6,
    --     ignore = 1.5,
    --     source_group = "BULLETS",
    --     files = {"Sound_SpiderAttack2.ogg"}
    -- },
    -- TeleporthSound = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "SPECIALS",
    --     files = {"Sound_Teleport.ogg"}
    -- },
    -- TeslaAttack = {
    --     loop = false,
    --     gain = 0.6,
    --     source_group = "BULLETS",
    --     files = {"Sound_Tesla_attack_1.ogg", "Sound_Tesla_attack_2.ogg"}
    -- },
    -- ThornSound = {
    --     loop = false,
    --     gain = 0.8,
    --     source_group = "SPECIALS",
    --     files = {"Sound_Thorn.ogg"}
    -- },
    -- VeznanAttack = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "BULLETS",
    --     files = {"Sound_EnemyVeznan_attack.ogg"}
    -- },
    -- VeznanDeath = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "BULLETS",
    --     files = {"Sound_EnemyVeznan_death.ogg"}
    -- },
    -- VeznanDemonFire = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "BULLETS",
    --     files = {"Sound_EnemyVeznan_demonFire.ogg"}
    -- },
    -- VeznanHoldCast = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "BULLETS",
    --     files = {"Sound_SpellTowerHold_Cast.ogg"}
    -- },
    -- VeznanHoldDissipate = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "BULLETS",
    --     files = {"Sound_SpellTowerHold_Dissipate.ogg"}
    -- },
    -- VeznanHoldHit = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "BULLETS",
    --     files = {"Sound_SpellTowerHold_Hit.ogg"}
    -- },
    -- VeznanHoldTrap = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "BULLETS",
    --     files = {"Sound_SpellTowerHold_Trap.ogg"}
    -- },
    -- VeznanPortalIn = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "BULLETS",
    --     files = {"Sound_DemonPortal_Telein.ogg"}
    -- },
    -- VeznanPortalSummon = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "BULLETS",
    --     files = {"Sound_DemonPortal_Summon.ogg"}
    -- },
    -- VeznanToDemon = {
    --     loop = false,
    --     gain = 1,
    --     source_group = "BULLETS",
    --     files = {"Sound_EnemyVeznan_toDemon.ogg"}
    -- },
    WolfAttack = {
        loop = false,
        gain = 0.6,
        ignore = 1.5,
        source_group = "BULLETS",
        files = {"Sound_WolfAttack.ogg", "Sound_WolfAttack2.ogg"}
    },
    MusicMap1 = {
        source_group = "MUSIC",
        gain = 0.8,
        loop = true,
        stream = true,
        files = {"MusicMap.ogg"}
    },
    GUIQuestCompleted = {
        loop = false,
        gain = 1,
        source_group = "GUI",
        files = {"Sound_QuestCompleted.ogg"}
    },
    GUIQuestFailed = {
        loop = false,
        gain = 1,
        source_group = "GUI",
        files = {"Sound_QuestFailed.ogg"}
    },
    MeleeSword = {
        -- ignore = 0.45,
        gain = 0.2,
        mode = "sequence",
        source_group = "SWORDS",
        loop = false,
        files = {"Sound_SoldiersFighting-01.ogg", "Sound_SoldiersFighting-02.ogg", "Sound_SoldiersFighting-03.ogg",
                 "Sound_SoldiersFighting-04.ogg", "Sound_SoldiersFighting-05.ogg"}
    },
}
