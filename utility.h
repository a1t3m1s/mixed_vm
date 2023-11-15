#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

// ���� ������ ---------------------------------//
struct alignas(1) command_1Type
{
    uint8_t code:7;
    uint8_t b:1;
};

struct alignas(1) command_2Type
{
    uint8_t code:7;
    uint8_t b:1;
    uint8_t m:2;
    uint8_t a2:3;
    uint8_t a3:3;

    void set_a1(const uint8_t& address) noexcept
    {
        b=address/4;
        m=address%4;
    }

    uint8_t get_a1() const noexcept
    {return (b*4+m);}

    void set_a2(const uint8_t& address) noexcept
    {a2 = address&7;}

    void set_a3(const uint8_t& address) noexcept
    {a3 = address&7;}
};

struct alignas(1) command_3Type
{
    uint8_t code:7;
    uint8_t b:1;
    uint16_t const_adr:16;
};

struct alignas(1) command_4Type
{
    uint8_t code:7;
    uint8_t a1_hi:1;
    uint8_t a1_lo:2;
    uint8_t a2:3;
    uint8_t a3:3;
    uint16_t const_adr:16;

    uint8_t get_a1() const noexcept
    {return (a1_hi*4+a1_lo);}

    uint8_t get_a2() const noexcept
    {return a2;}

    uint8_t get_a3() const noexcept
    {return a3;}

    void set_a1(const uint8_t& address) noexcept
    {
        a1_hi = (address&7)/4;
        a1_lo = (address&7)%4;
    }

    void set_a2(const uint8_t& address) noexcept
    {a2 = address&7;}

    void set_a3(const uint8_t& address) noexcept
    {a3 = address&7;}
};
//---------------------------------------------//

union cell
{
    uint32_t data; //��� ������ (� ������) � ������
    int32_t i; //����� ��������
    uint16_t u; //����� �����������
    float r; //�������
// ���� �������� //
    command_1Type _1B;
    command_2Type _2B;
    command_3Type _3B;
    command_4Type _4B;
};

enum constants
{
    memorySize = 65535,
    first_reg = 0,
    stack_pointer = 7
};

enum commandTypes:size_t {firstT=1,secondT,thirdT,fourthT};

enum commandCode:size_t
{
// ���������� ������ //
    STOP=0,
// ������� ���������� //
    RADD=1,
    RSUB=2,
    RMUL=3,
    RDIV=4,
// ������� �� ������������ //
    RET=5,
// �������� ���������� //
    UADD=6,
    USUB=7,
    UMUL=8,
    UDIV=9,
    UMOD=10,
// ������� ���������� //
    IADD=11,
    ISUB=12,
    IMUL=13,
    IDIV=14,
    IMOD=15,
// �������� �������� ��������� (16bit)//
    LOAD=16,
// ����������� ������� �� ��������� //
    JMP=17,
// �������� �������� �� ��������� //
    JZ=18, // X==Y
    JNZ=19, // X!=Y
    JG=20, // X>Y
    JNG=21, // X<=Y
    JL=22, // X<Y
    JNL=23, // X>=Y
// �������� � ���� //
    PUSH=24,
// ������� �� ����� //
    POP=25,
// ����������� ������� //
    CJMP=26,
// �������� �������� //
    CJZ=27, // X==Y
    CJNZ=28, // X!=Y
    CJG=29, // X>Y
    CJNG=30, // X<=Y
    CJL=31, // X<Y
    CJNL=32, // X>=Y
// ������� � ������������ //
    CALL=33,
// ����� �� ����� //
    IOUT=34,
    UOUT=35,
    ROUT=36,
// ���� ������������� //
    IIN=37,
    UIN=38,
    RIN=39,
// �������� �������� ��������� (32bit)//
    CLOAD=40
};

#endif // UTILITY_H_INCLUDED
