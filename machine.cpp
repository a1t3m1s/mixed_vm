#include "machine.h"
using namespace std;

// УСТАНОВИТЬ и ПОЛУЧИТЬ IP--------------------------------------------------//
void Machine::setIP(const uint16_t& IP) noexcept
{psw.IP = IP;}

uint16_t Machine::getIP() const noexcept
{return psw.IP;}
//---------------------------------------------------------------------------//

// УСТАНОВИТЬ и ПОЛУЧИТЬ N flag----------------------------------------------//
void Machine::set_NFlag(const bool& N) noexcept
{psw.N = N;}

bool Machine::get_NFlag() const noexcept
{return psw.N;}
//---------------------------------------------------------------------------//

// УСТАНОВИТЬ и ПОЛУЧИТЬ Z flag----------------------------------------------//
void Machine::set_ZFlag(const bool& Z) noexcept
{psw.Z = Z;}

bool Machine::get_ZFlag() const noexcept
{return psw.Z;}
//---------------------------------------------------------------------------//

// ИЗМЕНИТЬ и ПОЛУЧИТЬ ЗНАЧЕНИЕ в регистре//
uint16_t Machine::get_value(const uint8_t& index)const noexcept
{return regs[index];}

void Machine::set_value(const uint8_t& index,const uint16_t& value) noexcept
{regs[index] = value;}

// ВОЗВРАЩАЕТ ТИП КОМАНДЫ //
uint8_t Machine::command_type(const uint8_t& code) const noexcept
{
    if(code < 6) return commandTypes::firstT;
    else if(code < 24) return commandTypes::secondT;
    else if(code < 40) return commandTypes::thirdT;
    else return commandTypes::fourthT;
}

// СОЗДАНИЕ //
Machine::Machine()
{
    memory=new uint8_t[constants::memorySize];
    uint8_t counter = 128; //число комманд
    commands.assign(counter,nullptr);

    commands[commandCode::RADD] = new radd();
    commands[commandCode::RSUB] = new rsub();
    commands[commandCode::RMUL] = new rmul();
    commands[commandCode::RDIV] = new rdiv();
    commands[commandCode::RET] = new ret();
    commands[commandCode::UADD] = new uadd();
    commands[commandCode::USUB] = new usub();
    commands[commandCode::UMUL] = new umul();
    commands[commandCode::UDIV] = new udiv();
    commands[commandCode::UMOD] = new umod();
    commands[commandCode::IADD] = new iadd();
    commands[commandCode::ISUB] = new isub();
    commands[commandCode::IMUL] = new imul();
    commands[commandCode::IDIV] = new idiv();
    commands[commandCode::IMOD] = new imod();
    commands[commandCode::LOAD] = new load();
    commands[commandCode::JMP] = new jmp();
    commands[commandCode::JZ] = new jz();
    commands[commandCode::JNZ] = new jnz();
    commands[commandCode::JG] = new jg();
    commands[commandCode::JNG] = new jng();
    commands[commandCode::JL] = new jl();
    commands[commandCode::JNL] = new jnl();
    commands[commandCode::PUSH] = new push();
    commands[commandCode::POP] = new pop();
    commands[commandCode::CJMP] = new cjmp();
    commands[commandCode::CJZ] = new cjz();
    commands[commandCode::CJNZ] = new cjnz();
    commands[commandCode::CJG] = new cjg();
    commands[commandCode::CJNG] = new cjng();
    commands[commandCode::CJL] = new cjl();
    commands[commandCode::CJNL] = new cjnl();
    commands[commandCode::CALL] = new call();
    commands[commandCode::IOUT] = new output<int32_t>();
    commands[commandCode::UOUT] = new output<uint16_t>();
    commands[commandCode::ROUT] = new output<float>();
    commands[commandCode::IIN] = new input<int32_t>();
    commands[commandCode::UIN] = new input<uint16_t>();
    commands[commandCode::RIN] = new input<float>();
    commands[commandCode::CLOAD] = new cload();
}

// ЗАГРУЗКА и ПОЛУЧЕНИЕ ДАННЫХ -----------------------------------------------//
void Machine::load_data(const size_t& type,uint32_t value,uint16_t& ID) noexcept
{
    size_t counter = 0;
    while(counter < type)
    {
        uint8_t helper = (value >> (counter*8))&255;
        memory[ID] = helper;
        ++ID;
        ++counter;
    }
}

