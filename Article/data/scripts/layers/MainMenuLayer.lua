local Class = require("data.scripts.Class")
local Layer = require("data.scripts.Layer")
local UIContext = require("data.scripts.UIContext")

local function CreateMenuButton(context, name, text, transition_scene)
	local button = context:NewElement(name)
	button:SetContain(UI.Contain.Column)
	button:SetBehave(UI.Behave.Top)
	button:SetSize(200, 40)
	button:SetMargins({ top = 10, bottom = 10 })
	button:SetQuad("texture_white", 0.0, 0.0, 0.0, 1.0)

	local inner = context:NewElement(name .. "__INNER")
	inner:SetContain(UI.Contain.Column)
	inner:SetBehave(UI.Behave.Fill)
	inner:SetMargins({ bottom = 2 })
	inner:SetQuad("texture_white", 1.0, 1.0, 1.0, 1.0)
	button:AddChild(inner)

	-- TODO: make labels a lua class.
	local label = context:NewElement(name .. "__LABEL")
	label:SetContain(UI.Contain.Column)
	label:SetBehave(UI.Behave.Left, UI.Behave.Bottom)
	label:SetSize(1, 1)
	label:SetQuad("texture_white", 0.0, 0.0, 0.0, 1.0)
	label:SetMargins({ bottom = -20, left = 12 })
	label:SetLabel({
		text = text,
		pt_size = 24,
		font = "SourceCodePro-Regular"
	})

	button:SetEvents({
		OnClick = function(registry, entity, button)
			if button == 1 then
				App.TransitionScene(transition_scene, true)
				return true;
			end
			return false;
		end,
		OnMouseEnter = function(registry, entity)
			inner:SetQuad("texture_white", 0.9, 0.9, 0.9, 1.0)
			return true;
		end,
		OnMouseLeave = function(registry, entity)
			inner:SetQuad("texture_white", 1.0, 1.0, 1.0, 1.0)
			return true;
		end,
	})

	inner:AddChild(label)

	return button
end

local MainMenuLayer = Class(Layer)
function MainMenuLayer:Init(registry)
	self:_InitBase("MainMenuLayer", registry, {
		"RNG",
		"Flicker",
		"UIClick",
		"UIHover",
		"UILayout",
		"UISortTransforms",
		"UIRender"
	})

	self.context = UIContext(self.registry, 0.1, 0.9)

	-- Background layers.
	local bg_black = self.context:NewElement("bg_black")
	bg_black:SetContain(UI.Contain.Column)
	bg_black:SetBehave(UI.Behave.Fill)
	bg_black:SetQuad("texture_white", 0.0, 0.0, 0.0, 1.0)
	bg_black:SetMargins({ left = 6, right = 6, top = 6, bottom = 6 })
	self.context:AddChild(bg_black)

	local bg_white = self.context:NewElement("bg_white")
	bg_white:SetContain(UI.Contain.Column)
	bg_white:SetBehave(UI.Behave.Fill)
	bg_white:SetQuad("texture_white", 1.0, 1.0, 1.0, 1.0)
	bg_black:AddChild(bg_white)

	-- Buttons.
	local button_play = CreateMenuButton(self.context, "button_play", "PLAY", "GameScene")
	bg_white:AddChild(button_play)

	local button_level_editor = CreateMenuButton(self.context, "button_level_editor", "LEVEL EDITOR", "LevelEditorScene")
	bg_white:AddChild(button_level_editor)

	local button_example_video = CreateMenuButton(self.context, "button_example_video", "VIDEO", "VideoPlayerScene")
	bg_white:AddChild(button_example_video)

	self.context:Rebuild()
end

return MainMenuLayer