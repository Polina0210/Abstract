#ifndef CS_H
#define CS_H
#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "Unit.h"

class CsClass : public ClassUnit {
private:
    //Тип доступа для класса
    Flags accessesModifier_class;
public:
    explicit CsClass(const std::string& name, Flags flag = PRIVATE ) : ClassUnit(name) {

        //У C# имеется 6 типов доступа, поэтому изменяем размер на 6
        m_fields.resize(6);
        //Также у C# есть тип доступа самого класса, где также имеется 6 вариантов
        //Если пользователь ввёл не тот символ, программа выдаст соответствующее сообщение об этом
        if(flag >= 6)
        {
            throw std::runtime_error("In C# there is no accessModifire like this");
        }
        else
        {
            accessesModifier_class = flag;
        }

    }

    void add(const std::shared_ptr< Unit >& unit, Flags flags) override
    {
        //Проверка, существует ли объект
        if(unit == nullptr)
        {
            return;
        }
        //Определение типа доступа
        int accessModifier = PRIVATE;
        if(flags < 6)
        {
            accessModifier = flags;
        }
        //В случае некорректного ввода, программа выдаст соответствующее сообщение
        else
        {
            throw std::runtime_error("In C# there is no accessModifire like this");
        }
        //Добавление метода и его типа доступа
        m_fields[accessModifier].push_back(unit);
    }



    std::string compile(unsigned int level = 0) const override {
        //Сначала объявляем тип доступа класса
        std::string classAccess = "";
        if(accessesModifier_class != PRIVATE)
        {
            classAccess = ACCESS_MODIFIERS[accessesModifier_class] + ' ';
        }
        //После того, как определили тип доступа класса, переходим к непосредственному объявлению класса
        std::string result = generateShift(level) + classAccess + "class " + m_name + " {\n";
        //Аналогично С++, определяем методы и их тела
        for( size_t i = 0; i < m_fields.size(); ++i )
        {
            if( m_fields[ i ].empty() )
            {
                continue;
            }
            result += ACCESS_MODIFIERS[ i ] + ":\n";
            for (const auto& f : m_fields[i]) {
                result += f->compile(level + 1);
                result += "\n";
            }
        }
        result += generateShift(level) + "};\n";
        return result;

    }

};

class CsMethod : public MethodUnit {
public:
    CsMethod(const std::string& name, const std::string& returnType, Flags flags) : MethodUnit(name, returnType, flags) { }
    std::string compile(unsigned int level = 0) const override {
        std::string result = generateShift(level);

        if (m_flags & PUBLIC) result += "public ";
            else if (m_flags & PROTECTED) result += "protected ";
                else result += "private ";
        if (m_flags & STATIC) result += "static ";
            else if (m_flags & VIRTUAL) result += "virtual ";
                 else if (m_flags & ABSTRACT) result += "abstract ";
        if( m_flags & ASYNC ) result += "async ";
        if( m_flags & UNSAVE ) result += "unsave ";

        result += m_returnType + " ";
        result += m_name + "()";

        if (m_flags & CONST) result += " const";

        result += " {\n";

        for (const auto& b : m_body) result += b->compile(level + 1);
        result += generateShift(level) + "}\n";
        return result;
    }
};

class CsPrintOperator : public PrintOperatorUnit {
public:
     explicit CsPrintOperator(const std::string& text) : PrintOperatorUnit(text) { }

    std::string compile(unsigned int level = 0) const override {
        return generateShift(level) + "Console.WriteLine( \"" + m_text + "\"); \n";
    }
};

class CsFactory : public UnitFactory {
public:
    std::shared_ptr < ClassUnit > CreateClass(const std::string& name, Unit::Flags, Unit::Flags) const override {
        return std::make_shared< CsClass >(name);
    }
    std::shared_ptr < MethodUnit > CreateMethod(const std::string& name, const std::string& returnType, Unit::Flags flags) const override {
        return std::make_shared< CsMethod >(name, returnType, flags);
    }
    std::shared_ptr < PrintOperatorUnit > CreatePrintOperator(const std::string& text) const override {
        return std::make_shared< CsPrintOperator >(text);
    }
};
#endif // CS_H
