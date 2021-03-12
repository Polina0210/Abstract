#include <QCoreApplication>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "Unit.h"
#include "Cpp.h"
#include "Java.h"
#include "Cs.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //std::shared_ptr<CppFactory> UnitFactory(CppFactory);
    UnitFactory *factoryCpp = new CppFactory();
    CreateCode *codeCpp = new CreateCode(factoryCpp);
    std::cout <<"C++ programm:"<<std::endl<< codeCpp->generateProgram() << std::endl;

    UnitFactory *factoryJava = new JavaFactory();
    CreateCode *codeJava = new CreateCode(factoryJava);
    std::cout << "Java programm:" << std::endl << codeJava->generateProgram() << std::endl;


    UnitFactory *factoryCs = new CsFactory();
    CreateCode *codeCs = new CreateCode(factoryCs);
    std::cout << "C# programm:" << std::endl << codeCs->generateProgram() << std::endl;
    system("pause");

    return a.exec();
}

