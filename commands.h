#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

#pragma once
#include <iostream>
#include <typeinfo>
#include "utility.h"

class Machine;

#pragma pack(1)
class command
{public: virtual void operator()(const cell& code,Machine& processor) noexcept = 0;};

// “»œ€  ŒÃÃ¿Õƒ //
class command_1B: public command
{public: command_1B():command(){}};

class command_2B: public command
{public: command_2B():command(){}};

class command_3B: public command
{public: command_3B():command(){}};

class command_4B: public command
{public: command_4B():command(){}};

//  ŒÃ¿Õƒ€ œ≈–¬Œ√Œ “»œ¿ //
class ret: public command_1B
{public: void operator()(const cell& code,Machine& processor) noexcept override;};

class r_math: public command_1B
{
public:
    r_math():command_1B(){}

    void operator()(const cell& code,Machine& processor) noexcept override;
    virtual float get_result(const uint8_t& b,float L,float R) const noexcept = 0;
};
//---------------------------------------------------------------------------//
class radd: public r_math
{
public:
    float get_result(const uint8_t& b, float L, float R) const noexcept override
    {return (L+R);}
};

class rsub: public r_math
{
public:
    float get_result(const uint8_t& b,float L, float R) const noexcept override
    {
        if(b != 1) return (L-R);
        else return (R-L);
    }
};

class rmul: public r_math
{
public:
    float get_result(const uint8_t& b,float L, float R) const noexcept override
    {return (L*R);}
};

class rdiv: public r_math
{
public:
    float get_result(const uint8_t& b,float L, float R) const noexcept override
    {
        if(b != 1) return (L/R);
        else return (R/L);
    }
};
//---------------------------------------------------------------------------//

//  ŒÃ¿Õƒ€ ¬“Œ–Œ√Œ “»œ¿ //
 class load: public command_2B
 {public: void operator()(const cell& code,Machine& processor) noexcept override;};

 class jmp: public command_2B
 {public: void operator()(const cell& code,Machine& processor) noexcept override;};

class u_math: public command_2B
{
public:
    u_math():command_2B(){}

    void operator()(const cell& code,Machine& processor) noexcept override;
    virtual uint16_t get_result(uint16_t L,uint16_t R) const noexcept = 0;
};
//---------------------------------------------------------------------------//
class uadd: public u_math
{
public:
    uint16_t get_result(uint16_t L,uint16_t R) const noexcept override
    {return (L+R);}
};

class usub: public u_math
{
public:
    uint16_t get_result(uint16_t L,uint16_t R) const noexcept override
    {return (L-R);}
};

class umul: public u_math
{
public:
    uint16_t get_result(uint16_t L,uint16_t R) const noexcept override
    {return (L*R);}
};

class udiv: public u_math
{
public:
    uint16_t get_result(uint16_t L,uint16_t R) const noexcept override
    {return (L/R);}
};

class umod: public u_math
{
public:
    uint16_t get_result(uint16_t L,uint16_t R) const noexcept override
    {return (L%R);}
};
//---------------------------------------------------------------------------//

class i_math: public command_2B
{
public:
    i_math():command_2B(){}

    void operator()(const cell& code,Machine& processor) noexcept override;
    virtual int32_t get_result(int32_t L,int32_t R) const noexcept = 0;
};
//---------------------------------------------------------------------------//
class iadd: public i_math
{
public:
    int32_t get_result(int32_t L,int32_t R) const noexcept override
    {return (L+R);}
};

class isub: public i_math
{
public:
    int32_t get_result(int32_t L,int32_t R) const noexcept override
    {return (L-R);}
};

class imul: public i_math
{
public:
    int32_t get_result(int32_t L,int32_t R) const noexcept override
    {return (L*R);}
};

class idiv: public i_math
{
public:
    int32_t get_result(int32_t L,int32_t R) const noexcept override
    {return (L/R);}
};

class imod: public i_math
{
public:
    int32_t get_result(int32_t L,int32_t R) const noexcept override
    {return (L%R);}
};
//---------------------------------------------------------------------------//

class jcond: public command_2B
{
public:
    jcond():command_2B(){}

    void operator()(const cell& code,Machine& processor) noexcept override;
    virtual bool get_predicate(uint8_t Z,uint8_t N) const noexcept = 0;
};
//---------------------------------------------------------------------------//
class jz: public jcond
{
public:
    bool get_predicate(uint8_t Z,uint8_t N) const noexcept
    {return (Z==1);};
};

class jnz: public jcond
{
public:
    bool get_predicate(uint8_t Z,uint8_t N) const noexcept
    {return (Z!=1);};
};

class jg: public jcond
{
public:
    bool get_predicate(uint8_t Z,uint8_t N) const noexcept
    {return (Z!=1 && N!=1);};
};

class jng: public jcond
{
public:
    bool get_predicate(uint8_t Z,uint8_t N) const noexcept
    {return (Z==1 || N==1);};
};

class jl: public jcond
{
public:
    bool get_predicate(uint8_t Z,uint8_t N) const noexcept
    {return (Z!=1 && N==1);};
};

class jnl: public jcond
{
public:
    bool get_predicate(uint8_t Z,uint8_t N) const noexcept
    {return (Z==1 || N!=1);};
};
//---------------------------------------------------------------------------//

//  ŒÃ¿Õƒ€ “–≈“‹≈√Œ “»œ¿ //
 class call:public command_3B
 {public: void operator()(const cell& code,Machine& processor) noexcept override;};

class push: public command_3B
{public: void operator()(const cell& code,Machine& processor) noexcept override;};

class pop: public command_3B
{public: void operator()(const cell& code,Machine& processor) noexcept override;};

class cjmp: public command_3B
{public: void operator()(const cell& code,Machine& processor) noexcept override;};

class cjcond: public command_2B
{
public:
    cjcond():command_2B(){}

    void operator()(const cell& code,Machine& processor) noexcept override;
    virtual bool get_predicate(uint8_t Z,uint8_t N) const noexcept = 0;
};
//---------------------------------------------------------------------------//
class cjz: public cjcond
{
public:
    bool get_predicate(uint8_t Z,uint8_t N) const noexcept
    {return (Z==1);};
};

class cjnz: public cjcond
{
public:
    bool get_predicate(uint8_t Z,uint8_t N) const noexcept
    {return (Z!=1);};
};

class cjg: public cjcond
{
public:
    bool get_predicate(uint8_t Z,uint8_t N) const noexcept
    {return (Z!=1 && N!=1);};
};

class cjng: public cjcond
{
public:
    bool get_predicate(uint8_t Z,uint8_t N) const noexcept
    {return (Z==1 || N==1);};
};

class cjl: public cjcond
{
public:
    bool get_predicate(uint8_t Z,uint8_t N) const noexcept
    {return (Z!=1 && N==1);};
};

class cjnl: public cjcond
{
public:
    bool get_predicate(uint8_t Z,uint8_t N) const noexcept
    {return (Z==1 || N!=1);};
};
//---------------------------------------------------------------------------//

template<typename type>
class input: public command_3B
{public: void operator()(const cell& code,Machine& processor) noexcept override;};

template<typename type>
class output: public command_3B
{public: void operator()(const cell& code,Machine& processor) noexcept override;};

//  ŒÃ¿Õƒ€ ◊≈“¬≈–“Œ√Œ “»œ¿ //
 class cload: public command_4B
 {public: void operator()(const cell& code,Machine& processor) noexcept override;};


#endif // COMMANDS_H_INCLUDED
