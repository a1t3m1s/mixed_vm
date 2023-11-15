#include "commands.h"
#include "machine.h"
using namespace std;

// КОМАНДЫ ПЕРВОГО ТИПА //
void ret::operator()(const cell& code,Machine& processor) noexcept
{
    uint16_t adr = processor.regs[constants::first_reg];
    processor.setIP(adr-commandTypes::firstT);
}

void r_math::operator()(const cell& code,Machine& processor) noexcept
{
    uint16_t adr = processor.get_value(constants::stack_pointer); //получаем адрес вершины стека
    uint16_t new_adr = adr+sizeof(float);
    cell data;
    data.data = processor.unload_data(sizeof(float),adr); // получаем значение из стека
    float x = data.r;
    data.data = processor.unload_data(sizeof(float),new_adr); // получаем значение из стека
    float y = data.r;
    float result = get_result(data._1B.b,x,y);
    set_flags<float>(processor,result);

    cout << "Полученный результат: " << result << endl;

    processor.set_value(constants::stack_pointer,new_adr);
    data.r = result;
    processor.load_data(sizeof(float),data.data,new_adr);
}

// КОМАНДЫ ВТОРОГО ТИПА //
void load::operator()(const cell& code,Machine& processor) noexcept
{
    if(code._2B.get_a1() == 1)
        processor.set_value(code._2B.a2,processor.get_value(code._2B.a3));
}

void i_math::operator()(const cell& code,Machine& processor) noexcept
{
    uint16_t adr = processor.get_value(code._2B.get_a1());
    int32_t x = processor.unload_data(sizeof(int32_t),adr); // значение [а1]

    uint16_t joint_adr = processor.get_value(code._2B.a2 + code._2B.a3);
    int32_t y = processor.unload_data(sizeof(int32_t),joint_adr); // значение [а2+a3]

    int32_t result = get_result(x,y);
    set_flags<int32_t>(processor,result);

    cout << "Полученный результат: " << result << endl;

    cell data;
    data.i = result;

    processor.load_data(sizeof(int32_t),data.data,adr);
}

void u_math::operator()(const cell& code,Machine& processor) noexcept
{
    uint16_t x = processor.get_value(code._2B.a2);
    uint16_t y = processor.get_value(code._2B.a3);

    uint16_t result = get_result(x,y);
    set_flags<uint16_t>(processor,result);

    cout << "Полученный результат: " << result << endl;

    processor.set_value(code._2B.get_a1(),result);
}

void jmp::operator()(const cell& code,Machine& processor) noexcept
{
    int32_t displacement = code._2B.get_a1()+code._2B.a2+code._2B.a3;
    processor.setIP(processor.getIP()+displacement-commandTypes::secondT);
}

void jcond::operator()(const cell& code,Machine& processor) noexcept
{
    if(get_predicate(processor.get_ZFlag(),processor.get_NFlag()))
    {
        command* helper = new jmp();
        helper->operator()(code,processor);
    }
}

// КОМАНДЫ ТРЕТЬЕГО ТИПА //
void call::operator()(const cell& code,Machine& processor) noexcept
{
    // помещаем адрес возврата в регистр a0 //
    processor.set_value(constants::first_reg,processor.getIP()+commandTypes::thirdT);
    processor.setIP(code._3B.const_adr-commandTypes::thirdT);
}

void push::operator()(const cell& code,Machine& processor) noexcept
{
    processor.set_value(constants::stack_pointer,processor.get_value(constants::stack_pointer)-sizeof(float));
    uint16_t adr = processor.get_value(constants::stack_pointer);
    cell helper;
    helper.r = processor.unload_data(sizeof(float),code._3B.const_adr);
    processor.load_data(sizeof(float),helper.data,adr);
}

void pop::operator()(const cell& code,Machine& processor) noexcept
{
    uint16_t adr = processor.get_value(constants::stack_pointer);
    uint16_t save_adr = code._3B.const_adr;
    cell helper;
    helper.data = processor.unload_data(sizeof(float),adr);
    if(code._3B.b == 1)
        processor.set_value(constants::stack_pointer,adr);
    processor.load_data(sizeof(float),helper.data,save_adr);
}

void cjmp::operator()(const cell& code,Machine& processor) noexcept
{
    if(code._3B.b == 0)
        processor.setIP(code._3B.const_adr-commandTypes::thirdT);
    else
    {
        cell helper;
        helper.data = processor.unload_data(sizeof(uint16_t),code._3B.const_adr);
        processor.setIP(helper.u-commandTypes::thirdT);
    }
}

void cjcond::operator()(const cell& code,Machine& processor) noexcept
{
    if(get_predicate(processor.get_ZFlag(),processor.get_NFlag()))
    {
        command* helper = new cjmp();
        helper->operator()(code,processor);
    }
}

template<typename type>
void input<type>::operator()(const cell& code,Machine& processor) noexcept
{
    type value;
    cin >> value;

    cell helper;
    if(is_same<type,uint16_t>::value)
        helper.u = value;
    else if(is_same<type,int32_t>::value)
        helper.i = value;
    else if(is_same<type,float>::value)
        helper.r = value;

    uint16_t adr = code._3B.const_adr;
    processor.load_data(sizeof(type),helper.data,adr);
}
template class input<uint16_t>;
template class input<int32_t>;
template class input<float>;

template<typename type>
void output<type>::operator()(const cell& code,Machine& processor) noexcept
{
    cell helper;
    helper.data = processor.unload_data(sizeof(type),code._3B.const_adr);
    cout << "Число типа \"";
    if(is_same<type,uint16_t>::value)
        cout << "uint16_t\": " << helper.u << endl;
    else if(is_same<type,int32_t>::value)
        cout << "int32_t\": " <<  helper.i << endl;
    else if(is_same<type,float>::value)
        cout << "float\": " <<  helper.r << endl;
}

template class output<uint16_t>;
template class output<int32_t>;
template class output<float>;

// КОМАНДЫ ЧЕТВЕРТОГО ТИПА //
void cload::operator()(const cell& code,Machine& processor) noexcept
{
    if(code._4B.get_a1() == 2)
        processor.set_value(code._4B.a2,code._4B.const_adr);
    else if(code._4B.get_a1() == 3)
    {
        uint16_t helper = processor.get_value(code._4B.a3 + code._4B.const_adr);
        processor.set_value(code._4B.a2,helper);
    }
}
