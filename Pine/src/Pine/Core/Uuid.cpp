#include "pinepch.h"
#include "Uuid.h"

namespace Pine {

	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;
	
	Uuid::Uuid()
		: m_Uuid(s_UniformDistribution(s_Engine))
	{
		PINE_LOG_CORE_DEBUG("Generated uuid {0}", m_Uuid);
	}

	Uuid::Uuid(uint64_t uuid)
		: m_Uuid(uuid)
	{
	}

	Uuid::Uuid(const Uuid& other)
		: m_Uuid(other.m_Uuid)
	{
	}

}
