#pragma once

#include <Pine.h>

class SandboxLayer : public Pine::Layer {
public:
	SandboxLayer()
		: Layer("SandboxLayer") {};

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;

private:
};
