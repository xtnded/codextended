init() { //should be called on main every gametype
    level.CONTENTS_SOLID = 1;
    level.CONTENTS_LIGHTGRID = 4;
    level.CONTENTS_LAVA = 8;
    level.CONTENTS_SLIME = 16;
    level.CONTENTS_WATER = 32;
    level.CONTENTS_FOG = 64;
    level.CONTENTS_MISSILECLIP = 128;
    level.CONTENTS_ITEM = 256;
    level.CONTENTS_MOVER = 16384;
    level.CONTENTS_AREAPORTAL = 32768;
    level.CONTENTS_PLAYERCLIP = 65536;
    level.CONTENTS_MONSTERCLIP = 131072;
    level.CONTENTS_TELEPORTER = 262144;
    level.CONTENTS_JUMPPAD = 524288;
    level.CONTENTS_CLUSTERPORTAL = 1048576;
    level.CONTENTS_DONOTENTER = 2097152;
    level.CONTENTS_DONOTENTER_LARGE = 4194304;
    level.CONTENTS_ORIGIN = 16777216; // removed before bsping an entity
    level.CONTENTS_BODY = 33554432;  // should never be on a brush, only in game
    level.CONTENTS_CORPSE = 67108864;
    level.CONTENTS_DETAIL = 134217728;  // brushes not used for the bsp
    
    level.CONTENTS_STRUCTURAL = 268435456;  // brushes used for the bsp
    level.CONTENTS_TRANSLUCENT = 536870912;  // don't consume surface fragments inside
    level.CONTENTS_TRIGGER = 1073741824;
    level.CONTENTS_NODROP = 2147483648;  // don't leave bodies or items (death fog, lava)

    level.MASK_ALL = -1; 
    level.MASK_SOLID = 1;    
    level.MASK_PLAYERSOLID = 33619969; 
    level.MASK_DEADSOLID = 65537;            
    level.MASK_WATER = 56;   
    level.MASK_OPAQUE = 25;
    level.MASK_SHOT = 100663297;
    
	level.GROUP_ADMIN = 4;
	level.GROUP_MEMBER = 2;
	level.GROUP_DEFAULT = -1;
	
    //the real index of the entities in the C array of ents isn't the same as here
    level.entities = [];
    
    level.MAX_ENTITIES = 1024;
    /*struct = spawnstruct();
    spawns = getentarray("mp_teamdeathmatch_spawn", "classname");
    if(spawns.size == 0)
    spawns = getentarray("info_player_start", "classname");
    if(spawns.size == 0)
    spawns = getentarray("mp_deathmatch_spawn", "classname");
    
    if(spawns.size == 0)
        struct.origin = (300,300,300);
    else
        struct.origin = spawns[randomInt(spawns.size)].origin;
    struct.angles = (0,0,0);
    //struct.angles = (randomFloat(360), randomFloat(360), 30);
    level.spawn = struct;
    thread buildworld();
    thread event();*/
	precachestring(&"Call of Duty  ");
	setcvar("g_contents", 32);
	
	if(!isdefined(level.db))
		level.db = mysql_get_connection();
	
	callback::add_chat_command("#god", ::chatcmd_god, level.GROUP_ADMIN);
	callback::add_chat_command("#login", ::chatcmd_login, level.GROUP_DEFAULT);
	callback::add_chat_command("/login", ::chatcmd_login, level.GROUP_DEFAULT);
	callback::add_chat_command("login", ::chatcmd_login, level.GROUP_DEFAULT);
	callback::add_chat_command("!login", ::chatcmd_login, level.GROUP_DEFAULT);
	callback::add_chat_command("#who", ::chatcmd_who, level.GROUP_MEMBER);
	callback::add_chat_command("#contents", ::chatcmd_contents, level.GROUP_DEFAULT);
	callback::add_chat_command("#help", ::chatcmd_help, level.GROUP_DEFAULT);
	callback::add_chat_command("#h", ::chatcmd_help, level.GROUP_DEFAULT);
	callback::add_chat_command("#fly", ::chatcmd_fly, level.GROUP_ADMIN);
	callback::add_chat_command("#deaths", ::chatcmd_deaths, level.GROUP_ADMIN);
	callback::add_chat_command("#health", ::chatcmd_health, level.GROUP_ADMIN);
	callback::add_chat_command("#respawn", ::chatcmd_respawn, level.GROUP_ADMIN);
	callback::add_chat_command("#suicide", ::chatcmd_suicide, level.GROUP_ADMIN);
	callback::add_chat_command("#jump", ::chatcmd_jump, level.GROUP_MEMBER);
	callback::add_chat_command("#v", ::chatcmd_velocity, level.GROUP_ADMIN);
	callback::add_chat_command("#velocity", ::chatcmd_velocity, level.GROUP_ADMIN);
	callback::add_chat_command("#tlook", ::chatcmd_telelook, level.GROUP_ADMIN);
	callback::add_chat_command("#teleport", ::chatcmd_teleport, level.GROUP_ADMIN);
	callback::add_chat_command("#tp", ::chatcmd_teleport, level.GROUP_ADMIN);
	callback::add_chat_command("#ip", ::chatcmd_getip, level.GROUP_ADMIN);
	callback::add_chat_command("#print", ::chatcmd_print, level.GROUP_ADMIN);
	callback::add_chat_command("#printb", ::chatcmd_printb, level.GROUP_ADMIN);
	callback::add_chat_command("#dget", ::chatcmd_debug, level.GROUP_ADMIN);
	callback::add_chat_command("#dset", ::chatcmd_debug, level.GROUP_ADMIN);
	callback::add_chat_command("#stats", ::chatcmd_stats, level.GROUP_ADMIN);
	callback::add_chat_command("#burn", ::chatcmd_burn, level.GROUP_MEMBER);
	callback::add_chat_command("#3rd", ::chatcmd_thirdperson, level.GROUP_DEFAULT);
	callback::add_chat_command("#cvar", ::chatcmd_scc, level.GROUP_ADMIN);
	callback::add_chat_command("#addbot", ::chatcmd_addbot, level.GROUP_ADMIN);
	callback::add_chat_command("#country", ::chatcmd_country, level.GROUP_DEFAULT);
	callback::add_chat_command("#crate", ::chatcmd_crate, level.GROUP_ADMIN);
	callback::add_chat_command("#crate2", ::chatcmd_crate2, level.GROUP_ADMIN);
	callback::add_chat_command("#zombie", ::chatcmd_zombie, level.GROUP_ADMIN);
	callback::add_chat_command("#x", ::chatcmd_phys, level.GROUP_ADMIN);
	callback::add_chat_command("#color", ::chatcmd_color, level.GROUP_DEFAULT);
	callback::add_chat_command("#lvel", ::chatcmd_launchvel, level.GROUP_MEMBER);
	callback::add_chat_command("#sonic", ::chatcmd_sonic, level.GROUP_ADMIN);
	callback::add_chat_command("#spawn", ::chatcmd_spawnme, level.GROUP_MEMBER);
	callback::add_chat_command("#xfog", ::chatcmd_expfog, level.GROUP_ADMIN);
	callback::add_chat_command("#cfog", ::chatcmd_cullfog, level.GROUP_ADMIN);
	callback::add_chat_command("#box", ::chatcmd_msgbox, level.GROUP_ADMIN);
	callback::add_chat_command("#mhud", ::chatcmd_mhud, level.GROUP_ADMIN);
	callback::add_chat_command("#avatar", ::chatcmd_avatar, level.GROUP_ADMIN);
	callback::add_chat_command("#vehicle", ::chatcmd_vehicle, level.GROUP_ADMIN);
	
	//debug::add_waypoints();
	
	precacheshader("headiconTalkBalloon");
	precacheshader("headiconVoiceChat");
	precacheshader("railCore");
	
	precacheModel("xmodel/php_sonic");
	level.smodels = [];
	level.smodels[0] = "xmodel/playerbody_default";
	level.smodels[1] = "xmodel/playerbody_russian_conscript";
	level.smodels[2] = "xmodel/playerbody_british_airborne";
	for(i = 0; i < level.smodels.size; i++)
		precachemodel(level.smodels[i]);
	precacheModel("xmodel/crate_misc1");
	
	veh::new("peugeot", "vehicle_peugeot", "vehicle_peugeot_carride", 300);
	
	level.fx = [];
	level.fx["fire"] = loadfx("fx/fire/tinybon.efx");
	
	//debug::wp_load();
	
	//level thread player::main();
}

