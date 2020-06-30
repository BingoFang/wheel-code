/* printf库函数 */
#include <stdarg.h>
void printf( char *format, ... )
{
	va_list arg;
	char ch;
	char *str;
	va_start( arg, format );
/*** Get the format characters one by one.*/
	while( ( ch = *format++ ) != ’\0’ )
	{
		if( ch != ’%’ ){
		/*
		** Not a format code –– print the character verbatim.
		*/
		putchar( ch );
		continue;
	}
/*
** We got a % –– now get the format code and use it to format
** the next argument.
*/
	switch( *format != ’\0’ ? *format++ : ’\0’ )
	{
		case ’d’:
		print_integer( va_arg( arg, int ) );
		break;
		case ’f’:
		print_float( va_arg( arg, float ) );
		break;
		case ’c’:
		putchar( va_arg( arg, int ) );
		break;
		case ’s’:
		str = va_arg( arg, char * );
		while( *str != ’\0’ )
		putchar( *str++ );
		break;
	}
}