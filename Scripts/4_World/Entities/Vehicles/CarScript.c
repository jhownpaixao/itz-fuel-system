modded class CarScript
{
	protected ITZ_FS_VehicleProfile m_FS_VehicleProfile;
	protected bool 		m_FS_InitProfile = false;

	protected float 	m_FS_LastFuelAmount;
	protected vector 	m_FS_LastPosition;
	protected float 	m_FS_FuelConsumptionACC;

	override void EEInit()
	{
		super.EEInit();
		m_FS_LastFuelAmount = GetFuelAmount();
		m_FS_FuelConsumptionACC = 0;
	}

	int GetFuelType()
	{
		return ITZ_FS_Module.GetModule().GetVehicleFuelType(this);
	}

	override void SetActions()
	{
		AddAction(ITZ_FS_ActionStationFillGasoline);
		AddAction(ITZ_FS_ActionStationFillDiesel);
		AddAction(ITZ_FS_ActionStationFillKerosene);
		super.SetActions();
	}

	override void OnUpdate(float dt)
	{
		super.OnUpdate(dt);

		if(!m_FS_VehicleProfile && !m_FS_InitProfile)
		{
			m_FS_InitProfile = true;
			m_FS_VehicleProfile = ITZ_FS_Module.GetModule().GetVehicleProfile(this);
		}
			
		if (m_FS_VehicleProfile)
		{
			vector currentPos = GetPosition();
			bool engineOn = EngineIsOn();

			#ifdef EXPANSIONMODVEHICLE
			ExpansionHelicopterScript heliScript = ExpansionHelicopterScript.Cast(this);
			if(heliScript)
			{
				engineOn = heliScript.Expansion_EngineIsOn();
			}
			#endif

			if (engineOn)
			{
				float currentFuel = GetFuelAmount();
				// inibir o consumo vanilla
				if (m_FuelTankHealth > GameConstants.DAMAGE_DAMAGED_VALUE && m_FS_LastFuelAmount > currentFuel)
				{
					float oldFuel = m_FS_LastFuelAmount - currentFuel;
					Fill(CarFluid.FUEL, oldFuel);

					m_FS_LastFuelAmount = GetFuelAmount();
				}

				float fuelConsumption = ITZ_FS_Module.GetModule().CalcFuelConsumption(this, currentPos, m_FS_LastPosition, dt, m_FS_VehicleProfile.m_FuelConsumptionPerKm, ITZ_FS_Constants.DISTANCE_MDFR, ITZ_FS_Constants.IDLE_CONSUMPTION, ITZ_FS_Constants.RPM_MDFR);
				m_FS_FuelConsumptionACC += fuelConsumption;
			}

			if (m_FS_FuelConsumptionACC > 1e-3)
			{
				Leak(CarFluid.FUEL, m_FS_FuelConsumptionACC);

				m_FS_LastFuelAmount = GetFuelAmount();
				m_FS_FuelConsumptionACC = 0;
			}

			m_FS_LastPosition = currentPos;
		}
	}

	float GetFuelAmount()
	{
		return GetFluidCapacity(CarFluid.FUEL) * GetFluidFraction(CarFluid.FUEL);
	}
}
