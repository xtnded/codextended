add_waypoints() {
    level.wp = [];
	level.waypoints = [];
    level.wpsize = 0;
    level.bots = [];
    callback::addCommandListener("wp_new", ::wp_new);
    callback::addCommandListener("wp_del", ::wp_delete);
    callback::addCommandListener("wp_save", ::wp_save);
    callback::addCommandListener("wp_load", ::wp_load);
    callback::addCommandListener("wp_tele", ::wp_tele);
    callback::addCommandListener("bot", ::spawnbot);
}

get_waypoint_file() {
	mapname = getcvar("mapname");
	if(mapname=="")
	mapname="default";
	return "/home/cod/wp/"+mapname+".xtndedwp";
}

load_waypoints(file) {
	level.waypoints = [];
	if(!fexists(file)) {
		printconsole(file+" does not exist.\n");
		return;
	}
	in=fopen(file,"r");
	if(-1!=in){
		len=fsize(file);
		data = fread(len,in);
		
		in_block = -1;
		lines = std::strtok(data, "\n");
		for(v=0;v<lines.size;v++) {
			line = lines[v];
			parts = std::strtok(line, ":");
			if(parts.size!=5)
				continue;
			id = (int)parts[0];
			org = std::strtok(parts[1], ",");
			position = ((float)org[0], (float)org[1], (float)org[2]);
			parent_index = (int)parts[2];
			type = parts[3];
			child_s = std::strtok(parts[4], ",");
			
			s = spawnstruct();
			s.origin = position;
			s.index = id;
			s.parentIndex = parent_index;
			s.type = type;
			s.childIndexes = [];
			for(g = 0; g < child_s.size; g++) {
				if(child_s[g] == "")
					continue;
				s.childIndexes[g] = (int)child_s[g];
			}
			wp_new(position);
			level.waypoints[id] = s;
		}
		fclose(in);
	}
	
	if(level.waypoints.size == 0) {
		printconsole("Failed to load waypoints.\n");
	} else {
		printconsole("Loaded "+level.waypoints.size+" waypoints.\n");
		for(i = 0; i < level.waypoints.size; i++) {
			level.waypoints[i].parentNode = level.waypoints[level.waypoints[i].parentIndex];
			level.waypoints[i].childs = [];
			for(j = 0; j < level.waypoints[i].childIndexes.size; j++)
				level.waypoints[i].childs[j] = level.waypoints[level.waypoints[i].childIndexes[j]];
		}
	}
}

find_path() {
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
	self endon("killed");
	maxs = (30,30,70);
	mins = (-30,-30,0);
	self.walktime = 0;
	self.walktimer = 0;
	while(1) {
		wait .05;
		speed = randomFloat(50) + 100;
		org = self.origin + (0,0,30);
		player = ent::get_nearby_player(self.origin);
		if(isplayer(player)) {
			org2 = player.origin + (0,0,30);
			distq = distancesquared(org,org2);
			if(distq < 30*30)
				continue;
			trace = trace(org, player.origin, mins, maxs, player ent::num(), 1);
			if(trace[3] != 1) {
				result = self.walktimer - getTime();
				//iprintln("result = " + result);
				//if(result > 0)
					//continue;
				level.storeplayer = player;
				nodea = thread astar::generatePath(org2, org);
				if(isdefined(nodea)) {
					if(nodea.size>0) {
						//node = nodea[0];
						for(i = 0; i < nodea.size; i++) {
							node = nodea[i];
							time = distance(node.origin, self.origin)/speed;
							if(time >0) {
								self.walktime = time;
								self.walktimer = getTime() + time;
								self moveto(node.origin, time);
								self rotateto(vectortoangles(node.origin-org), .1);
								playfx(level.fx["fire"], node.origin);
								wait time;
							}
						}
					}
				}
			} else {
				self.walktimer = 0;
				time = distance(self.origin, player.origin) / speed;
				if(time>0) {
				self rotateto(vectortoangles(org2-org), .1);
				self moveto(player.origin, time);
				}
			}
		}
	}
}

