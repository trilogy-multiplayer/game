#include "feature_discord-rpc.hpp"

#include <networking/modules/module_player-sync.hpp>

#define APPLICATION_ID "1147699463368998933"

void networking::features::c_feature_discord_rpc::initialize()
{
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));

    Discord_Initialize(APPLICATION_ID, NULL, 1, NULL);
}

void networking::features::c_feature_discord_rpc::shutdown()
{

}

void networking::features::c_feature_discord_rpc::on_discord_rpc_thread()
{

}

void networking::features::c_feature_discord_rpc::update_rich_presence()
{
    static auto networking = c_networking::instance();
    static auto module_player_sync = networking::modules::c_module_player_sync::instance();

    if (module_player_sync->m_local_player == nullptr) return;
    if (module_player_sync->m_players.at(0) == nullptr) return;
    if (networking->m_address.host == NULL) return;
    
    DiscordRichPresence rich_presence;
    memset(&rich_presence, 0, sizeof(rich_presence));
    rich_presence.state = "Development server";

    rich_presence.details = 
        c_log::Join("Playing as: ", module_player_sync->m_local_player->m_name)
        .c_str();

    rich_presence.startTimestamp = std::time(0);

    rich_presence.largeImageKey = "trilogy";
    rich_presence.smallImageKey = "trilogy-round";

    rich_presence.partyId = networking->m_address.host;
    rich_presence.partyMax = 1000;

    rich_presence.partySize = 
        std::count_if(
            module_player_sync->m_players.begin(), module_player_sync->m_players.end(), [](auto player) { 
                return player != nullptr; 
            });

    Discord_UpdatePresence(&rich_presence);
}
