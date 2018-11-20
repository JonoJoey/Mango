#pragma once

#include <stdint.h>


namespace Mango::DiscordRPC
{
	void Setup(const char* application_id, const char* steam_id = nullptr);
	void Release();
	void Update(const char* details, const char* state,
		const char* large_img_key, const char* large_img_text,
		const char* small_img_key, const char* small_img_text,
		int64_t start_time, int64_t end_time);
	int64_t GetStartTime();
}