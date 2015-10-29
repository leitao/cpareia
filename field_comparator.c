#include "field_comparator.h"

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
  char *aux;

  aux = malloc(sizeof * aux * len);
  memcpy(aux, text, len);

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