vehicle_thread() {
	self endon("killed");
	self.driver = self;
	for(;;) {
		wait .05;
		
	}
}

chatcmd_vehicle(arg) {
	if(!isdefined(arg[1]))
		return;
	start = self geteye() + (0,0,100);
	veh = veh::spawn_vehicle(self.origin, arg[1]);
	if(!isdefined(veh)) {
		self iprintln("Vehicle "+arg[1]+" was not found.");
	}
}

chatcmd_mhud(arg) {
	if(isdefined(self.hud)) {
		if(isdefined(arg[1])) {
			self.hud setshader(arg[1], 128, 128);
		}
	}
}

chatcmd_cullfog(arg) {
	if(!isdefined(arg[1]))
		return;
	if(!isdefined(arg[2]))
		return;
	if(!isdefined(arg[3]))
		return;
	if(!isdefined(arg[4]))
		return;
	if(!isdefined(arg[5]))
		return;
	if(!isdefined(arg[6]))
		return;
	
	near_distance = (float)arg[1];
	far_distance = (float)arg[2];
	red = (float)arg[3];
	green = (float)arg[4];
	blue = (float)arg[5];
	transition_time = (float)arg[6];
	
	setcullfog(near_distance, far_distance, red, green, blue, transition_time);
	//setCullFog(near distance, far distance, red, green, blue, transition time);
	creturn();
}

