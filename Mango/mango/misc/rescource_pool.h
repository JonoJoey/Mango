#pragma once

#include <unordered_map>
#include <string>
#include <memory>


namespace Mango
{
	template <typename T>
	class RescourcePool
	{
	public:
		static RescourcePool<T>* Get()
		{
			static RescourcePool<T> instance;
			return &instance;
		}

		void Release()
		{
			m_rescources.clear();
		}

		template <typename ...pack>
		std::shared_ptr<T> AddRes(const std::string& name, pack... args)
		{
			printf("%s", name.c_str());

			auto ptr = std::shared_ptr<T>(new T(args...));
			m_rescources[name] = ptr;
			return ptr;
		}
		std::shared_ptr<T> GetRes(const std::string& name)
		{
			return m_rescources.at(name);
		}
		template <typename ...pack>
		std::shared_ptr<T> GetOrAddRes(const std::string& name, pack... args)
		{
			if (auto res = m_rescources.find(name); res != m_rescources.end())
				return res->second;
			
			return AddRes(name, args...);
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<T>> m_rescources;

	private:
		~RescourcePool() { Release(); }
	};
} // namespace Mango