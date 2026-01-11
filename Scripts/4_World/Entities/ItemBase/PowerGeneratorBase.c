modded class PowerGeneratorBase
{
	protected string m_FuelStationId;

    override void EEInit()
	{
		super.EEInit();
		ITZ_FS_Module.GetModule().RegisterGenerator(this);
        if(UseInFuelStation())
        {
            ITZ_FS_FuelStation fuelStation = ITZ_FS_Module.GetModule().GetStationProfile(this);
            if(fuelStation)
            {
                SetStationId(fuelStation.GetId());
            }
        }
	}

    override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		ITZ_FS_Module.GetModule().UnregisterGenerator(this);
	}

    override void OnWorkStart()
	{
        super.OnWorkStart();

        if(UseInFuelStation() && m_FuelStationId)
		{
			ITZ_FS_Module.GetModule().SetHasEnergy(m_FuelStationId, true);
		}
	}

    override void OnWorkStop()
	{
        super.OnWorkStop();

		if(UseInFuelStation() && m_FuelStationId)
		{
			ITZ_FS_Module.GetModule().SetHasEnergy(m_FuelStationId, false);
		}

	}

    void SetStationId(string id)
	{
		ITZ_FS_Logger.Debug("SetStationId: Atualizando ID da estação de combustível para " + id + " no gerador: " + GetWorldPosition());
		m_FuelStationId = id;
	}

    bool UseInFuelStation()
	{
		return false;
	}
}