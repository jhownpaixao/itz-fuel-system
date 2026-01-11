class ITZ_FS_StationFillFuelActionData : ActionData
{
	int m_FuelType;
}

class ITZ_FS_ActionStationFillFuelCB : ActionContinuousBaseCB
{
	private const float TIME_TO_REPEAT = 0.5;

	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new ITZ_FS_CAContinuousStationFillFuel( UAQuantityConsumed.FUEL, TIME_TO_REPEAT );
	}
};

class ITZ_FS_ActionStationFillFuelBase: ActionContinuousBase
{
	protected int m_FuelType;
	protected FuelStation m_Station;

	void ITZ_FS_ActionStationFillFuelBase()
	{
		m_CallbackClass = ITZ_FS_ActionStationFillFuelCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_EMPTY_VESSEL;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_FullBody = true;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		m_LockTargetOnUse = false;
		m_FuelType = LIQUID_GASOLINE;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem 	= new CCINone;
		m_ConditionTarget	= new CCTCursorNoRuinCheck;
	}

	override string GetText()
	{

		string text = Widget.TranslateString("#STR_FS_FILL_FUEL");
		string fuelName = ITZ_FS_Constants.GetFuelNameFromType(m_FuelType);
		fuelName.ToLower();

		text.Replace("#type#", fuelName);
		return text;
	}

	override typename GetInputType()
	{
		return ContinuousInteractActionInput; //!requerido senão não aparece a ação
	}

	override ActionData CreateActionData()
	{
		ITZ_FS_StationFillFuelActionData action_data = new ITZ_FS_StationFillFuelActionData();
		return action_data;
	}

	override bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = null)
	{
		if (super.SetupAction(player, target, item, action_data, extra_data))
		{
			ITZ_FS_StationFillFuelActionData data = ITZ_FS_StationFillFuelActionData.Cast(action_data);
			if(data)
			{
				data.m_FuelType = m_FuelType;
				return true;
			}	
		}
		
		return false;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if(!ITZ_FS_Module.GetModule().GetSettings().m_VehicleRefuelDirectly)
			return false;

		if (player.GetItemInHands())
			return false;
		
		if ( !target || !IsTransport(target) )
			return false;

		if (target.GetObject().IsDamageDestroyed())
			return false;

		m_Station = ITZ_FS_Module.GetModule().GetFuelPumpForTransport(Transport.Cast(target.GetObject()));

		if(m_Station)
        	ITZ_FS_Logger.Trace("ActionCondition: Estação de combustível encontrada: " + m_Station + " destruída: " + m_Station.HasDestroyed() + " energia: " + m_Station.HasEnergy() + " tipo de combustível: " + m_FuelType);

		if ( !m_Station || m_Station.HasDestroyed() || !m_Station.HasEnergy() || !m_Station.HasFuel(m_FuelType))
			return false;

		CarScript car = CarScript.Cast(target.GetObject());
		BoatScript boat = BoatScript.Cast(target.GetObject());
		if (car)
		{
			if (car.GetFluidFraction( CarFluid.FUEL ) >= 0.98 || car.GetFuelType() != m_FuelType)
				return false;
		}
		else if (boat)
		{
			if ( boat.GetFluidFraction( BoatFluid.FUEL ) >= 0.98 || boat.GetFuelType() != m_FuelType)
				return false;
		}
		else 
		{
			return false;
		}
		

		array<string> selections = new array<string>;
		target.GetObject().GetActionComponentNameList(target.GetComponentIndex(), selections);

		Transport vehicle = Transport.Cast(target.GetObject());
		if ( vehicle )
		{
			for (int s = 0; s < selections.Count(); s++)
			{
				if ( selections[s] == vehicle.GetActionCompNameFuel() )
				{
					float dist = vector.DistanceSq( vehicle.GetRefillPointPosWS(), player.GetPosition() );

					if ( dist < vehicle.GetActionDistanceFuel() * vehicle.GetActionDistanceFuel() )
						return true;
				}
			}
		}

		return false;
	}

	override bool ActionConditionContinue(ActionData action_data)
	{
		if (super.ActionConditionContinue(action_data))
		{
			if (m_Station && !m_Station.IsDamageDestroyed() && m_Station.HasEnergy() && m_Station.HasFuel(m_FuelType))
				return true;
		}
		return false;
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

};

class ITZ_FS_ActionStationFillGasoline : ITZ_FS_ActionStationFillFuelBase
{
	void ITZ_FS_ActionStationFillGasoline()
	{
		m_FuelType = LIQUID_GASOLINE;
	}
}

class ITZ_FS_ActionStationFillDiesel : ITZ_FS_ActionStationFillFuelBase
{
	void ITZ_FS_ActionStationFillDiesel()
	{
		m_FuelType = LIQUID_DIESEL;
	}
}

class ITZ_FS_ActionStationFillKerosene : ITZ_FS_ActionStationFillFuelBase
{
	void ITZ_FS_ActionStationFillKerosene()
	{
		m_FuelType = ITZ_FS_Constants.LIQUID_KEROSENE;
	}
}