class WelcomePackItem
{
    string Classname;
    int Quantity;
}

class WelcomePackConfig
{
    string DiscordWebhookURL;
    string DiscordAvatarURL;
    string ChestClassname;
    ref array<ref WelcomePackItem> Items;

    void WelcomePackConfig()
    {
        DiscordWebhookURL = "Your webhook url here";
        DiscordAvatarURL = "";
        ChestClassname = "SeaChest";
        Items = new array<ref WelcomePackItem>;
    }
}

class WelcomePackEntry
{
    string PlayerID;
    string receivedDate;
    void WelcomePackEntry(string id = "", string time = "") { PlayerID = id; receivedDate = time; }
}

class WelcomePackData
{
    ref map<string, ref WelcomePackEntry> Entries;
    void WelcomePackData() { Entries = new map<string, ref WelcomePackEntry>; }
}

class WelcomePackManager
{
    static ref WelcomePackData m_Data;
    static ref WelcomePackConfig m_Config;
    
    static string m_SaveDir;
    static string m_SaveFile;
    static string m_ConfigFile;
    
    static void Init()
    {
        // Inicializar logger primeiro
        WelcomePackLogger.Init();
        
        m_SaveDir = "$profile:FischerMods";
        m_SaveFile = m_SaveDir + "\\WelcomePack\\Data.json";
        m_ConfigFile = m_SaveDir + "\\WelcomePack\\Config.json";
        
        CreateDirectories();
        LoadData();
        LoadConfig();
    }
    
    static void CreateDirectories()
    {
        if (!FileExist(m_SaveDir)) 
            MakeDirectory(m_SaveDir);
        
        string welcomePackDir = m_SaveDir + "\\WelcomePack";

        if (!FileExist(welcomePackDir)) 
            MakeDirectory(welcomePackDir);
    }
    
    static void LoadData()
    {
        m_Data = new WelcomePackData();
        if (FileExist(m_SaveFile))
            JsonFileLoader<WelcomePackData>.JsonLoadFile(m_SaveFile, m_Data);
    }
    
    static void SaveData()
    {
        if (m_Data)
            JsonFileLoader<WelcomePackData>.JsonSaveFile(m_SaveFile, m_Data);
    }
    
    static void LoadConfig()
    {
        m_Config = new WelcomePackConfig();
        if (FileExist(m_ConfigFile))
        {
            JsonFileLoader<WelcomePackConfig>.JsonLoadFile(m_ConfigFile, m_Config);
        }
        else
        {
            // Criar configuração padrão
            m_Config.DiscordWebhookURL = "Your webhook url here";
            m_Config.DiscordAvatarURL = "";
            m_Config.ChestClassname = "SeaChest";

            WelcomePackItem defaultItem = new WelcomePackItem();
            defaultItem.Classname = "NailBox";
            defaultItem.Quantity = 3;
            m_Config.Items.Insert(defaultItem);

            JsonFileLoader<WelcomePackConfig>.JsonSaveFile(m_ConfigFile, m_Config);
            WelcomePackLogger.LogInfo("Default configuration created at " + m_ConfigFile);
        }
    }
    
    static WelcomePackData GetData()
    {
        return m_Data;
    }
    
    static WelcomePackConfig GetConfig()
    {
        return m_Config;
    }
    
    static void RegisterPlayer(string playerID, string currentTime)
    {
        if (!m_Data) m_Data = new WelcomePackData();
        if (!m_Data.Entries) m_Data.Entries = new map<string, ref WelcomePackEntry>;
        
        m_Data.Entries.Insert(playerID, new WelcomePackEntry(playerID, currentTime));
        SaveData();
        
        WelcomePackLogger.LogInfo("Player registered: " + playerID + " at " + currentTime);
    }
    
    static bool HasPlayerReceived(string playerID)
    {
        if (!m_Data || !m_Data.Entries) return false;
        return m_Data.Entries.Contains(playerID);
    }
}