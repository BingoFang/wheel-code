
/*
嵌入式系统开发者应该对 Little-endian 和 Big-endian 模式非常了解。采用 Little-endian 模式的 CPU
对操作数的存放方式低字低址，高位高址；而 Big-endian 模式对操作数的存放方式是高字低址，低字高址。
节。
*/

int checkCPU(void)
{
    union w
    {
        int a;
        int b;
    } c;
    c.a = 1;
    return (c.b == 1);
}

int main(void)
{
    int result;
    result = checkCPU();
    if (!result)
        printf("Big endina");
    else
        printf("Little endina");
    return 0;
}

/* Linux中判断大小端语句 */
#define ENDIANNESS ((char)endian_test.l)
static union
{
    char c[4];
    unsigned long l;
} endian_test = { { 'l', '?', '?', 'b' } };
int main(void)
{
    char result;
    result = ENDIANNESS;
    if (result == 'b')
        printf("Big endina");
    else if (result == 'l')
        printf("Little endina");
    else
        printf("No");
    return 0;
}