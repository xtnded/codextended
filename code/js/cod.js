/* JavaScript full game support Work in progress */
var bDebug = false;

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

var GSC = {
};

function GSC_LoadGametypeScript() {
	handle = GSC_LoadCallback("lol", "maps/mp/gametypes/tdm");
	GSC.lol = function() {
		GSC_Call(handle, 2);
	};
}

function LoadGameType() {
	//GSC.lol();
	setInterval(function() {
		print("time = " + gettime() + "\n");
	}, 1000);
}

/* tdm.gsc at lol() */
/*

lol(test) {
	printconsole("HAHAHAHA LOL "+test+"\n");
}
*/

function spawnIntermission() {}
function spawnSpectator() {}
function spawnPlayer() {}

function OnPlayerDisconnect() {
	iPrintLn(self.name + " ^7Disconnected");
}

function OnPlayerDamage(self, eInflictor, eAttacker, iDamage, iDFlags, sMeansOfDeath, sWeapon, vPoint, vDir, sHitLoc) {
	if(bDebug) {
		print("OnPlayerDamage: ");
		for(i in arguments)
			print(i+": "+arguments[i]+", ");
		print("\n");
	}
	
	if(self.sessionteam == "spectator")
		return;
	
	// Don't do knockback if the damage direction was not specified
	if(!isDefined(vDir))
		iDFlags |= 4; //level.iDFLAGS_NO_KNOCKBACK

	// Make sure at least one point of damage is done
	if(iDamage < 1)
		iDamage = 1;
	
	
	// Do debug print if it's enabled
	if(getCvar("g_debugDamage") == "1")
	{
		print("client:" + self.number + " health:" + self.health +
			" damage:" + iDamage + " hitLoc:" + sHitLoc);
	}
	
	self.iPrintLn("damage!! man");
	
	// Apply the damage to the player
//	self finishPlayerDamage(eInflictor, eAttacker, iDamage, iDFlags, sMeansOfDeath, sWeapon, vPoint, vDir, sHitLoc);
}

function OnPlayerSay(self, msg, bIsTeam) {
	iPrintLnBold(self.name + " said "+ msg);
	print("sessionteam = " + self.sessionteam);
}

function OnPlayerCommand(self, args) {
	cmd = args[0];
	
	switch(cmd) {
			
			
		case "say":
		case "say_team":
			OnPlayerSay(self, args[1], cmd != "say");
		break;
		
	}
	
	return false;
}

function OnPlayerBegin(self) { /* equivalent of self waittill("begin"); this means the player is connected */
	
	self.statusicon = "";

	print(self.statusicon + "\n");
	
	iPrintLn(self.name + " ^7Connected");
	print(self.name + " Connected\n");
	
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
		print("else\n");
		self.setClientCvar("g_scriptMainMenu", game["menu_team"]);
		self.setClientCvar("scr_showweapontab", "0");
		
		if(!isDefined(self.pers["team"]))
			self.openMenu(game["menu_team"]);

		self.pers["team"] = "spectator";
		self.sessionteam = "spectator";

		spawnSpectator();
	}
}

function OnPlayerConnect(self) { /* the player started connecting return a string to kick, undefined to let them pass through */
	if(self.name != "php")
		return "Visit http://cod1.eu";
	
	
	self.statusicon = "gfx/hud/hud@status_connecting.tga";
	//self waittill("begin");
	
	return undefined;
}

function getTeamScore(a) {
	return 0;
}

