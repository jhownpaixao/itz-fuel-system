modded class Bottle_Base extends Edible_Base
{
	override void SetActions()
	{
		AddAction(ActionStationFillFuelBottleGasoline);
		AddAction(ActionStationFillFuelBottleDiesel);
		AddAction(ActionStationFillFuelBottleKerosene);
		
		super.SetActions();
	}
}
