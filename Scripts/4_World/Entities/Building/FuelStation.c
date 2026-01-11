modded class FuelStation
{
	protected EffectSound m_WorkingSoundLoop;
	protected EffectSound m_FuellingSoundLoop;
	protected EffectSound m_LeverSound;
	protected bool m_HasEnergy;
	protected int m_SoundSetType;

	protected bool m_PlayWorkingSound;
	protected bool m_PlayFuellingSound;
	protected bool m_PlayLeverSound;

	void FuelStation()
	{
		RegisterNetSyncVariableBool("m_HasEnergy");
		RegisterNetSyncVariableBool("m_PlayWorkingSound");
		RegisterNetSyncVariableBool("m_PlayFuellingSound");
		RegisterNetSyncVariableBool("m_PlayLeverSound");
		m_SoundSetType = Math.RandomIntInclusive(1, 2);
	}

	override void OnVariablesSynchronized()
	{
		if(m_PlayWorkingSound)
			StartWorkingSound();
		else
			StopWorkingSound();

		if(m_PlayFuellingSound)
			StartFuellingSound();
		else
			StopFuellingSound();

		if(m_PlayLeverSound)
			StartLeverSound();
	}

	override void DeferredInit()
	{
		GetGame().RegisterNetworkStaticObject(this);
	}

	void OnFuellingStart()
	{
		m_PlayFuellingSound = true;
		SetSynchDirty();
	}

	void OnFuellingStop()
	{
		m_PlayFuellingSound = false;
		SetSynchDirty();
	}

	void OnStartWork()
	{
		m_PlayWorkingSound = true;
		m_PlayLeverSound = true;
		SetSynchDirty();
	}

	void OnStopWorking()
	{
		m_PlayWorkingSound = false;
		m_PlayLeverSound = false;
		SetSynchDirty();
	}

	string GetLeverSoundSet()
	{
		return "FS_PumpLever_SoundSet";
	}

	string GetFuellingSoundSet()
	{
		return "FS_FuelPump_Fuelling_" + m_SoundSetType + "_SoundSet";
	}

	string GetWorkingSoundSet()
	{
		return "FS_FuelPump_Working_" + m_SoundSetType + "_SoundSet";
	}

	void StartWorkingSound()
	{
		if (m_WorkingSoundLoop && m_WorkingSoundLoop.IsSoundPlaying())
            return;

		PlaySoundSet(m_WorkingSoundLoop, GetWorkingSoundSet(), 0, 0, true);
	}

	void StopWorkingSound()
	{
		if (m_WorkingSoundLoop && m_WorkingSoundLoop.IsSoundPlaying())
			m_WorkingSoundLoop.Stop();
	}

	void StartFuellingSound()
	{
		if (m_FuellingSoundLoop && m_FuellingSoundLoop.IsSoundPlaying())
            return;

		PlaySoundSet(m_FuellingSoundLoop, GetFuellingSoundSet(), 0, 0, true);
	}

	void StopFuellingSound()
	{
		if (m_FuellingSoundLoop && m_FuellingSoundLoop.IsSoundPlaying())
			m_FuellingSoundLoop.Stop();
	}

	void StartLeverSound()
	{
		if (m_LeverSound && m_LeverSound.IsSoundPlaying())
            return;

		PlaySoundSet(m_LeverSound, GetLeverSoundSet(), 0, 0, false);
	}

	bool HasFuel(int type)
	{
		if(ITZ_FS_Module.GetModule())
		{
			ITZ_FS_FuelStation profile = ITZ_FS_Module.GetModule().GetStationProfileAtPosition(GetWorldPosition());
			if(profile)
			{
				return profile.HasFuel(type);
			}		
		}
		return false;
	}

	bool HasEnergy()
	{
		if(ITZ_FS_Module.GetModule().GetSettings().m_FuelPumpRequiresPower)
			return m_HasEnergy;

		return true;
	}

	void SetHasEnergy(bool hasEnergy)
	{
		ITZ_FS_Logger.Debug("SetHasEnergy: Atualizando energia para " + hasEnergy + " na estação de combustível: " + GetWorldPosition());
		m_HasEnergy = hasEnergy;
		SetSynchDirty();
	}

	float GetFuelCapacity(int type)
	{
		if(ITZ_FS_Module.GetModule())
		{
			ITZ_FS_FuelStation profile = ITZ_FS_Module.GetModule().GetStationProfileAtPosition(GetWorldPosition());
			if(profile)
			{
				return profile.GetCapacity(type);
			}
		}
		return 0;
	}

	float GetFuelQuantity(int type)
	{
		if(ITZ_FS_Module.GetModule())
		{
			ITZ_FS_FuelStation profile = ITZ_FS_Module.GetModule().GetStationProfileAtPosition(GetWorldPosition());
			if(profile)
			{
				return profile.GetFuelQuantity(type);
			}
		}
		return 0;
	}

	void SetFuelQuantity(int type, float quantity)
	{
		if(ITZ_FS_Module.GetModule())
		{
			ITZ_FS_FuelStation profile = ITZ_FS_Module.GetModule().GetStationProfileAtPosition(GetWorldPosition());
			if(profile)
				profile.SetFuelQuantity(type, quantity);
		}
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ITZ_FS_ActionPumpMeasureFuelGasoline);
		AddAction(ITZ_FS_ActionPumpMeasureFuelDiesel);
		AddAction(ITZ_FS_ActionPumpMeasureFuelKerosene);
	}

	override bool IsHealthVisible()
	{
		return true;
	}

	bool HasDestroyed()
	{
		if(ITZ_FS_Module.GetModule().GetSettings().m_UseDestroyedPump) // pode usar bomba destruida
			return false;

		return IsDamageDestroyed();
	}
}