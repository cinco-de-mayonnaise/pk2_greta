//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//-------------------------
//It can be started with the "dev" argument to start the
//cheats and "test" follown by the episode and level to
//open directely on the level.
//	Exemple:
//	"./pekka-kana-2 dev test rooster\ island\ 2/level13.map"
//	Starts the level13.map on dev mode
//#########################
#include "pk2_main.hpp"

#include "engine/Piste.hpp"
#include "version.hpp"

#include "screens/screens_handler.hpp"
#include "gfx/text.hpp"
#include "game/game.hpp"
#include "episode/episodeclass.hpp"
#include "episode/mapstore.hpp"
#include "save.hpp"
#include "gui.hpp"
#include "system.hpp"
#include "language.hpp"
#include "settings.hpp"
#include <filesystem>

#include "game/prototypes_handler.hpp"

#include <cstring>
#include <algorithm>

#include <SDL.h>

static const char default_config[] = 
"-- Audio Buffer Size"
"\r\n-- low value = low audio latency; high value = less cpu usage"
"\r\n-- Default is 1024"
"\r\n-- Prefer a power of 2: 512 1024 2048 4096 default"
"\r\n---------------"
"\r\n*audio_buffer_size:    default"
"\r\n"
"\r\n"
"\r\n-- Multi thread audio"
"\r\n-- Change frequency in another thread"
"\r\n-- Default is yes"
"\r\n---------------"
"\r\n*audio_multi_thread:    yes"
"\r\n";


static void read_config() {

	PLang conf = PLang();
	PFile::Path path = PFile::Path(data_path + "config.txt");
	
	bool ok = conf.Read_File(path);
	if (!ok) {
		
		PFile::RW rw = path.GetRW2("w");
		rw.write(default_config, sizeof(default_config) - 1);
		rw.close();
		return;
	}

	PLog::Write(PLog::DEBUG, "PK2", "Found config file");

	int idx = conf.Search_Id("audio_buffer_size");
	if (idx != -1) {
		const char* txt = conf.Get_Text(idx);
		int val = atoi(txt);

		if (val > 0) {
			audio_buffer_size = val;
			

		}
	}
	PLog::Write(PLog::DEBUG, "PK2", "Audio buffer size set to %i", audio_buffer_size);

	idx = conf.Search_Id("audio_multi_thread");
	if (idx != -1) {
		const char* txt = conf.Get_Text(idx);

		if (strcmp(txt, "default") == 0)
			audio_multi_thread = true;
		else if (strcmp(txt, "yes") == 0)
			audio_multi_thread = true;
		else if (strcmp(txt, "no") == 0)
			audio_multi_thread = false;

		
	}
	PLog::Write(PLog::DEBUG, "PK2", "Audio multi thread is %s", audio_multi_thread? "ON" : "OFF");

}

static void start_test(const char* arg) {
	
	if (arg == NULL) return;

	PFile::Path path(arg);

	episode_entry episode;
	episode.name = path.GetDirectory();
	episode.is_zip = false;
	Episode = new EpisodeClass("test", episode);

	Game = new GameClass(path.GetFileName());

	PLog::Write(PLog::DEBUG, "PK2", "Testing episode '%s' level '%s'", episode.name.c_str(), Game->map_file.c_str());

}

// Clean memory (needed?)
static void quit() {

	//Settings_Save();

	PSound::stop_music();

	if (PUtils::Is_Mobile())
		GUI_Exit();

	if (Game!=nullptr){
		delete Game;
		Game = nullptr;
	}
	
	if (Episode!=nullptr){
		delete Episode;
		Episode = nullptr;
	}
	
	if(tekstit!=nullptr){
		delete tekstit;
		tekstit = nullptr;
	}

	Piste::terminate();
	PLog::Write(PLog::DEBUG, "PK2", "Terminated");
	PLog::Exit();

}


void convertToSpr2(const std::string& filename_in, const std::string& filename_out){
	PrototypesHandler handler(false, false, nullptr);
	try{
		PrototypeClass* prototype = handler.loadPrototype(filename_in);
		nlohmann::json j = *prototype;
		handler.savePrototype(prototype, filename_out);
		PLog::Write(PLog::INFO, "PK2", "Sprite %s converted to %s\n", filename_in.c_str(), filename_out.c_str());
	}
	catch(const std::exception&e){
		printf("%s\n", e.what());
	}
}

