/* JavaScript full game support Work in progress */

function randomInt(min, max) {
    return Math.floor(Math.random() * (max++ - min-- + 1)) + min;
}

function randomIntBetween(min, max) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
}

Player.prototype.setclientcvar = function(cvar, value) {
	this.sendservercommand("v "+cvar+" \""+value+"\"");
};

Player.prototype.closemenu = function() {
	this.sendservercommand("u");
};

Player.prototype.suicide = function() {
	/* kill self */
};

Player.prototype.openmenu = function(menu) {
	nomouse = 1;
	menuIndex = getscriptmenuindex(menu);
	if(menuIndex == -1)
		return;
	this.sendservercommand("t "+menuIndex+" "+nomouse+"");
};

Player.prototype.iprintlnbold = function(msg) { this.sendservercommand("g \""+msg+"\""); };
Player.prototype.iprintln = function(msg) { this.sendservercommand("e \""+msg+"\""); };
function iprintlnbold(msg) { sendservercommand("g \""+msg+"\""); }
function iprintln(msg) { sendservercommand("e \""+msg+"\""); }
function isdefined(a) { return a != null; }

function getscriptmenuindex(menu) {
	base = 1180;
	var i;
	for(i = 0; i < 32; i++) {
		str = getconfigstring(base + i);
		if(str != menu)
			continue;
		return i;
	}
	return -1;
}

function x_modelindex(modelname) {
	base = 268;
	var i;
	for(i = 0; i < 256; i++) {
		str = getconfigstring(base + i);
		if(str != modelname)
			continue;
		return i;
	}
	return 0;
}

function consolecommand(args) {
	players.forEach(function(self) {
		//print(self.statusicon);
		self.statusicon = args[1];
	});
}

var game = [];

function StartGameType() {
	// defaults if not defined in level script
	if(!isdefined(game["allies"]))
		game["allies"] = "american";
	if(!isdefined(game["axis"]))
		game["axis"] = "german";

	if(!isdefined(game["layoutimage"]))
		game["layoutimage"] = "default";
	layoutname = "levelshots/layouts/hud@layout_" + game["layoutimage"];
	precacheShader(layoutname);
	setcvar("scr_layoutimage", layoutname);
	makeCvarServerInfo("scr_layoutimage", ""); /* does nothing yet */

	// server cvar overrides
	if(getcvar("scr_allies") != "")
		game["allies"] = getcvar("scr_allies");	
	if(getcvar("scr_axis") != "")
		game["axis"] = getcvar("scr_axis");
	
	game["menu_team"] = "team_" + game["allies"] + game["axis"];
	game["menu_weapon_allies"] = "weapon_" + game["allies"];
	game["menu_weapon_axis"] = "weapon_" + game["axis"];
	game["menu_viewmap"] = "viewmap";
	game["menu_callvote"] = "callvote";
	game["menu_quickcommands"] = "quickcommands";
	game["menu_quickstatements"] = "quickstatements";
	game["menu_quickresponses"] = "quickresponses";
	game["headicon_allies"] = "gfx/hud/headicon@allies.tga";
	game["headicon_axis"] = "gfx/hud/headicon@axis.tga";
	
	precacheString("MPSCRIPT_PRESS_ACTIVATE_TO_RESPAWN");
	precacheString("MPSCRIPT_KILLCAM");

	precacheMenu(game["menu_team"]);
	precacheMenu(game["menu_weapon_allies"]);
	precacheMenu(game["menu_weapon_axis"]);
	precacheMenu(game["menu_viewmap"]);
	precacheMenu(game["menu_callvote"]);
	precacheMenu(game["menu_quickcommands"]);
	precacheMenu(game["menu_quickstatements"]);
	precacheMenu(game["menu_quickresponses"]);

	precacheShader("black");
	precacheShader("hudScoreboard_mp");
	precacheShader("gfx/hud/hud@mpflag_spectator.tga");
}

function LoadGameType() {
}

function spawnIntermission() {}
function spawnSpectator() {}
function spawnPlayer() {}

function OnPlayerDisconnect() {
	iprintln(self.name + " ^7Disconnected");
}

function OnPlayerConnect(self) {
	
	self.statusicon = "gfx/hud/hud@status_connecting.tga";
	//self waittill("begin");
	
	self.statusicon = "";

	iprintln(self.name + " ^7Connected");
	print(self.name + " Connected\n");
	print("HEY\n");
	if(game["state"] == "intermission") {
		spawnIntermission();
		return;
	}

	if(isdefined(self.pers["team"]) && self.pers["team"] != "spectator")
	{
		self.setclientcvar("scr_showweapontab", "1");

		if(self.pers["team"] == "allies")
		{
			self.sessionteam = "allies";
			self.setclientcvar("g_scriptMainMenu", game["menu_weapon_allies"]);
		}
		else
		{
			self.sessionteam = "axis";
			self.setclientcvar("g_scriptMainMenu", game["menu_weapon_axis"]);
		}
			
		if(isdefined(self.pers["weapon"]))
			spawnPlayer();
		else
		{
			spawnSpectator();

			if(self.pers["team"] == "allies")
				self.openmenu(game["menu_weapon_allies"]);
			else
				self.openmenu(game["menu_weapon_axis"]);
		}
	}
	else
	{
		self.setclientcvar("g_scriptMainMenu", game["menu_team"]);
		self.setclientcvar("scr_showweapontab", "0");
		
		if(!isdefined(self.pers["team"]))
			self.openmenu(game["menu_team"]);

		self.pers["team"] = "spectator";
		self.sessionteam = "spectator";

		spawnSpectator();
	}
}

function getTeamScore(a) {
	return 0;
}

function OnMenuResponse(self, menu, response) {
	setTimeout(function() {
	
	players.forEach(function(player) {
		player.iprintlnbold(self.name + " menuresponse (slowpoke)");
		player.score++;
		player.deaths++;
	});

	}, 1000);
}