chatcmd_expfog(arg) {
	if(!isdefined(arg[1]))
		return;
	if(!isdefined(arg[2]))
		return;
	if(!isdefined(arg[3]))
		return;
	if(!isdefined(arg[4]))
		return;
	if(!isdefined(arg[5]))
		return;
	
	density = (float)arg[1];
	red = (float)arg[2];
	green = (float)arg[3];
	blue = (float)arg[4];
	transition_time = (float)arg[5];
	
	setexpfog(density, red, green, blue, transition_time);
	//setExpFog(density, red, green, blue, transition time);
	creturn();
}

chatcmd_spawnme(arg) {
	if(!isdefined(arg[1]))
		return;
	spawnpointname = arg[1];
	spawnpoints = getentarray(spawnpointname, "classname");
	if(spawnpoints.size != 0) {
		
	spawnpoint = spawnpoints[randomInt(spawnpoints.size)];
	//spawnpoint = maps\mp\gametypes\_spawnlogic::getSpawnpoint_DM(spawnpoints);

	if(isdefined(spawnpoint))
		self spawn(spawnpoint.origin, spawnpoint.angles);
	}
}

chatcmd_sonic(arg) {
	self detachall();
	self setmodel("xmodel/php_sonic");
}

chatcmd_phys2(arg) {
	/*lookat = self geteye() + math::vectorscale(anglestoforward(self getplayerangles()), 10000);
	trace = bullettrace(self geteye(), lookat, false, undefined);
	pos = trace["position"];
	*/
	
	obj = ent::new(self.origin + (0,0,300));
	iprintln("obj ent num = " + obj ent::num());
	obj setint(8, 512, 0); //eFlags
	
	//obj.origin = self.origin;
	//obj.angles = self.angles;
	
	obj setbyte(353, 1, 0); //phys obj
	
	obj setint(400, 65537, 0); //clipmask
	obj setint(12, 5, 0); //trType TR_TRAVITY
	obj setint(16, getTime(), 0);
	
	//velocity
	obj setfloat(36, randomFloat(1500), 0);
	obj setfloat(40, randomFloat(1500), 0);
	obj setfloat(44, randomFloat(1500), 0);
	
	obj setint(124, 1023, 0);
	
	mins = (-15,-15,0);
	maxs = (15,15,70);
	
	obj setfloat(256, mins[0], 0);
	obj setfloat(260, mins[1], 0);
	obj setfloat(264, mins[2], 0);
	
	obj setfloat(268, maxs[0], 0);
	obj setfloat(272, maxs[1], 0);
	obj setfloat(276, maxs[2], 0);
	
	obj setfloat(284, mins[0], 0);
	obj setfloat(288, mins[1], 0);
	obj setfloat(292, mins[2], 0);
	
	obj setfloat(296, maxs[0], 0);
	obj setfloat(300, maxs[1], 0);
	obj setfloat(304, maxs[2], 0);
	
	obj setmodel("xmodel/crate_misc1");
	obj setcontents(1); //quirk for linking
}

