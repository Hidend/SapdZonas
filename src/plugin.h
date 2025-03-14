#pragma once

#include <windows.h>
#include <iostream>

#include "Utils/MinhookWrapper.hpp"
#include <RakHook/rakhook.hpp>

class c_plugin
{
public:
	c_plugin(HMODULE hmodule);
	~c_plugin();

	static void attach_console();

	static void game_loop();
	static c_hook<void(*)()> game_loop_hook;
private:
	HMODULE hmodule;
};
inline c_hook<void(*)()> c_plugin::game_loop_hook = { 0x561B10 };