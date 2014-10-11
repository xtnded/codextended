/*
	This file is part of CoDExtended.

    CoDExtended is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CoDExtended is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CoDExtended.  If not, see <http://www.gnu.org/licenses/>.
*/
/*

	To reload JavaScript type into the console 'js_reload'
	
	CoDExtended JavaScript
	
	ent_* functions
	ent_getorigin(entity)
	ent_getint(entity, offset, type) //relatively to the entityState* structure (Q3 for reference)
	//useless if you don't know any offset.
	//type can be 8, 16, 32 or 64 whether byte, short, int or long
	ent_setint(entity, offset, value, type) //same as ent_getint
	
	player_* functions
	player_sendcommand(player, command)
	player_getname(player)
	player_getip(player)
	player_setname(player, new_name)
	player_pm(player, message)
	player_kick(player, reason)
	player_getscore(player)
	player_getping(player)
	
	
	_* functions
	ent = spawn(classname, new Vector(x,y,z))
	//e.g health = spawn("item_health", new Vector(100,100,100));
	//e.g weap = spawn("mpweapon_mp44", ent_getorigin(entity));
	sendcommand(command)
	print(message) (not new line terminated)
	Com_Printf //same as print
	linkentity(entity) //Quake3 for reference what it does
	getconfigstring(index) //max is 2047 (Max of 2048 configstrings)
	setconfigstring(index, string) //same max as getconfigstring
	Cmd_Argv() //Q3 for reference
	Cmd_Argv(index) //Q3 for reference
	Cmd_ArgvBuffer(index) //Q3 for reference
	
	newhudelem() //unsafe to use
	dobjupdate(entity) //unsafe to use
	
*/

function randomInt(min, max) {
    return Math.floor(Math.random() * (max++ - min-- + 1)) + min;
}

function randomIntBetween(min, max) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
}

function player_pm(player, message) {
	playersendcommand(player, "i \"^2[PM] ^7"+message+"\"");
}

function player_setname(player, newname) {
	setclientcvar(player, "name", newname);
}

function setclientcvar(player, cvar, value) {
	if(player!=-1)
		playersendcommand(player, "v "+cvar+" \""+value+"\"");
	else
		sendcommand("v "+cvar+" \""+value+"\"");
}

function player_openmenu(player, menu) {
	nomouse = 1;
	menuIndex = getscriptmenuindex(menu);
	if(menuIndex == -1)
		return;
	playersendcommand(player, "t "+menuIndex+" "+nomouse+"");
}

function iprintlnbold(player,msg) {
	if(player!=-1)
	playersendcommand(player,"g \""+msg+"\"");
	else
	sendcommand("g \""+msg+"\"");
}

var iprintln = function(player,msg) { //this works fine as this JavaScript
	if(player!=-1)
	playersendcommand(player,"f \""+msg+"\"");
	else
	sendcommand("e \""+msg+"\"");
}

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

var Vector = function(x,y,z) {
	this.x = x;
	this.y = y;
	this.z = z;
	return this;
}

var players=[];

/*
	player_command
		- Executed when a player sends a console message like say_team or say.
		
	return 1 to stop default code from executing.
*/

function player_command(player) {
	var cmd = Cmd_Argv(0);
	if(cmd == "test") {
		player_pm(player, "hey");
		return true;
	} else if(cmd == "say" || cmd=="say_team") {
		chatcmd = Cmd_Argv(1);
		chatcmd2 = Cmd_Argv(2);
		print("chatcmd = "+chatcmd+"\n");
		if(chatcmd=="!score") {
			player_pm(player, "Your score is "+player_getscore(player));
		} else if(chatcmd=="*name") {
			
			player_setname(player, "Chandelier");
		}
		return false;
	} else if(cmd=="yay") {
		player_openmenu(player, Cmd_Argv(1)); //open een menutje bij naam
	} else if(cmd=="hi") {
		player_pm(player,"javascript rocks");
	} else if(cmd=="list") {
		idx = 3;
		player_pm(player, "Configstring "+idx+": "+getconfigstring(idx)+"");
	} else if(cmd=="q") {
		org = ent_getorigin(player);
		ent = spawn("script_model", org);
		print("spawn done\n");
		m = x_modelindex("xmodel/airborne");
		print("model index = "+m+"\n");
		ent_setint(ent, 373, m, 8);
		print("ent m8 = "+ent+"\n");
		//dobjupdate(ent); //unsafe to use yet
		linkentity(ent);
		return true;
	} else if(cmd=="iamphp") {
		org = ent_getorigin(player);
		print("org = "+org.x+","+org.y+","+org.z+"\n");
		org.z+=100;
		//ent_setorigin(player, org); //unsafe yet
	} else if(cmd=="ent") {
		ent_num = parseInt(Cmd_Argv(1));
		offset = parseInt(Cmd_Argv(2));
		value = ent_getint(ent_num, offset);
		player_pm("ent offset "+offset+" = "+value+"");
		return true;
	} else if(cmd="geef_mij_health") {
		org = ent_getorigin(player);
		org.z += 100; //zodat t uit de lucht valt
		ent = spawn("item_health", org);
		return true; //zodat je geen 'unknown command' krijgt
	}
	
	return false; //als je true returned dan word de originele functie overschreden, e.g "say" dan kun je iemand muten
}

print("parsed: "+parseInt("123")+"\n");
print("reloaded\n");
