#include "print.h"

size_t Print::write(const uint8_t *buffer, size_t size) {
	size_t n = 0;
	while (size--) {
	  write(*buffer++);
	  n++;
	}
	return n;
}

size_t Print::print(uint8_t b, int base) {
    return print((uint64)b, base);
}

size_t Print::print(const String &s)
{
  return write(s.c_str(), s.length());
}

size_t Print::print(char c) {
    return write(c);
}

size_t Print::print(const char str[]) {
    return write(str);
}

size_t Print::print(int n, int base) {
    return print((long)n, base);
}

size_t Print::print(unsigned int n, int base) {
    return print((unsigned long)n, base);
}

size_t Print::print(long n, int base) {
  if (base == BYTE)
  {
    return write((uint8_t)n);
  }
  if (n < 0) {
    print('-');
    n = -n;
  }
  return printNumber(n, base);
}

size_t Print::print(unsigned long n, int base) {
  size_t c=0;
  if (base == BYTE) {
    c= write((uint8_t)n);
  } else {
    c= printNumber(n, base);
  }
  return c;
}

size_t Print::print(double n, int digits) {
    return printFloat(n, digits);
}

size_t Print::print(const Printable& x)
{
  return x.printTo(*this);
}

size_t Print::println(const Printable& x)
{
  size_t n = print(x);
  n += println();
  return n;
}

size_t Print::println(void) 
{
	size_t n =  print('\r');
    n += print('\n');
	return n;
}

size_t Print::println(const String &s)
{
  size_t n = print(s);
  n += println();
  return n;
}

size_t Print::println(char c) {
    size_t n = print(c);
    n += println();
	return n;
}

size_t Print::println(const char c[]) {
    size_t n = print(c);
    n += println();
	return n;
}

size_t Print::println(uint8_t b, int base) {
    size_t n = print(b, base);
	n += println();
	return n;
}

size_t Print::println(int n, int base) {
    size_t s = print(n, base);
    s += println();
	return s;
}

size_t Print::println(unsigned int n, int base) {
    size_t s = print(n, base);
    s += println();
	return s;
}

size_t Print::println(long n, int base) {
    size_t s = print((long)n, base);
    s += println();
	return s;
}

size_t Print::println(unsigned long n, int base) {
    size_t s = print((unsigned long)n, base);
    s += println();
	return s;
}

size_t Print::println(double n, int digits) {
    size_t s = print(n, digits);
    s += println();
	return s;
}

size_t Print::printf(const char * format, ...)
{
  int ret_status = 0;

  char buf[256];
  int len;

  va_list ap;
  va_start(ap, format);

  len = vsnprintf(buf, 256, format, ap);
  this->write(buf, len);

  va_end(ap);
  return len;
}

/*
 * Private methods
 */

size_t Print::printNumber(unsigned long n, uint8_t base) {
  unsigned char buf[CHAR_BIT * sizeof(long)];
  unsigned char buf2[CHAR_BIT * sizeof(long)];
  unsigned long i = 0;
  size_t s=0;
  if (n == 0) {
    print('0');
    return 1;
  }

  while (n > 0) {
    buf[i++] = n % base;
    n /= base;
  }

  for (; i > 0; i--) {
    buf2[s++] = (char) (buf[i - 1] < 10 ?
                        '0' + buf[i - 1] :
                        'A' + buf[i - 1] - 10);

//    s += print((char)(buf[i - 1] < 10 ?
//        '0' + buf[i - 1] :
//        'A' + buf[i - 1] - 10));
  }

  s = write(buf2, s);
  return s;
}

size_t Print::printFloat(double number, uint8_t digits)
{
  char buf[256];
  size_t s=0;

  char format[] = "%.0f";
  format[2] += digits;

  s = snprintf(buf, 256, format, number);
  s = write(buf, s);
  return s;
}


#if 0
/* According to snprintf(),
 *
 * nextafter((double)numeric_limits<long long>::max(), 0.0) ~= 9.22337e+18
 *
 * This slightly smaller value was picked semi-arbitrarily. */
#define LARGE_DOUBLE_TRESHOLD (9.1e18)

/* THIS FUNCTION SHOULDN'T BE USED IF YOU NEED ACCURATE RESULTS.
 *
 * This implementation is meant to be simple and not occupy too much
 * code size.  However, printing floating point values accurately is a
 * subtle task, best left to a well-tested library function.
 *
 * See Steele and White 2003 for more details:
 *
 * http://kurtstephens.com/files/p372-steele.pdf
 */
size_t Print::printFloat(double number, uint8_t digits) {
size_t s=0;
    // Hackish fail-fast behavior for large-magnitude doubles
    if (abs(number) >= LARGE_DOUBLE_TRESHOLD) {
        if (number < 0.0) {
            s=print('-');
        }
        s+=print("<large double>");
        return s;
    }

    // Handle negative numbers
    if (number < 0.0) {
        s+=print('-');
        number = -number;
    }

    // Simplistic rounding strategy so that e.g. print(1.999, 2)
    // prints as "2.00"
    double rounding = 0.5;
    for (uint8_t i = 0; i < digits; i++) {
        rounding /= 10.0;
    }
    number += rounding;

    // Extract the integer part of the number and print it
    long long int_part = (long long)number;
    double remainder = number - int_part;
    s+=print(int_part);

    // Print the decimal point, but only if there are digits beyond
    if (digits > 0) {
        s+=print(".");
    }

    // Extract digits from the remainder one at a time
    while (digits-- > 0) {
        remainder *= 10.0;
        int to_print = (int)remainder;
        s+=print(to_print);
        remainder -= to_print;
    }
	return s;
}
#endif
