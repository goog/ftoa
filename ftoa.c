#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



// reserve digits digit number after point
int print_int(long int n, char *buf, size_t size, int digits)
{
    int sign = 0;
    int len = 0;  // return string length  
    char *start = buf; // original start pointer
    char *max_pointer = buf+size;  // the address cant be written
    if(buf == NULL || size <= 0)
    {
        return -1;
    }

    if(digits >= size)
    {
        fprintf(stderr, "the buffer size is small\n");
        return -1;
    }
    
    if(n < 0)
    {
        sign = 1;  // neg flag
        n = -n;
    }
    
    
    while(n && buf < max_pointer)
    {
        int mod = n % 10;
        *buf++ = '0' + mod;

        n = n / 10;
    }

    while(buf < (start + digits) && buf < max_pointer)  // for fractional part only
    {
        *(buf++) = '0';   // padding 0
    }

    if(sign)
    {
        if(buf < max_pointer)
        {
            *buf++ = '-';
        }   
        else
        {
            fprintf(stderr, "cant write sign\n");
            return -1;
        }
        
    }
    
    
    buf--;  // back to last char
    if(buf >= start)  // cal string length
    {
        len = buf - start + 1;
    }
    
    while(start < buf)  // reverse the string buffer
    {
        char tmp = *buf;
        *buf = *start;
        *start = tmp;

        start++;
        buf--;
    }

    return len;
}


int ftoa(double number, char *buffer, size_t size, int digits)
{
    
    int len = 0;
    int frac_len = 0;
    if(digits < 0)
    {
        return -1;
    }
    
    long int_part = (long)number;
    double frac_part = fmod(number, 1.0);
    
    
    len = print_int(int_part, buffer, size, 0);
    if(len == 0 && size > 0)  // integer part is 0
    {
        if(number < 0)  // -0.05 will enter the if condition
        {
            if(size < 3)
            {
                fprintf(stderr, "buffer is small\n");
                return -1;
            }
            
            *buffer = '-';
            *(buffer+1) = '0';
            len = 2;
        }
        else
        {
            *buffer = '0';
            len++;
        }
        
    }
    
    //printf("int part string %s\n", buffer);

    if(frac_part < 0)
        frac_part = -frac_part;

    // change fractional part to long int
    long int frac = frac_part * pow(10, digits);
    if(frac == 0)  // no fractional part 
    {
        return len;
    }
    // there is some loss 
    

    if(len < size)
    {
        *(buffer+len) = '.';
        len++;
    }
    
    if(len >= size)
    {
        fprintf(stderr, "buffer used up\n");
        return -1;
    }

    //printf("frac buffer size %ld\n", size - len);
    frac_len = print_int(frac, buffer+len, size - len, digits);
    if(frac_len < 0)
    {
        return -1;
    }
    
    return (len + frac_len);
}



int main()
{
    char buf[512] = {0};
    double test_number = 0;
    ftoa(test_number, buf, 512, 6);
    printf("number %f, convert string %s\n", test_number, buf);

    memset(buf, 0, 512);
    test_number = 12.345678967;
    ftoa(test_number, buf, 512, 6);
    printf("number %f, convert string %s\n", test_number, buf);
    
    //ftoa(-1000.14159265357, buf, 512, 6);
    memset(buf, 0, 512);
    test_number = -1000.14159265357;
    ftoa(test_number, buf, 512, 6);
    printf("number %f, convert string %s\n", test_number, buf);


    memset(buf, 0, 512);
    test_number = -0.14159265357;
    ftoa(test_number, buf, 512, 6);
    printf("number %f, convert string %s\n", test_number, buf);

    memset(buf, 0, 512);
    test_number = 0.14159265357;
    ftoa(test_number, buf, 512, 6);
    printf("number %f, convert string %s\n", test_number, buf);

    char buf2[5] = {0};
    int ret;
    test_number = 0.14159265357;
    ret = ftoa(test_number, buf2, 5, 6);
    printf("number %f, ret %d convert string %s\n", test_number, ret, buf2);
}
