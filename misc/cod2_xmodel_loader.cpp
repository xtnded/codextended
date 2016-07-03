/* 	This is part of my custom engine client/server to load CoD2 XModels
	Feel free to use this as reference or anything at all.
	- Richard
*/

bool load_xmodel_surfaces(model_obj *model, const char *lod) {
	char path[256] = { 0 };
	snprintf(path, sizeof(path) - 1, "xmodelsurfs/%s", lod);

#define hide_printf

	hide_printf("opening file %s\n", path);

	unsigned char *buf = NULL;
	size_t file_size=0;
	if (fs_readfile(path, (unsigned char**)&buf, &file_size)) {
		return false;
	}
	hide_printf("xmodelsurf filesize=%d\n", file_size);
#if 0
	Con::MsgBox("buf=%02X\n", buf);
	__asm int 3
#endif
	size_t idx = 0;

	if (*(uint16_t*)buf != 20) {
		hide_printf("not valid xmodel surf\n");
		xfree(buf);
		return false;
	}

	idx += 2;
#if 0
	for (int i = idx; i < file_size; i++) {
		hide_printf("%02X ", (char)buf[i]);
	}
#endif
	uint16_t nummeshes = *(uint16_t*)(buf + idx);
	idx += 2;

	hide_printf("num meshes=%d\n", nummeshes);

	for (unsigned i = 0; i < nummeshes; i++) {
#define MODELTYPE_RIGID 0
#define MODELTYPE_PLAYERBODY 2

		uint8_t modeltype = *(uint8_t*)(buf + idx); idx++;
		//assumingly filetype? 2 = playerbody? 0 = rigid?

		if (modeltype != 0) {
			model->meshes.clear();
			xfree(buf);
			return false;
		}
		
		uint16_t numverts = *(uint16_t*)(buf + idx); idx += 2;
		uint16_t numtriangles = *(uint16_t*)(buf + idx); idx += 2;
		uint16_t numverts2 = *(uint16_t*)(buf + idx); idx += 2; //idk

		hide_printf("numverts=%d(loc=%02X)\n", numverts,idx);
		hide_printf("numtriangles=%d(loc=%02X)\n", numtriangles, idx);
		hide_printf("numverts2=%d(loc=%02X)\n", numverts2,idx);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;

		bool y_up = false; //true = z up //false = y up

		for (int j = 0; j < numverts; j++) {
			idx += 12;

			uint32_t color = *(uint32_t*)(buf + idx);
			idx += 4; //0xffffffff?

			Vertex nv;
			float u, v;
			u = *(float*)(buf + idx); idx += 4;
			v = 1.0f - *(float*)(buf + idx); idx += 4;
			uvs.push_back(glm::vec2(u, v));

			idx += 24; //idk

			//uint8_t numweights = *(uint8_t*)(buf + idx);

			//positions
			float x, y, z;
			x = *(float*)(buf + idx); idx += 4;
			if (!y_up) {
				z = *(float*)(buf + idx) * -1; idx += 4;
				y = *(float*)(buf + idx); idx += 4;
			} else {
				y = *(float*)(buf + idx); idx += 4;
				z = *(float*)(buf + idx); idx += 4;
			}

#define CM_TO_INCH 1
//#define CM_TO_INCH 0.3937007874015748031496062992126 // 1cm = 50 / 127in
			positions.push_back(glm::vec3(x/ CM_TO_INCH, y/ CM_TO_INCH, z/ CM_TO_INCH) * BSP_SCALE);
			normals.push_back(glm::vec3(1/ CM_TO_INCH, 1/ CM_TO_INCH, 1/ CM_TO_INCH));

#if 0
			if (modeltype == MODELTYPE_PLAYERBODY)
				idx += 3;//3 extra bytes??
#endif

			//hide_printf("BLOCK SIZE = %02X\n", idx - cur_idx);
		}

		hide_printf("cur_idx=%d\n", idx);

		unsigned vIdx = 0;
		Mesh m;
		m.name = "lolz";
		if (i >= model->textures.size())
			m.texture = nullptr;
		else
			m.texture = model->textures[i];
		hide_printf("starting triangles indices at %02X\n", idx);
		for (int j = 0; j < numtriangles; j++) {
			uint16_t face[3];
			face[0] = *(uint16_t*)(buf + idx); idx += 2;
			face[1] = *(uint16_t*)(buf + idx); idx += 2;
			face[2] = *(uint16_t*)(buf + idx); idx += 2;

			//hide_printf("face(%d,%d,%d)\n", face[0], face[1], face[2]);

			for (int i = 0; i < 3; i++) {
				Vertex vv;

				uint16_t indice = face[i];
				if (indice < 0 || indice > positions.size()) {
					hide_printf("ERROR! indice = %d\n", indice);
					xfree(buf);
					return false;

					//break;
				}
				vv.origin = positions.at(indice);
				vv.normal = normals.at(indice);
				vv.uv = uvs.at(indice);
				m.vertices.push_back(vv);
				m.indices.push_back(vIdx++);
			}
		}
		model->meshes.push_back(m);
		hide_printf("idx=%d,filesize=%d\n", idx, file_size);
		//hide_printf("idx=%d,(%02X)\n", idx,idx);
	}

	xfree(buf);
	return true;
}