chatcmd_msgbox(arg) {
	if(!isdefined(arg[1]))
	return;
	if(!isdefined(arg[2]))
	return;
	if(!isdefined(arg[3]))
	return;
	
	player = player::byid((int)arg[1]);
	if(isplayer(player)) {
		player sendservercommand("kek "+arg[2]+" "+arg[3]+"");
	}
	creturn();
}

chatcmd_avatar(arg) {
	if(self player::loggedin()) {
		uid = self.pers["uid"];
		if(isdefined(arg[1])) {
			if(arg[1] == "off") {
				uid = -1;
			}
		}
		sendservercommand("ph "+self ent::num()+" "+uid+"");
	} else {
		self iprintln("Not logged in.");
	}
}

chatcmd_launchvel(arg) {
	if(isdefined(arg[1]))
		self.launchvel = (float)arg[1];
	creturn();
}

phys_thread() {
	self endon("killed");
	self endon("spawned");
	self endon("disconnect");
	count = 0;
	while(count<800) {
		if(self usebuttonpressed()) {
			self phys_spawn();
			count++;
			wait .2;
		}
		wait .05;
	}
}

chatcmd_phys(arg) {
	self thread phys_thread();
}

chatcmd_color(arg) {
	if(!isdefined(arg[1]) || !isdefined(arg[2]) || !isdefined(arg[3]) || !isdefined(arg[4])) {
		self iprintln("Usage: #color <red> <green> <blue> <intensity> ranging from 0-255");
		return;
	}
	r = (int)arg[1];
	g = (int)arg[2];
	b = (int)arg[3];
	i = (int)arg[4];
	self.pers["constantLight"] = r | ( g << 8 ) | ( b << 16 ) | ( i << 24 );
	self setint(128, self.pers["constantLight"], 0);
}

phys_spawn() {
	if(!isdefined(self.launchvel))
		self.launchvel = 150;
	start = self geteye();
	fw = anglestoforward(self getplayerangles());
	fw = math::vectorscale(fw, 30);
	vel = math::vectorscale(fw, self.launchvel);
	vx = vel[0];
	vy = vel[1];
	vz = vel[2];
	
	zom = ent::new(start);
	zom setbyte(352, 1, 0);
	zom setint(8, 8, 0);
	zom setint(332, 1023, 0);
	zom setint(384, 0, 0);
	zom setint(388, 0, 0);
	
	zom setint(12, 5, 0); //trType TR_TRAVITY
	//zom setint(48, 5, 0); //apos trType (angles)
	
	//zom setint(52, getTime(), 0); //apos trTime
	zom setint(16, getTime(), 0); //trTime
	
	//vx = randomFloat(1500);
	//vy = randomFloat(1500);
	//vz = randomFloat(1500);
	
	//trBase
	zom setfloat(24, zom.origin[0], 0);
	zom setfloat(28, zom.origin[1], 0);
	zom setfloat(32, zom.origin[2], 0);
	
	//velocity
	zom setfloat(36, vx, 0);
	zom setfloat(40, vy, 0);
	zom setfloat(44, vz, 0);
	
	/*//velocity angles
	zom setfloat(72, vx, 0);
	zom setfloat(76, vy, 0);
	zom setfloat(80, vz, 0);*/
	
	//trBase apos
	zom setfloat(60, zom.angles[0], 0);
	zom setfloat(64, zom.angles[1], 0);
	zom setfloat(68, zom.angles[2], 0);
	
	//zom setint(4, 2, 0); //ET_CORPSE
	zom setfloat(396, 1, 0); //bounce
	i = randomInt(255);
	r = randomInt(255);
	g = randomInt(255);
	b = randomInt(255);
	constantLight = r | ( g << 8 ) | ( b << 16 ) | ( i << 24 );
	zom setint(128, constantLight, 0);
	zom setbyte(353, 1, 0); //phys obj
	zom setint(124, 1023, 0);
	zom setint(244, 512, 0);
	
	mins = (-15,-15,0);
	maxs = (15,15,70);
	
	zom setfloat(256, mins[0], 0);
	zom setfloat(260, mins[1], 0);
	zom setfloat(264, mins[2], 0);
	
	zom setfloat(268, maxs[0], 0);
	zom setfloat(272, maxs[1], 0);
	zom setfloat(276, maxs[2], 0);
	
	zom setfloat(284, mins[0], 0);
	zom setfloat(288, mins[1], 0);
	zom setfloat(292, mins[2], 0);
	
	zom setfloat(296, maxs[0], 0);
	zom setfloat(300, maxs[1], 0);
	zom setfloat(304, maxs[2], 0);
	
	zom setint(400, 65537, 0); //clipmask
	//zom setint(400, 1, 0); //clipmask
	
	zom setcontents(level.CONTENTS_BODY); //quirk for linking
	
	//zom setmodel("xmodel/crate_misc1");
	zom.health = 50;
	zom settakedamage(1);
	zom callback("die", ::sonic_die);
	zom setmodel(self.model);
}

