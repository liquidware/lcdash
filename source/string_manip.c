#include <stdlib.h>
#include <inttypes.h>
#include <string.h>


void str_right_justify(char* s)
{
unsigned char space_cnt=0;
unsigned char len = strlen(s);
unsigned char x = len-1;

	while(s[x]==32)
	 {
	 space_cnt++;
	 x--;
	 }
	
	if (space_cnt > 0)
		{
		for(x=0;x< (len-space_cnt);x++) 
			s[len-x-1] = s[len-space_cnt-x-1];
		}
}


void str_pad_right(char * s, char pad_char, unsigned char total_length)
{
unsigned char x;
unsigned char len = strlen(s);
unsigned char amount = total_length - len;
unsigned char index = len-1;
	
	for (x=0;x<amount; x++)
		{
		s[index+x] = pad_char;
		}
	
	s[index+x]=0x00; //terminate the string

}




void str_left_justify(char* s)
{
int space_cnt=0;
int x;

	while(s[space_cnt]==32) 
		space_cnt++;


	for(x=0;x<strlen(s)-space_cnt;x++) 
		s[x] = s[x+space_cnt];

	s[x]=0x00;
}
