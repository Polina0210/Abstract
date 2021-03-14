#ifndef CPP_H
#define CPP_H
#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "Unit.h"

class CppClass : public ClassUnit {
public:
    explicit CppClass(const std::string& name) : ClassUnit(name) {
        //У С++ иммется три типа доступа, поэтому размер меняем на три
        m_fields.resize(3);
    }

    std::string compile(unsigned int level = 0) const override {
        std::string result = generateShift(level) + "class " + m_name + "{\n";
        for (size_t i = 0; i < ACCESS_MODIFIERS.size(); i++) {
            if (m_fields[i].empty()) continue;
            result += ACCESS_MODIFIERS[i] + ":\n";
            for (const auto& f : m_fields[i]) result += f->compile(level + 1);
            result += "\n";
        }
        result += generateShift(level) + "};\n";
        return result;
    }
};

class CppMethod : public MethodUnit {
public:
    explicit CppMethod(const std::string& name, const std::string& returnType, Flags flags) : MethodUnit(name, returnType, flags) { }

    std::string compile(unsigned int level = 0) const override {
        std::string result = generateShift(level);

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

class CppPrintOperator : public PrintOperatorUnit {
public:
    explicit CppPrintOperator(const std::string& text) : PrintOperatorUnit(text) { }

    std::string compile(unsigned int level = 0) const override {
        return generateShift(level) + "printf( \"" + m_text + "\"); \n";
    }
};

class CppFactory : public UnitFactory {
public:
    std::shared_ptr < ClassUnit > CreateClass(const std::string& name, Unit::Flags, Unit::Flags) const override {
        return std::make_shared< CppClass >(name);
    }
    std::shared_ptr < MethodUnit > CreateMethod(const std::string& name, const std::string& returnType, Unit::Flags flags) const override{
        return std::make_shared< CppMethod >(name, returnType, flags);
    }
    std::shared_ptr < PrintOperatorUnit > CreatePrintOperator(const std::string& text) const override {
        return std::make_shared< CppPrintOperator >(text);
    }
};
#endif // CPP_H