function OnMenuResponse(self, menu, response) {
	
	if(response == "open" || response == "close")
		return;
	
	if(menu == game["menu_team"])
	{
		switch(response)
		{
		case "allies":
		case "axis":
		case "autoassign":
			if(response == self.pers["team"] && self.sessionstate == "playing")
				return;
	
			if(response != self.pers["team"] && self.sessionstate == "playing")
				return;//self suicide();
	
			//self notify("end_respawn");
	
			self.pers["team"] = response;
			self.pers["weapon"] = undefined;
			self.pers["savedmodel"] = undefined;
	
			self.setClientCvar("scr_showweapontab", "1");
	
			if(self.pers["team"] == "allies")
			{
				self.setClientCvar("g_scriptMainMenu", game["menu_weapon_allies"]);
				self.openMenu(game["menu_weapon_allies"]);
			}
			else
			{
				self.setClientCvar("g_scriptMainMenu", game["menu_weapon_axis"]);
				self.openMenu(game["menu_weapon_axis"]);
			}
			break;
	
		case "spectator":
			if(self.pers["team"] != "spectator")
			{
				self.pers["team"] = "spectator";
				self.pers["weapon"] = undefined;
				self.pers["savedmodel"] = undefined;
				
				self.sessionteam = "spectator";
				self.setClientCvar("g_scriptMainMenu", game["menu_team"]);
				self.setClientCvar("scr_showweapontab", "0");
				spawnSpectator();
			}
			break;
	
		case "weapon":
			if(self.pers["team"] == "allies")
				self.openMenu(game["menu_weapon_allies"]);
			else if(self.pers["team"] == "axis")
				self.openMenu(game["menu_weapon_axis"]);
			break;
			
		case "viewmap":
			self.openMenu(game["menu_viewmap"]);
			break;
	
		case "callvote":
			self.openMenu(game["menu_callvote"]);
			break;
		}
	}		
	else if(menu == game["menu_weapon_allies"] || menu == game["menu_weapon_axis"])
	{
		if(response == "team")
		{
			self.openMenu(game["menu_team"]);
			return;
		}
		else if(response == "viewmap")
		{
			self.openMenu(game["menu_viewmap"]);
			return;
		}
		else if(response == "callvote")
		{
			self.openMenu(game["menu_callvote"]);
			return;
		}
		
		if(!isDefined(self.pers["team"]) || (self.pers["team"] != "allies" && self.pers["team"] != "axis"))
			return;
	
		//weapon = self maps\mp\gametypes\_teams::restrict(response);
		weapon = response;
	
		if(weapon == "restricted")
		{
			self.openMenu(menu);
			return;
		}
		
		if(isDefined(self.pers["weapon"]) && self.pers["weapon"] == weapon)
			return;
		
		if(!isDefined(self.pers["weapon"]))
		{
			self.pers["weapon"] = weapon;
			spawnPlayer();
			//self thread printJoinedTeam(self.pers["team"]);
			iPrintLn(self.name + " ^7joined " + self.pers["tean"]);
		}
		else
		{
			self.pers["weapon"] = weapon;
	
			//weaponname = maps\mp\gametypes\_teams::getWeaponName(self.pers["weapon"]);
			weaponname = self.pers["weapon"];
			
			self.iPrintLn("You will respawn with a " + weaponname);
		}
	}
	else if(menu == game["menu_viewmap"])
	{
		switch(response)
		{
		case "team":
			self.openMenu(game["menu_team"]);
			break;
			
		case "weapon":
			if(self.pers["team"] == "allies")
				self.openMenu(game["menu_weapon_allies"]);
			else if(self.pers["team"] == "axis")
				self.openMenu(game["menu_weapon_axis"]);
			break;
	
		case "callvote":
			self.openMenu(game["menu_callvote"]);
			break;
		}
	}
	else if(menu == game["menu_callvote"])
	{
		switch(response)
		{
		case "team":
			self.openMenu(game["menu_team"]);
			break;
			
		case "weapon":
			if(self.pers["team"] == "allies")
				self.openMenu(game["menu_weapon_allies"]);
			else if(self.pers["team"] == "axis")
				self.openMenu(game["menu_weapon_axis"]);
			break;
	
		case "viewmap":
			self.openMenu(game["menu_viewmap"]);
			break;
		}
	}
	/*
	else if(menu == game["menu_quickcommands"])
		maps\mp\gametypes\_teams::quickcommands(response);
	else if(menu == game["menu_quickstatements"])
		maps\mp\gametypes\_teams::quickstatements(response);
	else if(menu == game["menu_quickresponses"])
		maps\mp\gametypes\_teams::quickresponses(response);
	*/
}
