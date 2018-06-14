#ifndef LLVM_OBJECTIVECCLASSINFO_H
#define LLVM_OBJECTIVECCLASSINFO_H

#include "llvm/ADT/StringRef.h"

#include <vector>

namespace llvm {
    namespace ObjectiveC {

        enum ClassType {
            Initialized,
            Dummy,
        };

        class Method;
        class IVAR;
        class Protocol;

        class Base {
        public:
            Base(StringRef Classname) : Classname(Classname), Address(0) {}
            virtual ~Base() {}

            StringRef getClassName() {
                return Classname;
            }

            StringRef getSuperclass() {return Superclass;};
            void setSuperclass(StringRef s) {Superclass = s;};

            void setAddress(uint64_t a) { Address = a; }
            uint64_t getAddress() { return Address; }

            virtual ClassType getType() = 0;
        protected:
            StringRef Classname;
            StringRef Superclass;
            uint64_t Address;
        };

        class Class: public Base {
        public:
            typedef std::vector<std::string> ProtocolList_t;

            Class(StringRef Classname) : Base(Classname) {}

            virtual ClassType getType() { return ClassType::Initialized; };



            void addMethod(Method M);
            bool getMethod(StringRef Methodname, Method &M);

            void addIVAR(IVAR ivar);
            bool getIVAR(StringRef IVARName, IVAR &ivar);
            bool getIVAR(uint64_t IVAROffset, IVAR &ivar);

            void addProtocol(std::string protocol) {
                protocols.push_back(protocol);
            }

            ProtocolList_t &getProtocolList() {return protocols;}
        private:

            typedef std::vector<Method> MethodList_t;
            MethodList_t Methods;

            typedef std::vector<IVAR> IVARList_t;
            IVARList_t IVARs;

            ProtocolList_t protocols;
        };

        class DummyClass: public Base {
        public:
            DummyClass(StringRef Classname): Base(Classname) {}
            virtual ClassType getType() { return ClassType::Dummy; };
        };

        class Method {
            friend class Class;
        public:
            typedef std::pair<uint64_t, std::string> RegType_t;
            Method(StringRef Methodname, uint64_t IMP, StringRef type) : Methodname(Methodname), IMP(IMP), type(type), Parent(0) { parseType();}
            Method(const Method &M) : Methodname(M.Methodname), IMP(M.IMP), type(M.type), Parent(M.Parent), regTypes(M.regTypes){}

            StringRef getMethodname() {return Methodname;}
            std::vector<RegType_t> getRegTypes() {return regTypes; };
        private:
            StringRef Methodname;
            uint64_t IMP;
            std::string type;
            Base *Parent;
            std::vector<RegType_t> regTypes;

            void parseType();
        };

        class IVAR {
            friend class Class;
        public:
            IVAR(StringRef IVARName, uint64_t OffsetPtr, StringRef IVARType) : IVARName(IVARName), OffsetPtr(OffsetPtr), IVARType(IVARType) {};
            IVAR(const IVAR &ivar) : IVARName(ivar.IVARName), OffsetPtr(ivar.OffsetPtr), IVARType(ivar.IVARType) {};
            IVAR() {};

//            StringRef getID() {return std::string(ParentClass.str() + "." + IVARName.str());};
            std::string getID() {return ParentClass.str() + IVARName.str();};

            StringRef getType() {return IVARType;}
        private:
            StringRef IVARName;
            uint64_t OffsetPtr;
            StringRef ParentClass;
            StringRef IVARType;
        };

        class Protocol {
        public:
            Protocol(std::string protocolname) : protocolName(protocolname) {

            }

            Protocol(const Protocol &p) : protocolName(p.protocolName), instanceMethods(p.instanceMethods), classMethods(p.classMethods) {

            }

            Protocol() {}

            void addInstanceMethod(Method m) { instanceMethods.push_back(m); }
            void addClassMethod(Method m) { classMethods.push_back(m); }

            std::vector<Method> getInstanceMethods() {return instanceMethods;}
            std::vector<Method> getClassMethods() {return classMethods;}
        private:
            std::string protocolName;
            std::vector<Method> instanceMethods;
            std::vector<Method> classMethods;
        };
    }
}

#endif //LLVM_OBJECTIVECCLASSINFO_H
