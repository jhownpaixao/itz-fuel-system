class ITZ_FS_Logger
{
    static void Trace(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		if(ITZ_FS_Module.GetModule() && ITZ_FS_Module.GetModule().GetLogLevel() < ITZ_FS_LogLevel.TRACE) return;
		PrintFormat("[ITZ][FuelSystem][TRACE] %1", string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9));
	}

	static void Debug(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		if(ITZ_FS_Module.GetModule() && ITZ_FS_Module.GetModule().GetLogLevel() < ITZ_FS_LogLevel.DEBUG) return;
		PrintFormat("[ITZ][FuelSystem][DEBUG] %1", string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9));
	}

	static void Info(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		if(ITZ_FS_Module.GetModule() && ITZ_FS_Module.GetModule().GetLogLevel() < ITZ_FS_LogLevel.INFO) return;
		PrintFormat("[ITZ][FuelSystem][INFO] %1", string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9));
	}

	static void Warn(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		if(ITZ_FS_Module.GetModule() && ITZ_FS_Module.GetModule().GetLogLevel() < ITZ_FS_LogLevel.WARN) return;
		PrintFormat("[ITZ][FuelSystem][WARNING] %1", string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9));
	}

	static void Error(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		if(ITZ_FS_Module.GetModule() && ITZ_FS_Module.GetModule().GetLogLevel() < ITZ_FS_LogLevel.ERROR) return;
		PrintFormat("[ITZ][FuelSystem][ERROR] %1", string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9));
	}

	static void Critical(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		if(ITZ_FS_Module.GetModule() && ITZ_FS_Module.GetModule().GetLogLevel() < ITZ_FS_LogLevel.CRITICAL) return;
		PrintFormat("[ITZ][FuelSystem][CRITICAL] %1", string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9));
	}
}