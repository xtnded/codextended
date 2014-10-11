InteractMonitor() {
	level.interact = [];
	while(!level.mapended) {
		intr = getentarray("script_model", "classname");
		for(i = 0; i < intr.size; i++) {
			for(k = intr.size; k > 0; k--) {
				if(distance(intr[i].origin, intr[k].origin) < 40 && !intr[k] is("moving") && !intr[i] is("moving")) {
					intr[k] thread pushEntity(intr[i]);
				}
				players = std::players();
				for(j = 0; j < players.size; j++) {
					if(intr[i] is("moving"))
						continue;
					if(distance(intr[i].origin, players[j].origin) < 100) {
						players[j] thread pushEntity(intr[i]);
					}
				}
			}
		}
		wait .05;
	}
}

setupEvents() {
	self.event = [];
}

is(e) {
	return isDefined(self.event[e]);
}

addEvent(e) {
	self.event[e] = true;
}

killEvent(e) {
	self.event[e] = undefined;
}

pushEntity(e) {
	dir = vectortoangles(e.origin - self.origin);
	fw = 10;
	mv = e.origin + math::vectorScale(anglestoforward(dir), fw);
	endl = std::trap_trace(e.origin, mv, e.mins, e.maxs, e num(), -1);
	if(endl != e.origin) {
		//time = distance(e.origin,endl)/190; //uhh not sure
		time = .1;
		e addEvent("moving");
		e moveTo(endl,time);
		wait time;
		e killEvent("moving");
	}
}

spawnHittableAI(org, model) {
	e = spawn("script_model", org);
	e setupEvents();
	e setXModel("barrel_grey");
	//e setSize(30, 70);
	e.mins = (-15,-15,0);
	e.maxs = (15,15,50);
	e setSize(15,50);
	e setInteractable();
	e setHealth(200);
	e setTakeDamage(1);
	e setContents(level.CONTENTS_BODY);
	e thread callbackDamage();
	return e;
}

callbackAnimation() {
	while(entityAlive(self)) {
		for(i = 0; i < self.frames; i++) {
			if(entityAlive(self)) {
				self setXModel(self.animation + "" + i);
				wait self.fps;
			} else {
				return;
			}
		}
	}
}

getGameFrameTime() {
	//getcvar("sv_fps"); //todo
}

getAnimationBase() {
	return ""; //todo
}

spawnAnimatedEntity(org, animbase, fps, frames) {
	e = spawn("script_model", org);
	e setupEvents();
	s setXModel(animbase+"0");
	e.mins = (-40,-40,0);
	e.maxs = (40,40,60);
	//e.maxs = (0,0,0);
	//e.mins = (0,0,0);
	e setSize(40,60);
	e setInteractable();
	e setHealth(100);
	e setTakeDamage(1);
	e setContents(level.CONTENTS_BODY);
	e thread callbackDamage();
	e.fps = fps;
	e.animation = animbase;
	e.frames = frames;
	e thread callbackAnimation();
	return e;
}

setInteractable() {
	level.interact[level.interact.size] = self;
	self.interactive = true;
}

//Need to implement entity callbackDamage into the game engine to bypassing the CoDScript's maximum amount of threads called
callbackDamage() {
	if(!isDefined(self.damage))
		self.damage = [];
	while(entityAlive(self)) {
		self waittill("damage", damage, e);
		/*hitloc = e.origin;
		if(isplayer(e))
		hitloc = e geteye() + (0,0,20);
		dir = vectortoangles(self.origin - hitloc);
		dirn = vectortoangles(self.origin-e.origin);
		pow = damage/2;
		mv = self.origin + math::vectorScale(anglestoforward(dirn), pow);
		endl = std::trap_trace(self.origin, mv, self.mins, self.maxs, self num(), -1);
		if(endl != self.origin) {
		time = distance(self.origin, endl)/(pow*5);
		angle = vectortoangles((self.origin + self.maxs) - endl);
		self rotateTo((angle[0],randomFloat(360),angle[2]), time);
		self moveTo(endl, time);
		}*/
		
	}
	if(isPlayer(e)) {
	e.score += 10;
	e iprintlnbold("^2+10");
	}
	ground = bullettrace(self.origin, self.origin + (0,0,-10000), true, self);
	angle = vectortoangles(self.origin-e.origin);
	t = distance(self.origin,e.origin)/10;
	self rotateTo(angle, .2);
	wait .2;
	self delete();
}

setHealth(health) {
	self.maxhealth = health;
	self.health = health;
}

isDead() {
	if(!isDefined(self))
		return true;
	if(!isDefined(self.health) && self.health <= 0)
		return true;
	return false;
}

entityAlive(e) {
	return (isDefined(e) && isDefined(e.health) && e.health >= 0);
}

setXModel(model) {
	self setModel("xmodel/" + model);
}

setBoundaries(w, h) {
	return closer(16, self num(), w, h);
}

setBounds(w, h) {
	return setBoundaries(w,h);
}

setSize(w, h) {
	return setBoundaries(w, h);
}

setPhysics(a) {
	return closer(17,self num(),a);
}

setTakeDamage(a) {
	return closer(19,self num(),a);
}

num() {
	return self getEntityNumber();
}

linkf() {
	return closer(23, self num());
}

unlinkf() {
	return closer(24, self num());
}
