class ITZ_FS_StationFillFuelBottleActionData : ActionData
{
	int m_FuelType;
}

class ITZ_FS_ActionStationFillFuelBottleCB : ActionContinuousBaseCB
{
	private const float TIME_TO_REPEAT = 0.5;
	
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new ITZ_FS_CAContinuousStationFillFuelBottle(UAQuantityConsumed.FUEL, TIME_TO_REPEAT);
	}
};

class ActionStationFillFuelBottle: ActionContinuousBase
{
	protected int m_FuelType;
	protected FuelStation m_Station;


	void ActionStationFillFuelBottle()
	{
		m_CallbackClass		= ITZ_FS_ActionStationFillFuelBottleCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_FILLBOTTLEPOND;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_FullBody = true;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		m_LockTargetOnUse = false;
		m_FuelType = LIQUID_NONE;
	}
	
	override void CreateConditionComponents()
	{
		m_ConditionItem 	= new CCINone;
		m_ConditionTarget	= new CCTCursorNoRuinCheck;
	}

	override string GetText()
	{
		string text = Widget.TranslateString("#STR_FS_FILL_WITH_FUEL");
		string fuelName = ITZ_FS_Constants.GetFuelNameFromType(m_FuelType);
		fuelName.ToLower();

		text.Replace("#type#", fuelName);
		return text;
	}

	override ActionData CreateActionData()
	{
		ITZ_FS_StationFillFuelBottleActionData action_data = new ITZ_FS_StationFillFuelBottleActionData();
		return action_data;
	}

	override bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = null)
	{
		if (super.SetupAction(player, target, item, action_data, extra_data))
		{
			ITZ_FS_StationFillFuelBottleActionData data = ITZ_FS_StationFillFuelBottleActionData.Cast(action_data);
			if(data)
			{
				data.m_FuelType = m_FuelType;
				return true;
			}	
		}
		
		return false;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!target)
			return false;

		if (Class.CastTo(m_Station, target.GetParent()) || Class.CastTo(m_Station, target.GetObject()))
		{
        	ITZ_FS_Logger.Trace("ActionCondition: Estação de combustível encontrada: " + m_Station + " destruída: " + m_Station.HasDestroyed() + " energia: " + m_Station.HasEnergy() + " tipo de combustível: " + m_FuelType);

			if (!m_Station.HasDestroyed() && m_Station.HasEnergy() && m_Station.HasFuel(m_FuelType))
				return Liquid.CanFillContainer(item, m_FuelType);
		}

		ITZ_FS_Logger.Trace("ActionCondition: Estação de combustível não encontrada ou inválida.");
		return false;
	}
	
	override bool ActionConditionContinue(ActionData action_data)
	{
		return m_Station && !m_Station.HasDestroyed();
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		
		if (m_Station)
			m_Station.OnStartWork();
	}

	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);
		
		if (m_Station)
		{
			m_Station.OnStopWorking();
			m_Station.OnFuellingStop(); // em caso de ainda esta tocando o som
		}	
	}

	override void OnStartAnimationLoopServer(ActionData action_data)
	{
		super.OnStartAnimationLoopServer(action_data);
		
		if (m_Station)
			m_Station.OnFuellingStart();
	}

	override void OnEndAnimationLoopServer(ActionData action_data)
	{
		super.OnEndAnimationLoopServer(action_data);
		
		if (m_Station)
			m_Station.OnFuellingStop();
	}
		
	override bool IsLockTargetOnUse()
	{
		return false;
	}
}

class ActionStationFillFuelBottleGasoline : ActionStationFillFuelBottle
{
	void ActionStationFillFuelBottleGasoline()
	{
		m_FuelType = LIQUID_GASOLINE;
	}
}

class ActionStationFillFuelBottleDiesel : ActionStationFillFuelBottle
{
	void ActionStationFillFuelBottleDiesel()
	{
		m_FuelType = LIQUID_DIESEL;
	}
}

class ActionStationFillFuelBottleKerosene : ActionStationFillFuelBottle
{
	void ActionStationFillFuelBottleKerosene()
	{
		m_FuelType = ITZ_FS_Constants.LIQUID_KEROSENE;
	}
}
