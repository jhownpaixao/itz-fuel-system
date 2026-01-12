class ITZ_FS_Settings
{
    [NonSerialized()]
    static ref ITZ_FS_Settings  m_Instance;
    int     m_Version;
    int     m_ClientSyncCheckInterval; //intervalo em segundos para sincronizar estações de combustível com o cliente
    int     m_DataSaveInterval; //intervalo em segundos para salvar o estado e combustível das estações

    bool    m_UseGeneratorOverloadSystem; //se deve usar o sistema de sobrecarga do gerador
    int     m_GeneratorOverloadActiveTime; //tempo em segundos para sobrecarregar o gerador
    int     m_GeneratorOverloadCooldown; //tempo em segundos para o gerador voltar ao normal após aviso de sobrecarga

    int     m_RefillSelectionMaxRange; // distância máxima para selecionar uma bomba de combustível para reabastecimento (apartir do pixel do tank do veículo)
    bool    m_VehicleRefuelDirectly; // se o veículo pode ser reabastecido diretamente na bomba de combustível

    ITZ_FS_LogLevel m_LogLevel; // nível de log do sistema

    [NonSerialized()]
    int     old_FuelRespawnInterval; //!@deprecated
    [NonSerialized()]
    int     old_FuelStationRange; //!@deprecated
    [NonSerialized()]
    bool    old_FuelPumpRequiresPower; //!@deprecated
    [NonSerialized()]
    int     old_FuelPumpPowerConsumptionPerSec; //!@deprecated
    [NonSerialized()]
    bool    old_UseDestroyedPump; //!@deprecated
    [NonSerialized()]
    bool    old_CanMeasureFuel; //!@deprecated
    
    #ifdef SERVER
    void ITZ_FS_Settings()
    {
        //! DEFINIDO AQUI POIS VAI SER USADO ANTES DAS CONFIGURAÇÕES SEREM LIDAS
        m_LogLevel = ITZ_FS_LogLevel.INFO;

        if (FileExist(ITZ_FS_Constants.SETTING_FILE))
            LoadConfig();
        else
            OnDefaults();
    }

    void OnModuleInit()
    {
        if(m_Version != ITZ_FS_Constants.FS_VERSION)
        {
            OnMigrate();

            m_Version = ITZ_FS_Constants.FS_VERSION;
            SaveConfig();
        }
    }
    

    void LoadConfig()
    {
        JsonFileLoader<ITZ_FS_Settings>.JsonLoadFile(ITZ_FS_Constants.SETTING_FILE, this);

        if(m_Version < 102)
        {
            //! isto é certo?
            //! as opções foram removidas para diminuir lixo nos arquivos, vamos fazer um proxy aqui
            Print("[ITZ][FuelSystem] Migrando configurações para a versão 101.");
            OLD_ITZ_FS_Settings oldSettings = new OLD_ITZ_FS_Settings();
            JsonFileLoader<OLD_ITZ_FS_Settings>.JsonLoadFile(ITZ_FS_Constants.SETTING_FILE, oldSettings);

            old_FuelRespawnInterval = oldSettings.m_FuelRespawnInterval;
            old_FuelStationRange = oldSettings.m_FuelStationRange;
            old_FuelPumpRequiresPower = oldSettings.m_FuelPumpRequiresPower;
            old_FuelPumpPowerConsumptionPerSec = oldSettings.m_FuelPumpPowerConsumptionPerSec;
            old_UseDestroyedPump = oldSettings.m_UseDestroyedPump;
            old_CanMeasureFuel = oldSettings.m_CanMeasureFuel;

            Print("old_FuelRespawnInterval = " + old_FuelRespawnInterval);
            Print("old_FuelStationRange = " + old_FuelStationRange);
            Print("old_FuelPumpRequiresPower = " + old_FuelPumpRequiresPower);
            Print("old_FuelPumpPowerConsumptionPerSec = " + old_FuelPumpPowerConsumptionPerSec);
            Print("old_UseDestroyedPump = " + old_UseDestroyedPump);
            Print("old_CanMeasureFuel = " + old_CanMeasureFuel);

            SaveConfig();
        }
    }

    void SaveConfig()
    {
        JsonFileLoader<ITZ_FS_Settings>.JsonSaveFile(ITZ_FS_Constants.SETTING_FILE, this);
    }

    void OnDefaults()
    {
        Print("[ITZ][FuelSystem] Nenhuma configuração encontrada, usando valores padrão.");
        m_Version = ITZ_FS_Constants.FS_VERSION;
        
        m_ClientSyncCheckInterval = 1; // 1 segundo
        m_DataSaveInterval        = 3; // 3 segundos

        m_UseGeneratorOverloadSystem  = false;
        m_GeneratorOverloadActiveTime = 4 * 60; // 4 minutos
        m_GeneratorOverloadCooldown   = 30; // 30 segundos

        m_RefillSelectionMaxRange  = 6; // 6 mts 
        m_VehicleRefuelDirectly    = true;
        m_LogLevel                 = ITZ_FS_LogLevel.INFO;

        SaveConfig();
    }

    void OnMigrate() 
    {
        if(m_Version < 102)
        {
            Print("[ITZ][FuelSystem] Migrando configurações para a versão 102.");

            DeleteFile(ITZ_FS_Constants.SETTING_FILE);
            SaveConfig();
        } 
    }
    
    #endif

    static ITZ_FS_Settings GetSettings()
    {
        if (!m_Instance)
        {
            m_Instance = new ITZ_FS_Settings();
        }
            
        return m_Instance;
    }
}

class OLD_ITZ_FS_Settings extends ITZ_FS_Settings
{
    int     m_FuelRespawnInterval; //!@deprecated
    int     m_FuelStationRange; //!@deprecated
    bool    m_FuelPumpRequiresPower; //!@deprecated
    int     m_FuelPumpPowerConsumptionPerSec; //!@deprecated
    bool    m_UseDestroyedPump; //!@deprecated
    bool    m_CanMeasureFuel; //!@deprecated

    void LoadConfig()
    {
        //!stackoverflow
    }
	
	void OnDefaults()
	{
        //!stackoverflow
	}
}