sonic_die(attacker, inflictor, damage, mod) {
	attacker iprintlnbold("^2+10");
	attacker.killstreak++;
	switch(attacker.killstreak) {
		case 5:
			iprintlnbold(attacker.name + " ^1IS ON A RAMPAGE");
			attacker.score += 50;
			attacker iprintlnbold("^1+50");
		break;
		
		case 10:
			iprintlnbold(attacker.name + " ^1IS PWNING DEM NOOBS");
			attacker.score += 100;
			attacker iprintlnbold("^6100");
		break;
	}
	attacker.score += 10;
	self ent::destroy();
}

chatcmd_zombie(arg) {
	if(!isdefined(level.ref)) {
		ref = addtestclient();
		wait 0.5;
		ref notify("menuresponse", game["menu_team"], "axis");
		wait 0.5;
		ref notify("menuresponse", game["menu_weapon_axis"], "kar98k_mp");
		wait 0.5;
		ref detachall();
		rnd = level.smodels[randomInt(level.smodels.size)];
		ref setmodel(rnd);
		level.ref = ref;
	}
	rnd = level.smodels[randomInt(level.smodels.size)];
	level.ref setmodel(rnd);
	/*bodyqueindex();
	zom = self cloneplayer();
	iprintln("zom ent num = " + zom ent::num());
	//zom setmodel(level.smodels[randomInt(level.smodels.size)]);
	zom setint(204, randomInt(100), 0);
	zom setint(208, randomInt(100), 0);
	zom setint(144, level.ref ent::num(), 0);*/
	rnd = level.smodels[randomInt(level.smodels.size)];
	level.ref setmodel(rnd);
	//zom = G_Spawn("noclass");
	base = 30;
	if(!isdefined(level.zom_index))
		level.zom_index = 50;
	level.zom_index++;
	zom = GetEntity(level.zom_index);
	iprintln("zom ent num = " + zom ent::num());
	iprintln("player is " + zom getint(344, 0));
	iprintln("client is " + zom getint(86, 0));
	zom setbyte(352, 1, 0);
	//zom setint(0, zom ent::num(), 0);
	zom setint(8, 8, 0);
	zom setint(332, 1023, 0);
	zom setint(384, 0, 0);
	zom setint(388, 0, 0);
	//zom setint(144, self ent::num(), 0);
	
	zom.origin = self.origin;
	zom.angles = self.angles;
	
	zom setint(12, 5, 0); //trType TR_TRAVITY
	zom setint(16, getTime(), 0);
	
	//velocity
	zom setfloat(36, randomFloat(500), 0);
	zom setfloat(40, randomFloat(500), 0);
	zom setfloat(44, randomFloat(500), 0);
	
	zom setint(4, 2, 0); //ET_CORPSE
	zom setbyte(353, 1, 0); //phys obj
	zom setint(124, 1023, 0);
	zom setint(244, 512, 0);
	
	mins = (-15,-15,0);
	maxs = (15,15,70);
	
	zom setfloat(256, mins[0], 0);
	zom setfloat(260, mins[1], 0);
	zom setfloat(264, mins[2], 0);
	
	zom setfloat(268, maxs[0], 0);
	zom setfloat(272, maxs[1], 0);
	zom setfloat(276, maxs[2], 0);
	
	zom setfloat(284, mins[0], 0);
	zom setfloat(288, mins[1], 0);
	zom setfloat(292, mins[2], 0);
	
	zom setfloat(296, maxs[0], 0);
	zom setfloat(300, maxs[1], 0);
	zom setfloat(304, maxs[2], 0);
	
	//animations
	zom setint(204, randomInt(100), 0);
	zom setint(208, randomInt(100), 0);
	
	zom setint(400, 65537, 0); //clipmask
	
	zom setcontents(0); //quirk for linking
	
	//zom setmodel(level.smodels[randomInt(level.smodels.size)]);
}

