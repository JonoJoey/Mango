#include "drpc.h"

#include <cstring>
#include <time.h>

#include <discord_rpc.h>
#pragma comment(lib, "discord_rpc.lib")


namespace Mango::DiscordRPC
{
	bool is_init = false;
	int64_t start_time;

	void Setup(const char* application_id, const char* steam_id)
	{
		Discord_Initialize(application_id, nullptr, 1, steam_id);
		is_init = true;
		start_time = time(nullptr);
	}
	void Release()
	{
		Discord_ClearPresence();
		Discord_Shutdown();
		is_init = false;
	}
	void Update(const char* details, const char* state,
		const char* large_img_key, const char* large_img_text,
		const char* small_img_key, const char* small_img_text,
		int64_t start_time, int64_t end_time)
	{
		DiscordRichPresence rpc;
		memset(&rpc, 0, sizeof(DiscordRichPresence));

		rpc.details = details;
		rpc.state = state;

		rpc.largeImageKey = large_img_key;
		rpc.largeImageText = large_img_text;

		rpc.smallImageKey = small_img_key;
		rpc.smallImageText = small_img_text;

		rpc.startTimestamp = start_time;
		rpc.endTimestamp = end_time;

		Discord_UpdatePresence(&rpc);
	}
	int64_t GetStartTime()
	{
		return start_time;
	}
}