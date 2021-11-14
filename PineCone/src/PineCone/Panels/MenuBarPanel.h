#pragma once

#include "Panel.h"
#include "PineCone\Core\SceneContext.h"

#include <memory>

namespace Pine {

	class MenuBarPanel : public Panel
	{
	public:
		MenuBarPanel(std::shared_ptr<SceneContext> context)
			: m_Context(context) {}
		~MenuBarPanel() = default;

		virtual void OnAttach() override;
		virtual void OnRender(Timestep ts) override;

	private:
		void OpenScene() const;

		void SaveScene(const std::filesystem::path& path) const;
		void SaveSceneAs() const;

	public:
		static std::string GetName() { return "MenuBarPanel"; };

	private:
		std::shared_ptr<SceneContext> m_Context;
	};

}