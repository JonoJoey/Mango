#include "misc.h"

#include <algorithm>


namespace Mango
{
	std::string GetAppDataPath()
	{
		std::string path;
		char* buffer = nullptr;
		if (_dupenv_s(&buffer, nullptr, "APPDATA") || !buffer)
			return "";

		path = buffer;
		free(buffer);

		std::replace(path.begin(), path.end(), '\\', '/');
		return path;
	}
} // namespace Mango