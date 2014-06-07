abs(a) {
    if(a > 0)
        return a;
    return a*-1;
}

vectorscale(v, s) {
    return (v[0] * s, v[1] * s, v[2] * s);
}

vectordot(v1, v2) {
	return (v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2]);
}