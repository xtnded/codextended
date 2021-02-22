// Call of Duty Extended Callback

/*
    All the client their commands entered into the console or e.g via chatting which are sent to the server can be parsed here.
    When a player chats into the chat, then for example a ClientCommand would be sent to the server with say as the first argument.
    
    clientNum = player/client's number
    self (object) = player entity
*/

ip_login() {
	if(self player::loggedin())
		return;
	ip = mysql_real_escape_string(level.db, self get_ip());
	q = mysql_query(level.db, "SELECT * FROM mybb_users WHERE lastip='"+ip+"'");
	if(q!=0) {
		return;
	}
	result = mysql_store_result(level.db);
	amount = mysql_num_rows(result);
	num = mysql_num_fields(result);
	if(amount==1) {
		if(num>0) {
			row = mysql_fetch_row(result);
			if(isdefined(row)) {
				self.pers["uid"] = (int)row[0];
				self.pers["usergroup"] = (int)row[10];
			}
		}
	}
	mysql_free_result(result);
}

CodeCallback_PlayerCommand(clientNum) {
    cmd = cmd_argvbuffer(0);
	if(isdefined(level.commandlistener)) {
    if(isDefined(level.commandlistener[cmd])) {
        [[ level.commandlistener[cmd] ]]();
        creturn();
        return;
    }
	}
	switch(cmd) {
		case "login":
		if(self player::loggedin()) {
			self iprintln("You are already logged in.");
			creturn();
			return;
		}
		if(cmd_argc() < 3 || !isdefined(level.db)) {
			self iprintln("Usage: /login <username> <password>");
			creturn();
			return;
		}
		
		username = Cmd_Argv(1);
		username = mysql_real_escape_string(level.db, username);
		password = Cmd_Argv(2);
		q = mysql_query(level.db, "SELECT * FROM mybb_users WHERE username='"+username+"' LIMIT 1");
		if(q!=0) {
			return;
		}
		result = mysql_store_result(level.db);
		num = mysql_num_fields(result);
		if(num != 0) {
			/*while(1) {
				row = mysql_fetch_row(result);
				if(!isdefined(row))
					break;
				for(i = 0; i < row.size; i++) {
					printconsole(row[i]+"\n");
				}
			}*/
			row = mysql_fetch_row(result);
			if(isdefined(row)) {
				hash = salt_password(password, row[3]);
				if(hash == row[2]) {
					self.pers["uid"] = (int)row[0];
					self.pers["usergroup"] = (int)row[10];
					iprintln(username + " ^2[^7"+self player::group_to_string()+"^2] ^5logged in.");
				}
			}
		}
		mysql_free_result(result);
		creturn();
		break;
		
		case "say":
		case "say_team":
			if(cmd_argc() < 2)
				return;
			chat = getchat(1);
			tok = std::strtok(chat, " ");
			if(!isdefined(tok[0]))
				return;
			chatcmd = tok[0];
			if(chat[0] != "!" && chat[0] != "/" && chat[0] != "#")
				return;
			if(isdefined(level.chatcommand[chatcmd])) {
				if(self player::getrights() < level.chatcommand[chatcmd].rights) {
					if(!self player::loggedin())
					self iprintln("^2You are not logged in, go to www.cod1.eu and make a account.");
					else
					self iprintln("^1You do not have access to this command.");
					return;
				}
				self [[ level.chatcommand[chatcmd].call ]](tok);
			} else {
				self iprintln("^5This command does not exist. Use #help for a list of commands.");
			}
		break;
		
		default:
			//iprintln(self.name + " tried command: " + cmd);
		break;
	}
}

add_chat_command(cmd, call, r) {
	if(!isdefined(level.chatcommand)) {
		level.chatcommand = [];
		level.chatcommandsize = 0;
		level.chatcommandnames=[];
	}
    level.chatcommand[cmd] = spawnstruct();
	level.chatcommand[cmd].call = call;
	level.chatcommand[cmd].rights = r;
	level.chatcommandnames[level.chatcommandnames.size]=cmd;
    level.chatcommandsize++;
}

//By IzNoGoD
update_hud() {
	fov_horz = 90;
	fov_vert = 60;
	self endon("stop_hud");
	while(1) {
		wait .05;
		/*
			2d.x = (3d.x * fov) / 3d.z
			2d.y = (3d.y * fov) / 3d.z
		*/
		fw = anglestoforward(self getplayerangles());
		rg = anglestoright(self getplayerangles());
		up = anglestoup(self getplayerangles());
		vec = vectornormalize(self.hud.position - self.origin);
		/*if(vectordot(vec, fw) < 0) {
			self.hud.alpha = 0;
			continue;
		}*/
		horz = vectordot(vec, rg);
		vert = vectordot(vec, up);
		deg_horz = acos(horz);
		deg_vert = acos(vert);
		//printconsole("deg_horz = " + (deg_horz/fov_horz) + "\n");
		//printconsole("deg_vert = " + (deg_vert/fov_vert) + "\n");
		self.hud.x = 320 * (deg_horz / fov_horz); //top right is x = 0, y = 0 dacht ik, naar beneden/rechts is x++, y++
		self.hud.y = 240-(240 * (deg_vert / fov_vert)); //zou een minnetje bij kunnen... kweenie (-240*...
	}
}

addCommandListener(cmdname, xcmd_t) {
    if(!isDefined(level.commandlistener)) {
        level.commandlistener = [];
        level.commandlistenersize = 0;
    }
    level.commandlistener[cmdname] = xcmd_t;
    level.commandlistenersize++;
}

/*
======
ENTITY
======
*/

/*
	ONLY CALLED WHEN <ent> callback(<callback>, <function pointer>); is NOT set.
	
    Called when an non-player entity is damaged.
    This won't be called if you haven't set setTakeDamage on the entity.
        e.g entity setTakeDamage(1); and to turn off entity setTakeDamage(0);
*/

CodeCallback_EntityDamage(attacker, point, damage) {
}

CodeCallback_EntityKilled(attacker, inflictor, damage, mod) {
	self ent::destroy();
}

CodeCallback_EntityUse(activator) {
	
}

CodeCallback_EntityTouch() {

}

CodeCallback_EntityThink() {

}

/*================
================*/
callbackVoid()
{
}
