new(org) {
    if(!isDefined(org[1]) && org == 0)
        org = (0,0,0);
    e = spawn("script_model", org);
    e.dynamic = true;
    e.is = [];
    e.think = ::entity_think;
    e.nextthink = 0;
    level.entities[level.entities.size] = e;
    return e;
}

/*
    ENTITY
    THREADS
*/

is_on_ground() {
	i = self getint(124,0);
	if(i==-1)
		return false;
	return true;
}

entity_think() {
    
}

get_entitycount() {
    return getentarray().size;
}

destroy() {
	self notify("killed");
    self nextthink(0);
    self setTakeDamage(0);
    self delete();
}

set_model(model) {
    self setModel("xmodel/" + model);
}

set_health(health) {
    self.health = health;
}

num() {
    return self getentitynumber();
}

is_alive() {
    return (isDefined(self) && self.health > 0);
}

get_nearby_player(org) {
    players = std::players();
    for(i = 0; i < players.size; i++)
        if((!isDefined(c)) || (isDefined(c) && distancesquared(c.origin, org) > distancesquared(org, players[i].origin)))
            c=players[i];
    return c;
}

clear_events() {
    self.think = undefined;
    self.nextthink = 0;
}

event_move() {
    if(!isdefined(self.nextpos) || !isdefined(self.speed)) {
        self.is["moving"] = false;
        self clear_events();
        return;
    }
    d = 10;
    if(distancesquared(self.origin, self.nextpos) < d*d) {
        self.is["moving"] = false;
        self clear_events();
        return;
    }
    self.is["moving"] = true;
    st = self.speed*self.nextthink;
    angles = vectortoangles(self.nextpos - self.origin);
    step = self.origin + math::vectorscale(anglestoforward(angles), self.speed);
    time = distance(self.origin, step)/self.speed;
    self moveto(step, time);
}

move_to(org, speed) {
    self endon("stop_movement");
    s = distance(self.origin, org)/speed;
    self moveTo(org, s);
}

get_step(step) {
    st = self.origin + math::vectorscale(anglestoforward(self.angles), step);
    return st;
}

find() {
    self.pathfinding = true;
    while(self is_alive() && self.pathfinding) {
        wait .05;
        player = get_nearby_player(self.origin);
        if(isplayer(player)) {
            speed = 70 + randomFloat(50);
            playerdist = distanceSquared(player.origin, self.origin);
            width = 20;
            height = 70;
            if(playerdist > width*width) {
                //path = astar::generatePath (self, player, undefined);
                maxs = (width,width,height);
                width *= -1;
                mins = (width,width,0);
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
                p_org = player.origin;
                s_org = self.origin;
                trace = trace(s_org, end, mins, maxs, self ent::num(), -1);
                
                if(isDefined(self.yay) || isDefined(self.notfollow)) {
                    org = self.yay.origin;
                    self.yay = undefined;
                }
                trace = trace(self.origin, org, mins, maxs, self ent::num(), -1);
                end = trace[0];
                if(trace[3] != 1) {
                    if(!isDefined(self.yay))
                    self.yay = spawnstruct();
                    self.yay.angles = self.angles;
                    angles = (0, randomFloat(360), 0);
                    pos = self.origin + math::vectorscale(anglestoforward(angles), 100000);
                    self.yay.origin = pos;
                }
                
                dist = distance(end, self.origin);
                time = dist/speed;
                if(time != 0) {
                    self move_to(end, speed);
                    angle = vectortoangles(end - self.origin);
                    self rotateTo(angle, .1);
                    wait time/1.5;
                }
            }
        }
    }
}