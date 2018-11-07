#include "utility.h"

#include "../common.h"


namespace Mango
{
	std::deque<Utility*> Utility::m_utilities;

	void Utility::Cleanup()
	{
		for (auto util : m_utilities)
			util->Release();

		// Release() SHOULD call UnregisterUtility() (if you're not retarded)
		if (!m_utilities.empty())
			DBG_LOG("A Release() function does not call UnregisterUtility()");

		m_utilities.clear();
	}
	void Utility::RegisterUtil()
	{
		m_utilities.push_back(this);
	}

	bool Utility::IsUtilRegistered() const
	{
		for (auto util : m_utilities)
		{
			if (util == this)
				return true;
		}

		return false;
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