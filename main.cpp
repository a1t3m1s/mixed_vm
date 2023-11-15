#include <iostream>
#include <windows.h>
#include "utility.h"
#include "commands.h"
#include "machine.h"
using namespace std;

int main(int argc,char* argv[])
{
    SetConsoleOutputCP(1251);
    cout << "\nЗагрузка файла \"" << argv[1] << "\" . . .";
    Machine processor;
    uint16_t IP;
    uploader(processor,IP,string(argv[1]));
    processor.run(IP);
    cout << endl;
    system("pause");
    return 0;
}