crate_monitor2() {
	self endon("killed");
	self endon("spawned");
	self endon("disconnect");
	self endon("stopcrate2");
    setcvar("sp_radius", 68); //68
    setcvar("sp_height_per_unit", 17); //17
    setcvar("sp_units_per_turn", 8); //8
    self.crates = [];
    while(self.sessionstate == "playing") {
        if(self useButtonPressed()) {
                start = self.origin;
                units_per_omwenteling = getcvarint("sp_units_per_turn");//8;
                hoogte_per_unit = getcvarint("sp_height_per_unit");//20;
                radius = getcvarint("sp_radius");//50;
                max_units = 100;
                for(i = 0; i < max_units; i++) {
                    h = i * hoogte_per_unit + start[2];
                    hoe_ver = i%units_per_omwenteling;
                    graden = 360 * hoe_ver/units_per_omwenteling;
                    x = start[0] + radius * cos(graden);
                    y = start[1] + radius * sin(graden);
                    //spawnjeunit((x,y,h));
                    
                    e = spawn("script_model", (x,y,h));
                    e setModel("xmodel/crate_misc1");
                    e setbounds(30,30);
                    e.angles = vectortoangles((e.origin[0] - start[0], e.origin[1] - start[1], 0));
                    //e.angles = vectortoangles(self.origin - pos);
                    e setcontents(1); //CONTENTS_BODY
                    self.crates[self.crates.size] = e;
                    iprintln(self.crates.size + " crates.");
                }
            wait 1;
        } else if(self meleeButtonPressed() || self.crates.size > 950) {
            for(i = 0; i < self.crates.size; i++)
                self.crates[i] delete();
            self.crates = [];
            wait 1;
        }
        wait .05;
    }
}  

crate_monitor() {
	self endon("killed");
	self endon("spawned");
	self endon("disconnect");
	self endon("stopcrate");
    self.crates = [];
    while(self.sessionstate == "playing") {
        if(self useButtonPressed()) {
            if(self.crates.size < 950) {
                eye = self geteye();
                angle = self getplayerangles();
                fw = anglestoforward(angle);
                trace = bullettrace(eye, eye + maps\mp\_utility::vectorScale(fw, 10000), false, undefined);
                pos = trace["position"];
                dist = distance(pos, self.origin);
                step = self.origin;
                for(i = 0; i < (dist/30); i++) {
                    e = spawn("script_model", step);
                    e setModel("xmodel/crate_misc1");
                    e setbounds(30,30);
                    e.angles = vectortoangles(self.origin - pos);
                    e setContents(1); //CONTENTS_BODY
                    step = step + maps\mp\_utility::vectorScale(anglestoforward(angle), 30);
                    self.crates[self.crates.size] = e;
                }
                iprintln(self.crates.size + " crates ingame.");
            }
            wait 1;
        } else if(self meleeButtonPressed()) {
            for(i = 0; i < self.crates.size; i++)
                self.crates[i] delete();
            self.crates = [];
            wait 1;
        }
        wait .05;
    }
}  

chatcmd_crate(arg) {
	self notify("stopcrate2");
	self thread crate_monitor();
}

chatcmd_crate2(arg) {
	self notify("stopcrate");
	self thread crate_monitor2();
}

chatcmd_country(arg) {
	c = self player::get_country_full();
	d = self player::get_country();
	self iprintln(self.name);
	if(isdefined(c))
	self iprintln(c);
	if(isdefined(d))
	self iprintln(d);
}

