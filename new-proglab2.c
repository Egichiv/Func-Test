#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <inttypes.h>
#include <math.h>

struct uint1024_t
{
    uint8_t *number;
    uint16_t length;
};

void Error();
void run();
struct uint1024_t from_uint(unsigned int x);
void printf_value(struct uint1024_t uint_num);
struct uint1024_t add_op(struct uint1024_t x, struct uint1024_t y);
struct uint1024_t subtr_op(struct uint1024_t x, struct uint1024_t y);
struct uint1024_t mult_op(struct uint1024_t x, struct uint1024_t y);
struct uint1024_t remove_zeroes(struct uint1024_t new_number, uint16_t new_len);

int main()
{
    //struct uint1024_t number;
    // number.number = malloc(2 * sizeof(struct uint1024_t));
    // number.number[0] = 255;
    // printf("%" PRIu8 "\n", number.number[0]);
    run();
    return 0;
}

void run()
{
    unsigned int num = 12345;
    struct uint1024_t uint_num;
    uint_num = from_uint(num);
    unsigned int a = 1000;
    unsigned int b = 1000;
    struct uint1024_t uint_num1 = from_uint(a);
    struct uint1024_t uint_num2 = from_uint(b);
    uint16_t i = 0;
    printf_value(uint_num);
    struct uint1024_t new_unit_num = add_op(uint_num1, uint_num2);
    i = 0;
    printf("%u %u\n", a, b);
    printf_value(new_unit_num);
    struct uint1024_t new_unit_num2 = subtr_op(uint_num1, uint_num2);
    printf_value(new_unit_num2);
    unsigned int a2 = pow(2,32) - 1;
    unsigned int b2 = a2;
    struct uint1024_t uint_num3 = from_uint(a2);
    struct uint1024_t uint_num4 = from_uint(b2);
    struct uint1024_t new_unit_num3 = mult_op(uint_num3, uint_num4);
    printf("%u %u\n", a2, b2);
    printf_value(new_unit_num3);
    /*struct uint1024_t new_unit_num4 = mult_op(new_unit_num3, new_unit_num3);
    struct uint1024_t new_unit_num5 = mult_op(new_unit_num4, new_unit_num4);
    struct uint1024_t new_unit_num6 = mult_op(new_unit_num5, new_unit_num5);
    struct uint1024_t new_unit_num7 = mult_op(new_unit_num6, new_unit_num6);
    struct uint1024_t new_unit_num8 = mult_op(new_unit_num7, new_unit_num7);
    printf_value(new_unit_num8);*/
}


/*генерация из числа*/
struct uint1024_t from_uint(unsigned int x)
{
    struct uint1024_t number;
    number.length = 0;
    unsigned int quotient0 = x;
    uint8_t firs_value = 0;
    while ((quotient0 > 0) | firs_value == 0)
    {
        quotient0 = (unsigned int)(quotient0 / 100);
        number.length++;
        firs_value = 1;
    }
    number.number = malloc(number.length * sizeof(struct uint1024_t));
    unsigned int quotient = x;
    uint8_t divisor = 1;
    uint16_t i = 0;
    firs_value = 0;
    while ((quotient > 0) | firs_value == 0)
    {
        divisor = (uint8_t)(quotient % 100);
        quotient = (unsigned int)(quotient / 100);
        number.number[i] = divisor;
        i++;
        firs_value = 1;
    }
    return number;
}

/*Вывод*/
void printf_value(struct uint1024_t uint_num)
{
    int i = uint_num.length - 1;
    //printf("%" PRIu16 " -len\n", uint_num.length);
    while (i >= 0)
    {
        if (uint_num.number[i] > 9)
        {
            printf("%" PRIu8 "", uint_num.number[i]);
            i--;
        }
        else if (i < (uint_num.length - 1))
        {
            printf("0%" PRIu8 "", uint_num.number[i]);
            i--;
        }
        else
        {
            printf("%" PRIu8 "", uint_num.number[i]);
            i--;
        }
    }
    printf("\n");
}

