
#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int32 integer;
typedef uint32 uinteger;

typedef enum
{
	E_ENCODING_ASCII = 0,
	E_ENCODING_UTF8,
	E_ENCODING_SJIS,
	E_ENCODING_EUC,
	E_ENCODING_UTF16,
} E_ENCODING;

const uint32_t E_CONVERT_HW_CAPITAL_ALPHABET = 1;
const uint32_t E_CONVERT_HW_SMALL_ALPHABET = 2;
const uint32_t E_CONVERT_HW_NUMBER = 4;
const uint32_t E_CONVERT_HW_SYMBOL = 8;

#ifdef _WIN32
#pragma warning(disable:4996)
#endif

class utf8string
{
public:
	utf8string() {
        utf8stringbuf = "";
    }

	utf8string(const char* s, E_ENCODING enc = E_ENCODING_UTF8) {
		char *newstr = NULL;
		if (enc != E_ENCODING_UTF8) {
		}
		else {
			newstr = const_cast<char*>(s);
		}

		utf8stringbuf = newstr;
		rebuild(utf8stringbuf.c_str());
	}

	utf8string(std::string &s, E_ENCODING enc = E_ENCODING_UTF8) {
		char *newstr = NULL;
		if (enc != E_ENCODING_UTF8) {
		}
		else {
			newstr = const_cast<char*>(s.c_str());
		}
		utf8stringbuf = newstr;
		rebuild(utf8stringbuf.c_str());
	}

    ~utf8string() {
        const char *wp;
        for (uinteger i = 0; i < charlist.size(); i++) {
            delete[] charlist[i];
        }
    }

	uinteger length() const {return charlist.size();}

	const char *getCString() { return utf8stringbuf.c_str(); }

	const std::string getStdString() { return utf8stringbuf; }

	utf8string format(const utf8string &s, ...);

	std::vector<utf8string> split(const utf8string &s, const utf8string &delim) {
		std::vector<utf8string> ret;

		return ret;
	}

	utf8string at(const uinteger pos) const {
        utf8string s(charlist.at(pos));
        return s;
    }

	integer find(const utf8string &s, const uinteger index = 0) const {
		std::string::size_type ret = this->utf8stringbuf.find(s.utf8stringbuf, index);
		return ret == std::string::npos ? -1 : ret;
	}

	bool remove(const uinteger pos) {
		this->charlist.erase(this->charlist.begin() + pos);
		rebuild();

        return true;
	}

