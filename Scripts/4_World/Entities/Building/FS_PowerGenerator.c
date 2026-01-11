class FS_PowerGenerator extends PowerGeneratorStatic
{
	static protected const int EXPLOSION_EFFECT_TIME = 2 * 60 * 1000; // 2 minutes

    static protected const string TOGGLE_ON = "toggle_on";
    static protected const string TOGGLE_OFF = "toggle_off";

	static protected const int LIGHT_ERROR = 2;
	static protected const int LIGHT_ON = 3;
	static protected const int LIGHT_POWER = 4;
	static protected const int BTN_START_COVER = 5;

    static protected const string RED_LIGHT_GLOW		= "dz\\gear\\camping\\data\\battery_charger_light_r.rvmat";
	static protected const string GREEN_LIGHT_GLOW		= "dz\\gear\\camping\\data\\battery_charger_light_g.rvmat";
	static protected const string YELLOW_LIGHT_GLOW		= "dz\\gear\\camping\\data\\battery_charger_light_y.rvmat";
	static protected const string SWITCH_LIGHT_GLOW		= "dz\\gear\\camping\\data\\battery_charger_light_switch_on.rvmat";
	static protected const string DEFAULT_MATERIAL 		= "itz_fuelsystem\\Data\\Textures\\powergenerator.rvmat";
	static protected float m_BlinkingStatusLightInterval = 0.4; // How often the lights blink

	protected bool m_BlinkingStatusLightIsOn = false;	// Status of one blinking light
	protected bool m_IsOverloaded = false; // Is the generator overloaded?
	protected bool m_IsOverloading = false; // Is the generator overloading?
	protected bool m_BlinkingOverloadedLightIsOn = false;

	ref Timer m_UpdateStatusLightsTimer;
	ref Timer m_UpdateOverloadStatusLightsTimer;
	ref Timer m_OverloadMonitorTimer;

	protected TirePileLight		m_Light;
	protected Particle 			m_ParticleFire;
	protected EffectSound 		m_SoundFireLoop;
	
	protected ref AreaDamageManager m_AreaDamage;

	float m_OverloadLastTickTime; // Last time the generator was overloaded
	float m_OverloadACCTime;
	float m_OverloadRecoveryACCTime;

    void FS_PowerGenerator()	
	{
		m_UpdateStatusLightsTimer = new Timer( CALL_CATEGORY_SYSTEM );
		m_UpdateOverloadStatusLightsTimer = new Timer( CALL_CATEGORY_SYSTEM );
		m_OverloadMonitorTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
	}

	override void EEInit()
	{
		super.EEInit();
		
		SetFuel(0.0);
		ShowSimpleSelection(TOGGLE_ON, false);
		ShowSimpleSelection(TOGGLE_OFF, true);
	}

	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
	 	Explode(DamageType.EXPLOSION, "LandFuelFeed_Ammo");
		GetCompEM().SwitchOff();
		Print("FS_PowerGenerator::EEKilled - Generator destroyed at position: " + GetWorldPosition());
	}

	override void OnDamageDestroyed(int oldLevel)
	{
		super.OnDamageDestroyed(oldLevel);

		if (!m_AreaDamage)
		{
			InitDestroyedEffect();
		}

		Print("FS_PowerGenerator::OnDamageDestroyed - Generator destroyed at position: " + GetWorldPosition());

		OnOverloadRecovery(); //!restaura o estado das luzes
		StopOverloadMonitor();
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		Print("FS_PowerGenerator::EEDelete - Generator deleted at position: " + GetWorldPosition());
		StopDestructionEffect();
	}

    override void OnExplosionEffects(Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal, float energyFactor, float explosionFactor, bool isWater, string ammoType)
	{
		super.OnExplosionEffects(source, directHit, componentIndex, surface, pos, surfNormal, energyFactor, explosionFactor, isWater, ammoType);
		if ( !GetGame().IsDedicatedServer() )
		{	
			vector n = surfNormal.VectorToAngles() + "0 90 0";
			Particle p1 = ParticleManager.GetInstance().PlayInWorld(ParticleList.SMOKE_GENERIC_WRECK, pos);
			p1.SetOrientation(n);
		
			Particle p2 = ParticleManager.GetInstance().PlayInWorld(ParticleList.EXPLOSION_LANDMINE, pos);
			p2.SetOrientation(n);

			Particle p3 = ParticleManager.GetInstance().PlayInWorld(ParticleList.IMPACT_METAL_RICOCHET, pos);
			p3.SetOrientation(n);
		
			Particle p4 = ParticleManager.GetInstance().PlayInWorld(ParticleList.IMPACT_GRAVEL_RICOCHET, pos);
			p4.SetOrientation(n);
		}
	}

	protected void InitDestroyedEffect()
	{
		#ifndef SERVER
		// object creation - create effects (light, particles, sound)
		if (!m_Light)
		{
			m_Light = TirePileLight.Cast(ScriptedLightBase.CreateLight(TirePileLight, Vector(0,0,0), 1));
			m_Light.AttachOnMemoryPoint(this, m_Light.m_MemoryPoint);
			m_Light.FadeBrightnessTo(m_Light.m_FireBrightness, 0);
			m_Light.FadeRadiusTo(m_Light.m_FireRadius, 0);
		}
		
		if (!m_ParticleFire)
		{
			m_ParticleFire = ParticleManager.GetInstance().PlayOnObject(ParticleList.TIREPILE_FIRE, this, Vector(0,0,0), Vector(0,0,0), true);
		}
		
		if (!m_SoundFireLoop)
		{
			PlaySoundSetLoop(m_SoundFireLoop, "HeavyFire_SoundSet", 1.0, 2.0);
		}
		#endif

		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			delete m_UTSSettings;
			delete m_UTSource;
			delete m_UTSLEngine;

 			m_UTSSettings 						= new UniversalTemperatureSourceSettings();
			m_UTSSettings.m_UpdateInterval		= 2;
			m_UTSSettings.m_Updateable			= true;
			m_UTSSettings.m_TemperatureItemCap 	= GameConstants.ITEM_TEMPERATURE_NEUTRAL_ZONE_MIDDLE;
			m_UTSSettings.m_TemperatureCap		= 25;
			m_UTSSettings.m_RangeFull			= 4;
			m_UTSSettings.m_RangeMax			= 6;
			
			m_UTSLEngine 						= new UniversalTemperatureSourceLambdaConstant();
			m_UTSource 							= new UniversalTemperatureSource(this, m_UTSSettings, m_UTSLEngine);
			m_UTSource.SetActive(true);

			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.CreateAreaDamage, 1000, false);
		}

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.StopDestructionEffect, EXPLOSION_EFFECT_TIME, false);
	}

	protected void StopDestructionEffect()
	{
		#ifndef SERVER
		// outside of bubble - do effects cleanup (light, particles, sound)
		
		if (m_Light)
			m_Light.FadeOut();
		
		if (m_ParticleFire)
			m_ParticleFire.Stop();
			
		if (m_SoundFireLoop)
			m_SoundFireLoop.SoundStop();
		#endif
		
		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			if(m_UTSource)
				m_UTSource.SetActive(false);
			DestroyAreaDamage();
		}
	}

	protected void CreateAreaDamage()
	{
		//destroy area damage if some already exists
		DestroyAreaDamage();
		
		//create new area damage
		m_AreaDamage = new AreaDamageLoopedDeferred(this);
		m_AreaDamage.SetDamageComponentType(AreaDamageComponentTypes.HITZONE);
		m_AreaDamage.SetExtents("-2.0 0 -2.0", "2.0 2.0 2.0");
		m_AreaDamage.SetLoopInterval(0.5);
		m_AreaDamage.SetDeferDuration(0.5);
		m_AreaDamage.SetHitZones({ "Head","Torso","LeftHand","LeftLeg","LeftFoot","RightHand","RightLeg","RightFoot" });
		m_AreaDamage.SetAmmoName("FireDamage");
		m_AreaDamage.Spawn();
	}
	
	protected void DestroyAreaDamage()
	{
		if (m_AreaDamage) 
			m_AreaDamage.Destroy();
	}


    override vector GetSmokeParticlePosition()
	{
		return GetMemoryPointPos("smoke");
	}
	
	override vector GetSmokeParticleOrientation()
	{
		return "0 0 23";
	}
    
    override bool HasSparkplug()
    {
        return true;
    }

    // Taking item into inventory
	override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}

	// Taking item into inventory
	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

    override bool IsTakeable()
	{
		return false;
	}

    override bool DisableVicinityIcon()
    {
        return true;
    }

    override void OnSwitchOn()
	{
        super.OnSwitchOn();
		ShowSimpleSelection(TOGGLE_ON, true);
		ShowSimpleSelection(TOGGLE_OFF, false);
	}
	
	override void OnSwitchOff()
	{
        super.OnSwitchOff();
		ShowSimpleSelection(TOGGLE_ON, false);
		ShowSimpleSelection(TOGGLE_OFF, true);
	}

    override void OnWorkStart()
	{
        super.OnWorkStart();
		if ( GetGame().IsClient()  ||  !GetGame().IsMultiplayer() )
		{
			UpdateStatusLights();
			m_UpdateStatusLightsTimer.Run( m_BlinkingStatusLightInterval/2 , this, "UpdateStatusLights", NULL, true);
		}

		StartOverloadMonitor();
	}
		
	override void OnWorkStop()
	{
        super.OnWorkStop();
		if ( GetGame().IsClient()  ||  !GetGame().IsMultiplayer() )
		{
			UpdateStatusLights();
			m_UpdateStatusLightsTimer.Stop();
		}
	}

	override bool CanAddFuel(ItemBase container)
	{
		if (container)
		{
			// Get the liquid
			int liquid_type	= container.GetLiquidType();
			
			// Do all checks
			if ( container.GetQuantity() > 0 && GetCompEM().GetEnergy() < GetCompEM().GetEnergyMax() && (liquid_type & LIQUID_GASOLINE))
			{
				return true;
			}
		}
		
		return false;
	}

    void UpdateStatusLights()
    {
        if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
            if (GetCompEM().IsWorking())
			{
                LightOn();
                
                if(m_IsLowEnergy)
                {
                    if (m_BlinkingStatusLightIsOn)
						PowerLightOn();
					else
						PowerLightOff();

                    m_BlinkingStatusLightIsOn = !m_BlinkingStatusLightIsOn;
                }
                else
                {
                    PowerLightOn();
                }
            }  
            else
            {
                LightOff();
                PowerLightOff();
            }
        }
    }

	void UpdateOverloadStatusLights()
    {
        if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			if( m_IsOverloaded || m_IsOverloading)
			{
				if (m_BlinkingOverloadedLightIsOn)
						ErrorLightOn();
					else
						ErrorLightOff();

                m_BlinkingOverloadedLightIsOn = !m_BlinkingOverloadedLightIsOn;
			}
        }
    }

    void ErrorLightOn()
	{
		SetObjectMaterial( LIGHT_ERROR, RED_LIGHT_GLOW );
	}

    void ErrorLightOff()
	{
		SetObjectMaterial( LIGHT_ERROR, DEFAULT_MATERIAL );
	}

    void LightOn()
    {
        SetObjectMaterial( LIGHT_ON, GREEN_LIGHT_GLOW );
    }

    void LightOff()
    {
        SetObjectMaterial( LIGHT_ON, DEFAULT_MATERIAL );
    }

    void PowerLightOn()
    {
        SetObjectMaterial( LIGHT_POWER, YELLOW_LIGHT_GLOW );
    }

    void PowerLightOff()
    {
        SetObjectMaterial( LIGHT_POWER, DEFAULT_MATERIAL );
    }

	void BtnCoverLightOn()
    {
        SetObjectMaterial( BTN_START_COVER, RED_LIGHT_GLOW );
    }

	void BtnCoverLightOff()
    {
        SetObjectMaterial( BTN_START_COVER, DEFAULT_MATERIAL );
    }

	private void ShowSimpleSelection(string selectionName, bool hide = false)
	{
		TStringArray selectionNames = new TStringArray;
		ConfigGetTextArray("simpleHiddenSelections", selectionNames);
		int selectionId = selectionNames.Find(selectionName);
		SetSimpleHiddenSelectionState(selectionId, hide);
	};

	void OnOverloadAlert()
	{
		BtnCoverLightOn();

		if ( GetGame().IsClient()  ||  !GetGame().IsMultiplayer() )
		{
			UpdateOverloadStatusLights();
			m_UpdateOverloadStatusLightsTimer.Run( m_BlinkingStatusLightInterval/2 , this, "UpdateOverloadStatusLights", NULL, true);
		}
	}

	void OnOverload()
	{
		if (GetGame().IsServer())
		{
			AddHealth( "", "", -GetMaxHealth("", ""));
		}	
	}

	void OnOverloadRecovery()
	{
		ErrorLightOff();
		BtnCoverLightOff();

		if ( GetGame().IsClient()  ||  !GetGame().IsMultiplayer() )
		{
			UpdateOverloadStatusLights();
			m_UpdateOverloadStatusLightsTimer.Stop();
		}
	}

	void StartOverloadMonitor()
	{
		if(ITZ_FS_Module.GetModule().GetSettings().m_UseGeneratorOverloadSystem)
		{
			if (!m_OverloadMonitorTimer.IsRunning())
			{
				m_OverloadMonitorTimer.Run(1.0, this, "OnOverloadMonitorTick", NULL, true);
			}
		}	
	}

	void StopOverloadMonitor()
	{
		m_OverloadMonitorTimer.Stop();
	}

	void OnOverloadMonitorTick()
	{
		float activeTime = ITZ_FS_Module.GetModule().GetSettings().m_GeneratorOverloadActiveTime;
		float alertTime = activeTime * 0.7;
		float recoveryTime = ITZ_FS_Module.GetModule().GetSettings().m_GeneratorOverloadCooldown; // em segundos

		// definir o delta time
		float tickTime = GetGame().GetTickTime();
		if(!m_OverloadLastTickTime)
			m_OverloadLastTickTime = tickTime;
		
		float dtTime = tickTime - m_OverloadLastTickTime;

		if (GetCompEM().IsWorking())
		{
			m_OverloadACCTime += dtTime;
		}

		if (!m_IsOverloading && m_OverloadACCTime >= alertTime)
		{
			m_IsOverloading = true;
			OnOverloadAlert();
		}

		if (!m_IsOverloaded && m_OverloadACCTime >= activeTime)
		{
			m_IsOverloaded = true;
			m_OverloadLastTickTime = 0;
			m_OverloadRecoveryACCTime = 0;
			m_OverloadACCTime = 0;
			OnOverload(); // entra em sobrecarga
		}

		if (m_IsOverloading || m_IsOverloaded)
		{
			// Só inicia recuperação se estiver desligado
			if (!GetCompEM().IsWorking())
			{
				m_OverloadRecoveryACCTime += dtTime;

				if (m_OverloadRecoveryACCTime >= recoveryTime)
				{
					m_IsOverloaded = false;
					m_IsOverloading = false;
					m_OverloadRecoveryACCTime = 0;
					m_OverloadLastTickTime = 0;
					m_OverloadACCTime = 0;
					OnOverloadRecovery(); // volta ao normal
					StopOverloadMonitor();
					return;
				}
			}
			else
			{
				// Reset se religarem antes de completar recuperação
				m_OverloadRecoveryACCTime = 0;
			}
		}

		m_OverloadLastTickTime = tickTime;
	}

	override bool UseInFuelStation()
	{
		return true;
	}
}