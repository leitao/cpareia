#include "field_comparator.h"

static char *
common_chars(char *s, char *t, int ss, int st, int halflen, int *size){
  char common[1000], copy[1000];
  int i, j, min, sz;
  char ch, *ret;
  short found;

  found = 0;
  *size = 0;

  strcpy(copy, t);
  memset(common, 0, 1000);

  for(i = 0; i < ss; i++){
    ch = s[i];
    found = 0;
    min = i + halflen > st ? st : i + halflen;
    for(j = i - halflen > 0 ? i - halflen : 0; ! found && j < min; j++){
      if(copy[j] == ch){
        found = 1;
        common[*size] = ch;
        (*size)++;
        copy[j] = '*';
      }
    }
  }
  sz = sizeof(char) * (*size + 1);
  ret = malloc(sz);
  strncpy(ret, common, sz);
  return ret;
}

int transpositions(char *s, char *t, int ss){
  int transpositions, i;

  transpositions = 0;

  for (i = 0; i < ss; i++) {
    if (s[i] != t[i]){
      transpositions++;
    }
  }
  return transpositions / 2;
}

float jaro(char *s,  char *t, size_t ss, size_t st){
  int halflen, transpos;
  char *common1, *common2;
  float retval;
  int sc1, sc2;

  halflen = (ss > st) ? ss / 2 + 1 : st / 2 + 1;

  common1 = common_chars(s, t, ss, st, halflen, &sc1);
  common2 = common_chars(t, s, st, ss, halflen, &sc2);
  retval = 0.0F;

  if(sc1 == sc2 && sc1 != 0){
    transpos = transpositions(common1, common2, sc1);
    retval = (sc1 / ss + sc2 / st + (sc1 - transpos) / sc1) / 3.0F;
  }
  free(common1);
  free(common2);
  return retval;
}

int
common_prefix_length(int max, char *c1, char *c2, int s1, int s2) {
  int i, n;
  n = MIN3(max, s1, s2);

  for (i = 0; i < n; i++) {
    if (c1[i] != c2[i]) return i;
  }
  return n;
}

float
winkler_score(char *s,  char *t, int ss, int st, int max) {
  return common_prefix_length(max, s, t, ss, st);
}

double
winkler(char *s, char * t){
  float dist, pref_length;
  size_t ss, st;

  if (!strcmp(s, t))
    return 1.0;

  ss = strlen(s);
  st = strlen(t);

  dist = jaro(s, t, ss, st);
  pref_length = winkler_score(s, t, ss, st, 4);
  dist = dist + pref_length * 0.1 * (1 - dist);

  return dist;
}

void soundex(char *text, char *buffer, size_t len) {
	char code = '0';
	char lastcode = '0';
	unsigned int pos = 0;

	if (len < 5){
		buffer[0] = '\0';
		return;
	}
	memset(buffer, '\0', len);

	memset(buffer, '0', 4);

	if (!text) {
		return;
	}

	while (*text && !isalpha(*text)){
		text++;
	}
	if (*text) {
		buffer[pos++] = *text++;
	} else {
		return;
	}

	while (pos < len -1) {
		switch (*text) {
		case 'b':
		case 'p':
		case 'f':
		case 'v':
			code = '1';
			break;
		case 'c':
		case 's':
		case 'k':
		case 'g':
		case 'j':
		case 'q':
		case 'x':
		case 'z':
			code = '2';
			break;
		case 'd':
		case 't':
			code = '3';
			break;
		case 'l':
			code = '4';
			break;
		case 'm':
		case 'n':
			code = '5';
			break;
		case 'r':
			code = '6';
			break;
		case 'a':
		case 'e':
		case 'i':
		case 'o':
		case 'u':
		case 'y':
		case 'w':
		case 'h':
			code = '0';
			break;
		default:
			break;
		}

		if (code != '0' && code != lastcode) {
			buffer[pos++]  = code;
			lastcode = code;
		}
		if (*text)
			text++;
		else
			break;
	}
}

void brsoundex(char *text, char *buffer, size_t len) {
	char *aux = strdup(text);

	switch(text[0]){
		case 'w':
			if (text[1] == 'o' || text[1] == 'a'){
				aux[0] = 'v';
			}
			break;
		case 'k':
			if (text[1] == 'a' || text[1] == 'o' || text[1] == 'u'){
				aux[0] = 'c';
			}
			break;
		case 'y':
			aux[0] = 'i';
			case 'c':
			if (text[1] == 'e' || text[1] == 'i'){
				aux[0] = 's';
			}
			break;
		case 'g':
			if (text[1] == 'e' || text[1] == 'i'){
				aux[0] = 'j';
			}
			break;
		case 'h':
			strcpy(aux, ((char *) text + sizeof(char)));
			break;
	}
	soundex(aux, buffer, len);
	free(aux);
}
