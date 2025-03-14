#include "plugin.h"

#include <RakNet/PacketEnumerations.h>
#include <RakNet/StringCompressor.h>
#include <cpr/cpr.h>
#include "samp/samp.h"
#include "ZoneManager.h"
#include <subauth.h>

inline bool on_spawn_info(unsigned char& id, RakNet::BitStream* bs) {
	if (id != 68) 
		return true;

	static bool first_spawn = true;

	if (first_spawn) {
		c_zone_manager::get()->loadZones();
		first_spawn = false;
	}

	return true;
}

void c_plugin::game_loop() {
	static bool initialized = false;

	if (initialized || !rakhook::initialize() || c_chat::get()->ref() == nullptr) {
		return game_loop_hook.call_original();

	}

	initialized = true;
	StringCompressor::AddReference();


	c_chat::get()->ref()->add_message(-1, "{87CEEB}SAPD Zonas{FFFFFF} by {00FF00}Hidend {FFFFFF}// {FF0021}<3");
	c_input::get()->ref()->add_command("zonas", [](const char* param) {
		c_zone_manager::get()->loadZones();
		});
	c_input::get()->ref()->add_command("hidend", [](const char* param) {
		attach_console();
		});

	rakhook::on_receive_rpc += on_spawn_info;

    return game_loop_hook.call_original();
}

void c_plugin::attach_console() {
	if (!AllocConsole())
		return;

	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	freopen_s(&f, "CONOUT$", "w", stderr);
	freopen_s(&f, "CONIN$", "r", stdin);
}

c_plugin::c_plugin(HMODULE hmodule) : hmodule(hmodule)
{
	game_loop_hook.add(&c_plugin::game_loop);
}


c_plugin::~c_plugin()
{
	rakhook::destroy();
}
