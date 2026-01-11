class ITZ_FS_ProfileConstructor
{
    static array<ref ITZ_FS_FuelStation> GetDefaultFuelStations()
    {

        array<ref ITZ_FS_FuelStation> stations = new array<ref ITZ_FS_FuelStation>;

		float mapSize = GetGame().GetWorld().GetWorldSize();
        float centerX = mapSize / 2;
        float centerZ = mapSize / 2;
        vector centerCoords = Vector(centerX, 0, centerZ);

        ITZ_FS_Logger.Debug("mapSize: " + mapSize + ", centerCoords: " + centerCoords + ", centerX: " + centerX + ", centerZ: " + centerZ);
        array<vector> registredPositions = new array<vector>;
        array<ref Param3<string, string, vector>> worldLocations = GetWorldLocations();

        array<Object> objects = new array<Object>;
        GetGame().GetObjectsAtPosition(centerCoords, mapSize, objects, null);

        ITZ_FS_Logger.Debug("Found " + objects.Count() + " objects in the world.");
        for (int i = 0; i < objects.Count(); i++)
        {
            FuelStation station = FuelStation.Cast(objects[i]);
            if (station)
            {
                ITZ_FS_Logger.Debug("Found FuelStation at position: " + station.GetWorldPosition());
                bool isRegistred = false;
                vector stationPos = station.GetWorldPosition();

                foreach (vector pos: registredPositions)
                {
                    if (vector.DistanceSq(pos, stationPos) < 1000) // 1000 is a threshold for close proximity
                    {
                        isRegistred = true;
                        break;
                    }
                }

                if (!isRegistred)
                {
                    ITZ_FS_Logger.Debug("Registering new FuelStation at position: " + stationPos);
                    registredPositions.Insert(stationPos);
                    Param3<string, string, vector> closestLocation = GetClosestLocation(worldLocations, stationPos);
                    string locationCardnality = GetLocationCardinalityRelation(closestLocation.param3, stationPos);
                    ITZ_FS_FuelStation profile = new ITZ_FS_FuelStation();
                    profile.SetPosition(stationPos);
                    profile.SetFuels(GetFuelProfiles(closestLocation.param2));
                    profile.SetId(locationCardnality + "." + closestLocation.param1);
                    stations.Insert(profile);
                }
            }
        }
        
        return stations;
    }

    static array<ref ITZ_FS_FuelProfile> GetFuelProfiles(string locationType = "")
    {
        array<ref ITZ_FS_FuelProfile> profiles = new array<ref ITZ_FS_FuelProfile>;
        
        float capacityGasoline = 0.0;
        float minRespawnGasoline = 0.0;
        float maxRespawnGasoline = 0.0;

        float capacityDiesel = 0.0;
        float minRespawnDiesel = 0.0;
        float maxRespawnDiesel = 0.0;

        float capacityKerosene = 0.0;
        float minRespawnKerosene = 0.0;
        float maxRespawnKerosene = 0.0;


        switch (locationType)
        {
            case "Military":
                capacityGasoline = 33.0;
                minRespawnGasoline = 2.0;
                maxRespawnGasoline = 2.7;

                capacityDiesel = 55.0;
                minRespawnDiesel = 2.0;
                maxRespawnDiesel = 3.0;

                capacityKerosene = 0.0;
                minRespawnKerosene = 0.0;
                maxRespawnKerosene = 0.0;
                break;

            case "MB":
                capacityGasoline = 40.0;
                minRespawnGasoline = 2.0;
                maxRespawnGasoline = 2.7;

                capacityDiesel = 70.0;
                minRespawnDiesel = 2.0;
                maxRespawnDiesel = 3.0;

                capacityKerosene = 0.0;
                minRespawnKerosene = 0.0;
                maxRespawnKerosene = 0.0;
                break;

            case "Capital":
                capacityGasoline = 50.0;
                minRespawnGasoline = 2.0;
                maxRespawnGasoline = 2.7;

                capacityDiesel = 30.0;
                minRespawnDiesel = 1.8;
                maxRespawnDiesel = 2.2;

                capacityKerosene = 0.0;
                minRespawnKerosene = 0.0;
                maxRespawnKerosene = 0.0;
                break;

            case "City":
                capacityGasoline = 30.0;
                minRespawnGasoline = 2.0;
                maxRespawnGasoline = 2.7;

                capacityDiesel = 15.0;
                minRespawnDiesel = 1.8;
                maxRespawnDiesel = 2.2;

                capacityKerosene = 0.0;
                minRespawnKerosene = 0.0;
                maxRespawnKerosene = 0.0;
                break;

            case "Village":
                capacityGasoline = 15.0;
                minRespawnGasoline = 2.0;
                maxRespawnGasoline = 2.7;

                capacityDiesel = 25.0;
                minRespawnDiesel = 2.0;
                maxRespawnDiesel = 2.2;

                capacityKerosene = 0.0;
                minRespawnKerosene = 0.0;
                maxRespawnKerosene = 0.0;
                break;

            case "Local":
                capacityGasoline = 10.0;
                minRespawnGasoline = 2.0;
                maxRespawnGasoline = 2.0;

                capacityDiesel = 10.0;
                minRespawnDiesel = 2.0;
                maxRespawnDiesel = 2.0;

                capacityKerosene = 0.0;
                minRespawnKerosene = 0.0;
                maxRespawnKerosene = 0.0;
                break;

            case "Camp":
                capacityGasoline = 15.0;
                minRespawnGasoline = 1.8;
                maxRespawnGasoline = 2.2;

                capacityDiesel = 30.0;
                minRespawnDiesel = 2.0;
                maxRespawnDiesel = 2.0;

                capacityKerosene = 0.0;
                minRespawnKerosene = 0.0;
                maxRespawnKerosene = 0.0;
                break;

            case "AF":
                capacityGasoline = 25.0;
                minRespawnGasoline = 1.8;
                maxRespawnGasoline = 2.2;

                capacityDiesel = 50.0;
                minRespawnDiesel = 2.0;
                maxRespawnDiesel = 2.0;

                capacityKerosene = 150.0;
                minRespawnKerosene = 2.0;
                maxRespawnKerosene = 2.9;
                break;

            default:
                capacityGasoline = 30.0;
                minRespawnGasoline = 1.8;
                maxRespawnGasoline = 2.8;

                capacityDiesel = 30.0;
                minRespawnDiesel = 1.8;
                maxRespawnDiesel = 2.8;

                capacityKerosene = 0.0;
                minRespawnKerosene = 0;
                maxRespawnKerosene = 0;
        }

        ITZ_FS_FuelProfile profile1 = new ITZ_FS_FuelProfile();
        profile1.SetLiquidType(LIQUID_GASOLINE);
        profile1.SetCapacity(capacityGasoline);
        profile1.SetMin(minRespawnGasoline);
        profile1.SetMax(maxRespawnGasoline);
        profiles.Insert(profile1);

        ITZ_FS_FuelProfile profile2 = new ITZ_FS_FuelProfile();
        profile2.SetLiquidType(LIQUID_DIESEL);
        profile2.SetCapacity(capacityDiesel);
        profile2.SetMin(minRespawnDiesel);
        profile2.SetMax(maxRespawnDiesel);
        profiles.Insert(profile2);

        ITZ_FS_FuelProfile profile3 = new ITZ_FS_FuelProfile();
        profile3.SetLiquidType(ITZ_FS_Constants.LIQUID_KEROSENE);
        profile3.SetCapacity(capacityKerosene);
        profile3.SetMin(minRespawnKerosene);
        profile3.SetMax(maxRespawnKerosene);
        profiles.Insert(profile3);

        return profiles;
    }

    static Param3<string, string, vector> GetClosestLocation(array<ref Param3<string, string, vector>> locations, vector pos)
    {
        Param3<string, string, vector> closest;
        float minDistance = float.MAX;

        foreach (Param3<string, string, vector> loc : locations)
        {
            float distance = GetDistance2D(pos, loc.param3);

            if (distance < minDistance)
            {
                minDistance = distance;
                closest = loc;
            }
        }

        return closest;
    }

    static array<ref Param3<string, string, vector>> GetWorldLocations()
    {
        array<ref Param3<string, string, vector>> locations = new array<ref Param3<string, string, vector>>();

        string worldName;
		GetGame().GetWorldName(worldName);
		string config = "CfgWorlds " + worldName + " Names";
        int locationsCount = GetGame().ConfigGetChildrenCount(config);

        for (int i = 0; i < locationsCount; i++) {
			string locationConfig;
			GetGame().ConfigGetChildName(config, i, locationConfig);

            string locationName = GetGame().ConfigGetTextOut(string.Format("%1 %2 name", config, locationConfig));
            if (!locationName || locationName == "")
                continue;

            string locationType = GetGame().ConfigGetTextOut(string.Format("%1 %2 type", config, locationConfig));

            if(locationConfig.Contains("_MB_"))
            {
                locationType = "MB";
            }
            else if(locationName.Contains("Military"))
            {
                locationType = "Military";
            }
            else if(locationConfig.Contains("_AF_"))
            {
                locationType = "AF";
            }

            TFloatArray arPosition = new TFloatArray;
            GetGame().ConfigGetFloatArray(string.Format("%1 %2 position", config, locationConfig), arPosition);
			
            vector position;
            position[0] = arPosition[0];
			position[1] = 0;
			position[2] = arPosition[1];

            Param3<string, string, vector> loc = new Param3<string, string, vector>(locationName, locationType, position);
            locations.Insert(loc);
		}

        ITZ_FS_Logger.Debug("Found " + locations.Count() + " world locations.");
        return locations;
    }

    static string GetLocationCardinalityRelation(vector locationPos, vector targetPos)
    {
        vector dir = targetPos - locationPos;
        float angle = Math.Atan2(dir[0], dir[2]); // x e z
        angle = angle * Math.RAD2DEG;

        if (angle < 0)
            angle += 360;

        if (angle >= 337.5 || angle < 22.5)
            return "Norte";
        else if (angle >= 22.5 && angle < 67.5)
            return "Nordeste";
        else if (angle >= 67.5 && angle < 112.5)
            return "Leste";
        else if (angle >= 112.5 && angle < 157.5)
            return "Sudeste";
        else if (angle >= 157.5 && angle < 202.5)
            return "Sul";
        else if (angle >= 202.5 && angle < 247.5)
            return "Sudoeste";
        else if (angle >= 247.5 && angle < 292.5)
            return "Oeste";
        else if (angle >= 292.5 && angle < 337.5)
            return "Noroeste";

        return "UNKNOWN";
    }

    static float GetDistance2D(vector posA, vector posB)
    {
        float dx = posB[0] - posA[0]; // eixo X
        float dy = posB[2] - posA[2]; // eixo Y no plano XZ (porque Y é altura no DayZ)
        return Math.Sqrt(dx * dx + dy * dy);
    }

    static array<ref ITZ_FS_VehicleProfile> GetDefaultVehicleProfiles()
    {
        array<ref ITZ_FS_VehicleProfile> profiles = new array<ref ITZ_FS_VehicleProfile>;
		
		ITZ_FS_VehicleProfile profile;
        profile = new ITZ_FS_VehicleProfile();
        profile.m_Classname = "CarScript";
        profile.m_FuelType = ITZ_FS_Constants.NAMED_FUEL_GASOLINE;
        profile.m_FuelConsumptionPerKm = 1.15;
        profiles.Insert(profile);

        profile = new ITZ_FS_VehicleProfile();
        profile.m_Classname = "BoatScript";
        profile.m_FuelType = ITZ_FS_Constants.NAMED_FUEL_GASOLINE;
        profile.m_FuelConsumptionPerKm = 1.15;
        profiles.Insert(profile);
		
        profile = new ITZ_FS_VehicleProfile();
        profile.m_Classname = "CivilianSedan";
        profile.m_FuelType = ITZ_FS_Constants.NAMED_FUEL_GASOLINE;
        profile.m_FuelConsumptionPerKm = 1.22;
        profiles.Insert(profile);

        profile = new ITZ_FS_VehicleProfile();
        profile.m_Classname = "Hatchback_02";
        profile.m_FuelType = ITZ_FS_Constants.NAMED_FUEL_GASOLINE;
        profile.m_FuelConsumptionPerKm = 1.11;
        profiles.Insert(profile);

        profile = new ITZ_FS_VehicleProfile();
        profile.m_Classname = "Sedan_02";
        profile.m_FuelType = ITZ_FS_Constants.NAMED_FUEL_GASOLINE;
        profile.m_FuelConsumptionPerKm = 1.13;
        profiles.Insert(profile);

        profile = new ITZ_FS_VehicleProfile();
        profile.m_Classname = "OffroadHatchback";
        profile.m_FuelType = ITZ_FS_Constants.NAMED_FUEL_DIESEL;
        profile.m_FuelConsumptionPerKm = 1.21;
        profiles.Insert(profile);

        profile = new ITZ_FS_VehicleProfile();
        profile.m_Classname = "Offroad_02";
        profile.m_FuelType = ITZ_FS_Constants.NAMED_FUEL_DIESEL;
        profile.m_FuelConsumptionPerKm = 1.24;
        profiles.Insert(profile);

        profile = new ITZ_FS_VehicleProfile();
        profile.m_Classname = "Truck_01_Base";
        profile.m_FuelType = ITZ_FS_Constants.NAMED_FUEL_DIESEL;
        profile.m_FuelConsumptionPerKm = 1.4;
        profiles.Insert(profile);

        #ifdef RFWC
        profile = new ITZ_FS_VehicleProfile();
        profile.m_Classname = "RFWC_base";
        profile.m_FuelType = ITZ_FS_Constants.NAMED_FUEL_GASOLINE;
        profile.m_FuelConsumptionPerKm = 1.4;
        profiles.Insert(profile);
        #endif

        #ifdef RFFS_HELI
        profile = new ITZ_FS_VehicleProfile();
        profile.m_Classname = "RFFSHeli_base";
        profile.m_FuelType = ITZ_FS_Constants.NAMED_FUEL_KEROSENE;
        profile.m_FuelConsumptionPerKm = 2.5;
        profiles.Insert(profile);
        #endif

        #ifdef EXPANSIONMODVEHICLE
        profile = new ITZ_FS_VehicleProfile();
        profile.m_Classname = "ExpansionHelicopterScript";
        profile.m_FuelType = ITZ_FS_Constants.NAMED_FUEL_KEROSENE;
        profile.m_FuelConsumptionPerKm = 2.3;
        profiles.Insert(profile);
        #endif

        return profiles;
    }
}