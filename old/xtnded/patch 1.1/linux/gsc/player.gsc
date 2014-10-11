main() {
	precachestring(&"vec dot ");
	for(;;) {
		level waittill("spawn player", player);
		if(isplayer(player)) {
			//player thread update_hud();
		}
	}
}


//by IzNoGoD
update_hud() {
	fov_horz = 90;
	fov_vert = 60;
	self endon("killed");
	if(isdefined(self.hud))
		self.hud destroy();
	h = newclienthudelem(self);
	h.origin = self.origin;
	h.position = self.origin;
	h.x = 240;
	h.y = 320;
	h.color = (0,1,0);
	h.label = &"vec dot ";
	h.archived=false;
	self.hud =h;
	for(;;) {
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
		printconsole("deg_horz = " + (deg_horz/fov_horz) + "\n");
		printconsole("deg_vert = " + (deg_vert/fov_vert) + "\n");
		//self.hud.x = -320 * (deg_horz / fov_horz); //top right is x = 0, y = 0 dacht ik, naar beneden/rechts is x++, y++
		//self.hud.y = (-240 * (deg_vert / fov_vert)); //zou een minnetje bij kunnen... kweenie (-240*...
	}
}

thr_dot() {
	self endon("killed");
	if(isdefined(self.dotHUD))
		self.dotHUD destroy();
	if(isdefined(self.dotENT))
		self.dotENT ent::destroy();
	ent = ent::new(self.origin);
	ent ent::set_model("barrel_black1");
	ent setcontents(0);
	self.dotENT = ent;
	
	h = newclienthudelem(self);
	h.x = 240;
	h.y = 320;
	h.color = (0,1,0);
	h.fontscale = 2;
	h.archived = false;
	h.label = &"vec dot ";
	h setvalue(0);
	self.dotHUD = h;
	selectrad = 90;
	for(;;) {
		view = self.dotENT.origin;
		normalvec = vectorNormalize(view-self geteye());
		veccomp = vectorNormalize((view-(0,0,selectrad*2))-self geteye());
		insidedot = vectordot(normalvec,veccomp);
		
		anglevec = anglestoforward(self getplayerangles());
		vectordot = vectordot(anglevec,normalvec);
		self.dotHUD setvalue(insidedot);
		wait .05;
	}
}

angletotrace(range, track_players) {
    start = self geteye();
    fw = start + math::vectorscale(anglestoforward(self getplayerangles()), range);
    trace = bullettrace(start, fw, track_players, self);
    return trace;
}

is_playing() {
	return (isdefined(self.sessionstate) && self.sessionstate == "playing");
}

//quick addon by php

/*
got little list from
http://fasteri.com/list/2/short-names-of-countries-and-iso-3166-codes
with little javascript 

text = "";
for(i = 3; i < a.childNodes.length; i++) {
fullname=a.childNodes[i].childNodes[3].innerHTML;
shortname = a.childNodes[i].childNodes[1].childNodes[0].text;
text += "list['"+shortname+"'] = '"+fullname+"';";
}
*/


