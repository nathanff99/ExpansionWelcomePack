class CfgPatches
{
    class WelcomePack
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data"};
    };
};

class CfgMods
{
    class WelcomePack
    {
        dir = "WelcomePack";
        name = "Welcome Pack";
        type = "mod";

        dependencies[] = {"Mission"};

        class defs
        {
            class gameScriptModule
            {
                files[] = {"WelcomePack/scripts/3_Game"};
            };
            class worldScriptModule
            {
                files[] = {"WelcomePack/scripts/4_World"};
            };
            class missionScriptModule
            {
                files[] = {"WelcomePack/scripts/5_Mission"};
            };
        };
    };
};