uint32_t Machine::unload_data(const size_t& type,uint16_t IP) noexcept
{
    uint32_t value = 0;
    size_t counter = 0;

    while(counter < type)
    {
        uint32_t helper = memory[IP];
        helper <<= (counter*8);
        value |= helper;
        ++IP;
        ++counter;
    }
    return value;
}
//---------------------------------------------------------------------------//

// ВЫПОЛНЕНИЕ ПРОГРАММЫ //
void Machine::run(const uint16_t& IP)
{
    setIP(IP); //установить IP
    uint8_t code = unload_data(commandTypes::firstT,getIP())&127;
    cell data; //через него будем получать данные

    cout << "\nВыполнение программы:" << endl << endl;

    while(code!=commandCode::STOP)
    {
        uint8_t commandT = command_type(code);
        data.data = unload_data(commandT,getIP());

        cout << "IP = " << getIP() << "     Команда: ";

        if(commandT == 1)
            cout << (int)data._1B.code << " " << (int)data._1B.b << endl;
        else if(commandT == 2)
            cout << (int)data._2B.code << " " << (int)data._2B.get_a1() << " " << (int)data._2B.a2 << " " << (int)data._2B.a3 <<endl;
        else if(commandT==3)
            cout << (int)data._3B.code << " " << (int)data._3B.b << " " << (int)data._3B.const_adr<< endl;
        else  cout << (int)data._4B.code << " " << (int)data._4B.get_a1() <<  " " << (int)data._4B.get_a2() <<  " " << (int)data._4B.get_a3() <<  " " << (int)data._4B.const_adr << endl;

        if(commands[code])
            commands[code]->operator()(data,*this);
        cout << endl;

        setIP(getIP()+commandT);
        code = unload_data(commandTypes::firstT,getIP())&127;
    }
}

// ЗАГРУЗЧИК //
void uploader(Machine& processor,uint16_t& IP,string file)
{
    ifstream f(file);
    if(!f)
    {
        cerr << "\nОшибка чтения файла!\n";
        return;
    }
    uint16_t ID = 0; // адрес загрузки программы
    while(!f.eof())
    {
        string read;
        getline(f,read);
        if(read.size() == 0) continue;

        istringstream parse(read);

        char teg;
        parse >> teg;

        cell data;
        data.data = 0;

        if(teg == 'l')
        {
            parse >> ID;
            processor.regs[constants::stack_pointer]=ID-1; // устанавливаем указатель стека SP
        }
        else if(teg == 's') parse >> IP;
        else if(teg == 'u')
        {
            uint16_t value;
            parse >> value;
            data.u = value;
            processor.load_data(sizeof(value),data.data,ID);
        }
        else if(teg == 'i')
        {
            uint32_t value;
            parse >> value;
            data.i = value;
            processor.load_data(sizeof(value),data.data,ID);
        }
        else if(teg == 'r')
        {
            float value;
            parse >> value;
            data.r = value;
            processor.load_data(sizeof(value),data.data,ID);
        }
        else if(teg == 'c')
        {
            uint16_t code; //КОП
            parse >> code;

            uint8_t type = processor.command_type(code);

            if(type == 1)
            {
                command_1Type command_1T;
                command_1T.code = code;
                uint8_t b;
                parse >> b;
                command_1T.b = b;
                data._1B = command_1T;
            }
            else if(type == 2)
            {
                command_2Type command_2T;
                command_2T.code = code;
                uint8_t a;
                parse >> a;
                command_2T.set_a1(a);
                parse >> a;
                command_2T.set_a2(a);
                parse >> a;
                command_2T.set_a3(a);
                data._2B = command_2T;
            }
            else if(type == 3)
            {
                command_3Type command_3T;
                command_3T.code = code;
                uint8_t b;
                parse >> b;
                command_3T.b = b;
                uint16_t address;
                parse >> address;
                command_3T.const_adr = address;
                data._3B = command_3T;
            }
            else if(type == 4)
            {
                command_4Type command_4T;
                command_4T.code = code;
                uint8_t a;
                parse >> a;
                command_4T.set_a1(a);
                parse >> a;
                command_4T.set_a2(a);
                parse >> a;
                command_4T.set_a3(a);
                uint16_t address;
                parse >> address;
                command_4T.const_adr = address;
                data._4B = command_4T;
            }
            processor.load_data(type,data.data,ID);
        }
        else if(teg == 'e')
        {
            command_1Type command_1T = {0,0};
            data._1B = command_1T;
            processor.load_data(commandTypes::firstT,data.data,ID);
        }
        else if(teg == ';') continue;
    }
    f.close();
    cout << "\n\nПрограмма успешно загружена!" << endl;
}
