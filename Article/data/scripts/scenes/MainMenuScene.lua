local Class = require("data.scripts.Class")

local MainMenuScene = Class()

function MainMenuScene:Deinit()
	App.PopLayer("MainMenuLayer")
end

function MainMenuScene:OnShow()
	App.SetState(AppState.MainMenu)
	App.SetDebugUIBlockEvents(true)
	App.PushScriptLayer("MainMenuLayer")
end

return MainMenuScene