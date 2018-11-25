#include "utility.h"

#include "../common.h"


namespace Mango
{
	std::deque<Utility*> Utility::m_utilities;

	void Utility::Cleanup()
	{
		for (auto util : m_utilities)
			util->Release();
		m_utilities.clear();
	}

	void Utility::RegisterUtil()
	{
		m_utilities.push_back(this);
	}
	void Utility::UnregisterUtil()
	{
		for (size_t i = 0; i < m_utilities.size(); i++)
		{
			if (m_utilities[i] == this)
			{
				m_utilities.erase(m_utilities.begin() + i);
				return;
			}
		}
	}
} // namespace Mango