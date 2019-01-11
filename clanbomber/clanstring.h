/*
	$Id: clanstring.h,v 1.2 2002/06/14 04:05:00 dok666 Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

//! component="System"

#ifndef header_clanstring
#define header_clanstring

#include <string>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <ctype.h>

using namespace std;

class CL_String
{

// Why is this function not supported under Linux???

void itoa(int num, char *str, const int number_format)
{
	int num1 = num;
	int num_chars = 0;
	
	while (num1>0)
	{
		num_chars++;
		num1 /= number_format;
	}

	if (num_chars == 0) num_chars = 1;

	str[num_chars] = 0; // Null-terminate the str
	
	for (int pos = num_chars-1; pos>=0; pos--)
	{
		int cur_char = num % number_format;
		
		if (cur_char < 10) // Insert number
		{
			str[pos] = cur_char + '0';
		}
		else // Insert letter
		{
			str[pos] = cur_char-10 + 'A';
		}
		
		num /= number_format;
	}
}

	char *str;

	char *int_to_string(const char *prefix, const int number)
	{
		char buf[20];
		itoa(number, buf, 10);

		int len=0;
		if (prefix!=NULL) len=strlen(prefix);

		char *text=new char[len+strlen(buf)+1];
		if (prefix!=NULL)
		{
			strcpy(text, prefix);
			strcat(text, buf);
		}
		else
		{
			strcpy(text, buf);
		}

		return text;
	}
	
	char *float_to_string(const char *prefix, const float _float_number)
	{
		char buf[25];
		buf[0]=0;

		int decimal, sign;
		int precision=4;
		char temp;

		float float_number = _float_number;

		const char *_float_buffer = fcvt(
			float_number,
			precision,
			&decimal,
			&sign);
			
		char *float_buffer = new char[strlen(_float_buffer)+1];
		strcpy(float_buffer, _float_buffer);

		if (sign) strcpy(buf, "-");
		if (decimal<=0)
		{
			strcat(buf, "0.");
			strcat(buf, float_buffer);
			goto copy_strings;
		}

		temp=float_buffer[decimal];
		float_buffer[decimal]=0;
		strcat(buf, float_buffer);
		float_buffer[decimal]=temp;

		strcat(buf, ".");
		strcat(buf, &float_buffer[decimal]);

	copy_strings:
		int len=0;
		if (prefix!=NULL) len=strlen(prefix);

		char *text=new char[len+strlen(buf)+1];
		if (prefix!=NULL)
		{
			strcpy(text, prefix);
			strcat(text, buf);
		}
		else strcpy(text, buf);

		return text;
	}

	char *append_string(const char *prefix, const char *postfix)
	{
		int len=0;
		if (prefix!=NULL) len=strlen(prefix);
		if (postfix!=NULL) len+=strlen(postfix);
		char *text=new char[len+1];
		if (prefix!=NULL)
		{
			strcpy(text, prefix);
			if (postfix!=NULL) strcat(text, postfix);
		}
		else if (postfix!=NULL)
		{
			strcpy(text, postfix);
		}

		return text;
	}

public:
	CL_String()
	{
		str=NULL;
	}
	
	CL_String(const string &str)
	{
		this->str = append_string(NULL, str.c_str());
	}

	CL_String(const int value)
	{
		str=int_to_string(NULL, value);
	}

	CL_String(const float float_value)
	{
		str=float_to_string(NULL, float_value);
	}

	CL_String(const char *text)
	{
		str=append_string(NULL, text);
	}
	
	CL_String(const CL_String &clone)
	{
		str=append_string(NULL, clone.str);
	}

	CL_String(const CL_String &text1, const CL_String &text2)
	{
		str=append_string(text1.str, text2.str);
	}

	CL_String(const char *text1, const char *text2)
	{
		str=append_string(text1, text2);
	}

	CL_String(const char *text, int length)
	{
		str = new char[length+1];
		memcpy(str, text, length);
		str[length] = 0;
	}

	virtual ~CL_String()
	{
		delete str;
	}

	CL_String operator+ (const CL_String &other_string)
	{
		return CL_String(append_string(str, other_string.str));
	}

	CL_String operator+ (const char *other_text)
	{
		return CL_String(append_string(str, other_text));
	}

	CL_String operator+ (const int number)
	{
		return CL_String(int_to_string(str, number));
	}

	CL_String operator+ (const float number)
	{
		return CL_String(float_to_string(str, number));
	}

	CL_String &operator+= (const CL_String &other_string)
	{
		char *new_string=append_string(str, other_string.str);
		delete str;
		str=new_string;

		return *this;
	}

	CL_String &operator+= (const char *other_text)
	{
		char *new_string=append_string(str, other_text);
		delete str;
		str=new_string;

		return *this;
	}

	CL_String &operator+= (const int number)
	{
		char *new_string=int_to_string(str, number);
		delete str;
		str=new_string;

		return *this;
	}

	CL_String &operator+= (const float number)
	{
		char *new_string=float_to_string(str, number);
		delete str;
		str=new_string;

		return *this;
	}

	CL_String &operator+= (double float_number)
	{
		return operator+= ((float) float_number);
	}

	CL_String &operator= (const CL_String &other_string)
	{
		delete str;
		str=append_string(NULL, other_string.str);

		return *this;
	}

	CL_String &operator= (const char *other_text)
	{
		if (str!=NULL) delete str;
		str=append_string(NULL, other_text);

		return *this;
	}

	CL_String &operator= (const int number)
	{
		if (str!=NULL) delete str;
		str=int_to_string(NULL, number);
		return *this;
	}

	CL_String &operator= (const float float_number)
	{
		delete str;
		str=float_to_string(NULL, float_number);
		return *this;
	}

	CL_String &operator= (double float_number)
	{
		return operator= ((float) float_number);
	}

	bool operator== (const CL_String &other_string)
	{
		if (str==NULL || other_string.str==NULL) return false;
		if (strcmp(str, other_string.str)==0) return true;
		return false;
	}

	bool operator== (const char *other_string)
	{
		if (str==NULL || other_string==NULL) return false;
		if (strcmp(str, other_string)==0) return true;
		return false;
	}
	
	bool operator!= (const CL_String &other_string)
	{
		return !operator==(other_string);
	}

	bool operator!= (const char *other_text)
	{
		return !operator==(other_text);
	}

	char *get_string()
	{
		if (str==NULL)
		{
			return "";
		}
		return str;
	}

	int get_length()
	{
		if (str==NULL) return 0;
		return strlen(str);
	}

	int get_as_int()
	{
		return atoi(str);
	}

	float get_as_float()
	{
		return atof(str);
	}

	operator const char* ()
	{
		return get_string();
	}
	
	operator std::string ()
	{
		if (str == NULL) return std::string("");
		return std::string(str);
	}

	CL_String &operator<< (const CL_String &other_string)
	{
		char *new_string=append_string(str, other_string.str);
		delete str;
		str=new_string;

		return *this;
	}

	CL_String &operator<< (const char *other_string)
	{
		char *new_string=append_string(str, other_string);
		if (str!=NULL) delete str;
		str=new_string;

		return *this;
	}

	CL_String &operator<< (const unsigned char *other_string)
	{
		char *new_string=append_string(str, (const char *) other_string);
		if (str!=NULL) delete str;
		str=new_string;

		return *this;
	}

	CL_String &operator<< (const unsigned short number)
	{
		char *new_string=int_to_string(str, number);
		if (str!=NULL) delete str;
		str=new_string;

		return *this;
	}

	CL_String &operator<< (const short number)
	{
		char *new_string=int_to_string(str, number);
		if (str!=NULL) delete str;
		str=new_string;

		return *this;
	}

	CL_String &operator<< (const unsigned int number)
	{
		char *new_string=int_to_string(str, number);
		if (str!=NULL) delete str;
		str=new_string;

		return *this;
	}

	CL_String &operator<< (const int number)
	{
		char *new_string=int_to_string(str, number);
		if (str!=NULL) delete str;
		str=new_string;

		return *this;
	}

	CL_String &operator<< (const float float_number)
	{
		char *new_string=float_to_string(str, float_number);
		if (str!=NULL) delete str;
		str=new_string;

		return *this;
	}

	CL_String &operator<< (double float_number)
	{
		return operator<< ((float) float_number);
	}

	CL_String get_word(int word_no)
	{
		if (str==NULL) return CL_String("");

		int len=get_length();
		int cur_word=-1;
		int word_start=0;

		int cur_state=0;
		for (int i=0; i<len; i++)
		{
			if (cur_state == 0)
			{
				if (str[i]!=32)
				{
					if (str[i]==34)
					{
						word_start = i+1;
						cur_state = 2;
					}
					else
					{
						word_start = i;
						cur_state = 1;
					}
					cur_word++;
				}
			}
			else if (cur_state == 1)
			{
				if (str[i]==32)
				{
					if (cur_word == word_no) return mid(word_start, i-word_start);
					cur_state = 0;
				}
			}
			else if (cur_state == 2)
			{
				if (str[i]==34)
				{
					if (cur_word == word_no) return mid(word_start, i-word_start);
					cur_state = 0;
				}
			}
		}

		if (cur_word == word_no) return mid(word_start);

		return CL_String("");
	}

	CL_String mid(int start, int len=-1)
	{
		if (len==-1) len=get_length()-start;

		char *new_string = new char[len+1];
		for (int i=start; i<start+len; i++) new_string[i-start]=str[i];
		new_string[len]=0;

		CL_String retval(new_string);

		delete[] new_string;

		return retval;
	}

	CL_String right(int len)
	{
		int strlength = get_length();

		if (strlength < len) return *this;

		char *new_string = new char[len+1];
		for (int i=0; i<len+1; i++) new_string[i] = str[strlength-len+i];

		CL_String retval(new_string);

		delete[] new_string;

		return retval;
	}

	int find(int character, int start_pos=0)
	{
		char *pos = strchr(str+start_pos, character);
		if (pos == NULL) return -1;
		return int(pos-str);
	}

	void to_lower()
	{
		if (str == NULL) return;

		char *p = str;
		while (*p != 0)
		{
			*p = tolower(*p);
			p++;
		}
	}

	void to_upper()
	{
		if (str == NULL) return;

		char *p = str;
		while (*p != 0)
		{
			*p = toupper(*p);
			p++;
		}
	}
	
	bool case_compare(const char *other)
	{
		if (str==NULL || other==NULL) return false;

		if (strcasecmp(str, other)==0) return true;
		return false;
	}
};

#endif

