/*! \file string_manip.h
	\brief Some helper functions for strings
*/

#ifndef _STRING_MANIP_H_
#define _STRING_MANIP_H_


/*! Right justify a string. Use spaces on the right side to align the result. 
	provide: "my_str***" , Where * is a space
	result:  "***my_str"
	\param s The string to be right justified.
*/
void str_right_justify(char* s);


/*! Pad a string with characters on the right
	\param s The terminated string
	\param pad_char	The character to pad the string with
	\param total_length The desired total length of the string
*/
void str_pad_right(char * s, char pad_char, unsigned char total_length);


/*! Left justifies a string.
	\param s The string to be left justified.
*/
void str_left_justify(char* s);

#endif
