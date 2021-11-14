#pragma once

#include <xhash>

namespace Pine {

	// "UUID" (universally unique identifier) or GUID is (usually) a 128-bit integer
	// used to "uniquely" identify information. In Hazel, even though we use the term
	// GUID and UUID, at the moment we're simply using a randomly generated 64-bit
	// integer, as the possibility of a clash is low enough for now.
	// This may change in the future.
	class Uuid
	{
	public:
		Uuid();
		Uuid(uint64_t uuid);
		Uuid(const Uuid& other);

		operator uint64_t () { return m_Uuid; }
		operator const uint64_t() const { return m_Uuid; }
	private:
		uint64_t m_Uuid;
	};
}

namespace std {

	template <>
	struct hash<Pine::Uuid>
	{
		std::size_t operator()(const Pine::Uuid& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};

}
