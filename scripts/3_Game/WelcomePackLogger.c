class WelcomePackLogger
{
    static string m_LogFile;
    static bool m_IsInitialized = false;
    
    static void Init()
    {
        if (m_IsInitialized) return;
        
        string logDir = "$profile:FischerMods";
        m_LogFile = logDir + "\\WelcomePack\\WelcomePack.log";
        
        // Criar diretório se não existir
        if (!FileExist(logDir))
            MakeDirectory(logDir);

        string welcomePackDir = logDir + "\\WelcomePack";
        if (!FileExist(welcomePackDir))
            MakeDirectory(welcomePackDir);

        m_IsInitialized = true;
    }
    
    static void LogInfo(string message)
    {
        WriteLog("INFO", message);
    }
    
    static void LogWarning(string message)
    {
        WriteLog("WARNING", message);
    }
    
    static void LogError(string message)
    {
        WriteLog("ERROR", message);
    }
    
    static void LogDebug(string message)
    {
        WriteLog("DEBUG", message);
    }
    
    static void LogPlayer(string playerName, string playerID, string action, string details = "")
    {
        string playerInfo = "Player: " + playerName + " (" + playerID + ") - " + action;
        if (details != "")
            playerInfo = playerInfo + " - " + details;
        
        LogInfo(playerInfo);
    }
    
    private static void WriteLog(string level, string message)
    {
        if (!m_IsInitialized) Init();
        
        string timestamp = GetCurrentTimestamp();
        string logEntry = "[" + timestamp + "] [" + level + "] " + message;
        
        // Escrever no arquivo de log
        FileHandle file = OpenFile(m_LogFile, FileMode.APPEND);
        if (file)
        {
            FPrintln(file, logEntry);
            CloseFile(file);
        }
    }
    
    private static string GetCurrentTimestamp()
    {
        int year, month, day, hour, minute, second;
        GetYearMonthDay(year, month, day);
        GetHourMinuteSecond(hour, minute, second);
        
        string timestamp = string.Format("%1-%2-%3 %4:%5:%6", year, month.ToStringLen(2), day.ToStringLen(2), hour.ToStringLen(2), minute.ToStringLen(2), second.ToStringLen(2));
        
        return timestamp;
    }
}