/*Сложение*/
struct uint1024_t add_op(struct uint1024_t x, struct uint1024_t y)
{
    struct uint1024_t new_number;
    uint16_t new_len = x.length;
    if (y.length > x.length)
    {
        new_len = y.length;
    }
    new_number.number = malloc(new_len * sizeof(struct uint1024_t));
    uint16_t x_len = x.length;
    uint16_t y_len = y.length;
    uint8_t x_2digits = 0;
    uint8_t y_2digits = 0;
    uint8_t new_2digits = 0;
    uint8_t carry_byte = 0;
    uint16_t i = 0;
    while (i <= new_len)
    {
        if ((i < x_len) && (i < y_len))
        {
            x_2digits = x.number[i];
            y_2digits = y.number[i];
            new_2digits = (x_2digits + y_2digits + carry_byte) % 100;
            carry_byte = (uint8_t)((x_2digits + y_2digits + carry_byte) / 100);
            new_number.number[i] = new_2digits;
            i++;
        }
        else if (i < x_len)
        {
            x_2digits = x.number[i];
            new_2digits = (x_2digits + carry_byte) % 100;
            carry_byte = (uint8_t)((x_2digits + carry_byte) / 100);
            new_number.number[i] = new_2digits;
            i++;
        }
        else if (i < y_len)
        {
            y_2digits = y.number[i];
            new_2digits = (y_2digits + carry_byte) % 100;
            carry_byte = (uint8_t)((y_2digits + carry_byte) / 100);
            new_number.number[i] = new_2digits;
            i++;
        }
        else if (carry_byte > 0)
        {
            new_len = new_len + 1;
            new_number.length = new_len;
            new_number.number = realloc(new_number.number, new_len * sizeof(struct uint1024_t));
            new_number.number[i] = carry_byte;
            carry_byte = 0;
            i++;
        }
        else
            break;
    }
    new_number.length = new_len;
    return new_number;
}

/*Вычетание*/
struct uint1024_t subtr_op(struct uint1024_t x, struct uint1024_t y)
{
    struct uint1024_t new_number;
    uint16_t new_len = x.length;
    if (x.length < y.length)
    {
        Error();
        exit(0);
    }
    new_number.number = malloc(new_len * sizeof(struct uint1024_t));
    uint16_t x_len = x.length;
    uint16_t y_len = y.length;
    uint8_t x_2digits = 0;
    uint8_t y_2digits = 0;
    uint8_t new_2digits = 0;
    uint8_t carry_byte = 0;
    uint8_t carry_byte_new = 0;
    uint16_t i = 0;
    while (i < new_len)
    {
        if ((i < x_len) && (i < y_len))
        {
            x_2digits = x.number[i];
            y_2digits = y.number[i];
            carry_byte_new = 0;
            if ((x_2digits - y_2digits - carry_byte) < 0)
            {
                carry_byte_new = 1;
            }
            new_2digits = (x_2digits - y_2digits + carry_byte_new * 100 - carry_byte) % 100;
            carry_byte = carry_byte_new;
            new_number.number[i] = new_2digits;
            i++;
        }
        else if (i < x_len)
        {
            x_2digits = x.number[i];
            new_2digits = (x_2digits - carry_byte) % 100;
            carry_byte = 0;
            new_number.number[i] = new_2digits;
            i++;
        }
        if ((carry_byte > 0) && (i == (x_len)) && (i == (y_len)))
        {
            Error();
            exit(0);
        }
    }

    new_number.length = new_len;
    new_number = remove_zeroes(new_number, new_len);
    return new_number;
}

/*Умножение*/
struct uint1024_t mult_op(struct uint1024_t x, struct uint1024_t y)
{
    struct uint1024_t new_number;
    uint16_t x_len = x.length;
    uint16_t y_len = y.length;
    uint16_t new_len = x_len + y_len - 1;
    new_number.number = calloc(new_len, sizeof(struct uint1024_t));
    uint8_t x_2digits = 0;
    uint8_t y_2digits = 0;
    uint8_t new_2digits = 0;
    uint8_t carry_byte = 0;
    uint16_t x_ind = 0;
    int new_digits;

    for (uint16_t i = 0; i < y_len; i++)
    {
        x_ind = 0;
        for (uint16_t j = 0; j < x_len; j++)
        {
            x_2digits = x.number[j];
            y_2digits = y.number[i];
            new_digits = (new_number.number[i + j] + x_2digits * y_2digits + carry_byte);
            new_number.number[i + j] = new_digits % 100;
            carry_byte = (uint16_t)(new_digits / 100);
            x_ind++;
        }
        while (carry_byte > 0)
        {
            if ((i + x_ind) >= new_len)
            {
                new_len += 1;
                new_number.number = realloc(new_number.number, new_len * sizeof(struct uint1024_t));
            }
            new_digits = (new_number.number[i + x_ind] + carry_byte);
            new_number.number[i + x_ind] = new_digits % 100;
            carry_byte = (uint16_t)(new_digits / 100);
            x_ind++;
        }
    }
    new_number.length = new_len;
    new_number = remove_zeroes(new_number, new_len);
    return new_number;
}

struct uint1024_t remove_zeroes(struct uint1024_t new_number, uint16_t new_len)
{
    uint16_t zero_num = 0;
    uint8_t all_before_zero = 1;
    if (new_number.length == 1){
        return new_number;
    }
    for (int i = new_number.length - 1; i >= 0; i--)
    {
        if ((new_number.number[i] == 0) && (all_before_zero == 1))
        {
            zero_num += 1;
        }
        else
        {
            all_before_zero = 0;
        }
    }
    if (zero_num > 0)
    {
        new_len = new_len - 1;
        new_number.length = new_len;
        new_number.number = realloc(new_number.number, new_len * sizeof(struct uint1024_t));
    }
    return new_number;
}
void Error()
{
    printf("Subtraction value is negative \n");
}