get_country_full() {
	if(!isdefined(level.countries)) {
		list = [];
		list["AD"] = "Andorra";
		list["AE"] = "United Arab Emirates";
		list["AF"] = "Afghanistan";
		list["AG"] = "Antigua and Barbuda";
		list["AI"] = "Anguilla";
		list["AL"] = "Albania";
		list["AM"] = "Armenia";
		list["AO"] = "Angola";
		list["AQ"] = "Antarctica";
		list["AR"] = "Argentina";
		list["AS"] = "American Samoa";
		list["AT"] = "Austria";
		list["AU"] = "Australia";
		list["AW"] = "Aruba";
		list["AX"] = "Åland Islands";
		list["AZ"] = "Azerbaijan";
		list["BA"] = "Bosnia and Herzegovina";
		list["BB"] = "Barbados";
		list["BD"] = "Bangladesh";
		list["BE"] = "Belgium";
		list["BF"] = "Burkina Faso";
		list["BG"] = "Bulgaria";
		list["BH"] = "Bahrain";
		list["BI"] = "Burundi";
		list["BJ"] = "Benin";
		list["BL"] = "Saint Barthélemy";
		list["BM"] = "Bermuda";
		list["BN"] = "Brunei Darussalam";
		list["BO"] = "Bolivia, Plurinational State of";
		list["BQ"] = "Bonaire, Sint Eustatius and Saba";
		list["BR"] = "Brazil";
		list["BS"] = "Bahamas";
		list["BT"] = "Bhutan";
		list["BV"] = "Bouvet Island";
		list["BW"] = "Botswana";
		list["BY"] = "Belarus";
		list["BZ"] = "Belize";
		list["CA"] = "Canada";
		list["CC"] = "Cocos (Keeling) Islands";
		list["CD"] = "Congo, the Democratic Republic of the";
		list["CF"] = "Central African Republic";
		list["CG"] = "Congo";
		list["CH"] = "Switzerland";
		list["CI"] = "Côte d'Ivoire";
		list["CK"] = "Cook Islands";
		list["CL"] = "Chile";
		list["CM"] = "Cameroon";
		list["CN"] = "China";
		list["CO"] = "Colombia";
		list["CR"] = "Costa Rica";
		list["CU"] = "Cuba";
		list["CV"] = "Cape Verde";
		list["CW"] = "Curaçao";
		list["CX"] = "Christmas Island";
		list["CY"] = "Cyprus";
		list["CZ"] = "Czech Republic";
		list["DE"] = "Germany";
		list["DJ"] = "Djibouti";
		list["DK"] = "Denmark";
		list["DM"] = "Dominica";
		list["DO"] = "Dominican Republic";
		list["DZ"] = "Algeria";
		list["EC"] = "Ecuador";
		list["EE"] = "Estonia";
		list["EG"] = "Egypt";
		list["EH"] = "Western Sahara";
		list["ER"] = "Eritrea";
		list["ES"] = "Spain";
		list["ET"] = "Ethiopia";
		list["FI"] = "Finland";
		list["FJ"] = "Fiji";
		list["FK"] = "Falkland Islands (Malvinas)";
		list["FM"] = "Micronesia, Federated States of";
		list["FO"] = "Faroe Islands";
		list["FR"] = "France";
		list["GA"] = "Gabon";
		list["GB"] = "United Kingdom";
		list["GD"] = "Grenada";
		list["GE"] = "Georgia";
		list["GF"] = "French Guiana";
		list["GG"] = "Guernsey";
		list["GH"] = "Ghana";
		list["GI"] = "Gibraltar";
		list["GL"] = "Greenland";
		list["GM"] = "Gambia";
		list["GN"] = "Guinea";
		list["GP"] = "Guadeloupe";
		list["GQ"] = "Equatorial Guinea";
		list["GR"] = "Greece";
		list["GS"] = "South Georgia and the South Sandwich Islands";
		list["GT"] = "Guatemala";
		list["GU"] = "Guam";
		list["GW"] = "Guinea-Bissau";
		list["GY"] = "Guyana";
		list["HK"] = "Hong Kong";
		list["HM"] = "Heard Island and McDonald Islands";
		list["HN"] = "Honduras";
		list["HR"] = "Croatia";
		list["HT"] = "Haiti";
		list["HU"] = "Hungary";
		list["ID"] = "Indonesia";
		list["IE"] = "Ireland";
		list["IL"] = "Israel";
		list["IM"] = "Isle of Man";
		list["IN"] = "India";
		list["IO"] = "British Indian Ocean Territory";
		list["IQ"] = "Iraq";
		list["IR"] = "Iran, Islamic Republic of";
		list["IS"] = "Iceland";
		list["IT"] = "Italy";
		list["JE"] = "Jersey";
		list["JM"] = "Jamaica";
		list["JO"] = "Jordan";
		list["JP"] = "Japan";
		list["KE"] = "Kenya";
		list["KG"] = "Kyrgyzstan";
		list["KH"] = "Cambodia";
		list["KI"] = "Kiribati";
		list["KM"] = "Comoros";
		list["KN"] = "Saint Kitts and Nevis";
		list["KP"] = "Korea, Democratic People's Republic of";
		list["KR"] = "Korea, Republic of";
		list["KW"] = "Kuwait";
		list["KY"] = "Cayman Islands";
		list["KZ"] = "Kazakhstan";
		list["LA"] = "Lao People's Democratic Republic";
		list["LB"] = "Lebanon";
		list["LC"] = "Saint Lucia";
		list["LI"] = "Liechtenstein";
		list["LK"] = "Sri Lanka";
		list["LR"] = "Liberia";
		list["LS"] = "Lesotho";
		list["LT"] = "Lithuania";
		list["LU"] = "Luxembourg";
		list["LV"] = "Latvia";
		list["LY"] = "Libyan Arab Jamahiriya";
		list["MA"] = "Morocco";
		list["MC"] = "Monaco";
		list["MD"] = "Moldova, Republic of";
		list["ME"] = "Montenegro";
		list["MF"] = "Saint Martin (French part)";
		list["MG"] = "Madagascar";
		list["MH"] = "Marshall Islands";
		list["MK"] = "Macedonia, the former Yugoslav Republic of";
		list["ML"] = "Mali";
		list["MM"] = "Myanmar";
		list["MN"] = "Mongolia";
		list["MO"] = "Macao";
		list["MP"] = "Northern Mariana Islands";
		list["MQ"] = "Martinique";
		list["MR"] = "Mauritania";
		list["MS"] = "Montserrat";
		list["MT"] = "Malta";
		list["MU"] = "Mauritius";
		list["MV"] = "Maldives";
		list["MW"] = "Malawi";
		list["MX"] = "Mexico";
		list["MY"] = "Malaysia";
		list["MZ"] = "Mozambique";
		list["NA"] = "Namibia";
		list["NC"] = "New Caledonia";
		list["NE"] = "Niger";
		list["NF"] = "Norfolk Island";
		list["NG"] = "Nigeria";
		list["NI"] = "Nicaragua";
		list["NL"] = "Netherlands";
		list["NO"] = "Norway";
		list["NP"] = "Nepal";
		list["NR"] = "Nauru";
		list["NU"] = "Niue";
		list["NZ"] = "New Zealand";
		list["OM"] = "Oman";
		list["PA"] = "Panama";
		list["PE"] = "Peru";
		list["PF"] = "French Polynesia";
		list["PG"] = "Papua New Guinea";
		list["PH"] = "Philippines";
		list["PK"] = "Pakistan";
		list["PL"] = "Poland";
		list["PM"] = "Saint Pierre and Miquelon";
		list["PN"] = "Pitcairn";
		list["PR"] = "Puerto Rico";
		list["PS"] = "Palestinian Territory, Occupied";
		list["PT"] = "Portugal";
		list["PW"] = "Palau";
		list["PY"] = "Paraguay";
		list["QA"] = "Qatar";
		list["RE"] = "Réunion";
		list["RO"] = "Romania";
		list["RS"] = "Serbia";
		list["RU"] = "Russian Federation";
		list["RW"] = "Rwanda";
		list["SA"] = "Saudi Arabia";
		list["SB"] = "Solomon Islands";
		list["SC"] = "Seychelles";
		list["SD"] = "Sudan";
		list["SE"] = "Sweden";
		list["SG"] = "Singapore";
		list["SH"] = "Saint Helena, Ascension and Tristan da Cunha";
		list["SI"] = "Slovenia";
		list["SJ"] = "Svalbard and Jan Mayen";
		list["SK"] = "Slovakia";
		list["SL"] = "Sierra Leone";
		list["SM"] = "San Marino";
		list["SN"] = "Senegal";
		list["SO"] = "Somalia";
		list["SR"] = "Suriname";
		list["ST"] = "Sao Tome and Principe";
		list["SV"] = "El Salvador";
		list["SX"] = "Sint Maarten (Dutch part)";
		list["SY"] = "Syrian Arab Republic";
		list["SZ"] = "Swaziland";
		list["TC"] = "Turks and Caicos Islands";
		list["TD"] = "Chad";
		list["TF"] = "French Southern Territories";
		list["TG"] = "Togo";
		list["TH"] = "Thailand";
		list["TJ"] = "Tajikistan";
		list["TK"] = "Tokelau";
		list["TL"] = "Timor-Leste";
		list["TM"] = "Turkmenistan";
		list["TN"] = "Tunisia";
		list["TO"] = "Tonga";
		list["TR"] = "Turkey";
		list["TT"] = "Trinidad and Tobago";
		list["TV"] = "Tuvalu";
		list["TW"] = "Taiwan, Province of China";
		list["TZ"] = "Tanzania, United Republic of";
		list["UA"] = "Ukraine";
		list["UG"] = "Uganda";
		list["UM"] = "United States Minor Outlying Islands";
		list["US"] = "United States";
		list["UY"] = "Uruguay";
		list["UZ"] = "Uzbekistan";
		list["VA"] = "Holy See (Vatican City State)";
		list["VC"] = "Saint Vincent and the Grenadines";
		list["VE"] = "Venezuela, Bolivarian Republic of";
		list["VG"] = "Virgin Islands, British";
		list["VI"] = "Virgin Islands, U.S.";
		list["VN"] = "Viet Nam";
		list["VU"] = "Vanuatu";
		list["WF"] = "Wallis and Futuna";
		list["WS"] = "Samoa";
		list["YE"] = "Yemen";
		list["YT"] = "Mayotte";
		list["ZA"] = "South Africa";
		list["ZM"] = "Zambia";
		list["ZW"] = "Zimbabwe";
		level.countries = list;
	}
	c = self get_country();
	if(isdefined(c)) {
		if(isdefined(level.countries[c]))
			return level.countries[c];
	}
	return undefined;
}

