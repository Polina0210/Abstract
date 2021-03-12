#ifndef CS_H
#define CS_H
#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "Unit.h"

class CsClass : public ClassUnit {
public:
    CsClass(const std::string& name) : ClassUnit(name) { }

    std::string compile(unsigned int level = 0) const {
        std::string result = generateShift(level) + "class " + m_name + "{\n";
        for (size_t i = 0; i < ACCESS_MODIFIERS.size(); i++) {
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
    std::string compile(unsigned int level = 0) const {
        std::string result = generateShift(level);

        if (m_flags & PUBLIC) result += "public ";
            else if (m_flags & PROTECTED) result += "protected ";
                else result += "private ";
        if (m_flags & STATIC) result += "static ";
            else if (m_flags & VIRTUAL) result += "virtual ";

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
    CsPrintOperator(const std::string& text) : PrintOperatorUnit(text) { }

    std::string compile(unsigned int level = 0) const {
        return generateShift(level) + "Console.WriteLine( \"" + m_text + "\"); \n";
    }
};

class CsFactory : public UnitFactory {
public:
    std::shared_ptr < ClassUnit > CreateClass(const std::string& name) {
        return std::make_shared< CsClass >(name);
    }
    std::shared_ptr < MethodUnit > CreateMethod(const std::string& name, const std::string& returnType, Unit::Flags flags) {
        return std::make_shared< CsMethod >(name, returnType, flags);
    }
    std::shared_ptr < PrintOperatorUnit > CreatePrintOperator(const std::string& text) {
        return std::make_shared< CsPrintOperator >(text);
    }
};
#endif // CS_H
