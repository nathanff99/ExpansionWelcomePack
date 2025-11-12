modded class MissionServer
{
    string m_ServerHostname;

    override void OnInit()
    {
        super.OnInit();

        m_ServerHostname = ServerConfigManager.GetHostname();

        // if (m_ServerHostname.IndexOf("Fronteira") == -1)
        // {
        //     WelcomePackLogger.LogError("Servidor não autenticado. Desligando o servidor...");
        //     GetGame().RequestExit(0);
        //     return;
        // } else {
        //     WelcomePackLogger.LogInfo("Servidor autenticado.");
        // }

        WelcomePackManager.Init();
    }

    override void OnEvent(EventType eventTypeId, Param params)
    {
        super.OnEvent(eventTypeId, params);

        if (eventTypeId != ChatMessageEventTypeID) return;

        ChatMessageEventParams chatParams = ChatMessageEventParams.Cast(params);
        string senderName = chatParams.param2;
        string message = chatParams.param3;
        string senderPlainId = chatParams.param4;

        if (!message || message == "") return;
        if (message != "/welcomepack" && message.IndexOf("adm") == -1) return;

        // Encontra player
        PlayerBase player = null;
        array<Man> players = new array<Man>;
        GetGame().GetPlayers(players);

        for (int i = 0; i < players.Count(); i++)
        {
            PlayerBase pb = PlayerBase.Cast(players.Get(i));
            if (!pb) continue;
            PlayerIdentity pid = pb.GetIdentity();
            if (!pid) continue;

            if (senderPlainId && senderPlainId != "" && pid.GetPlainId() == senderPlainId)
            {
                player = pb;
                break;
            }

            if (!player && senderName && senderName != "" && pid.GetName() == senderName)
            {
                player = pb;
            }
        }

        if (!player)
        {
            WelcomePackLogger.LogWarning("Player not found -> id:'" + senderPlainId + "' name:'" + senderName + "'");
            return;
        }

        // Avisar o jogador para abrir ticket
        if (message.IndexOf("adm") != -1) {
            ExpansionNotification("Aviso automático", "Abra um ticket no Discord para falar com um administrador.").Error(player.GetIdentity());
        }

        // Executa ação de entregar kit inicial
        if (message == "/welcomepack") {

            string playerID = "unknown";
            PlayerIdentity identity = player.GetIdentity();
            if (identity)
            {
                playerID = identity.GetPlainId();
            }

            if (!WelcomePackManager.HasPlayerReceived(playerID))
            {
                WelcomePackLogger.LogPlayer(senderName, playerID, "Requesting welcome pack");
                GiveWelcomePack(player);
                RegisterWelcomePack(player, playerID);
            }
            else
            {
                WelcomePackLogger.LogPlayer(senderName, playerID, "Attempted to get welcome pack", "Already received");
                ExpansionNotification("Welcome Pack", "Você já recebeu seu kit inicial.").Error(player.GetIdentity());
            }
        }
    }

    void GiveWelcomePack(PlayerBase player)
    {
        WelcomePackConfig config = WelcomePackManager.GetConfig();
        if (!player || !config || !config.Items || !config.ChestClassname) return;

        vector pos = player.GetPosition();
        // spawna ligeiramente abaixo para evitar clipping
        vector chestPos = pos;
        chestPos[1] = chestPos[1] + 0.1;

        ItemBase chest = ItemBase.Cast(GetGame().CreateObject(config.ChestClassname, chestPos, false, false, true));
        if (!chest)
        {
            WelcomePackLogger.LogError("Failed to create welcome pack chest for player " + player.GetIdentity().GetPlainId() + " (" + player.GetIdentity().GetName() + ")");
            ExpansionNotification("Welcome Pack", "Erro ao gerar o kit inicial. Contate o admin via ticket.").Error(player.GetIdentity());
            return;
        }

        int totalItems = 0;
        foreach (WelcomePackItem item : config.Items)
        {
            if (!item || !item.Classname) continue;
            for (int i = 0; i < item.Quantity; i++)
            {
                ItemBase newItem = ItemBase.Cast(chest.GetInventory().CreateInInventory(item.Classname));
                if (newItem)
                    totalItems++;
            }
        }

        ExpansionNotification("Welcome Pack", "Seu kit inicial foi entregue em um baú aos seus pés!").Success(player.GetIdentity());
        WelcomePackLogger.LogPlayer(player.GetIdentity().GetName(), player.GetIdentity().GetPlainId(), 
            "Welcome pack delivered", "Items: " + totalItems.ToString() + ", Chest: " + config.ChestClassname);
        SendToDiscord(player);
    }

    void RegisterWelcomePack(PlayerBase player, string playerID)
    {
        string currentTime = GetCurrentTimestamp();
        WelcomePackManager.RegisterPlayer(playerID, currentTime);
    }

    void SendToDiscord(PlayerBase player)
    {
        PlayerIdentity identity = player.GetIdentity();
        if (!identity) 
        {
            WelcomePackLogger.LogError("Player identity not available for Discord notification");
            return;
        }

        WelcomePackConfig config = WelcomePackManager.GetConfig();
        if (!config) 
        {
            WelcomePackLogger.LogError("Config not available for Discord notification");
            return;
        }

        string playerName = identity.GetName();
        string playerId = identity.GetPlainId();
        
        string description = "Jogador " + playerName + " (SteamID: " + playerId + ") recebeu o kit inicial.";

        FischerDiscordMessage msg = new FischerDiscordMessage("FischerMods", config.DiscordAvatarURL);
        msg.SetAuthor("Welcome Pack", config.DiscordAvatarURL);
        msg.SetMessage(description);
        msg.SetFooter(m_ServerHostname);
        msg.SetColor(77, 144, 29); // #4d901d
        msg.AddWebhooks({config.DiscordWebhookURL});
        GetFischerDiscordAPI().SendMessage(msg);
        
        WelcomePackLogger.LogInfo("Discord notification sent for player: " + playerName + " (" + playerId + ")");
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