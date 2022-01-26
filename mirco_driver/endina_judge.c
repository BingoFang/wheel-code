
/*
Ƕ��ʽϵͳ������Ӧ�ö� Little-endian �� Big-endian ģʽ�ǳ��˽⡣���� Little-endian ģʽ�� CPU
�Բ������Ĵ�ŷ�ʽ���ֵ�ַ����λ��ַ���� Big-endian ģʽ�Բ������Ĵ�ŷ�ʽ�Ǹ��ֵ�ַ�����ָ�ַ��
�ڡ�
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

/* Linux���жϴ�С����� */
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