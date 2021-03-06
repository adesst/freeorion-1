// -*- C++ -*-
#ifndef AI_INTERFACE
#define AI_INTERFACE

#include <string>
#include <vector>

struct CombatData;
class Empire;
class Tech;
class DiplomaticMessage;
class Universe;
struct DiplomaticStatusUpdateInfo;
struct GalaxySetupData;

/* AI logic modules implement this class, and AIClientApps contain one, and call it to generate orders */
class AIBase {
public:
    virtual ~AIBase();

    virtual void                GenerateOrders();                                           ///< Called when the server has sent a new turn update.  AI should review the new gamestate and send orders for this turn.
    virtual void                HandleChatMessage(int sender_id, const std::string& msg);   ///< Called when another player sends a chat message to this player.  AI can respond or ignore.
    virtual void                HandleDiplomaticMessage(const DiplomaticMessage& msg);      ///< Called when another player sends a diplomatic message that affects this player. AI can respond or ignore.
    virtual void                HandleDiplomaticStatusUpdate(const DiplomaticStatusUpdateInfo& u);  ///< Called when empires' diplomatic status changes
    virtual void                StartNewGame();                                             ///< Called when a new game (not loaded) is started.  AI should clear its state and prepare to start a new game
    virtual void                ResumeLoadedGame(const std::string& save_state_string);     ///< Called when a game is loaded from save.  AI should extract any state information stored in \a save_state_string so as to be able to continue generating orders when asked to do so
    virtual const std::string&  GetSaveStateString();                                       ///< Called when the server is saving the game.  AI should store any state information it will need to resume at a later time, and return this information in the save_state_string
    void                        SetAggression(int aggr);
protected:
    int m_aggression;
};

/* Public interface providing relatively easy-to use and somewhat conveniently grouped-together functions that
   a class that implements AIBase can call to get information from the AIClientApp about the gamestate, and which
   can be used to interat with the gamestate by issueing orders, ending the AI player's turn, or sending message
   to other players. */
namespace AIInterface {
    /** Gamestate Accessors */ //@{
    const std::string&  PlayerName();                   ///< returns the player name of this client
    const std::string&  PlayerName(int player_id);      ///< returns the name of player with \a player_id

    int                 PlayerID();                     ///< returns the player ID of this client
    int                 EmpirePlayerID(int empire_id);  ///< returns ID of player controlling empire with id \a empire_id
    std::vector<int>    AllPlayerIDs();                 ///< returns vector containing IDs of all players in game

    bool                PlayerIsAI(int player_id);      ///< returns true iff the player with id \a player_id is an AI
    bool                PlayerIsHost(int player_id);    ///< returns true iff the player with id \a player_id is the game host

    int                 EmpireID();                     ///< returns the empire ID of this client
    int                 PlayerEmpireID(int player_id);  ///< returns ID of empire controlled by player with id \a player_id
    std::vector<int>    AllEmpireIDs();                 ///< returns vector containing IDs of all empires in game

    const Empire*       GetEmpire();                    ///< returns empire of this client's player
    const Empire*       GetEmpire(int empire_id);       ///< returns empire with id \a empire_id

    const Universe&     GetUniverse();                  ///< returns Universe known to this player

    const Tech*         GetTech(const std::string& tech_name);  ///< returns Tech with name \a name

    int                 CurrentTurn();                  ///< returns the current game turn

    std::string         GetAIConfigStr();               ///< returns the OptionsDB ai-config value
    std::string         GetAIDir();                     ///< returns the full AI directory path

    const GalaxySetupData&  GetGalaxySetupData();       ///< returns the setup data used to generate the game universe
    //@}

    /** Gamestate Prediction Utilites */ //@{
    void                InitTurn();                     ///< initializes and calculates meters, resource pools and queues so info based on latest turn update
    void                UpdateMeterEstimates(bool pretend_unowned_planets_owned_by_this_ai_empire = false);  ///< sets object meters to what they are expected to be during the next turn processing phase, after orders are submitted.  if \a pretend_unowned_planets_owned_by_this_ai_empire is true, unowned planets known of by this player will have this player added as an owner before meter values are calculated, so that their max meter values will be what they would be if those planets were colonized by this empire
    void                UpdateResourcePools(); ///< determines how much of each resource is available at each object owned by this empire, and updates resource pool amounts and spending on queues accordingly
    void                UpdateResearchQueue();
    void                UpdateProductionQueue();
    //@}

    /** Order-Giving */ //@{
    int                 IssueRenameOrder(int object_id, const std::string& new_name);
    int                 IssueScrapOrder(const std::vector<int>& object_ids);
    int                 IssueScrapOrder(int object_id);
    int                 IssueFleetMoveOrder(int fleet_id, int destination_id);
    int                 IssueNewFleetOrder(const std::string& fleet_name, const std::vector<int>& ship_ids);
    int                 IssueNewFleetOrder(const std::string& fleet_name, int ship_id);
    int                 IssueFleetTransferOrder(int ship_id, int new_fleet_id);
    int                 IssueColonizeOrder(int ship_id, int planet_id);
    int                 IssueInvadeOrder(int ship_id, int planet_id);
    int                 IssueBombardOrder(int ship_id, int planet_id);
    int                 IssueDeleteFleetOrder();
    int                 IssueAggressionOrder(int object_id, bool aggressive);
    int                 IssueGiveObjectToEmpireOrder(int object_id, int recipient_id);

    int                 IssueChangeFocusOrder(int planet_id, const std::string& focus);

    int                 IssueEnqueueTechOrder(const std::string& tech_name, int position);
    int                 IssueDequeueTechOrder(const std::string& tech_name);

    int                 IssueEnqueueBuildingProductionOrder(const std::string& item_name, int location_id);
    int                 IssueEnqueueShipProductionOrder(int design_id, int location_id);
    int                 IssueChangeProductionQuantityOrder(int queue_index, int new_quantity, int new_blocksize);
    int                 IssueRequeueProductionOrder(int old_queue_index, int new_queue_index);
    int                 IssueDequeueProductionOrder(int queue_index);

    int                 IssueCreateShipDesignOrder(const std::string& name, const std::string& description,
                                                   const std::string& hull,
                                                   const std::vector<std::string> parts,
                                                   const std::string& graphic, const std::string& model, bool nameDescInStringTable);

    void                SendPlayerChatMessage(int recipient_player_id, const std::string& message_text);
    void                SendDiplomaticMessage(const DiplomaticMessage& diplo_message);

    void                DoneTurn();        ///< AI player is done submitting orders for this turn
    //@}

    /** Logging */ //@{
    void                LogOutput(const std::string& log_text);     ///< output text to as DEBUG
    void                ErrorOutput(const std::string& log_text);   ///< output text to as ERROR
    //@}
};

#endif
