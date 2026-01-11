class CfgPatches
{
	class itz_fuelsystem
	{
		units[] = {""};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data", "DZ_Scripts", "JM_CF_Scripts"};
	};
};

class CfgMods
{
	class itz_fuelsystem
	{
		dir = "itz_fuelsystem";
		picture = "";
		action = "";
		hideName = 0;
		name = "Fuel System";
		hidePicture = 0;
		credits = "Jhonnata Paixão";
		author = "iTzChronuZ";
		authorID = "76561199441861559";
		version = "1.0.0";
		extra = 0;
		type = "mod";
		dependencies[] = {"Game", "World", "Mission"};

		class defs
		{
			class engineScriptModule
			{
				value = "";
				files[] = {"itz_fuelsystem/Scripts/Common", "itz_fuelsystem/Scripts/1_Core"};
			};

			class gameScriptModule
			{
				value = "";
				files[] = {"itz_fuelsystem/Scripts/Common", "itz_fuelsystem/Scripts/3_Game"};
			};

			class worldScriptModule
			{
				value = "";
				files[] = {"itz_fuelsystem/Scripts/Common", "itz_fuelsystem/Scripts/4_World"};
			};

			class missionScriptModule
			{
				value = "";
				files[] = {"itz_fuelsystem/Scripts/Common", "itz_fuelsystem/Scripts/5_Mission"};
			};
		};
	};
};

class CfgVehicles
{
	class PowerGeneratorStatic;
	class Container_Base; // External class reference
	class Bottle_Base;	  // External class reference
	class WaterPouch_ColorBase;
	class FS_PowerGenerator: PowerGeneratorStatic
	{
		scope=2;
		displayName="$STR_CfgVehicles_PowerGenerator0";
		descriptionShort="$STR_CfgVehicles_PowerGenerator1";
		weight=75000;
		fuelTankCapacity=60000;
		model="itz_fuelsystem\Data\Models\fs_powergenerator.p3d";
		physLayer="item_large";
		carveNavmesh=1;
		fuelTankCapacity=5000;
		attachments[]={};
		simpleHiddenSelections[] = 
		{
			"toggle_on",
			"toggle_off"
		};
		hiddenSelections[]=
		{
			"btn_start",
			"fuel_tank",
			"light_error",
			"light_on",
			"light_power",
			"btn_start_cover",
			"toggle_3",
			"toggle_4"
		};
		hiddenSelectionsTextures[]=
		{
			"itz_fuelsystem\Data\Textures\powergenerator_co.paa",
			"itz_fuelsystem\Data\Textures\powergenerator_co.paa",
			"itz_fuelsystem\Data\Textures\powergenerator_co.paa",
			"itz_fuelsystem\Data\Textures\powergenerator_co.paa",
			"itz_fuelsystem\Data\Textures\powergenerator_co.paa",
			"itz_fuelsystem\Data\Textures\powergenerator_co.paa",
			"itz_fuelsystem\Data\Textures\powergenerator_co.paa",
			"itz_fuelsystem\Data\Textures\powergenerator_co.paa",
			"itz_fuelsystem\Data\Textures\powergenerator_co.paa",
		};
		hiddenSelectionsMaterials[]=
		{
			"itz_fuelsystem\Data\Textures\powergenerator.rvmat",
			"itz_fuelsystem\Data\Textures\powergenerator.rvmat",
			"itz_fuelsystem\Data\Textures\powergenerator.rvmat",
			"itz_fuelsystem\Data\Textures\powergenerator.rvmat",
			"itz_fuelsystem\Data\Textures\powergenerator.rvmat",
			"itz_fuelsystem\Data\Textures\powergenerator.rvmat",
			"itz_fuelsystem\Data\Textures\powergenerator.rvmat",
			"itz_fuelsystem\Data\Textures\powergenerator.rvmat",
			"itz_fuelsystem\Data\Textures\powergenerator.rvmat",
		};