    bool toHalfWidth(uint32_t options) {
        char *tmp = NULL;
        uint32_t code;
        bool converted = false;

        std::ios::fmtflags orgflags = std::cout.flags();
        std::cout.setf(std::ios::hex, std::ios::basefield);
        for (uinteger i = 0; i < charlist.size(); i++) {
            tmp = charlist.at(i);

            if (strlen(tmp) != 3) {
                // supported letters are should be 3 bytes
                continue;
            }

            code = (tmp[0] & 0xff) << 16 | (tmp[1] & 0xff) << 8 | (tmp[2] & 0xff);
            if (options & E_CONVERT_HW_CAPITAL_ALPHABET) {
                if ((0xefbca1 <= code) && (code <= 0xefbcba)) {
                    tmp[0] = code - (0xefbc80 - 0x20);
                    tmp[1] = 0;
                    tmp[2] = 0;
                    converted = true;
                    continue;
                }
            }
            if (options & E_CONVERT_HW_SMALL_ALPHABET) {
                if ((0xefbd81 <= code) && (code <= 0xefbd9a)) {
                    tmp[0] = code - (0xefbd81 - 0x61);
                    tmp[1] = 0;
                    tmp[2] = 0;
                    converted = true;
                    continue;
                }
            }
            if (options & E_CONVERT_HW_NUMBER) {
                if ((0xefbc90 <= code) && (code <= 0xefbc99)) {
                    tmp[0] = code - (0xefbc80 - 0x20);
                    tmp[1] = 0;
                    tmp[2] = 0;
                    converted = true;
                    continue;
                }
            }
            if (options & E_CONVERT_HW_SYMBOL) {
                if (((0xefbc81 <= code) && (code <= 0xefbc8f)) ||
                    ((0xefbc9a <= code) && (code <= 0xefbca0)) ||
                    ((0xefbcbb <= code) && (code <= 0xefbcbf))) {
                    tmp[0] = code - (0xefbc80 - 0x20);
                    tmp[1] = 0;
                    tmp[2] = 0;
                    converted = true;
                    continue;
                }
                else if (0xefbd80 == code) {
                    tmp[0] = code - (0xefbd80 - 0x60);
                    tmp[1] = 0;
                    tmp[2] = 0;
                    converted = true;
                    continue;
                }
                else if ((0xefbd9b <= code) && (code <= 0xefbd9e)) {
                    tmp[0] = code - (0xefbd9b - 0x7b);
                    tmp[1] = 0;
                    tmp[2] = 0;
                    converted = true;
                    continue;
                }
                else if (0xe38080 == code) {
                    tmp[0] = 0x20;
                    tmp[1] = 0;
                    tmp[2] = 0;
                    converted = true;
                    continue;
                }
                else if (0xe3809c == code) {
                    tmp[0] = 0x7e;
                    tmp[1] = 0;
                    tmp[2] = 0;
                    converted = true;
                    continue;
                }
                else if (0xe383bc == code) {
                    tmp[0] = 0x2d;
                    tmp[1] = 0;
                    tmp[2] = 0;
                    converted = true;
                    continue;
                }
            }
        }

        if (converted) {
            rebuild();
        }

        std::cout.flags(orgflags);
        return true;
    }

	bool contain(const utf8string &chars) const {
		return this->find(chars) == -1 ? false : true;
	}

	utf8string trim(const utf8string &s, const utf8string &trimchars = " \r\n") const {
		utf8string result;
        result = s;

		for (uinteger i = 0; i < result.length();) {
			if (trimchars.contain(result.at(i))) {
				result.remove(i);
			}
			else {
				i++;
			}
		}

		return result;
	}

	bool operator==(const utf8string s) {
		return s.utf8stringbuf == this->utf8stringbuf;
	}

	void operator+=(const utf8string s) {
		this->utf8stringbuf + s.utf8stringbuf;
	}

	void operator=(const char* s) {
		utf8stringbuf = s;
		rebuild(utf8stringbuf.c_str());
	}

	void operator=(const utf8string &s) {
		utf8stringbuf = s.utf8stringbuf;
		rebuild(utf8stringbuf.c_str());
	}

	utf8string operator+(const utf8string &s) {
        std::string buf = this->utf8stringbuf + s.utf8stringbuf;
        utf8string newbuf(buf);
		return newbuf;
	}

private:
	void rebuild(const char*s) {
		charlist.clear();
		char *wp = const_cast<char*>(s);
		while(*wp) {
			int pos = get_utf8_charlength(wp);
			char *tmp = new char[pos+1];
			strncpy(tmp, wp, pos);
			tmp[pos] = '\0';
			charlist.push_back(tmp);
			wp += pos;
		}
	}

	void rebuild() {
		std::string tmpstr = "";

		for(uinteger i = 0; i < charlist.size(); i++) {
			tmpstr += charlist.at(i);
		}

		utf8stringbuf = tmpstr;
	}

	integer get_utf8_charlength(const char *s) {
	int len = strlen(s);

	if (len >= 4 && (s[0] & 0xf0) == 0xf0 &&
        (s[1] & 0x80) && (s[2] & 0x80) && (s[3] & 0x80)) {
        return 4;
    } else if (len >= 3 && (s[0] & 0xe0) == 0xe0 &&
        (s[1] & 0x80) && (s[2] & 0x80)) {
        return 3;
    } else if (len >= 2 && (s[0] & 0xc0) == 0xc0 && (s[1] & 0x80)) {
        return 2;
    }

    return 1;
}

private:
	std::string utf8stringbuf;
	std::vector<char*> charlist;
};
