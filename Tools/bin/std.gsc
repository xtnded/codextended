/*
	:: MDLL GSC //php
*/

players() {
	return getentarray("player", "classname");
}

num() {
	return self getEntityNumber();
}

getVelocity() {
	return closer(10,self num());
}

setVelocity(v) {
	return closer(11,self num(),v);
}

addVelocity(v) {
	return self setVelocity(self getVelocity() + v);
}

removeVelocity(v) {
	return self setVelocity(self getVelocity() - v);
}

getPlayerAngles() {
	return closer(12,self num());
}

jumpButtonPressed() {
	return closer(18,self num());
}

patchJump() {
	return closer(15,self num());
}

getCommand() {
	return closer(14,self num());
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

trap_trace(start, end, mins, maxs, ignore, mask) {
	return closer(3,start,end,mins,maxs,ignore,mask);
}

getChat() {
	return closer(20,self num());
}

file_read(a, b) {
	return closer(70, a, b);
}

file_write(a, b, c) {
	return closer(71, a, b, c);
}

file_exists(a) {
	return closer(72, a);
}

getPing() {
	return closer(22,self num());
}

getAim() {
	return closer(25, self num());
}

DropClient(r) {
	return closer(26, self num(), r);
}
