#ifndef JAVA_H
#define JAVA_H
#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "Unit.h"

class JavaClass : public ClassUnit {
private:
    //тип доступа для класса
    Flags accessesModifier_class;
    //модификатор для класса
    Flags Modifier;
public:
    explicit JavaClass(const std::string& name, Flags classAccess = PUBLIC /*public, private, protected*/, Flags classModifier = 0 /*Final, abstract*/ ) : ClassUnit(name) {
        //У Java имеется 3 типа доступа, поэтому изменяем размер на 3
        m_fields.resize(3);
        Modifier = 0;
        //Определение модификатора
        if (classModifier & MethodUnit::ABSTRACT)
        {
           Modifier = MethodUnit::ABSTRACT;
        }

        else if(classModifier & MethodUnit::FINAL)
        {
            Modifier = MethodUnit::FINAL;
        }
        //определение типа доступа класса
        if(classAccess<2)
        {
            accessesModifier_class = classAccess;
        }
        else
        {
            accessesModifier_class = PUBLIC;
            throw std::runtime_error("In Java there is no accessModifire for classes like this");
        }
     }

    void add(const std::shared_ptr< Unit >& unit, Flags flags) override
    {
        //Проверка существования объекта
        if(unit == nullptr)
        {
            return;
        }
        //Определение типа доступа функции
        int accessModifier = PUBLIC;
        if(flags < 3)
        {
            accessModifier = flags;
        }
        else
        {
            throw std::runtime_error("In Java there is no accessModifire for classes like this");
        }
        //Добавление в вектор метода и его типа доступа
        m_fields[accessModifier].push_back(unit);
    }

    std::string compile(unsigned int level = 0) const {
        std::string classAccess = "";
        std::string Modifier = "";

        classAccess = ACCESS_MODIFIERS[accessesModifier_class] + ' ';

        if(accessesModifier_class & MethodUnit::FINAL)
        {
            Modifier = "final";
        }
        else if (accessesModifier_class & MethodUnit::ABSTRACT)
        {
            Modifier = "abstract";
        }
        //После того, как определили тип доступа класса, переходим к непосредственному объявлению класса
        std::string result = generateShift(level) + classAccess + Modifier + "class " + m_name + " {\n";
        //Определяем методы и их тела
        for (size_t i = 0; i < ACCESS_MODIFIERS.size(); i++) {
            //if (m_fields[i].empty()) continue;
            for (const auto& f : m_fields[i]) {
                result += f->compile(level + 1);
                result += "\n";
            }
        }
        result += generateShift(level) + "};\n";
        return result;
    }
};



class JavaMethod : public MethodUnit {
public:
    JavaMethod(const std::string& name, const std::string& returnType, Flags flags) : MethodUnit(name, returnType, flags) { }
    std::string compile(unsigned int level = 0) const {
        std::string result = generateShift(level);

        if (m_flags & PUBLIC) result += "public ";
            else if (m_flags & PROTECTED) result += "protected ";
                else result += "private ";


        if(m_flags & STATIC) result += "static ";

        if(m_flags & ABSTRACT) result += "abstract ";

        // при объявлении метода не могут стоять одновременно abstract и final
        else if( m_flags & FINAL ) result += "final ";



        result += m_returnType + " ";
        result += m_name + "()";
        result += " {\n";

        for (const auto& b : m_body) result += b->compile(level + 1);
        result += generateShift(level) + "}\n";
        return result;
    }
};

class JavaPrintOperator : public PrintOperatorUnit {
public:
    JavaPrintOperator(const std::string& text) : PrintOperatorUnit(text) { }

    std::string compile(unsigned int level = 0) const {
        return generateShift(level) + "System.out.println( \"" + m_text + "\"); \n";
    }
};

class JavaFactory : public UnitFactory {
public:
    std::shared_ptr < ClassUnit > CreateClass(const std::string& name, Unit::Flags classAccess = ClassUnit::PUBLIC, Unit::Flags classModifier = 0) const override {
        return std::make_shared< JavaClass >(name);
    }
    std::shared_ptr < MethodUnit > CreateMethod(const std::string& name, const std::string& returnType, Unit::Flags flags)  const override {
        return std::make_shared< JavaMethod >(name, returnType, flags);
    }
    std::shared_ptr < PrintOperatorUnit > CreatePrintOperator(const std::string& text) const override {
        return std::make_shared< JavaPrintOperator >(text);
    }
};
#endif // JAVA_H
