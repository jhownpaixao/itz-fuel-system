
[CF_RegisterModule(ITZ_FS_Module)]
class ITZ_FS_Module : CF_ModuleWorld
{
    static ITZ_FS_Module                    m_Instance;
    ref array<ref ITZ_FS_FuelStation>       m_Stations;
    ref array<PowerGeneratorBase>        	m_Generators;
    ref array<ref ITZ_FS_VehicleProfile>    m_VehicleProfiles;
    ref ITZ_FS_FuelStationVariables         m_GlobalStationVariables;
    protected ITZ_FS_Settings               m_Settings;

    protected float m_UpdateCheckElapsed;
    protected float m_FuelIncrementElapsed;
    protected bool  m_SaveDirty;
    protected float m_SaveCheckElapsed;

    void ITZ_FS_Module()
    {
        m_Instance = this;
		m_SaveDirty = false;
        m_SaveCheckElapsed = 0.0;
        m_UpdateCheckElapsed = 0.0;
        m_FuelIncrementElapsed = 0.0;
    }

    static ITZ_FS_Module GetModule()
    {
        return m_Instance;
    }

    override void OnInit()
    {
        super.OnInit();

        CheckPaths();
        m_VehicleProfiles = new array<ref ITZ_FS_VehicleProfile>();
        m_Stations = new array<ref ITZ_FS_FuelStation>();
        m_Generators = new array<PowerGeneratorBase>();
        m_Settings = ITZ_FS_Settings.GetSettings();

        EnableRPC();
        #ifdef SERVER
        EnableInvokeConnect();
        EnableMissionFinish();
        EnableMissionStart();
        EnableUpdate();
        #endif

        ITZ_FS_Logger.Info("Iniciando módulo principal");
    }

    #ifdef SERVER
    override void OnMissionStart(Class sender, CF_EventArgs args)
    {
        super.OnMissionStart(sender, args);

        // create, migrate, load profiles
        if (FileExist(ITZ_FS_Constants.OLD_STATION_FILE) && m_Settings.m_Version < 102)
            MigrateOldStationProfiles();
        else if (FileExist(ITZ_FS_Constants.STATION_FILE))
            LoadStationProfiles();
        else
            GenerateFuelStationExample();

        // create, migrate, load profiles
        if (FileExist(ITZ_FS_Constants.OLD_VEHICLE_PROFILE_FILE) && m_Settings.m_Version < 102)
            MigrateOldVehicleProfiles();
        else if (FileExist(ITZ_FS_Constants.VEHICLE_PROFILE_FILE))
            LoadVehicleProfiles();
        else
            GenerateVehicleProfileExample();

        m_Settings.OnModuleInit();

        InitGenerators();
    }

    override void OnInvokeConnect(Class sender, CF_EventArgs args)
    {
        super.OnInvokeConnect(sender, args);
        auto playerArgs = CF_EventPlayerArgs.Cast(args);

        SyncToClient(playerArgs.Identity);
    }

    override void OnUpdate(Class sender, CF_EventArgs args)
    {
        super.OnUpdate(sender, args);
        CF_EventUpdateArgs updateArgs = CF_EventUpdateArgs.Cast(args);

        m_UpdateCheckElapsed += updateArgs.DeltaTime;
        m_SaveCheckElapsed += updateArgs.DeltaTime;
        m_FuelIncrementElapsed += updateArgs.DeltaTime;

        if (m_UpdateCheckElapsed >= m_Settings.m_ClientSyncCheckInterval)
        {
            m_UpdateCheckElapsed = 0.0;
            SyncStations();
        }

        if (m_SaveCheckElapsed >= m_Settings.m_DataSaveInterval && m_SaveDirty)
        {
            m_SaveCheckElapsed = 0.0;
            SaveStates();
        }
    }

    void SaveStates()
    {
        ITZ_FS_Logger.Info("Salvando estados das estações de combustível.");

        map<string, ref map<string, float>> quantityMap = new map<string, ref map<string, float>>();

        for (int i = 0; i < m_Stations.Count(); i++)
        {
            ITZ_FS_FuelStation station = m_Stations[i];
            if (!station) continue;

            map<string, float> fuelQuantities = new map<string, float>();
            for (int j = 0; j < station.GetFuels().Count(); j++)
            {
                ITZ_FS_FuelProfile fuelProfile = station.GetFuels()[j];
                if (!fuelProfile) continue;

                fuelQuantities.Insert(fuelProfile.GetFuelType(), fuelProfile.GetQuantity());
            }

            quantityMap.Insert(station.GetId(), fuelQuantities);
        }

        JsonFileLoader<map<string, ref map<string, float>>>.JsonSaveFile(ITZ_FS_Constants.PERSISTENT_FILE, quantityMap);

        m_SaveDirty = false;
    }
    #endif
    

    override int GetRPCMin()
    {
        return ITZ_FS_ModuleRPC.INVALID;
    }

    override int GetRPCMax()
    {
        return ITZ_FS_ModuleRPC.COUNT;
    }

