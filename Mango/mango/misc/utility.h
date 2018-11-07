#pragma once

#include <deque>


namespace Mango
{
	// example:
	// 
	// class ExampleUtility : private Utility
	// {
	// public:
	//     ~ExampleUtility() { Release(); }
	// 
	// 	   void Release() override
	// 	   {
	// 	   	   if (!IsUtilRegistered())
	// 	   	       return;
	// 	   	   UnregisterUtil();
	// 	   	   
	// 	   	   release code here...
	// 	   }
	// }
	// 
	class Utility
	{
	public:
		Utility() { RegisterUtil(); };
		Utility(const Utility&) = delete;
		virtual ~Utility() { UnregisterUtil(); };

		virtual void Release() = 0;

		bool IsUtilRegistered() const;
		void UnregisterUtil();

	public:
		static void Cleanup();

	private:
		void RegisterUtil();

	private:
		static std::deque<Utility*> m_utilities;
	};
} // namespace Mango