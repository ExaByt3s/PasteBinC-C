/*
* we can use NXJSON for this project. it is small and will work just fine why would i roll out my own JSON praser for this shit?!
*/
#include "json.h"
#include "C Runtime.h"
#pragma warning(disable:4244)
#ifndef NX_JSON_CALLOC
#define NX_JSON_CALLOC() HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(nx_json))
#define NX_JSON_FREE(json) HeapFree(GetProcessHeap(), 0, json)
#endif
//int _ltod3 = 0;
#define IS_WHITESPACE(c) ((unsigned char)(c)<=(unsigned char)' ')
static const nx_json dummy; // { 0 };

static nx_json* create_json(nx_json_type type, const char* key, nx_json* parent) {
	nx_json* js = NX_JSON_CALLOC();
	// ^ new line
	js->type = type;
	js->key = key;
	if (!parent->last_child) {
		parent->child = parent->last_child = js;
	}
	else {
		parent->last_child->next = js;
		parent->last_child = js;
	}
	parent->length++;
	return js;
}

void nx_json_free(const nx_json* js) {
	nx_json* p = js->child;
	nx_json* p1;
	while (p) {
		p1 = p->next;
		nx_json_free(p);
		p = p1;
	}
	NX_JSON_FREE(js);
}

static int unicode_to_utf8(unsigned int codepoint, char* p, char** endp) {
	if (codepoint < 0x80) *p++ = codepoint;
	else if (codepoint < 0x800) *p++ = 192 + codepoint / 64, *p++ = 128 + codepoint % 64;
	else if (codepoint - 0xd800u < 0x800) return 0;
	else if (codepoint < 0x10000) *p++ = 224 + codepoint / 4096, *p++ = 128 + codepoint / 64 % 64, *p++ = 128 + codepoint % 64;
	else if (codepoint < 0x110000) *p++ = 240 + codepoint / 262144, *p++ = 128 + codepoint / 4096 % 64, *p++ = 128 + codepoint / 64 % 64, *p++ = 128 + codepoint % 64;
	else return 0; // error
	*endp = p;
	return 1;
}

nx_json_unicode_encoder nx_json_unicode_to_utf8 = unicode_to_utf8;

static inline int hex_val(char c) {
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	if (c >= 'A' && c <= 'F') return c - 'A' + 10;
	return -1;
}

static char* unescape_string(char* s, char** end, nx_json_unicode_encoder encoder) {
	char* p = s;
	char* d = s;
	char c;
	while ((c = *p++)) {
		if (c == '"') {
			*d = '\0';
			*end = p;
			return s;
		}
		else if (c == '\\') {
			switch (*p) {
			case '\\':
			case '/':
			case '"':
				*d++ = *p++;
				break;
			case 'b':
				*d++ = '\b'; p++;
				break;
			case 'f':
				*d++ = '\f'; p++;
				break;
			case 'n':
				*d++ = '\n'; p++;
				break;
			case 'r':
				*d++ = '\r'; p++;
				break;
			case 't':
				*d++ = '\t'; p++;
				break;
			case 'u': // unicode
				if (!encoder) {
					// leave untouched
					*d++ = c;
					break;
				}
				char* ps = p - 1;
				UNREFERENCED_PARAMETER(ps);
				int h1, h2, h3, h4;
				if ((h1 = hex_val(p[1])) < 0 || (h2 = hex_val(p[2])) < 0 || (h3 = hex_val(p[3])) < 0 || (h4 = hex_val(p[4])) < 0) {
					return 0;
				}
				unsigned int codepoint = h1 << 12 | h2 << 8 | h3 << 4 | h4;
				if ((codepoint & 0xfc00) == 0xd800) { // high surrogate; need one more unicode to succeed
					p += 6;
					if (p[-1] != '\\' || *p != 'u' || (h1 = hex_val(p[1])) < 0 || (h2 = hex_val(p[2])) < 0 || (h3 = hex_val(p[3])) < 0 || (h4 = hex_val(p[4])) < 0) {
						return 0;
					}
					unsigned int codepoint2 = h1 << 12 | h2 << 8 | h3 << 4 | h4;
					if ((codepoint2 & 0xfc00) != 0xdc00) {
						return 0;
					}
					codepoint = 0x10000 + ((codepoint - 0xd800) << 10) + (codepoint2 - 0xdc00);
				}
				if (!encoder(codepoint, d, &d)) {
					return 0;
				}
				p += 5;
				break;
			default:
				// leave untouched
				*d++ = c;
				break;
			}
		}
		else {
			*d++ = c;
		}
	}
	return 0;
}

static char* skip_block_comment(char* p) {
	// assume p[-2]=='/' && p[-1]=='*'
	char* ps = p - 2;
	UNREFERENCED_PARAMETER(ps);
	if (!*p) {
		return 0;
	}
REPEAT:
	p = _strchr(p + 1, '/');
	if (!p) {
		return 0;
	}
	if (p[-1] != '*') goto REPEAT;
	return p + 1;
}

