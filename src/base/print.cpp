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
