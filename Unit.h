#ifndef UNIT_H
#define UNIT_H
#pragma once
#include <iostream>
#include <memory>
#include <vector>

class Unit {
public:
    using Flags = unsigned int;
    virtual ~Unit() = default;

    //Виртуальная функция-член предназначена для добавления
    //вложенных элементов (передача происходит через умный указатель)
    // умный указатель - std::shared_ptr
    //По умолчанию add() выбрасывает исключение.

    virtual void add(const std::shared_ptr <Unit>&, Flags) {throw std::runtime_error("Do not supported");}

    //Compile() - Функция генерирует код на C++, соответствующий
    //содержимому элемента. Результат возвращается в виде строки
    //std::string.
    //В качестве аргумента функция принимает параметр level, указывающий
    //на уровень вложенности узла дерева. Это требуется для корректной
    //расстановки отступов в начале строк генерируемого кода.
    virtual std::string compile(unsigned int level = 0) const = 0;


protected:

    //generateShift() - Вспомогательная функция-член всего лишь
    //возвращает строку, состоящую из нужного числа пробелов. Результат
    //зависит от уровня вложенности.
    virtual std::string generateShift(unsigned int level) const {
        static const auto DEFAULT_SHIFT = "    ";
        std::string result;
        for (unsigned int i = 0; i < level; i++) result += DEFAULT_SHIFT;
        return result;
    }
};

class ClassUnit : public Unit{
public:
    enum AccessModifier {
        PUBLIC,
        PROTECTED,
        PRIVATE
    };
    static const std::vector< std::string > ACCESS_MODIFIERS;
    explicit ClassUnit(const std::string& name) : m_name(name) {m_fields.resize(ACCESS_MODIFIERS.size());}



    void add(const std::shared_ptr<Unit>& unit, Flags flags) {
        unsigned int accessModifier = PRIVATE;
        if (flags < ACCESS_MODIFIERS.size()) accessModifier = flags;
        m_fields[accessModifier].push_back(unit);
    }
    virtual std::string compile(unsigned int level = 0) const = 0;
    virtual ~ClassUnit() {}
protected:
    using Fields = std::vector <std::shared_ptr <Unit> >;
    std::string m_name;
    std::vector<Fields> m_fields;
};

const std::vector< std::string > ClassUnit::ACCESS_MODIFIERS = {"public", "protected", "private"};

class MethodUnit : public Unit {
public:
    enum Modifier {
        PUBLIC = 1,
        PROTECTED = 1 << 1,
        PRIVATE = 1 << 2,
        STATIC = 1 << 3,
        CONST = 1 << 4,
        VIRTUAL = 1 << 5
    };
    MethodUnit(const std::string& name, const std::string& returnType, Flags flags) : m_name(name), m_returnType(returnType), m_flags(flags) { }
    void add(const std::shared_ptr<Unit>& unit, Flags /* flags */ = 0) {m_body.push_back(unit);}
    virtual std::string compile(unsigned int level = 0) const = 0;
    virtual ~MethodUnit() {}
protected:
    std::string m_name;
    std::string m_returnType;
    Flags m_flags;
    std::vector< std::shared_ptr<Unit> > m_body;
};

class PrintOperatorUnit : public Unit {
public:
    explicit PrintOperatorUnit(const std::string& text) : m_text(text) { }
    virtual std::string compile(unsigned int level = 0) const = 0;
    virtual ~PrintOperatorUnit() {}
protected:
    std::string m_text;
};

class UnitFactory {
public:
    virtual std::shared_ptr <ClassUnit> CreateClass(const std::string& name) = 0;
    virtual std::shared_ptr <MethodUnit> CreateMethod(const std::string& name, const std::string& returnType, Unit::Flags flags) = 0;
    virtual std::shared_ptr <PrintOperatorUnit> CreatePrintOperator(const std::string& text) = 0;
    virtual ~UnitFactory() {};
};

class CreateCode {
public:
    UnitFactory *factory;
    CreateCode(UnitFactory *anotherfactory) { factory = anotherfactory; }

    std::string generateProgram() {
    std::shared_ptr<ClassUnit> myClass = factory->CreateClass("myClass");
    myClass->add(factory->CreateMethod("testFunc1", "void", MethodUnit::PUBLIC), ClassUnit::PUBLIC);
    myClass->add(factory->CreateMethod("testFunc2", "void", MethodUnit::STATIC | MethodUnit::PRIVATE), ClassUnit::PRIVATE);
    myClass->add(factory->CreateMethod("testFunc3", "void", MethodUnit::VIRTUAL | MethodUnit::CONST | MethodUnit::PUBLIC), ClassUnit::PUBLIC);
    std::shared_ptr< MethodUnit > method = factory->CreateMethod("testFunc4", "void", MethodUnit::STATIC | MethodUnit::PROTECTED);
    method->add(factory->CreatePrintOperator(R"(Hello, world!)"));
    myClass->add(method, ClassUnit::PROTECTED);
    return myClass->compile();
    }
};


#endif // UNIT_H
