#ifdef RFWC
modded class RFWC_base
{
	//! @note: RFWC_base ignora o super.OnUpdate() então isso deve ser redeclarado aqui
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

			if (m_engine_on)
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
				// CalcFuelConsumption(currentPos, dt);
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
}
#endif