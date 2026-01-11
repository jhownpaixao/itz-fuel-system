class ITZ_FS_ActionPumpMeasureFuelBase : ActionInteractBase
{	
	FuelStation m_Station;
	int m_FuelType;

	void ITZ_FS_ActionPumpMeasureFuelBase()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_LockTargetOnUse = false;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem 	= new CCINone;
		m_ConditionTarget	= new CCTCursorNoRuinCheck;
	}

	override string GetText()
	{
		string text = Widget.TranslateString("#STR_FS_MEASURE_FUEL");
		string fuelName = ITZ_FS_Constants.GetFuelNameFromType(m_FuelType);
		fuelName.ToLower();

		text.Replace("#type#", fuelName);
		return text;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if(!ITZ_FS_Module.GetModule().GetSettings().m_CanMeasureFuel)
			return false;

		if (player.GetItemInHands())
			return false;

        if (Class.CastTo(m_Station, target.GetParent()) || Class.CastTo(m_Station, target.GetObject()))
			return true;

		return false;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		if(!m_Station || !action_data.m_Player)
			return;

		float fuelQuantity = m_Station.GetFuelQuantity(m_FuelType);
		string text;
		if(fuelQuantity <= 0)
			text =  Widget.TranslateString("#STR_FS_NO_FUEL");
		else
			text =  Widget.TranslateString("#STR_FS_FUEL_AMOUNT");

		string fuelName = ITZ_FS_Constants.GetFuelNameFromType(m_FuelType);
		fuelName.ToLower();

		text.Replace("#amount#", fuelQuantity.ToString());
		text.Replace("#type#", fuelName);

		GetGame().RPCSingleParam(action_data.m_Player, ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>(text), true, action_data.m_Player.GetIdentity());
	}
};

class ITZ_FS_ActionPumpMeasureFuelGasoline : ITZ_FS_ActionPumpMeasureFuelBase
{
	void ITZ_FS_ActionPumpMeasureFuelGasoline()
	{
		m_FuelType = LIQUID_GASOLINE;
	}
}

class ITZ_FS_ActionPumpMeasureFuelDiesel : ITZ_FS_ActionPumpMeasureFuelBase
{
	void ITZ_FS_ActionPumpMeasureFuelDiesel()
	{
		m_FuelType = LIQUID_DIESEL;
	}
}

class ITZ_FS_ActionPumpMeasureFuelKerosene : ITZ_FS_ActionPumpMeasureFuelBase
{
	void ITZ_FS_ActionPumpMeasureFuelKerosene()
	{
		m_FuelType = ITZ_FS_Constants.LIQUID_KEROSENE;
	}
}