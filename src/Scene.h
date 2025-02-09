#pragma once

#include "Module.h"
#include "Player.h"
#include "Power.h"
#include "Enemy.h"
#include "Item.h"
#include "Bonfire.h"
#include "Poison.h"
#include "GuiControlButton.h"
#include "UI.h"
#include "Npc.h"

struct SDL_Texture;

enum class LOAD {
	INITIAL, RESPAWN, DEBUG, LOAD
};

enum class ENEMY {
	CREATEALL, CLEARDEADS, CREATEXML
};
enum class ITEM {
	CREATEALL, CLEARDEADS, CREATEXML
};

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	void LoadAssets();

	void LoadFx();

	void SetupUI();

	// Called before all Updates
	bool PreUpdate();

	void HandleCamera(Engine& engine);

	void HandlePowers();

	void HandleGui();

	void DebugMode();

	void ActiveBonfires();

	// Called each loop iteration
	bool Update(float dt);

	void SaveKillEnemy(int id);

	void SaveGrabbedItem(int id);

	void RestartEntities();

	void RestartSave();

	void RemoveLevelEnemies(int level);

	int GetLowestId(int type);

	void LoadState(LOAD load);

	void SaveState();

	void ChangeMusic();

	void CreateEvents();

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void RespawnPoison();
	
	void LoadNextLevel();

	int GetActualLevel() {
		return level;
	}

	bool IsPause() {
		return pause;
	}

	Vector2D GetPlayerPosition() {
		return player->GetPosition();
	}

	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

private:

	// --- SCENE PARAMETERS ---
	int level;                     // Current level
	bool firstTimeLoad;            // Flag for the first-time loading of the scene
	bool exitGame;                 // Flag to exit the game
	int coolIntro = 200;		   // Time for finish Credts screen

	// --- TILE AND DEBUG ---
	SDL_Texture* mouseTileTex = nullptr;  // Texture for the mouse tile
	std::string tilePosDebug = "[0,0]";   // Debug position of the tile
	bool once = false;                    // Flag for one-time operations

	// --- PLAYER AND ENTITIES ---
	Player* player;                       // Player instance
	std::map<Enemy*, int> enemyList;      // List of enemies
	std::map<Item*, int> itemList;        // List of items
	std::vector<Power*> fireballList;     // List of fireballs
	std::map<Bonfire*, int> bonfireList;  // List of bonfires
	std::vector<Poison*> poisonList;      // List of poison traps
	std::vector<Npc*> npcList;			  // List of npcs
	std::vector<int> levelsLoadedBonfire; // Levels with bonfires loaded
	std::vector<int> levelsLoadedEnemies; // Levels with enemies loaded
	std::vector<int> levelsLoadedItems;   // Levels with items loaded
	std::vector<int> levelsLoadedNpcs;    // Levels with npcs loaded
	int idEnemy = 1;                      // Identifier for enemies
	bool bossActive = false;              // Flag to indicate if the boss is active

	// --- BONFIRE ---
	int idBonfire;        // Bonfire ID
	int idNameBonfire;    // Bonfire name ID

	// --- AUDIO FX ---
	int bonfireSFX;       // Sound effect for bonfire activation
	int saveSFX;          // Sound effect for saving progress
	int loadSFX;		  // Sound effect for loading progress
	int stone_doorSFX;    // Sound effect for level cahnge
	int button_clickSFX;  // Sound effect for button clicked


	// --- USER INTERFACE (UI) ---
	SDL_Texture* creditsScreen;
	SDL_Texture* helpMenu;			 // Menu texture
	SDL_Texture* OptionsBook;	     // Menu texture
	SDL_Texture* TitleScreen;        // Title screen texture
	SDL_Texture* gui;				 // General GUI texture
	SDL_Texture* lifePlayer;         // Player life texture
	SDL_Texture* powerOn;
	SDL_Texture* pouch;
	SDL_Texture* pouchfull;
	SDL_Texture* powerOff;
	SDL_Texture* sliderBackground;   // Slider background texture
	SDL_Texture* sliderMovement;     // Slider movement texture
	SDL_Texture* menuButtonNormal;   // Slider menu
	SDL_Texture* menuButtonFocused;
	SDL_Texture* menuButtonPressed;
	SDL_Texture* menuButtonDisabled;
	SDL_Texture* gameOver;
	SDL_Texture* gameWin;
	SDL_Texture* checkOff;
	SDL_Texture* checkOn;
	SDL_Texture* creditsScreenMenu;
	SDL_Texture* escapeExit;

	bool pressedSoundPlayed = false;// checks if button is pressed and sound played
	bool showSettings = false;       // Flag to show settings menu
	bool showCreditsMenu = false;
	bool showTp = false;
	bool showExitHelp = false;          

	// --- FADE-IN EFFECT ---
	Uint8 alpha;         // Alpha transparency for fade-in effect
	bool fadeIn;         // Flag for fade-in state
	bool isTransitioning = false;

	// --- HELP AND TELEPORTATION ---
	bool help;				// Flag for showing help
	bool enableTp;			// Teleportation optimization flag
	bool pause = false;		// Flag for is pause the game or not
	int playerRespawnCool;  // Respawn position
	int winRestartCool;      
	int coordYMenuTp;		// Y-coordinate for menu teleportation

	// --- MUSIC ---
	const char* mainMenuMusic;
	const char* levelMusic;
	const char* bossMusic;
	const char* endingMusic;


	bool load = false;
	int timer = 0;
	int start = 0;
	Timer countTime;

};