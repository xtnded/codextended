/* JavaScript full game support Work in progress */

function randomInt(min, max) {
    return Math.floor(Math.random() * (max++ - min-- + 1)) + min;
}

function randomIntBetween(min, max) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
}

Player.prototype.setClientCvar = function(cvar, value) {
	this.sendServerCommand("v "+cvar+" \""+value+"\"");
};

Player.prototype.closeMenu = function() {
	this.sendServerCommand("u");
};

Player.prototype.suicide = function() {
	/* kill self */
};

Player.prototype.openMenu = function(menu) {
	nomouse = 1;
	menuIndex = getScriptMenuIndex(menu);
	if(menuIndex == -1)
		return;
	this.sendServerCommand("t "+menuIndex+" "+nomouse+"");
};

Player.prototype.iPrintLnBold = function(msg) { this.sendServerCommand("g \""+msg+"\""); };
Player.prototype.iPrintLn = function(msg) { this.sendServerCommand("e \""+msg+"\""); };
function iPrintLnBold(msg) { sendServerCommand("g \""+msg+"\""); }
function iPrintLn(msg) { sendServerCommand("e \""+msg+"\""); }
function isDefined(a) { return a != null; }
function isPlayer(p) { return p instanceof Player };

function getScriptMenuIndex(menu) {
	base = 1180;
	var i;
	for(i = 0; i < 32; i++) {
		str = getConfigString(base + i);
		if(str != menu)
			continue;
		return i;
	}
	return -1;
}

function X_ModelIndex(modelname) {
	base = 268;
	var i;
	for(i = 0; i < 256; i++) {
		str = getConfigString(base + i);
		if(str != modelname)
			continue;
		return i;
	}
	return 0;
}

function ConsoleCommand(args) {
	players.forEach(function(self) {
		//print(self.statusicon);
		self.statusicon = args[1];
	});
}

var game = [];

function StartGameType() {
	// defaults if not defined in level script
	if(!isDefined(game["allies"]))
		game["allies"] = "american";
	if(!isDefined(game["axis"]))
		game["axis"] = "german";

	if(!isDefined(game["layoutimage"]))
		game["layoutimage"] = "default";
	layoutname = "levelshots/layouts/hud@layout_" + game["layoutimage"];
	precacheShader(layoutname);
	setCvar("scr_layoutimage", layoutname);
	makeCvarServerInfo("scr_layoutimage", ""); /* does nothing yet */

	// server cvar overrides
	if(getCvar("scr_allies") != "")
		game["allies"] = getCvar("scr_allies");	
	if(getCvar("scr_axis") != "")
		game["axis"] = getCvar("scr_axis");
	
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
	iPrintLn(self.name + " ^7Disconnected");
}

function OnPlayerDamage(self, eInflictor, eAttacker, iDamage, iDFlags, sMeansOfDeath, sWeapon, vPoint, vDir, sHitloc) {
	if(self.sessionteam == "spectator")
		return;
	
	// Don't do knockback if the damage direction was not specified
	if(!isDefined(vDir))
		iDFlags |= level.iDFLAGS_NO_KNOCKBACK;

	// Make sure at least one point of damage is done
	if(iDamage < 1)
		iDamage = 1;
	
	
	// Do debug print if it's enabled
	if(getCvar("g_debugDamage"))
	{
		print("client:" + self.number + " health:" + self.health +
			" damage:" + iDamage + " hitLoc:" + sHitLoc);
	}
	
	
	// Apply the damage to the player
//	self finishPlayerDamage(eInflictor, eAttacker, iDamage, iDFlags, sMeansOfDeath, sWeapon, vPoint, vDir, sHitLoc);

	if(self.sessionstate != "dead")
	{
		lpselfnum = self.number;
		lpselfname = self.name;
		lpselfteam = self.pers["team"];
		lpattackerteam = "";

		if(isPlayer(eAttacker))
		{
			lpattacknum = eAttacker.number;
			lpattackname = eAttacker.name;
			lpattackerteam = eAttacker.pers["team"];
		}
		else
		{
			lpattacknum = -1;
			lpattackname = "";
			lpattackerteam = "world";
		}

		logPrint("D;" + lpselfnum + ";" + lpselfteam + ";" + lpselfname + ";" + lpattacknum + ";" + lpattackerteam + ";" + lpattackname + ";" + sWeapon + ";" + iDamage + ";" + sMeansOfDeath + ";" + sHitLoc + "\n");
	}
}

function OnPlayerConnect(self) {
	
	self.statusicon = "gfx/hud/hud@status_connecting.tga";
	//self waittill("begin");
	
	self.statusicon = "";

	iPrintLn(self.name + " ^7Connected");
	print(self.name + " Connected\n");
	print("HEY\n");
	if(game["state"] == "intermission") {
		spawnIntermission();
		return;
	}
	
	if(!isDefined(self.pers))
		self.pers=[];
	
	if(isDefined(self.pers["team"]) && self.pers["team"] != "spectator")
	{
		self.setClientCvar("scr_showweapontab", "1");

		if(self.pers["team"] == "allies")
		{
			self.sessionteam = "allies";
			self.setClientCvar("g_scriptMainMenu", game["menu_weapon_allies"]);
		}
		else
		{
			self.sessionteam = "axis";
			self.setClientCvar("g_scriptMainMenu", game["menu_weapon_axis"]);
		}
			
		if(isDefined(self.pers["weapon"]))
			spawnPlayer();
		else
		{
			spawnSpectator();

			if(self.pers["team"] == "allies")
				self.openMenu(game["menu_weapon_allies"]);
			else
				self.openMenu(game["menu_weapon_axis"]);
		}
	}
	else
	{
		self.setClientCvar("g_scriptMainMenu", game["menu_team"]);
		self.setClientCvar("scr_showweapontab", "0");
		
		if(!isDefined(self.pers["team"]))
			self.openMenu(game["menu_team"]);

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
			player.iPrintLnBold(self.name + " menuresponse (slowpoke)");
			player.score++;
			player.deaths++;
		});

	}, 1000);
}