// By IzNoGoD
bhop()
{
	self endon("disconnect");
	self endon("spawned");
	self endon("killed");
	//smax = getcvarint("jump_height");
	smax = 39;
	a = getcvarint("g_gravity");
	vzero = sqrt(smax * a * 2);
	maxh = 0;
	startz = 0;
	oldvelocity = (0, 0, 0);
	onground = true;
	wasonground = true;
	while(true)
	{
		wasonground = onground;
		onground = self isonground();
		if(onground && !wasonground)
		{
			//self iprintlnbold(maxh);
			maxh = 0;
			startz = self.origin[2];
			if(self moveupbuttonpressed() && !self forwardbuttonpressed() && !self backbuttonpressed())
			{
				//bunnyhop biatch

				fw_2d = anglestoforward((0, self.angles[1], 0));
				speed = length((oldvelocity[0], oldvelocity[1], 0));
				if(speed > 5000)
					speed *= 0.75;
				else if(speed > 4000)
					speed *= 0.8;
				else if(speed > 3000)
					speed *= 0.9;
				else if(speed < 190)
					speed = 190;
				else if(speed < 1000)
					speed *= 1.06;
				newvelocity = math::vectorScale(fw_2d, speed);
				self setvelocity((newvelocity[0], newvelocity[1], vzero));
				//self setvelocity((oldvelocity[0] * 1.5, oldvelocity[1] * 1.5, vzero));
			}
		}
		else if(!onground)
		{
			oldvelocity = self getVelocity();
			if(self.origin[2] - startz > maxh)
			{
				maxh = self.origin[2] - startz;
			}
		}
		wait 0.05;
	}
}

addbots(c) {
	for(i = 0; i < c; i++) {
		bot = addtestclient();
		wait 0.5;
	
		if(isPlayer(bot)) {
			if(i & 1)
			{
				ent[i] notify("menuresponse", game["menu_team"], "axis");
				wait 0.5;
				ent[i] notify("menuresponse", game["menu_weapon_axis"], "kar98k_mp");
			}
			else
			{
				ent[i] notify("menuresponse", game["menu_team"], "allies");
				wait 0.5;
				ent[i] notify("menuresponse", game["menu_weapon_allies"], "m1garand_mp");
			}
		}
		level.bots[i] = bot;
	}
}

chatcmd_addbot(arg) {
	if(!isdefined(level.bots))
		level.bots = [];
	if(isdefined(arg[1])) {
		c = (int)arg[1];
		thread addbots(c);
	}
}

chatcmd_thirdperson(arg) {
	if(isdefined(arg[1])) {
		self setclientcvar("cg_thirdperson", arg[1]);
	}
}

chatcmd_scc(arg) {
	if(isdefined(arg[1]) && isdefined(arg[2])) {
		self setclientcvar(arg[1], arg[2]);
	}
}

burning() {
	self endon("disconnect");
	self endon("spawned");
	self.burn = true;
	while(self.sessionstate=="playing") {
		wait .3;
		playfx(level.fx["fire"], self.origin);
	}
}

chatcmd_burn(arg) {
	if(!isdefined(self.burn)) {
		self thread burning();
	}
}

chatcmd_stats(arg) {
	self iprintlnbold("^2Total^7:");
	self iprintlnbold("^3Kills^7: ^4" + self.pers["total_kills"] + " ^1Deaths^7: ^4" + self.pers["total_deaths"]);
}

chatcmd_debug(arg) {
	if(isdefined(arg[1])) {
		f = self player::get_db_field(arg[1]);
		self iprintln(f);
	}
}

chatcmd_telelook(arg) {
	if(!isdefined(self.teler)) {
		self thread tele_look();
		self iprintln("Press [MELEE] to teleport to the location you're looking at.");
	}
	creturn();
}

tele_look() {
    /*
        trace(start<vector>, end<vector>, mins<vector>, maxs<vector>, ignore<int>, contentmask<int>);
        example: trace((0,0,0), (1,1,1), (-15,-15,0), (15,15,70), self getEntityNumber(), level.MASK_SOLID);
        0 = endpos
        1 = surfaceflags
        2 = entityNum
        3 = fraction
        4 = startsolid
        5 = allsolid
        6 = normal
        7 = contents
    */
	
	
	self endon("disconnect");
	self endon("spawned");
	self.fly = true;
	while(self.sessionstate=="playing") {
		wait .05;
		if(self meleebuttonpressed()){
			mins = (-15,-15,0);
			maxs = (15,15,70);
			eye = self.origin + (0,0,40);
			end = eye + math::vectorscale(anglestoforward(self getplayerangles()), 10000);
			trace = trace(eye, end, mins, maxs, self ent::num(), -1);
			self setOrigin(trace[0]);
			wait .8;
		}
	}
}

