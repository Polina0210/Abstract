#ifndef JAVA_H
#define JAVA_H
#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "Unit.h"

class JavaClass : public ClassUnit {
public:
    JavaClass(const std::string& name) : ClassUnit(name) { }

    std::string compile(unsigned int level = 0) const {
        std::string result = generateShift(level) + "class " + m_name + "{\n";
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
        if (m_flags & STATIC) result += "static ";
            else if (!(m_flags & VIRTUAL)) result += "final ";


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
    std::shared_ptr < ClassUnit > CreateClass(const std::string& name) {
        return std::make_shared< JavaClass >(name);
    }
    std::shared_ptr < MethodUnit > CreateMethod(const std::string& name, const std::string& returnType, Unit::Flags flags) {
        return std::make_shared< JavaMethod >(name, returnType, flags);
    }
    std::shared_ptr < PrintOperatorUnit > CreatePrintOperator(const std::string& text) {
        return std::make_shared< JavaPrintOperator >(text);
    }
};
#endif // JAVA_H
