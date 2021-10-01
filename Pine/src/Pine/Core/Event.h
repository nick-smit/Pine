#pragma once

#include "Input.h"

#include <functional>
#include <vector>
#include <utility>

namespace Pine {

	template<typename T>
	using EventFn = std::function<bool(const T&)>;

	template <typename T>
	using ListenerPair = std::pair<uint32_t, EventFn<T>>;

	template<typename T>
	class EventDispatcher {
	public:
		static std::function<void()> Listen(EventFn<T> handler)
		{
			uint32_t id = s_NextId++;
			ListenerPair<T> pair = { id, handler };
			s_Listeners.push_back(pair);

			return [id]() {
				for (auto it = s_Listeners.cbegin(); it != s_Listeners.cend(); it++) {
					if ((*it).first == id) {
						s_Listeners.erase(it);
						break;
					}
				}
			};
		}

		static void Dispatch(const T& e)
		{
			for (auto it = s_Listeners.crbegin(); it != s_Listeners.crend(); ++it) {
				EventFn<T> fn = (*it).second;

				if (fn(e))
					break;
			}
		}

	private:
		static uint32_t s_NextId;
		static std::vector<ListenerPair<T>> s_Listeners;
	};

	template<typename T>
	std::vector<ListenerPair<T>> EventDispatcher<T>::s_Listeners;
	template<typename T>
	uint32_t EventDispatcher<T>::s_NextId = 0;

	// Window events
	struct WindowResizeEvent {
		uint32_t Width, Height;
	};
	struct WindowCloseEvent {};
	struct WindowFocusEvent {};
	struct WindowBlurEvent {};

	// Mouse events
	struct MouseButtonPressedEvent {
		MouseButton Button;
		uint8_t Mods;
	};

	struct MouseButtonReleasedEvent {
		MouseButton Button;
		uint8_t Mods;
	};

	struct MouseMovedEvent {
		float X, Y;
	};

	struct MouseScrolledEvent {
		float XOffset, YOffset;
	};

	// key events
	struct KeyPressedEvent {
		Key Key;
		uint8_t Mods;
	};

	struct KeyReleasedEvent {
		Key Key;
		uint8_t Mods;
	};

	struct KeyRepeatedEvent {
		Key Key;
		uint8_t Mods;
	};

	struct KeyTypedEvent {
		Key Key;
	};


}