//get_country thanks to IzNoGoD
//mysql_wrapper by izno

mysql_wrapper(query, save)
{
	ret = mysql_query(level.db, query);
	if(ret)
	{
		printconsole(query + "\n");
		printconsole("errno = " + mysql_errno(level.db) + " error = " + mysql_error(level.db) + "\n");
		mysql_close(level.db);
		return undefined;
	}
	if(save)
	{
		result = mysql_store_result(level.db);
		return result;
	}
	else
		return undefined;
}

get_country() {
	ip = self get_ip();
	ip_parts = [];
	part = 0;
	ip_parts[part] = "";
	prev_found = 0;
	for(i = 0; i < ip.size; i++)
	{
		if(ip[i] != ".")
			ip_parts[part] += ip[i];
		else
		{
			part++;
			ip_parts[part] = "";
		}
	}
	multi = 1;
	num = 0;
	for(i = ip_parts.size - 1; i >= 0; i--)
	{
		num += (int)(ip_parts[i]) * multi;
		multi *= 256;
	}
	result = mysql_wrapper("SELECT country FROM countries WHERE ip < " + num + " ORDER BY ip DESC LIMIT 1", true);
	if(isdefined(result))
	{
		rowcount = mysql_num_rows(result);
		if(rowcount)
		{
			row = mysql_fetch_row(result);
			if(isdefined(row[0]))
				self.country = row[0];
		}
		mysql_free_result(result);
	}
	return self.country;
}

