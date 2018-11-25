#pragma once

#include <deque>


namespace Mango
{
	class Utility
	{
	public:
		Utility() { RegisterUtil(); };
		Utility(const Utility&) = delete;
		virtual ~Utility() { UnregisterUtil(); };

		virtual void Release() = 0;

	public:
		static void Cleanup();

	private:
		void RegisterUtil();
		void UnregisterUtil();

	private:
		static std::deque<Utility*> m_utilities;
	};
} // namespace Mango