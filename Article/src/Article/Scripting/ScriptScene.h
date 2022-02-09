#pragma once

#include "Lithium/Core/Scene.h"
#include "sol/sol.hpp"

class ScriptScene : public Li::Scene
{
public:
	ScriptScene(const std::string& lua_class_name);
	virtual ~ScriptScene();

	virtual void OnShow() override;
	virtual void OnTransition() override;

private:
	std::string m_ClassName;
	sol::table m_Scene;
};
