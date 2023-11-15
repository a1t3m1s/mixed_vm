#ifndef MACHINE_H_INCLUDED
#define MACHINE_H_INCLUDED

#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "utility.h"
#include "commands.h"

#pragma pack(1)
class Machine
{
public:
    uint8_t *memory;
    std::vector<command*> commands;
    uint16_t regs[8]; //àäğåñíûå ğåãèñòğû

    struct alignas(1) PSW
    {
        uint16_t IP;
        bool N; //<0
        bool Z; //==0
    } psw;
//------------------------------------------------//
    Machine();
    Machine(const Machine& R)=delete;
    Machine& operator=(const Machine& R)=delete;
    ~Machine(){delete [] memory;}
// ÓÑÒÀÍÎÂÊÀ è ÏÎËÓ×ÅÍÈÅ IP //
    void setIP(const uint16_t& IP) noexcept;
    uint16_t getIP() const noexcept;
// ÓÑÒÀÍÎÂÊÀ è ÏÎËÓ×ÅÍÈÅ ÔËÀÃÎÂ //
    void set_NFlag(const bool& N) noexcept;
    bool get_NFlag() const noexcept;
    void set_ZFlag(const bool& Z) noexcept;
    bool get_ZFlag() const noexcept;

    template<typename type>
    friend void set_flags(Machine& processor,const type& result);
// ÏÎËÓ×ÅÍÈÅ ÄÀÍÍÛÕ èç ïàìÿòè //
    uint16_t get_value(const uint8_t& index)const noexcept;
    void set_value(const uint8_t& index,const uint16_t& value) noexcept;

    uint8_t command_type(const uint8_t& code) const noexcept;
// ÇÀÃĞÓÇÊÀ è ÏÎËÓ×ÅÍÈÅ ÄÀÍÍÛÕ èç ïàìÿòè //
    void load_data(const size_t& type,uint32_t value,uint16_t& ID) noexcept;
    uint32_t unload_data(const size_t& type,uint16_t IP) noexcept;
// ÂÛÏÎËÍÅÍÈÅ ÏĞÎÃĞÀÌÌÛ //
    void run(const uint16_t& IP);
// ÇÀÃĞÓÇ×ÈÊ //
    friend void uploader(Machine& processor,uint16_t& IP,std::string file);
};
#pragma pop()

// ÓÑÒÀÍÎÂÊÀ ÔËÀÃÎÂ //
template<typename type>
void set_flags(Machine& processor,const type& result)
{
    cell data;
    if(std::is_same<type,uint16_t>::value)
        data.u = result;
    else if(std::is_same<type,int32_t>::value)
        data.i = result;
    else if(std::is_same<type,float>::value)
        data.r = result;

    processor.set_NFlag(data.data >> (8*sizeof(type)-1));
    processor.set_ZFlag(!result);
}

#endif // MACHINE_H_INCLUDED
