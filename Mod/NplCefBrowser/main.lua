--[[
Title: NplCefBrowser
Author(s): leio
Date: 2016.11.24
Desc: 
use the lib:
------------------------------------------------------------
NPL.load("(gl)Mod/NplCefBrowser/main.lua");
local NplCefBrowser = commonlib.gettable("Mod.NplCefBrowser");
------------------------------------------------------------
]]
local NplCefBrowser = commonlib.inherit(commonlib.gettable("Mod.ModBase"),commonlib.gettable("Mod.NplCefBrowser"));

-- name of the generator
NplCefBrowser.generator_name = "NplCefBrowser";

function NplCefBrowser:ctor()
	-- please note this mode only works with 32bits dll.
	if(ParaEngine.GetAttributeObject():GetField("Is64BitsSystem", false)) then
		self.dll_path = "NplCefBrowser_64bits.dll";
	else
		self.dll_path = "NplCefBrowser.dll";
	end
end

-- virtual function get mod name

function NplCefBrowser:GetName()
	return "NplCefBrowser"
end

-- virtual function get mod description 

function NplCefBrowser:GetDesc()
	return "NplCefBrowser is a plugin in paracraft"
end

function NplCefBrowser:init()
	LOG.std(nil, "info", "NplCefBrowser", "plugin initialized");
	self:RegisterCommand();
end

function NplCefBrowser:OnLogin()
end
-- called when a new world is loaded. 

function NplCefBrowser:OnWorldLoad()
end
-- called when a world is unloaded. 

function NplCefBrowser:OnLeaveWorld()
end

function NplCefBrowser:OnDestroy()
end
function NplCefBrowser:RegisterCommand()
	local Commands = commonlib.gettable("MyCompany.Aries.Game.Commands");
	Commands["nplbrowser"] = {
		name="nplbrowser", 
		quick_ref="/nplbrowser", 
		desc="open a web browser", 
		handler = function(cmd_name, cmd_text, cmd_params, fromEntity)
			self:Call();
		end,
	};
end

function NplCefBrowser:Call()
	NPL.activate(self.dll_path);
end
