#ifndef CONVERSIONS_H
#define CONVERSIONS_H

using Int128 = __int128;

/**
 * Convert Int128 to char* with optional decimal character
 * Maximal value of Int128 is 39 decimal digits long. Including optional sign, decimal separator and null character, effective buffer length is 42.
 * With safety margin, this function expects a buffer at least 48 characters long. 
 * @param value		Value to convert
 * @param scale		Number of decimal digits
 * @param buffer		Magic buffer (at least 48 bytes)
 * @return			Number as char*
 */
char* dec128_to_pchar(Int128 value, size_t scale, char* buffer) {
	bool sign = (value < 0);
	char* s = &buffer[47];
	size_t w = 0;
	*(--s) = 0;
	if (sign)
		value = -value;
	while (value) {
		*(--s) = (value % 10) + '0';
		if ((++w) == scale)
			*(--s) = '.';
		value /= 10;
	}
	while (w < scale) {
		*(--s) = '0';
		if ((++w) == scale)
			*(--s) = '.';
	}
	if (w == scale)
		*(--s) = '0';
	if (sign)
		*(--s) = '-';
	return s;
}

/**
 * Convert Int128 to char*
 * Maximal value of Int128 is 39 decimal digits long. Including optional sign and null character, effective buffer length is 41.
 * With safety margin, this function expects a buffer at least 48 characters long. 
 * @param value		Value to convert
 * @param buffer		Magic buffer (at least 48 bytes)
 * @return			Number as char*
 */
char* int128_to_pchar(Int128 value, char* buffer) {
	bool sign = (value < 0);
	char* s = &buffer[47];
	*(--s) = 0;
	if (sign)
		value = -value;
	if (value == 0)
		*(--s) = '0';
	else {
		while (value) {
			*(--s) = (value % 10) + '0';
			value /= 10;
		}
		if (sign)
			*(--s) = '-';
	}
	return s;
}

#endif /* CONVERSIONS_H */

