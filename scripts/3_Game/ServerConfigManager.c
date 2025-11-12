class ServerConfigManager
{
    static string GetConfigValue(string configKey, string defaultValue = "")
    {
        FileHandle file = OpenFile("$CurrentDir:serverDZ.cfg", FileMode.READ);

        if (!file)
        {
            Print("Unable to locate serverDZ.cfg");
            return defaultValue;
        }

        string line;
        while (FGets(file, line) > 0)
        {
            line.TrimInPlace();
            int equalIndex = line.IndexOf("=");
            int semiColonIndex = line.IndexOf(";");

            if (equalIndex != -1 && semiColonIndex != -1)
            {
                string key = line.Substring(0, equalIndex).Trim();

                if (key == configKey)
                {
                    string value = line.Substring(equalIndex + 1, semiColonIndex - equalIndex - 1).Trim();
                    value.Replace("\"", string.Empty);
                    CloseFile(file);
                    return value;
                }
            }
        }

        CloseFile(file);
        return defaultValue;
    }

    static string GetHostname()
    {
        return GetConfigValue("hostname", "DayZ Server");
    }
}