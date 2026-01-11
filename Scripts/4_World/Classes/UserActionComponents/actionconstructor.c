modded class ActionConstructor
{
	override void RegisterActions(TTypenameArray actions)
	{
		super.RegisterActions(actions);
		actions.Insert(ITZ_FS_ActionStationFillGasoline);
		actions.Insert(ITZ_FS_ActionStationFillDiesel);
		actions.Insert(ITZ_FS_ActionStationFillKerosene);
		actions.Insert(ITZ_FS_ActionPumpMeasureFuelGasoline);
		actions.Insert(ITZ_FS_ActionPumpMeasureFuelDiesel);
		actions.Insert(ITZ_FS_ActionPumpMeasureFuelKerosene);

		actions.Insert(ActionStationFillFuelBottleGasoline);
		actions.Insert(ActionStationFillFuelBottleDiesel);
		actions.Insert(ActionStationFillFuelBottleKerosene);

	}
};