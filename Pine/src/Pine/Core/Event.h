#pragma once

#include "Input.h"

namespace Pine {

	template<typename T>
	class EventDispatcher {
	public:
		static std::function<void()> Listen(bool(*handler)(const T&, void*), void* data = nullptr)
		{
			std::pair<bool(*)(const T&, void*), void*> pair = { handler, data };
			s_Handlers.push_back(pair);

			return [pair]() {
				s_Handlers.erase(std::find(s_Handlers.begin(), s_Handlers.end(), pair));
			};
		}

		static void Dispatch(const T& e)
		{
			for (auto pair = s_Handlers.crbegin(); pair != s_Handlers.crend(); ++pair) {
				bool(*fn)(const T&, void*) = (*pair).first;
				void* ptr = (*pair).second;

				if ((*fn)(e, ptr))
					break;
			}
		}

	private:
		static std::vector<std::pair<bool(*)(const T&, void*), void*>> s_Handlers;
	};

	template<typename T>
	std::vector<std::pair<bool(*)(const T&, void*), void*>> EventDispatcher<T>::s_Handlers;

	// Window events
	struct WindowResizeEvent {
		uint32_t Width, Height;
	};
	struct WindowCloseEvent {};
	struct WindowFocusEvent {};
	struct WindowBlurEvent {};

	// Mouse events
	struct MouseButtonPressedEvent {
		MouseButton button;
		uint8_t mods;
	};

	struct MouseButtonReleasedEvent {
		MouseButton button;
		uint8_t mods;
	};

	struct MouseMovedEvent {
		float x, y;
	};

	struct MouseScrolledEvent {
		float x_offset, y_offset;
	};

	// key events
	struct KeyPressedEvent {
		Key key;
		uint8_t mods;
	};

	struct KeyReleasedEvent {
		Key key;
		uint8_t mods;
	};

	struct KeyRepeatedEvent {
		Key key;
		uint8_t mods;
	};

	struct KeyTypedEvent {
		Key key;
	};


}
