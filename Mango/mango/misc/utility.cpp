#include "utility.h"

#include "../common.h"


namespace Mango
{
	std::deque<Utility*> Utility::m_utilities;

	void Utility::Cleanup()
	{
		while (!m_utilities.empty())
		{
			m_utilities.front()->Release();
			m_utilities.pop_front();
		}
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
				m_utilities.erase(m_utilities.begin() + i);
		}
	}
} // namespace Mango