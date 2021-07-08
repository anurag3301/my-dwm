int
drawstatustext(int x) {
	char cd[] = "[c]", id[] = "[i]";
	char buf1[256] = {0};
	Bool parse = True;
	Bool cs = False, is = False;
	int stextw = 0;
	
	strcpy(buf1, stext);
	while(parse) {
		char *cp = 0, *ip = 0;
		
		cp = strstr(buf1, cd);
		ip = strstr(buf1, id);
		if(cp != NULL || ip != NULL) {
			char buf2[256] = {0}, buf3[256] = {0};
			size_t clen2 = -1, ilen2 = -1, len2 = 0, dlen = 0, offset3 = 0, len3 = 0;
			
			if(cp != NULL)
				clen2 = cp - buf1;
			if(ip != NULL)
				ilen2 = ip - buf1;
			if(clen2 < 0) {
				len2 = ilen2;
				dlen = strlen(id);
			}
			else if(ilen2 < 0) {
				len2 = clen2;
				dlen = strlen(cd);
			}
			else if(clen2 < ilen2) {
				len2 = clen2;
				dlen = strlen(cd);
			}
			else {
				len2 = ilen2;
				dlen = strlen(id);
			}
			strncpy(buf2, buf1, len2);
			offset3 = len2 + dlen;
			len3 = strlen(buf1) - offset3;
			strncpy(buf3, buf1 + offset3, len3);
			if(buf2 != NULL && strlen(buf2) > 0)
				stextw = stextw + TEXTW(buf2);
			
			memset(buf1, '\0', sizeof(buf1));
			strcpy(buf1, buf3);
		}
		else {
			stextw = stextw + TEXTW(buf1);
			parse = False;
		}
	}
	cd.x = ww - stextw;
	
	memset(buf1, '\0', sizeof(buf1));
	strcpy(buf1, stext);
	parse = True;
	while(parse) {
		char *cp = 0, *ip = 0;
		
		cp = strstr(buf1, cd);
		ip = strstr(buf1, id);
		if(cp != NULL || ip != NULL) {
			char buf2[256] = {0}, buf3[256] = {0};
			size_t clen2 = -1, ilen2 = -1, len2 = 0, dlen = 0, offset3 = 0, len3 = 0;
			Bool cs3 = cs, is3 = is;
			
			if(cp != NULL)
				clen2 = cp - buf1;
			if(ip != NULL)
				ilen2 = ip - buf1;
			if(clen2 < 0) {
				len2 = ilen2;
				dlen = strlen(id);
				is3 = !is;
			}
			else if(ilen2 < 0) {
				len2 = clen2;
				dlen = strlen(cd);
				cs3 = !cs;
			}
			else if(clen2 < ilen2) {
				len2 = clen2;
				dlen = strlen(cd);
				cs3 = !cs;
			}
			else {
				len2 = ilen2;
				dlen = strlen(id);
				is3 = !is;
			}
			
			strncpy(buf2, buf1, len2);
			offset3 = len2 + dlen;
			len3 = strlen(buf1) - offset3;
			strncpy(buf3, buf1 + offset3, len3);
			if(buf2 != NULL && strlen(buf2) > 0) {
				dc.w = TEXTW(buf2);
				if(cs) {
					if(is)
						drawtext(buf2, dc.sel, True);
					else
						drawtext(buf2, dc.sel, False);
				}
				else {
					if(is)
						drawtext(buf2, dc.norm, True);
					else
						drawtext(buf2, dc.norm, False);
				}
				dc.x = dc.x + dc.w;
			}
			
			cs = cs3;
			is = is3;
			memset(buf1, '\0', sizeof(buf1));
			strcpy(buf1, buf3);
		}
		else {
			dc.w = TEXTW(buf1);
			dc.x = ww - dc.w;
			if(dc.x < x) {
				dc.x = x;
				dc.w = ww - x;
			}
			if(cs) {
				if(is)
					drawtext(buf1, dc.sel, True);
				else
					drawtext(buf1, dc.sel, False);
			}
			else {
				if(is)
					drawtext(buf1, dc.norm, True);
				else
					drawtext(buf1, dc.norm, False);
			}
			parse = False;
		}
	}
	
	return(stextw);
}