std::string cod2_get_color_map_from_material(const char *matpath) {

	char *buf = NULL;
	size_t file_size = 0;
	if (fs_readfile(matpath, (unsigned char**)&buf, &file_size)) {
		//printf("COULD NOT OPEN MATERIAL file %s\n", matpath);
		return "";
	}

	size_t idx = 0;

	uint32_t matname_i = *(uint32_t*)(buf + idx); idx += 4;
	uint32_t colormap_i = *(uint32_t*)(buf + idx);

	std::string ret = (char*)(buf + colormap_i);

	xfree(buf);
	return ret;
}

bool model_obj::load_xmodel(const char *path) {
	std::vector<std::string> lodstrings;
	int numlods = 0;
	bool read_mats = true;

#if 1
	this->m_fullpath = path;
	char *buf=NULL;
	size_t file_size;
	if (fs_readfile(path, (unsigned char**)&buf, &file_size)) {
		return false;
	}
#if 0
	printf("filesize=%d\n", file_size);
	for (int i = 0; i < file_size; i++) {
		printf("%02X ", (char)buf[i]);
	}
#endif

	bool success = false;
	size_t idx = 0;
	uint16_t xmodelver = *(uint16_t*)buf;

	if (xmodelver != 20) {
		con_warn("xmodel version is not 20!\n");
		goto _ret;
	}
	idx += 2;

	idx += 25; //cod4/cod2 have 25 bytes here idk

	for (int i = 0; i < 4; i++) {
		uint32_t some_int = *(uint32_t*)(buf + idx);
		idx += 4;

		char *lodfilename = (char*)(buf + idx);
		if (*lodfilename) {
			printf("lod=%s\n", lodfilename);
			lodstrings.push_back(lodfilename);
			numlods++;
		}

		idx += strlen(lodfilename) + 1;
	}
#ifdef BUILD_SERVER
	read_mats = false;
#else
	read_mats = true;
#endif
	if (*(uint32_t*)(buf + idx) != -1)
		read_mats = false;
	idx += 8;

#if 0
	uint32_t count = *(uint32_t*)(buf + idx);
	printf("count=%d\n", count);
	for (int cc = 0; cc != count; cc++) {
		uint32_t subcount = *(uint32_t*)(buf + idx);
		idx += (subcount * 48) + 36;
	}
	printf("STIEKEM GEDANST IDX = %02X\n", idx);
	idx += 8;
#endif
	{
		if (read_mats) {
			std::vector<std::string> matstrings;
			for (int i = 0; i < numlods; i++) {
				uint16_t num_materials = *(uint16_t*)(buf + idx);
				idx += 2;

				printf("num_materials=%d\n", num_materials);

				for (int k = 0; k < num_materials; k++) {
					char *matname = (char*)(buf + idx);
#if 1
					char matpath[128] = { 0 };
					snprintf(matpath, sizeof(matpath) - 1, "materials/%s", matname);
					std::string colmap = "images/" + cod2_get_color_map_from_material(matpath) + ".iwi";
					//printf("colmap=%s\n", colmap.c_str());
					az_texture *texture = az_texture::create(colmap.c_str());
					if (texture) {
#if 1
						if (strstr(lodstrings[0].c_str(), "bush") || strstr(lodstrings[0].c_str(), "grass"))
							texture->set_effect(texture->get_effect() | TEXTURE_EFFECT_SIN_WAVE);
#endif
						texture->set_name(matname);
						this->textures.push_back(texture);
					}
#endif
					matstrings.push_back(matname);

					idx += strlen(matname) + 1;
				}
			}

			for (auto & m : matstrings)
				printf("material: %s\n", m.c_str());
		}

		load_xmodel_surfaces(this, lodstrings[0].c_str());
	}
	success = true;
	_ret:
	xfree(buf);
	this->calculate_bounds();
	return success;
#endif
}