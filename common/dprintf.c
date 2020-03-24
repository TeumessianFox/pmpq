#include <stdarg.h>
#include <limits.h>
#include <stdint.h>
#include "dprintf.h"

#define TIMEOUT_ERROR -1
#define UNDEFINED_IDENTIFIER -2
#define MAX_LEN_INT 11

static void unsigned_long_long_to_str(uint64_t n, char *buffer, int *counter)
{
  int i = 0;
  uint64_t n1 = n;

  while(n1!=0){
    buffer[i++] = n1%10+'0';
    n1=n1/10;
    (*counter)++;
  }

  buffer[i] = '\0';

  for(int t = 0; t < i/2; t++){
    buffer[t] ^= buffer[i-t-1];
    buffer[i-t-1] ^= buffer[t];
    buffer[t] ^= buffer[i-t-1];
  }

  if(n == 0){
    buffer[0] = '0';
    buffer[1] = '\0';
    (*counter)++;
  }
}

static void signed_int_to_str(int n, char *buffer, int *counter)
{
  int i = 0;
  int isNeg = n<0;
  unsigned int n1 = isNeg ? -n : n;

  while(n1!=0){
    buffer[i++] = n1%10+'0';
    n1=n1/10;
    (*counter)++;
  }

  if(isNeg)
    buffer[i++] = '-';

  buffer[i] = '\0';

  for(int t = 0; t < i/2; t++){
    buffer[t] ^= buffer[i-t-1];
    buffer[i-t-1] ^= buffer[t];
    buffer[t] ^= buffer[i-t-1];
  }

  if(n == 0){
    buffer[0] = '0';
    buffer[1] = '\0';
    (*counter)++;
  }
}

static void unsigned_int_to_str(uint32_t n, char *buffer, int *counter)
{
  int i = 0;
  unsigned int n1 = n;

  while(n1!=0){
    buffer[i++] = n1%10+'0';
    n1=n1/10;
    (*counter)++;
  }

  buffer[i] = '\0';

  for(int t = 0; t < i/2; t++){
    buffer[t] ^= buffer[i-t-1];
    buffer[i-t-1] ^= buffer[t];
    buffer[t] ^= buffer[i-t-1];
  }

  if(n == 0){
    buffer[0] = '0';
    buffer[1] = '\0';
    (*counter)++;
  }
}

static void int_to_hex_str(uint32_t n, char *buffer, int *counter)
{
  int i = 0;
  unsigned int n1 = n;
  while(n1!=0){
    int t = 0;
    t = n1 % 16;

    if(t<10){
      buffer[i] = t+48;
      i++;
    }else{
      buffer[i] = t+55;
      i++;
    }
    n1=n1/16;
    (*counter)++;
  }

  buffer[i] = '\0';

  for(int t = 0; t < i/2; t++){
    buffer[t] ^= buffer[i-t-1];
    buffer[i-t-1] ^= buffer[t];
    buffer[t] ^= buffer[i-t-1];
  }

  if(n == 0){
    buffer[0] = '0';
    buffer[1] = '\0';
    (*counter)++;
  }
}

int kprintf(char *format,...)
{
  int count = 0;
  char *traverse = format;
  char c;
  char *s;

  va_list arg;
  va_start(arg,format);

  while(*traverse != '\0'){
    if(*traverse == '%'){
      traverse++;
      int status = 0;
      char str[50];
      switch(*traverse){
      case '0':
        traverse++;
        if(*traverse == '8'){
          traverse++;
          int count_char = 0;
          if(*traverse == 'p'){
            void *p = va_arg(arg, void *);
            int_to_hex_str((uint32_t)p, str, &count_char);
            hal_send_str("0x");
            count += 2;
            int length = 8 - count_char;
            while(length > 0){
              hal_send_char('0');
              length--;
              count_char++;
            }
            hal_send_str(str);
            count += count_char;
            break;
          }else{
            return UNDEFINED_IDENTIFIER;
          }
        }else{
          return UNDEFINED_IDENTIFIER;
        }
      case 'c':
        c = va_arg(arg, int);
        hal_send_char(c);
        count++;
        break;
      case 's':
        s = va_arg(arg, char *);
        count += hal_send_str(s);
        break;
      case 'x':
        ;
        int_to_hex_str(va_arg(arg, uint32_t), str, &count);
        hal_send_str(str);
        break;
      case 'i':
        signed_int_to_str(va_arg(arg, int), str, &count);
        hal_send_str(str);
        break;
      case 'u':
        unsigned_int_to_str(va_arg(arg,uint32_t), str, &count);
        hal_send_str(str);
        break;
      case 'l':
        traverse++;
        if(*traverse == 'l'){
          traverse++;
          if(*traverse == 'u'){
            unsigned_long_long_to_str(va_arg(arg, uint64_t), str, &count);
            hal_send_str(str);
          }
        }
        break;
      case 'p':
        ;
        void *p = va_arg(arg, void *);
        int_to_hex_str((uint32_t)p, str, &count);
        hal_send_str("0x");
        count += 2;
        hal_send_str(str);
        break;
      case '%':
        hal_send_char(*traverse);
        count++;
        break;
      default:
        return UNDEFINED_IDENTIFIER;
      }
    }else{
      hal_send_char(*traverse);
      count++;
    }
    traverse++;
  }

  va_end(arg);
  return count;
}