spawnbot() {
	fw = math::vectorscale(anglestoforward(self getplayerangles()), 150);
    bot = ent::new(self.origin + fw);
    bot settakedamage(1);
    bot ent::set_health(100);
    //bot setcontents(level.CONTENTS_BODY);
    //bot setcontents(0);
    //bot setbounds(30, 70);
	bot detachall();
    bot setModel(self.model);
    bot thread find_path();
    level.bots[level.bots.size] = bot;
}

wp_tele() {
    if(!isDefined(level.waypoints))
        return;
    n = cmd_argvbuffer(1);
    if(n != "") {
        index = (int)n;
        if(index >= 0)
            self setOrigin(level.waypoints[index].origin);
    }
}

wp_load() {
	mapname = getcvar("mapname");
	if(mapname == "")
		mapname = "default";
    /*filename = cmd_argvbuffer(1);
    if(filename != "") {*/
		load_waypoints(get_waypoint_file());
    //}
}

wp_new(realorg) {
    org = self.origin;
	if(isdefined(realorg))
		org = realorg;
    if(level.wpsize > 0) {
		prev = level.wp[level.wpsize-1];
        mins = (-15,-15,0);
        maxs = (15,15,70);
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
		if(!isdefined(realorg)) {
        trace = trace(prev.origin + (0,0,maxs[2]), org + (0,0,maxs[2]), mins, maxs, self ent::num(), level.MASK_SOLID);
        if(trace[3] != 1) {
            self iprintln("^1Cannot see previous waypoint.");
            return;
        }
		}
    }
	ent = ent::new(org);
	ent ent::set_model("barrel_black1");
	ent setcontents(0);
	ent.childs = [];
	if(level.wpsize == 0)
		ent.parentIndex = 0;
	else
	{
		ent.parentIndex = level.wpsize-1;
		for(i = 1; i < level.wpsize; i++) {
			child_size = level.wp[level.wpsize-i].childs.size;
			level.wp[level.wpsize-i].childs[child_size] = level.wpsize;
		}
	}
	if(!isdefined(realorg))
    self iprintln("^2Placed a waypoint at ^7" + self.origin);
    level.wp[level.wpsize] = ent;
    level.wpsize++;
}

wp_delete() {
	level.wp[level.wpsize] ent::destroy();
	level.wp[level.wpsize] = undefined;
    self iprintln("^1Deleted waypoint ^7" + level.wpsize);
    if(level.wpsize > 0)
		level.wpsize--;
}

out(txt) {
    out = "/home/cod/debug.txt";
    file = fopen(out, "a+");
    if(-1!=file) {
        fwrite(txt, file);
        fclose(file);
    }
}

wp_save() {
	mapname = getcvar("mapname");
	if(mapname == "")
		mapname = "default";
    out = "/home/cod/wp/"+mapname+".xtndedwp";
    file = fopen(out, "w");
    if(-1!=file) {
        for(i = 0; i < level.wpsize; i++) {
            wp = level.wp[i];
            str = i+":"+wp.origin[0]+","+wp.origin[1]+","+wp.origin[2]+":"+wp.parentIndex+":default:";
			for(g=0;g<wp.childs.size;g++) {
				if(g!=wp.childs.size && g!=0)
					str += ",";
				str += wp.childs[g]+"";
			}
			str += "\n";
            fwrite(str, file);
        }
        self iprintln("^2Saved!");
        fclose(file);
    }
}

log(notes) {
    if(!isdefined(notes))
        notes = "";
    logc = "[LOG - "+getTime()+"] ";
    if(isdefined(self)) {
        if(isplayer(self))
            logc += "(Player "+self ent::num()+": "+self.name+") ";
        else
            logc += "(Entity "+self ent::num()+") ";
    }
    if(!isdefined(notes))
        notes = "(undefined)";
    logc += notes;
    printconsole(logc + "\n");
}