    override void OnRPC(Class sender, CF_EventArgs args)
    {
        super.OnRPC(sender, args);
        auto rpc = CF_EventRPCArgs.Cast(args);

        switch (rpc.ID)
        {
            case ITZ_FS_ModuleRPC.SYNC:
                RPC_OnClientSync(rpc.Context, rpc.Sender, rpc.Target);
                break;
            case ITZ_FS_ModuleRPC.SYNC_DIRTY:
                RPC_OnClientSynchDirty(rpc.Context, rpc.Sender, rpc.Target);
                break;
        }
    }

    void RPC_OnClientSync(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
    {

        // m_Stations = new array<ref ITZ_FS_FuelStation>();
        
        // int stationCount;
        // ctx.Read(stationCount);

        // for (int i = 0; i < stationCount; i++)
        // {
        //     ITZ_FS_FuelStation station = new ITZ_FS_FuelStation();

        //     ctx.Read(station.m_ID);
        //     ctx.Read(station.m_Position);
        //     ctx.Read(station.m_Variables);

        //     int fuelCount;
        //     ctx.Read(fuelCount);
        //     for (int j = 0; j < fuelCount; j++)
        //     {
        //         ITZ_FS_FuelProfile profile = station.m_Fuels[j];
        //         ctx.Read(profile);

        //         station.m_Fuels.Insert(profile);
        //     }

        //     Print("RPC_OnClientSync: Estação sincronizada: " + station.GetId());
        //     m_Stations.Insert(station);
        // }

        
        // m_VehicleProfiles = new array<ref ITZ_FS_VehicleProfile>();

        // int vehicleProfileCount;
        // ctx.Read(vehicleProfileCount);

        // for (int k = 0; k < vehicleProfileCount; k++)
        // {
        //     ITZ_FS_VehicleProfile vProfile;
        //     ctx.Read(vProfile);

        //     Print("RPC_OnClientSync: Perfil de veículo sincronizado: " + vProfile.m_Classname);
        //     m_VehicleProfiles.Insert(vProfile);
        // }

        // ctx.Read(m_Settings);

        ctx.Read(m_Stations);
        ctx.Read(m_VehicleProfiles);
        ctx.Read(m_Settings);

        ITZ_FS_Logger.Debug("RPC_OnClientSync: Estações de combustível sincronizadas com o cliente: " + m_Stations.Count());
        ITZ_FS_Logger.Debug("RPC_OnClientSync: Perfis de veículos sincronizados com o cliente: " + m_VehicleProfiles.Count());
        ITZ_FS_Logger.Debug("RPC_OnClientSync: Configurações sincronizadas com o cliente");
    }

    void RPC_OnClientSynchDirty(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
    {
        array<ref ITZ_FS_FuelStation> stations = new array<ref ITZ_FS_FuelStation>();
        if (!ctx.Read(stations))
            return;

        foreach (ITZ_FS_FuelStation station: stations)
        {
            if (station)
            {
                int index = m_Stations.Find(station);
                ITZ_FS_FuelStation existingStation = GetStationById(station.GetId());
                if (existingStation)
                {
                    existingStation.SetSynchDirty(false);
                    existingStation.SetPosition(station.GetPosition());
                    existingStation.SetFuels(station.GetFuels());
                }   
            }
        }
    }

    void CheckPaths()
    {
        if (!FileExist(ITZ_FS_Constants.ITZ_PROFILE_FOLDER_NAME))
            MakeDirectory(ITZ_FS_Constants.ITZ_PROFILE_FOLDER_NAME);

        if (!FileExist(ITZ_FS_Constants.GENERAL_PATH))
            MakeDirectory(ITZ_FS_Constants.GENERAL_PATH);
    }

    void LoadStationProfiles()
    {
        map<string, ref map<string, float>> quantityMap = new map<string, ref map<string, float>>();

        if(FileExist(ITZ_FS_Constants.PERSISTENT_FILE))
        {
            JsonFileLoader<map<string, ref map<string, float>>>.JsonLoadFile(ITZ_FS_Constants.PERSISTENT_FILE, quantityMap);
        }

        CF_XML_Document document;
        CF_XML.ReadDocument(ITZ_FS_Constants.STATION_FILE, document);

        if (document)
		{
            CF_XML_Tag root = document.Get("stations")[0];
            if (!root)
			{
				CF.FormatError("No root tag 'stations' in '%1'", ITZ_FS_Constants.STATION_FILE);
				return;
			}

            //! defaults
            CF_XML_Tag defaults = root.GetTag("defaults")[0];
            m_GlobalStationVariables = new ITZ_FS_FuelStationVariables();
            if(defaults)
            {
                m_GlobalStationVariables.m_RespawnInterval = ITZ_FS_XMLHelper.GetTagContentAttributeFloat(defaults, "respawntime", "value", 300.0);
                m_GlobalStationVariables.m_MaxRange = ITZ_FS_XMLHelper.GetTagContentAttributeFloat(defaults, "range", "value", 100.0);
                
                //global flags
                CF_XML_Tag gFlags = defaults.GetTag("flags")[0];
                if(gFlags)
                {
                    m_GlobalStationVariables.m_RequiresEnergy = ITZ_FS_XMLHelper.GetAttributeInt(gFlags, "require_energy", 0) == 1;
                    m_GlobalStationVariables.m_RespawnFuels = ITZ_FS_XMLHelper.GetAttributeInt(gFlags, "respawn_fuel", 0) == 1;
                    m_GlobalStationVariables.m_CanUseDestroyed = ITZ_FS_XMLHelper.GetAttributeInt(gFlags, "use_destroyed", 0) == 1;
                    m_GlobalStationVariables.m_CanMeasure = ITZ_FS_XMLHelper.GetAttributeInt(gFlags, "can_measure", 0) == 1;
                }
            }

            //! stations
            array<CF_XML_Tag> stationTags = root.GetTag("station");
            foreach (CF_XML_Tag stationTag: stationTags)
            {
                string id = ITZ_FS_XMLHelper.GetAttributeString(stationTag, "name", "");
                if(id == "")
                {
                    ITZ_FS_Logger.Warn("LoadStationProfiles: Estação de combustível sem ID encontrada, ignorando.");
                    continue;
                }

                ITZ_FS_FuelStation station = new ITZ_FS_FuelStation();
                station.SetId(id);

                string pos = ITZ_FS_XMLHelper.GetAttributeString(stationTag, "pos", "0 0 0");
                station.SetPosition(pos.ToVector());

                //!variables
                ITZ_FS_FuelStationVariables variables = new ITZ_FS_FuelStationVariables();
                variables.m_RespawnInterval = ITZ_FS_XMLHelper.GetTagContentAttributeFloat(stationTag, "respawntime", "value", m_GlobalStationVariables.m_RespawnInterval);
                variables.m_MaxRange =  ITZ_FS_XMLHelper.GetTagContentAttributeFloat(stationTag, "range", "value", m_GlobalStationVariables.m_MaxRange);
                variables.m_RequiresEnergy = m_GlobalStationVariables.m_RequiresEnergy;
                variables.m_RespawnFuels = m_GlobalStationVariables.m_RespawnFuels;
                variables.m_CanUseDestroyed = m_GlobalStationVariables.m_CanUseDestroyed;
                variables.m_CanMeasure = m_GlobalStationVariables.m_CanMeasure;

                // station flags
                CF_XML_Tag flags = stationTag.GetTag("flags")[0];
                if(flags)
                {
                    variables.m_RequiresEnergy = ITZ_FS_XMLHelper.GetAttributeBool(flags, "require_energy", m_GlobalStationVariables.m_RequiresEnergy);
                    variables.m_RespawnFuels = ITZ_FS_XMLHelper.GetAttributeBool(flags, "respawn_fuel", m_GlobalStationVariables.m_RespawnFuels);
                    variables.m_CanUseDestroyed = ITZ_FS_XMLHelper.GetAttributeBool(flags, "use_destroyed", m_GlobalStationVariables.m_CanUseDestroyed);
                    variables.m_CanMeasure = ITZ_FS_XMLHelper.GetAttributeBool(flags, "can_measure", m_GlobalStationVariables.m_CanMeasure);
                }

                station.SetVariables(variables);

                map<string, float> fuelQuantities = new map<string, float>();
                if(quantityMap.Get(id))
                    fuelQuantities = quantityMap.Get(id);

                // fuels
                array<CF_XML_Tag> fuelTags = stationTag.GetTag("fuel");
                foreach (CF_XML_Tag fuelTag: fuelTags)
                {
                    string type = ITZ_FS_XMLHelper.GetAttributeString(fuelTag, "type", "");
                    if (type == "")
                    {
                        ITZ_FS_Logger.Warn("LoadStationProfiles: Perfil de combustível sem tipo encontrado na estação " + id + ", ignorando.");
                        continue;
                    }

                    float capacity = ITZ_FS_XMLHelper.GetAttributeFloat(fuelTag, "capacity", 1000.0);
                    float minRespawn = ITZ_FS_XMLHelper.GetAttributeFloat(fuelTag, "min", 10.0);
                    float maxRespawn = ITZ_FS_XMLHelper.GetAttributeFloat(fuelTag, "max", 50.0);

                    ITZ_FS_FuelProfile fuelProfile = new ITZ_FS_FuelProfile();
                    fuelProfile.SetLiquidType(type);
                    fuelProfile.SetCapacity(capacity);
                    fuelProfile.SetMin(minRespawn);
                    fuelProfile.SetMax(maxRespawn);

                    float quantity = capacity;
                    if(fuelQuantities.Contains(type))
                        quantity = fuelQuantities.Get(type);
                    
                    fuelProfile.SetQuantity(quantity);

                    station.GetFuels().Insert(fuelProfile);
                }

                station.RespawnFuels(); // timeout ou restart
                m_Stations.Insert(station);

            }
        }   

        ITZ_FS_Logger.Info("LoadStationProfiles: Estações de combustível carregadas: " + m_Stations.Count());
    }

    void MigrateOldStationProfiles()
    {
        JsonFileLoader<array<ref ITZ_FS_FuelStation>>.JsonLoadFile(ITZ_FS_Constants.OLD_STATION_FILE, m_Stations);
        ITZ_FS_Logger.Info("LoadStationProfiles: Estações de combustível carregadas: " + m_Stations.Count());

        m_GlobalStationVariables = new ITZ_FS_FuelStationVariables();
        m_GlobalStationVariables.m_RespawnInterval = m_Settings.old_FuelRespawnInterval;
        m_GlobalStationVariables.m_MaxRange = m_Settings.old_FuelStationRange;
        m_GlobalStationVariables.m_RequiresEnergy = m_Settings.old_FuelPumpRequiresPower;
        m_GlobalStationVariables.m_RespawnFuels = true;
        m_GlobalStationVariables.m_CanUseDestroyed = m_Settings.old_UseDestroyedPump;
        m_GlobalStationVariables.m_CanMeasure = m_Settings.old_CanMeasureFuel;

        for (int i = 0; i < m_Stations.Count(); i++)
        {
            ITZ_FS_FuelStation station = m_Stations[i];
            if (!station) continue;

            for (int j = 0; j < station.GetFuels().Count(); j++)
            {
                ITZ_FS_FuelProfile fuelProfile = station.GetFuels()[j];
                if (!fuelProfile) continue;

                // migrate min/max respawn values
                
                fuelProfile.SetMin(fuelProfile.m_MinLiquidRespawn);
                fuelProfile.SetMax(fuelProfile.m_MaxLiquidRespawn);
            }

            station.SetVariables(m_GlobalStationVariables);
        }

        SaveStationProfiles();

        DeleteFile(ITZ_FS_Constants.OLD_STATION_FILE);
    }

    void LoadVehicleProfiles()
    {
        CF_XML_Document document;
        CF_XML.ReadDocument(ITZ_FS_Constants.VEHICLE_PROFILE_FILE, document);

        if (document)
        {
            CF_XML_Tag root = document.Get("vehicles")[0];
            if (!root)
            {
                CF.FormatError("No root tag 'vehicles' in '%1'", ITZ_FS_Constants.VEHICLE_PROFILE_FILE);
                return;
            }

            array<CF_XML_Tag> vehicleTags = root.GetTag("vehicle");
            foreach (CF_XML_Tag vehicleTag: vehicleTags)
            {
                string type = ITZ_FS_XMLHelper.GetAttributeString(vehicleTag, "type", "");
                if(type == "")
                {
                    ITZ_FS_Logger.Warn("LoadVehicleProfiles: Perfil de veículo sem tipo encontrado, ignorando.");
                    continue;
                }

                string fuel = ITZ_FS_XMLHelper.GetAttributeString(vehicleTag, "fuel", ITZ_FS_Constants.NAMED_FUEL_GASOLINE);
                float consumption = ITZ_FS_XMLHelper.GetAttributeFloat(vehicleTag, "consumption", 1.0);

                ITZ_FS_VehicleProfile profile = new ITZ_FS_VehicleProfile();
                profile.m_Classname = type;
                profile.m_FuelType = fuel;
                profile.m_FuelConsumptionPerKm = consumption;

                m_VehicleProfiles.Insert(profile);
            }
        }
       
        ITZ_FS_Logger.Info("LoadVehicleProfiles: Perfis de veículos carregados: " + m_VehicleProfiles.Count());
        OnVehicleProfileLoaded();
    }

    void MigrateOldVehicleProfiles()
    {
        JsonFileLoader<array<ref ITZ_FS_VehicleProfile>>.JsonLoadFile(ITZ_FS_Constants.OLD_VEHICLE_PROFILE_FILE, m_VehicleProfiles);
        SaveVehicleProfiles();

        ITZ_FS_Logger.Info("LoadVehicleProfiles: Perfis de veículos migrados para o novo formato: " + m_VehicleProfiles.Count());
        OnVehicleProfileLoaded();

        DeleteFile(ITZ_FS_Constants.OLD_VEHICLE_PROFILE_FILE);
    }

    void OnVehicleProfileLoaded()
    {
        array<string> vehicleNames = new array<string>();
        ITZ_FS_VehicleProfile profile;

        for (int i = 0; i < m_VehicleProfiles.Count(); i++)
        {
            profile = m_VehicleProfiles[i];
            if (!profile) continue;

            vehicleNames.Insert(profile.m_Classname);
        }

        if(m_Settings.m_Version < 101)
        {
            // Adicionar suporte ao BoatScript
            if(vehicleNames.Find("BoatScript") == -1)
            {
                profile = new ITZ_FS_VehicleProfile();
                profile.m_Classname = "BoatScript";
                profile.m_FuelType = ITZ_FS_Constants.NAMED_FUEL_GASOLINE;
                profile.m_FuelConsumptionPerKm = 1.15;
                m_VehicleProfiles.Insert(profile);
                SaveVehicleProfiles();
            }
        }
    }
    
    void SaveStationProfiles()
    {
        map<string, ref map<string, float>> quantityMap = new map<string, ref map<string, float>>();

        CF_XML_Document document = new CF_XML_Document();
        CF_XML_Tag root = document.CreateTag("stations");

        //! defaults
        CF_XML_Tag defaults = root.CreateTag("defaults");

        CF_XML_Tag gRespawnTime = defaults.CreateTag("respawntime");
        CF_XML_Attribute gRespawnTimeAttr = gRespawnTime.CreateAttribute("value");
        gRespawnTimeAttr.SetValue(m_GlobalStationVariables.m_RespawnInterval.ToString()); // segundos

        CF_XML_Tag gRange = defaults.CreateTag("range");
        CF_XML_Attribute gRangeAttr = gRange.CreateAttribute("value");
        gRangeAttr.SetValue(m_GlobalStationVariables.m_MaxRange.ToString());

        //global flags
        CF_XML_Tag gFlags = defaults.CreateTag("flags");
        CF_XML_Attribute gRequireEnergy = gFlags.CreateAttribute("require_energy");
        string gRequireEnergyValue = "0";
        if(m_GlobalStationVariables.m_RequiresEnergy)
            gRequireEnergyValue = "1";
        gRequireEnergy.SetValue(gRequireEnergyValue);

        CF_XML_Attribute gRespawnFuel = gFlags.CreateAttribute("respawn_fuel");
        string respawnFuelValue = "0";
        if(m_GlobalStationVariables.m_RespawnFuels)
            respawnFuelValue = "1";
        gRespawnFuel.SetValue(respawnFuelValue);

        string useDestroyedValue = "0";
        if(m_GlobalStationVariables.m_CanUseDestroyed)
            useDestroyedValue = "1";
        CF_XML_Attribute gUseDestroyed = gFlags.CreateAttribute("use_destroyed");
        gUseDestroyed.SetValue(useDestroyedValue);

        string canMeasureValue = "0";
        if(m_GlobalStationVariables.m_CanMeasure)
            canMeasureValue = "1";
        CF_XML_Attribute gCanMeasure = gFlags.CreateAttribute("can_measure");
        gCanMeasure.SetValue(canMeasureValue);

        //! stations
        for(int i = 0; i < m_Stations.Count(); i++)
        {
            ITZ_FS_FuelStation station = m_Stations[i];
            if (!station) continue;

            CF_XML_Tag stationTag = root.CreateTag("station");

            CF_XML_Attribute nameAttr = stationTag.CreateAttribute("name");
            nameAttr.SetValue(station.GetId());

            CF_XML_Attribute posAttr = stationTag.CreateAttribute("pos");
            vector pos = station.GetPosition();
            posAttr.SetValue(pos.ToString(false));

            // CF_XML_Tag respawnTime = stationTag.CreateTag("respawntime");
            // CF_XML_Attribute respawnTimeAttr = respawnTime.CreateAttribute("value");
            // respawnTimeAttr.SetValue("300"); // segundos

            // CF_XML_Tag range = stationTag.CreateTag("range");
            // CF_XML_Attribute rangeAttr = range.CreateAttribute("value");
            // rangeAttr.SetValue("50.0");

            // // flags
            // CF_XML_Tag flags = stationTag.CreateTag("flags");

            // CF_XML_Attribute requireEnergy = flags.CreateAttribute("require_energy");
            // requireEnergy.SetValue("1");
            // CF_XML_Attribute respawnFuel = flags.CreateAttribute("respawn_fuel");
            // respawnFuel.SetValue("1");
            // CF_XML_Attribute useDestroyed = flags.CreateAttribute("use_destroyed");
            // useDestroyed.SetValue("1");
            // CF_XML_Attribute canMeasure = flags.CreateAttribute("can_measure");
            // canMeasure.SetValue("1");

            // fuels
            map<string, float> fuelQuantities = new map<string, float>();
            for(int j = 0; j < station.GetFuels().Count(); j++)
            {
                ITZ_FS_FuelProfile fuelProfile = station.GetFuels()[j];
                if(!fuelProfile) continue;

                CF_XML_Tag fuelTag = stationTag.CreateTag("fuel");

                CF_XML_Attribute typeTag = fuelTag.CreateAttribute("type");
                typeTag.SetValue(fuelProfile.GetFuelType());

                CF_XML_Attribute capacityTag = fuelTag.CreateAttribute("capacity");
                capacityTag.SetValue(fuelProfile.GetCapacity().ToString());

                CF_XML_Attribute minTag = fuelTag.CreateAttribute("min");
                minTag.SetValue(fuelProfile.GetMin().ToString());

                CF_XML_Attribute maxTag = fuelTag.CreateAttribute("max");
                maxTag.SetValue(fuelProfile.GetMax().ToString());

                fuelQuantities.Insert(fuelProfile.GetFuelType(), fuelProfile.GetQuantity());
            }

            quantityMap.Insert(station.GetId(), fuelQuantities);
        }

        document.Save(ITZ_FS_Constants.STATION_FILE);
        JsonFileLoader<map<string, ref map<string, float>>>.JsonSaveFile(ITZ_FS_Constants.PERSISTENT_FILE, quantityMap);
    }

    void SaveVehicleProfiles()
    {
        CF_XML_Document document = new CF_XML_Document();
        CF_XML_Tag root = document.CreateTag("vehicles");

        foreach (ITZ_FS_VehicleProfile profile: m_VehicleProfiles)
        {
            CF_XML_Tag vehicleTag = root.CreateTag("vehicle");

            CF_XML_Attribute typeAttr = vehicleTag.CreateAttribute("type");
            typeAttr.SetValue(profile.m_Classname);

            CF_XML_Attribute fuelAttr = vehicleTag.CreateAttribute("fuel");
            fuelAttr.SetValue(profile.m_FuelType);

            CF_XML_Attribute consumptionAttr = vehicleTag.CreateAttribute("consumption");
            consumptionAttr.SetValue(profile.m_FuelConsumptionPerKm.ToString());
        }

        document.Save(ITZ_FS_Constants.VEHICLE_PROFILE_FILE);

    }

    void GenerateFuelStationExample()
    {
        ITZ_FS_Logger.Info("Gerando exemplo de estação de combustível.");
        m_Stations = ITZ_FS_ProfileConstructor.GetDefaultFuelStations();

        m_GlobalStationVariables = new ITZ_FS_FuelStationVariables();
        m_GlobalStationVariables.m_RespawnInterval = 300.0;
        m_GlobalStationVariables.m_MaxRange = 50.0;
        m_GlobalStationVariables.m_RequiresEnergy = true;
        m_GlobalStationVariables.m_RespawnFuels = true;
        m_GlobalStationVariables.m_CanUseDestroyed = true;
        m_GlobalStationVariables.m_CanMeasure = true;

        SaveStationProfiles();
    }

    
    void GenerateVehicleProfileExample()
    {
        ITZ_FS_Logger.Info("Gerando exemplo de perfil de veículo.");
        m_VehicleProfiles = ITZ_FS_ProfileConstructor.GetDefaultVehicleProfiles();
        SaveVehicleProfiles();
    }

    void SyncToClient(PlayerIdentity identity)
    {
        if (!identity)
            return;

        ITZ_FS_Logger.Debug("Sincronizando dados com o cliente: " + identity.GetName());

        ScriptRPC rpc = new ScriptRPC();

        
        // rpc.Write(m_Stations.Count());
        // for (int i = 0; i < m_Stations.Count(); i++)
        // {
        //     ITZ_FS_FuelStation station = m_Stations[i];
        //     rpc.Write(station.m_ID);
        //     rpc.Write(station.m_Position);
        //     rpc.Write(station.m_Variables);

        //     rpc.Write(station.m_Fuels.Count());
        //     for (int j = 0; j < station.m_Fuels.Count(); j++)
        //     {
        //         ITZ_FS_FuelProfile profile = station.m_Fuels[j];
        //         rpc.Write(profile);
        //     }

        //     Print("Syncing station: " + station.GetId());
        // }

        // rpc.Write(m_VehicleProfiles.Count());
        // for (int l = 0; l < m_VehicleProfiles.Count(); l++)
        // {
        //     ITZ_FS_VehicleProfile vProfile = m_VehicleProfiles[l];
        //     Print("Syncing vehicle profile: " + vProfile.m_Classname);
        //     rpc.Write(vProfile);
        // }

        // Print("Syncing settings");
        // rpc.Write(m_Settings);


        rpc.Write(m_Stations);
        rpc.Write(m_VehicleProfiles);
        rpc.Write(m_Settings);
        rpc.Send(NULL, ITZ_FS_ModuleRPC.SYNC, true, identity);
    }

    void RegisterGenerator(PowerGeneratorBase generator)
    {
        if (generator && m_Generators.Find(generator) == -1)
        {
            m_Generators.Insert(generator);
            ITZ_FS_Logger.Debug("Gerador registrado: " + generator);
        }
    }

    void UnregisterGenerator(PowerGeneratorBase generator)
    {
        int idx = m_Generators.Find(generator);
        if(idx != -1)
            m_Generators.Remove(idx);
    }

    void InitGenerators()
    {
        
        ITZ_FS_Logger.Debug("Inicializando geradores.");

        // Registra todos os geradores existentes no módulo
       for (int i = 0; i < m_Generators.Count(); i++)
        {
            PowerGeneratorBase generator = m_Generators[i];
            if (generator)
            {
                ITZ_FS_FuelStation fuelStation = ITZ_FS_Module.GetModule().GetStationProfile(generator);
                if(fuelStation)
                {
                    generator.SetStationId(fuelStation.GetId());
                }
            }
        }
    
    }

    ITZ_FS_FuelStation GetStationProfile(EntityAI target)
    {
        if(!target)
            return null;

        return GetStationProfileAtPosition(target.GetPosition());
    }

    ITZ_FS_FuelStation GetStationProfileAtPosition(vector pos)
    {
        foreach (ITZ_FS_FuelStation station: m_Stations)
        {
            if (station && Math.IsPointInCircle(station.GetPosition(), station.m_Variables.m_MaxRange, pos))
                return station;
		}

        return null;
    }

    void SetHasEnergy(string fuelId, bool hasEnergy)
    {
        ITZ_FS_FuelStation profile = GetStationById(fuelId);
        if (profile)
        {
            array<FuelStation> stations = GetFuelPumpsAtPosition(profile.GetPosition());
            ITZ_FS_Logger.Debug("SetHasEnergy: Atualizando energia para " + stations.Count() + " estações de combustível na posição: " + profile.GetPosition());

            foreach (FuelStation station: stations)
            {
                if (station)
                {
                    station.SetHasEnergy(hasEnergy);
                }
            }
        } 
    }

    ITZ_FS_FuelStation GetStationById(string id)
    {
        foreach (ITZ_FS_FuelStation station: m_Stations)
        {
            if (station && station.GetId() == id)
            {
                return station;
            }
        }
        return null;
    }
    
    void SyncStations()
    {
        array<ref ITZ_FS_FuelStation> stations = new array<ref ITZ_FS_FuelStation>();

        foreach (ITZ_FS_FuelStation station: m_Stations)
        {
            if (station && station.GetSynchDirty())
            {
                stations.Insert(station);
                station.SetSynchDirty(false);
                m_SaveDirty = true;
            }
        }

        if (stations.Count() > 0)
        {
            ITZ_FS_Logger.Debug("SyncStations: Enviando " + stations.Count() + " estações sujas para o cliente.");
            ScriptRPC rpc = new ScriptRPC();
            rpc.Write(stations);
            rpc.Send(NULL, ITZ_FS_ModuleRPC.SYNC_DIRTY, true, null);
        }
    }

    FuelStation GetFuelPumpAtPosition(vector position)
    {
        array<Object> objects = new array<Object>;
        GetGame().GetObjectsAtPosition(position, m_Settings.m_RefillSelectionMaxRange, objects, null);
        
        foreach (Object obj: objects)
        {
            if (obj.IsInherited(FuelStation))
            {
                FuelStation station = FuelStation.Cast(obj);
              
                if (station)
                {
                    float distance = vector.DistanceSq(position, station.GetWorldPosition());
                    if (distance < m_Settings.m_RefillSelectionMaxRange)
                    {
                        return station;
                    }
                }
            }
        }
		return null;
    }

    FuelStation GetFuelPumpForTransport(Transport transport)
    {
        if (transport)
		return ITZ_FS_Module.GetModule().GetFuelPumpAtPosition( transport.GetRefillPointPosWS() );

        return null;
    }

    array<FuelStation> GetFuelPumpsAtPosition(vector position)
    {
        ITZ_FS_FuelStation stationProfile = GetStationProfileAtPosition(position);
        
        if (!stationProfile)
            return new array<FuelStation>;
        

        array<FuelStation> stations = new array<FuelStation>;
        
        array<Object> objects = new array<Object>;
        GetGame().GetObjectsAtPosition(position, stationProfile.m_Variables.m_MaxRange, objects, null);

        foreach (Object obj: objects)
        {
            FuelStation station = FuelStation.Cast(obj);
            if (station)
                stations.Insert(station);
        }

		return stations;
    }

    void LogFuelStation(ITZ_FS_FuelStation station)
    {
        return;
        if (station)
        {
            ITZ_FS_Logger.Trace("LogFuelStation: Station ID: " + station.GetId());
            ITZ_FS_Logger.Trace("LogFuelStation: Station Position: " + station.GetPosition());
            ITZ_FS_Logger.Trace("LogFuelStation: Station Fuel Profiles: " + station.GetFuels().Count());

            ITZ_FS_Logger.Trace("LogFuelStation: Station Variables - Respawn Interval: " + station.m_Variables.m_RespawnInterval);
            ITZ_FS_Logger.Trace("LogFuelStation: Station Variables - Max Range: " + station.m_Variables.m_MaxRange);
            ITZ_FS_Logger.Trace("LogFuelStation: Station Variables - Requires Energy: " + station.m_Variables.m_RequiresEnergy);
            ITZ_FS_Logger.Trace("LogFuelStation: Station Variables - Respawn Fuels: " + station.m_Variables.m_RespawnFuels);
            ITZ_FS_Logger.Trace("LogFuelStation: Station Variables - Can Use Destroyed: " + station.m_Variables.m_CanUseDestroyed);
            ITZ_FS_Logger.Trace("LogFuelStation: Station Variables - Can Measure: " + station.m_Variables.m_CanMeasure);

            foreach (ITZ_FS_FuelProfile profile: station.GetFuels())
            {
                ITZ_FS_Logger.Trace("LogFuelStation: Fuel Profile Liquid Type: " + profile.GetLiquidType());
                ITZ_FS_Logger.Trace("LogFuelStation: Fuel Profile Capacity: " + profile.GetCapacity());
                ITZ_FS_Logger.Trace("LogFuelStation: Fuel Profile Quantity: " + profile.GetQuantity());
                ITZ_FS_Logger.Trace("LogFuelStation: Fuel Profile Min Liquid : " + profile.GetMin());
                ITZ_FS_Logger.Trace("LogFuelStation: Fuel Profile Max Liquid : " + profile.GetMax());
            }
        }
    }

    ITZ_FS_Settings GetSettings()
    {
        return m_Settings;
    }

    ITZ_FS_VehicleProfile GetVehicleProfile(Object vehicle)
    {
        ITZ_FS_VehicleProfile vehicleProfile;

        if(vehicle && m_VehicleProfiles)
        {
            for (int i = 0; i < m_VehicleProfiles.Count(); i++)
            {
                ITZ_FS_VehicleProfile profile = m_VehicleProfiles[i];
                if(!profile) continue;

                //! se for o mesmo nome retorna aqui
                if(profile.m_Classname == vehicle.GetType())
                {
                    return profile;
                }

                //! verificar mais proximo por ordem definida (sobreescrita)
                if (vehicle.IsKindOf(profile.m_Classname))
                {
                    vehicleProfile = profile;
                }
            }
        }
        
        return vehicleProfile;
    }

    int GetVehicleFuelType(Object vehicle)
    {
        ITZ_FS_VehicleProfile profile = GetVehicleProfile(vehicle);
        if (profile)
        {
            return ITZ_FS_Constants.GetFuelTypeFromNamedType(profile.m_FuelType);
        }

        return LIQUID_GASOLINE; // Default fuel type
    }

    float CalcFuelConsumption(Object transport, vector currentPos, vector lastPos, float dt, float fuelPerKm, float distMdfr, float idleConsumption, float rpmMdfr)
	{
        CarScript car = CarScript.Cast(transport);
        BoatScript boat = BoatScript.Cast(transport);

        #ifdef RFWC
        RFWC_base rfwBoat = RFWC_base.Cast(transport);
        #endif

        #ifdef RFFS_HELI
        RFFSHeli_base rffHeli = RFFSHeli_base.Cast(transport);
        #endif

        #ifdef EXPANSIONMODVEHICLE
		ExpansionHelicopterScript expansionHeli = ExpansionHelicopterScript.Cast(transport);
        #endif

        float rpm;
		float rpmMax;
		float rpmIdle;

        if(car)
        {
            rpm = car.EngineGetRPM();
            rpmMax = car.EngineGetRPMMax();
            rpmIdle = car.EngineGetRPMIdle();
        }
        else if(boat)
        {
            rpm = boat.EngineGetRPM();
            rpmMax = boat.EngineGetRPMMax();
            rpmIdle = boat.EngineGetRPMIdle();
        }

        #ifdef RFWC
        if (rfwBoat)
        {
            rpm = Math.AbsFloat(rfwBoat.m_throttle_level);
            rpmMax = 20.0;
            rpmIdle = 0.1 * rpmMax;
        }
        #endif

        #ifdef RFFS_HELI
        if (rffHeli)
        {
            rpm = Math.AbsFloat(rffHeli.m_collective_level);
            rpmMax = 20.0;
            rpmIdle = 0.1 * rpmMax;
        }
        #endif

        #ifdef EXPANSIONMODVEHICLE
        if (expansionHeli)
        {
            rpm = Math.AbsFloat(expansionHeli.m_Simulation.m_MainRotorSpeed);
            rpmMax = 1.0;
            rpmIdle = 0.20 * rpmMax;
        }
        #endif

		float fuelToConsume = 0;
		float rpmFactor = Math.Clamp((rpm / rpmMax) * rpmMdfr, 0.0, rpmMdfr);

		if(rpm > rpmIdle && lastPos != vector.Zero)
		{
			float dtDistance = vector.Distance(currentPos, lastPos); // em metros
			float baseConsumption = fuelPerKm * (dtDistance / 1000.0);
			fuelToConsume = baseConsumption * distMdfr * rpmFactor;
		}
		else
		{
			fuelToConsume = idleConsumption * dt * rpmFactor;
		}

		// Print(string.Format("TRANSPORT: %1 | RPM: %2 | RPMIdle: %3 | Distance: %4 | FuelToConsume: %5", transport, rpm, rpmIdle, dtDistance, fuelToConsume));
		return fuelToConsume;
	}

    ITZ_FS_LogLevel GetLogLevel()
    {
        if (!m_Settings)
        {
            return ITZ_FS_LogLevel.TRACE;
        }

        return m_Settings.m_LogLevel;
    }
}