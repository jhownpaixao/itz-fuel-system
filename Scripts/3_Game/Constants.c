
class ITZ_FS_Constants
{
    static const int FS_VERSION = 102;

    static const int LIQUID_KEROSENE = 536870912;

    static const string ITZ_PROFILE_FOLDER_NAME = "$profile:iTzMods";
    static const string GENERAL_PATH = ITZ_PROFILE_FOLDER_NAME + "\\FuelSystem";

    static const string OLD_STATION_FILE = GENERAL_PATH + "\\Stations.json";
    static const string STATION_FILE = GENERAL_PATH + "\\stations.xml";

    static const string OLD_VEHICLE_PROFILE_FILE = GENERAL_PATH + "\\Vehicles.json";
    static const string VEHICLE_PROFILE_FILE = GENERAL_PATH + "\\vehicles.xml";
    static const string PERSISTENT_FILE = GENERAL_PATH + "\\persistent.json";

    static const string SETTING_FILE = GENERAL_PATH + "\\Settings.json";


    static const string NAMED_FUEL_NONE = "";
    static const string NAMED_FUEL_GASOLINE = "GASOLINE";
    static const string NAMED_FUEL_DIESEL = "DIESEL";
    static const string NAMED_FUEL_KEROSENE = "KEROSENE";

    static const float IDLE_CONSUMPTION = 0.0003; // litros por segundo (0.018 L/min)
	static const float RPM_MDFR = 1.3; // mdfr de multiplicação para o consumo baseado no RPM (consumo total * RPM_MDFR)
	static const float DISTANCE_MDFR = 1.3; // mdfr para corrigir o consumo por km, necessário para tornar o calculo de consumo exato (foi projetado medindo ingame a distancia percorrida e o consumo de gasolina, e depois dividindo o consumo pela distancia percorrida em km)


    static string GetFuelNameFromType(int type)
    {
		string name;
        switch (type)
        {
            case ITZ_FS_Constants.LIQUID_KEROSENE:
                name = Widget.TranslateString("#STR_FS_AVIATION_FUEL");
				break;
            case LIQUID_GASOLINE:
			    name = Widget.TranslateString("#inv_inspect_gasoline");
		    	break;
			case LIQUID_DIESEL:
			    name = Widget.TranslateString("#inv_inspect_diesel");
            	break;
			default:
			    name = "ERROR";
				break;
        }
		
		return name;
    }

    static int GetFuelTypeFromNamedType(string name)
    {
        int type;

        switch (name)
        {
            case ITZ_FS_Constants.NAMED_FUEL_GASOLINE:
                type = LIQUID_GASOLINE;
                break;
            case ITZ_FS_Constants.NAMED_FUEL_DIESEL:
                type = LIQUID_DIESEL;
                break;
            case ITZ_FS_Constants.NAMED_FUEL_KEROSENE:
                type = ITZ_FS_Constants.LIQUID_KEROSENE;
                break;
            default:
                type = LIQUID_GASOLINE;
        }
        return type;
    }

    static string GetNamedTypeFromFuelType(int type)
    {
        string name;

        switch (type)
        {
            case LIQUID_GASOLINE:
                name = ITZ_FS_Constants.NAMED_FUEL_GASOLINE;
                break;
            case LIQUID_DIESEL:
                name = ITZ_FS_Constants.NAMED_FUEL_DIESEL;
                break;
            case ITZ_FS_Constants.LIQUID_KEROSENE:
                name = ITZ_FS_Constants.NAMED_FUEL_KEROSENE;
                break;
            default:
                name = ITZ_FS_Constants.NAMED_FUEL_GASOLINE;
        }
        return name;
    }
}

enum ITZ_FS_LogLevel
{
	NONE = 0,
	CRITICAL,
	ERROR,
	WARN,
	INFO,
	DEBUG,
	TRACE
};

enum ITZ_FS_ModuleRPC
{
    INVALID = 36983332,
    SYNC,
    SYNC_DIRTY,
    COUNT
}