static void set_paths() {

	PFile::SetDefaultAssetsPath();
	
	#ifndef __ANDROID__

	#ifdef PK2_PORTABLE

	data_path = "data" PE_SEP;
	PFile::CreateDirectory(data_path);

	#else

	char* data_path_p = SDL_GetPrefPath(NULL, PK2_NAME);
	if (data_path_p == NULL) {

		PLog::Write(PLog::FATAL, "PK2", "Failed to init data path");
		quit();
		//return 1;

	}

	data_path = data_path_p;
	SDL_free(data_path_p);

	#endif //PK2_PORTABLE

	#else //__ANDROID__

	const char* ptr = SDL_AndroidGetExternalStoragePath(); //TODO 1.5 - external path must be a writeable path
	if (ptr) {
		External_Path = ptr;
		SDL_free((void*)ptr);
	} else {
		PLog::Write(PLog::ERR, "PK2", "Couldn't find External Path");
	}

	ptr = SDL_AndroidGetInternalStoragePath();
	if (ptr) {
		Internal_Path = ptr;
		SDL_free((void*)ptr);
	} else {
		PLog::Write(PLog::ERR, "PK2", "Couldn't find Internal Path");
	}

	External_Path += PE_SEP;
	Internal_Path += PE_SEP;

	PLog::Write(PLog::DEBUG, "PK2", "External %s", External_Path.c_str());
	PLog::Write(PLog::DEBUG, "PK2", "Internal %s", Internal_Path.c_str());

	PLog::Write(PLog::DEBUG, "PK2", "Allow %i", SDL_AndroidGetExternalStorageState);


	// Choose between internal or external path on Android
	// Prioritize internal
	if (PUtils::ExternalWriteable()) {

		PLog::Write(PLog::DEBUG, "PK2", "External access allowed");

		PFile::Path settings_f = PFile::Path("settings.ini");
		settings_f.SetPath(Internal_Path);
		PLog::Write(PLog::DEBUG, "PK2", "%s", settings_f.c_str());
		if (!settings_f.Find()) {

			PLog::Write(PLog::DEBUG, "PK2", "Settings not found on internal");

			settings_f.SetPath(External_Path);
			if (settings_f.Find()) {

				PLog::Write(PLog::DEBUG, "PK2", "Settings found on external");
				external_dir = true;
			
			}
			else {

				PLog::Write(PLog::DEBUG, "PK2", "Settings not found on external");
				external_dir = false;

			}
		} else {

			PLog::Write(PLog::DEBUG, "PK2", "Settings found on internal");
			external_dir = false;

		}
	} else {

		PLog::Write(PLog::DEBUG, "PK2", "External access not allowed");
		external_dir = false;

	}

	if (external_dir)
		data_path = External_Path;
	else
		data_path = Internal_Path;

	#endif //__ANDROID__

}


static void log_data() {

	PLog::Write(PLog::DEBUG, "PK2", "Pekka Kana 2 started!");
	PLog::Write(PLog::DEBUG, "PK2", "Game version: %s", PK2_VERSION_STR);
	PLog::Write(PLog::DEBUG, "PK2", "Data path - %s", data_path.c_str());

}
bool pk2_setAssetsPath(const std::string& path){
	PFile::SetAssetsPath(path);
	return true;
}

std::string pk2_get_version(){
	return PK2_VERSION_STR;
}

void pk2_init(){

	set_paths();
	PLog::Init(PLog::ALL, PFile::Path(data_path + "log.txt"));
	Prepare_DataPath();
}

void pk2_main(bool _dev_mode, bool _show_fps, bool _test_level, const std::string& test_path){
	
	dev_mode = _dev_mode;
	show_fps = _show_fps;
	test_level = _test_level;

	log_data();
	
	Settings_Open();
	
	read_config();

	Piste::init(screen_width, screen_height, PK2_NAME, "gfx" PE_SEP "icon_new.png", audio_buffer_size, audio_multi_thread);
	if (!Piste::is_ready()) {

		PLog::Write(PLog::FATAL, "PK2", "Failed to init PisteEngine");
		quit();
	}

	ScreensHandler *handler = nullptr;
	try{
		handler = new ScreensHandler();
		Screen::next_screen = SCREEN_INTRO;
		if(dev_mode){
			Screen::next_screen = SCREEN_MENU;
		}
		
		if(test_level){
			start_test(test_path.c_str());
			Screen::next_screen = SCREEN_GAME;
		}

		Piste::loop(std::bind(&ScreensHandler::Loop, handler)); //The game loop
	}
	catch(const std::exception& e){
		PLog::Write(PLog::FATAL, "PK2", e.what());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal error!", e.what(), nullptr);
	}

	if(handler!=nullptr){
		delete handler;
		handler = nullptr;
	}

	quit();
}

bool pk2_convertToSpr2(const std::string& filename_in, const std::string& filename_out){

	std::string filename_out2;
	PLog::Init(PLog::ALL, PFile::Path(data_path + "log.txt"));
	if(filename_in.empty()){
		printf("You have to specify the sprite to convert!");
		return false;
	}

	else if(filename_out.empty()){
		if(filename_in.size()>4 && filename_in.substr(filename_in.size()-4,4)==".spr"){
			filename_out2 = filename_in.substr(0, filename_in.size() -4) + ".spr2";
		}
		else{
			filename_out2 = filename_out + ".spr2";
		}
	}
	convertToSpr2(filename_in, filename_out2);
	return true;
}