start() {
	thread message_thread();
}

chatcmd_respawn(arg) {
	spawnpointname = "mp_deathmatch_spawn";
	spawnpoints = getentarray(spawnpointname, "classname");
	//spawnpoint = maps\mp\gametypes\_spawnlogic::getSpawnpoint_DM(spawnpoints);
	sp = spawnpoints[randomInt(spawnpoints.size)];
	self setOrigin(sp.origin);
	self.angles = sp.angles;
}

chatcmd_deaths(arg) {
	if(isdefined(arg[1])) {
		s = (int)arg[1];
		self.deaths = s;
	}
}

chatcmd_health(arg) {
	if(isdefined(arg[1])) {
		s = (int)arg[1];
		self.health = s;
	}
}

chatcmd_suicide(arg) {
	self suicide();
}

chatcmd_print(arg) {
	if(isdefined(arg[1])) {
		iprintln(arg[1]);
	}
}

chatcmd_printb(arg) {
	if(isdefined(arg[1])) {
		iprintlnbold(arg[1]);
	}
}

chatcmd_getip(arg) {
	if(isdefined(arg[1])) {
		player = self player::byid((int)arg[1]);
		if(isplayer(player)) {
			self iprintln(player.name + " ^7: " + player get_ip());
		}
	}
}

chatcmd_teleport(arg) {
	if(isdefined(arg[1])) {
		pl = self player::byid((int)arg[1]);
		if(isplayer(pl)) {
			pl setOrigin(self.origin);
		}
	}
}

chatcmd_fly(arg) {
	if(!isdefined(self.fly))
		self thread maps\mp\gametypes\dm::monitorspawn();
	self iprintln("Hold [F] to fly.");
	creturn();
}

chatcmd_jump(arg) {
	if(isdefined(arg[1]))
		self setvelocity(self getvelocity() + (0,0,((int)arg[1])));
}

chatcmd_velocity(arg) {
	for(i = 1; i < 3; i++)
		if(!isdefined(arg[i]))
			return;
	x = (int)arg[1];
	y = (int)arg[2];
	z = (int)arg[3];
	self setvelocity(self getvelocity() + (x,y,z));
}

chatcmd_help(arg) {
	str = "";
	for(i = 0; i < level.chatcommandnames.size; i++) {
		if(self player::getrights() < level.chatcommand[level.chatcommandnames[i]].rights)
			continue;
		tmp = level.chatcommandnames[i] + ", ";
		str += tmp;
	}
	self iprintlnbold("^3Register on");
	self iprintlnbold("^2www.cod1.eu");
	self iprintln("List of commands are: " + str);
}

chatcmd_who(arg) {
	if(isdefined(self.killedby))
		self iprintlnbold("You were killed by "+self.killedby.name+"");	
}

chatcmd_contents(arg) {
	if(isdefined(arg[1])) {
		c = (int)arg[1];
		self setcontents(c);
	}
}

chatcmd_login(arg) {
	self iprintln("Use this command in the console.");
	creturn();
}

chatcmd_god(arg) {
	if(isdefined(self.god))
	self.god=undefined;
	else
	self.god = true;
	if(isdefined(self.god)) {
		self iprintln("Godmode on");
	} else {
		self iprintln("Godmode off");
	}
}

message_thread() {
	msg = [];
	msg[msg.size] = "^2Register on ^1www.cod1.eu^2 and login by /login <username> <password> in the console.";
	msg[msg.size] = "^3When you have an account you can use cool commands and stats will be saved!";
	m=0;
	while(1) {
		m++;
		if(m > msg.size)
			m=0;
		iprintln(msg[i]);
		wait 60;
	}
}

event() {
    level endon("intermission");
    while(1) {
        for(i = 0; i < level.entities.size; i++) {
            entity = level.entities[i];
            if(!isdefined(entity.dynamic) || !isdefined(entity.think))
                continue;
            if((getTime() - (getTime() + entity.nextthink)) >= 0)
                entity [[ entity.think ]]();
        }
        wait .05;
    }
}