		class EnergyManager
		{
			hasIcon=1;
			autoSwitchOff=0;
			energyStorageMax=5000;
			energyUsagePerSecond=8;
			reduceMaxEnergyByDamageCoef=0.5;
			energyAtSpawn=200;
			powerSocketsCount=0;
			compatiblePlugTypes[]={};
		};

		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=10500;
					healthLevels[]=
					{
						{
							1,
							
							{
								"itz_fuelsystem\Data\Textures\powergenerator.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"itz_fuelsystem\Data\Textures\powergenerator.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\gear\camping\data\power_generator_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\gear\camping\data\power_generator_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\gear\camping\data\power_generator_destruct.rvmat"
							}
						}
					};
				};
			};
			class DamageZones
			{
				class FuelTank
				{
					displayName="$STR_CfgVehicleDmg_FuelTank0";
					fatalInjuryCoef=-1;
					componentNames[]=
					{
						"fuel_tank"
					};
					class Health
					{
						hitpoints=500;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"itz_fuelsystem\Data\Textures\powergenerator.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"itz_fuelsystem\Data\Textures\powergenerator.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"DZ\gear\camping\data\power_generator_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"DZ\gear\camping\data\power_generator_damage.rvmat"
								}
							},
							
							{
								0,
								
								{
									"DZ\gear\camping\data\power_generator_destruct.rvmat"
								}
							}
						};
					};
					inventorySlots[]={};
					inventorySlotsCoefs[]={};
				};
			};
		};
	};

	//# VANILLA
	class Barrel_ColorBase : Container_Base
	{
		liquidContainerType = "1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512 + 1024 + 2048 + 4096 + 8192 + 16384 + 32768 + 65536  + 131072 + 262144 + 524288 + 1048576 + 2097152 + 4194304 + 8388608 + 16777216 + 33554432 + 67108864 + 134217728 + 268435456 + 536870912 - (1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256) - 32768";
	};

	class Pot : Bottle_Base
	{
		liquidContainerType = "1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512 + 1024 + 2048 + 4096 + 8192 + 16384 + 32768 + 65536  + 131072 + 262144 + 524288 + 1048576 + 2097152 + 4194304 + 8388608 + 16777216 + 33554432 + 67108864 + 134217728 + 268435456 + 536870912 - (1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256) - 32768";
	};

	class Cauldron : Bottle_Base
	{
		liquidContainerType = "1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512 + 1024 + 2048 + 4096 + 8192 + 16384 + 32768 + 65536  + 131072 + 262144 + 524288 + 1048576 + 2097152 + 4194304 + 8388608 + 16777216 + 33554432 + 67108864 + 134217728 + 268435456 + 536870912 - (1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256) - 32768";
	};

	class Canteen : Bottle_Base
	{
		liquidContainerType = "1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512 + 1024 + 2048 + 4096 + 8192 + 16384 + 32768 + 65536  + 131072 + 262144 + 524288 + 1048576 + 2097152 + 4194304 + 8388608 + 16777216 + 33554432 + 67108864 + 134217728 + 268435456 + 536870912 - (1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256) - 32768";
	};

	class WaterBottle : Bottle_Base
	{
		liquidContainerType = "1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512 + 1024 + 2048 + 4096 + 8192 + 16384 + 32768 + 65536  + 131072 + 262144 + 524288 + 1048576 + 2097152 + 4194304 + 8388608 + 16777216 + 33554432 + 67108864 + 134217728 + 268435456 + 536870912 - (1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256) - 32768";
	};

	class GlassBottle : Bottle_Base
	{
		liquidContainerType = "1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512 + 1024 + 2048 + 4096 + 8192 + 16384 + 32768 + 65536  + 131072 + 262144 + 524288 + 1048576 + 2097152 + 4194304 + 8388608 + 16777216 + 33554432 + 67108864 + 134217728 + 268435456 + 536870912 - (1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256) - 32768";
	};

	class WaterPouch_ColorBase : WaterPouch_ColorBase
	{
		liquidContainerType = "1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512 + 1024 + 2048 + 4096 + 8192 + 16384 + 32768 + 65536  + 131072 + 262144 + 524288 + 1048576 + 2097152 + 4194304 + 8388608 + 16777216 + 33554432 + 67108864 + 134217728 + 268435456 + 536870912 - (1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256) - 32768";
	};

	class CanisterGasoline : Bottle_Base
	{
		liquidContainerType = "1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512 + 1024 + 2048 + 4096 + 8192 + 16384 + 32768 + 65536  + 131072 + 262144 + 524288 + 1048576 + 2097152 + 4194304 + 8388608 + 16777216 + 33554432 + 67108864 + 134217728 + 268435456 + 536870912 - (1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256) - 32768";
	};
}

class CfgSoundShaders
{
	class FS_PumpLever_SoundShader
	{
		range = 50;
		samples[] = {{"itz_fuelsystem\Data\Sounds\lever", 1}};
		volume = 1.0;
	};

	class FS_FuelPump_Fuelling_1_SoundShader
	{
		range = 50;
		samples[] = {{"itz_fuelsystem\Data\Sounds\pump_1_fuelling", 1}};
		volume = 1.0;
	};

	class FS_FuelPump_Working_1_SoundShader
	{
		range = 50;
		samples[] = {{"itz_fuelsystem\Data\Sounds\pump_1_working.ogg", 1}};
		volume = 1.0;
	};

	class FS_FuelPump_Fuelling_2_SoundShader
	{
		range = 50;
		samples[] = {{"itz_fuelsystem\Data\Sounds\pump_2_fuelling", 1}};
		volume = 1.0;
	};

	class FS_FuelPump_Working_2_SoundShader
	{
		range = 50;
		samples[] = {{"itz_fuelsystem\Data\Sounds\pump_2_working.ogg", 1}};
		volume = 1.0;
	};
};

class CfgSoundSets
{
	class FS_PumpLever_SoundSet
	{
		soundShaders[] = {"FS_PumpLever_SoundShader"};
	};

	class FS_FuelPump_Fuelling_1_SoundSet
	{
		soundShaders[] = {"FS_FuelPump_Fuelling_1_SoundShader"};
	};

	class FS_FuelPump_Working_1_SoundSet
	{
		soundShaders[] = {"FS_FuelPump_Working_1_SoundShader"};
	};

	class FS_FuelPump_Fuelling_2_SoundSet
	{
		soundShaders[] = {"FS_FuelPump_Fuelling_2_SoundShader"};
	};
	class FS_FuelPump_Working_2_SoundSet
	{
		soundShaders[] = {"FS_FuelPump_Working_2_SoundShader"};
	};
};

class cfgLiquidDefinitions
{
	class KeroseneLiquid
	{
		type = 536870912;
		displayName = "$STR_FS_AVIATION_FUEL";
		flammability = 60;

		class Nutrition
		{
			fullnessIndex = 1;
			energy = -10;
			water = 0;
			nutritionalIndex = 75;
			toxicity = 1;
			digestibility = 2;
			agents = 16;
		};
	};
};