static char* parse_key(const char** key, char* p, nx_json_unicode_encoder encoder) {
	// on '}' return with *p=='}'
	char c;
	while ((c = *p++)) {
		if (c == '"') {
			*key = unescape_string(p, &p, encoder);
			if (!*key) return 0; // propagate error
			while (*p && IS_WHITESPACE(*p)) p++;
			if (*p == ':') return p + 1;
			return 0;
		}
		else if (IS_WHITESPACE(c) || c == ',') {
			// continue
		}
		else if (c == '}') {
			return p - 1;
		}
		else if (c == '/') {
			if (*p == '/') { // line comment
				char* ps = p - 1;
				UNREFERENCED_PARAMETER(ps);
				p = _strchr(p + 1, '\n');
				if (!p) {
					return 0; // error
				}
				p++;
			}
			else if (*p == '*') { // block comment
				p = skip_block_comment(p + 1);
				if (!p) return 0;
			}
			else {
				return 0; // error
			}
		}
		else {
			return 0; // error
		}
	}
	return 0; // error
}

static char* parse_value(nx_json* parent, const char* key, char* p, nx_json_unicode_encoder encoder) {
	nx_json* js;
	while (1) {
		switch (*p) {
		case '\0':
			return 0; // error
		case ' ': case '\t': case '\n': case '\r':
		case ',':
			// skip
			p++;
			break;
		case '{':
			js = create_json(NX_JSON_OBJECT, key, parent);
			p++;
			while (1) {
				const char* new_key;
				p = parse_key(&new_key, p, encoder);
				if (!p) return 0; // error
				if (*p == '}') return p + 1; // end of object
				p = parse_value(js, new_key, p, encoder);
				if (!p) return 0; // error
			}
		case '[':
			js = create_json(NX_JSON_ARRAY, key, parent);
			p++;
			while (1) {
				p = parse_value(js, 0, p, encoder);
				if (!p) return 0; // error
				if (*p == ']') return p + 1; // end of array
			}
		case ']':
			return p;
		case '"':
			p++;
			js = create_json(NX_JSON_STRING, key, parent);
			js->text_value = unescape_string(p, &p, encoder);
			if (!js->text_value) return 0; // propagate error
			return p;
		case '-': case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		{

			js = create_json(NX_JSON_INTEGER, key, parent);
			char* pe;

			js->int_value = _strtol(p, &pe, 0);
			if (pe == p) {

				return 0; // error
			}

			if (*pe == '.' || *pe == 'e' || *pe == 'E') { // double value
				js->type = NX_JSON_DOUBLE;
				js->dbl_value = _strtod(p, &pe);
				if (pe == p) {

					return 0; // error
				}

			}
			else {
				js->dbl_value = js->int_value;
			}
			return pe;
		}
		case 't':
			if (!_strncmp(p, "true", 4)) {
				js = create_json(NX_JSON_BOOL, key, parent);
				js->int_value = 1;
				return p + 4;
			}
			return 0; // error
		case 'f':
			if (!_strncmp(p, "false", 5)) {
				js = create_json(NX_JSON_BOOL, key, parent);
				js->int_value = 0;
				return p + 5;
			}
			return 0; // error
		case 'n':
			if (!_strncmp(p, "null", 4)) {
				create_json(NX_JSON_NULL, key, parent);
				return p + 4;
			}
			return 0; // error
		case '/': // comment
			if (p[1] == '/') { // line comment
				char* ps = p;
				UNREFERENCED_PARAMETER(ps);
				p = _strchr(p + 2, '\n');
				if (!p) {
					return 0; // error
				}
				p++;
			}
			else if (p[1] == '*') { // block comment
				p = skip_block_comment(p + 2);
				if (!p) return 0;
			}
			else {
				return 0; // error
			}
			break;
		default:
			return 0; // error
		}
	}
}

const nx_json* nx_json_parse_utf8(char* text) {
	return nx_json_parse(text, unicode_to_utf8);
}

const nx_json* nx_json_parse(char* text, nx_json_unicode_encoder encoder) {
	nx_json js;
	_ZeroMemory(&js, sizeof(nx_json));
	if (!parse_value(&js, 0, text, encoder)) {
		if (js.child) nx_json_free(js.child);
		return 0;
	}
	return js.child;
}

const nx_json* nx_json_get(const nx_json* json, const char* key) {

	if (!json || !key) return &dummy; // never return null
	nx_json* js;
	for (js = json->child; js; js = js->next) {
		if (js->key && !_strcmp(js->key, key)) return js;
	}
	return &dummy; // never return null
}

const nx_json* nx_json_item(const nx_json* json, int idx) {
	if (!json) return &dummy; // never return null
	nx_json* js;
	for (js = json->child; js; js = js->next) {
		if (!idx--) return js;
	}
	return &dummy; // never return null
}