byid(id) {
	players = std::players();
	for(i = 0; i < players.size; i++)
		if(players[i] getentitynumber() == id)
			return players[i];
	return undefined;
}

array_by_name(name) {
	players = std::players();
	arr = [];
	for(i = 0; i < players.size; i++) {
		if(std::_strpos(players[i].name, name) != -1)
			arr[arr.size] = players[i];
	}
	return arr;
}

get(arg) {
	arr = array_by_name(arg[0]);
	if(arr.size == 0) {
		self iprintln("^5No player was found.");
	} else if(arr.size == 1) {
		return arr[0];
	} else {
		str = "^2Players found: ^7";
		for(i = 0; i < arr.size; i++) {
			tmp = "^7["+arr[i] getentitynumber()+"] "+arr[i].name+"^7, ";
			str += tmp;
		}
		self iprintln(str);
	}
	return undefined;
}

by_name(name) {
	players = std::players();
	for(i = 0; i < players.size; i++) {
		if(std::_strpos(players[i], name) != -1)
			return players[i];
	}
	return undefined;
}

set_db_field(fieldname, value) {
	ret = "";
	query = "SELECT * FROM mybb_profilefields WHERE name='"+fieldname+"' LIMIT 1";
	mysql_query(level.db, query);
	r = mysql_store_result(level.db);
	num = mysql_num_fields(r);
	if(num != 0) {
		row = mysql_fetch_row(r);
		if(isdefined(row)) {
			mysql_query(level.db, "UPDATE mybb_userfields SET fid"+row[0]+"='"+value+"' WHERE ufid='"+self.pers["uid"]+"' LIMIT 1");
		}
	}
	mysql_free_result(r);
}

update_db_fields() {
	self set_db_field("Kills", self.pers["total_kills"]);
	self set_db_field("Deaths", self.pers["total_deaths"]);
}

get_db_field(fieldname) {
	//should be using JOIN etc eh...
	ret = "";
	//don't need to real_escape assumingly you wouldn't take raw input as argument
	query = "SELECT * FROM mybb_profilefields WHERE name='"+fieldname+"' LIMIT 1";
	mysql_query(level.db, query);
	r = mysql_store_result(level.db);
	num = mysql_num_fields(r);
	if(num != 0) {
		row = mysql_fetch_row(r);
		if(isdefined(row)) {
			mysql_query(level.db, "SELECT fid"+row[0]+" FROM mybb_userfields WHERE ufid='"+self.pers["uid"]+"' LIMIT 1");
			r2=mysql_store_result(level.db);
			if(mysql_num_fields(r2) > 0) {
				row2=mysql_fetch_row(r2);
				if(isdefined(row2)) {
					if(isdefined(row2[0])) {
						//printconsole("row2="+row2[0]+"\n");
						ret = row2[0];
					}
				}
			}
			mysql_free_result(r2);
		}
	}
	mysql_free_result(r);
	return ret;
}

loggedin() {
	return isdefined(self.pers["uid"]);
}

logout() {
	self.pers["uid"] = undefined;
}

is(a) {
	g = self.pers["usergroup"];
	if(g == a)
		return true;
	self iprintln("^1You do not have the rights to use this.");
	return false;
}

getrights() {
	if(isdefined(self.pers["usergroup"]))
		return self.pers["usergroup"];
	return -1;
}

group_to_string() {
	if(!isdefined(self.pers["usergroup"]))
		return "Member";
	switch(self.pers["usergroup"]) {
		
		case 4:
			return "Administrator";
		break;
		
		default:
			return "Member";
		break;
	}
}

print_but_me(msg, w) {
	players=std::players();
	for(i = 0; i < players.size; i++) {
		if(players[i] == self)
			continue;
		if(w == 0)
			players[i] iprintln(msg);
		else
			players[i] iprintlnbold(msg);
	}
}