precache_model(model) {
    precacheModel("xmodel/" + model);
}

get_model(model) {
	return "xmodel/" + model;
}

strpos(a, b) {
	for(i=0;i<a.size;i++)
		if(substr(a,i,b.size) == b)
			return i;
	return -1;
}

_strpos(a, b) {
	len = 0;
	for(i = 0; i < a.size; i++) {
		for(j = 0; j < b.size; j++) {
			if(b[j] == a[i])
				len++;
			if(len == a.size)
				return (i-len);
		}
	}
	return -1;
}

substr(str, begin, end) {
	if(!isdefined(begin))
		return str;
	if(!isdefined(end))
		end = str.size;
	result = "";
	for(i=begin;i<end;i++)
		result+=str[i];
	return result;
}

StrTok( s, delimiter )
{
	j = 0;
	temparr[ j ] = "";	

	for ( i = 0; i < s.size; i++ )
	{
		if ( s[ i ] == delimiter )
		{
			j++;
			temparr[ j ] = "";
		}
		else
			temparr[ j ] += s[i];
	}
	return temparr;
}

in_array(array, search) {
    for(i = 0; i < array.size; i++)
        if(array[i] == search)
            return true;
    return false;
}

players() {
    return getentarray("player", "classname");
}

