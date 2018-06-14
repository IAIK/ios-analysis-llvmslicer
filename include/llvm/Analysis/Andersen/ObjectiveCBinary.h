#ifndef LLVM_OBJECTIVECBINARY_H
#define LLVM_OBJECTIVECBINARY_H

#include "llvm/Object/MachO.h"

#include <map>
#include "llvm/Analysis/Andersen/ObjectiveCClassInfo.h"
#include "llvm/ADT/SmallString.h"

namespace llvm {

    namespace ObjectiveC {
        class IVAR;
    }

    class ObjectiveCBinary {
    public:
        typedef std::shared_ptr<llvm::ObjectiveC::Base> BaseClassPtr_t;
        typedef std::shared_ptr<llvm::ObjectiveC::Class> ClassPtr_t;
        typedef std::map<std::string, BaseClassPtr_t> ClassMap_t;
        typedef std::map<std::string, ObjectiveC::Protocol> ProtocolMap_t;

        ObjectiveCBinary(llvm::StringRef Path);
        bool isValidAddress(uint64_t Address);

        bool isAddressInSection(uint64_t Address, object::section_iterator Section);
        bool isSelectorRef(uint64_t Address);
        bool isClassRef(uint64_t Address);
        bool isMethname(uint64_t Address);
        bool isData(uint64_t Address);
        bool isConst(uint64_t Address);
        bool isIVAR(uint64_t Address);

        bool getValue(const uint64_t Address, uint64_t &Value);
        bool getData(const uint64_t Address, StringRef &Content);

        bool getClass(const uint64_t Address, StringRef &Classname);
        bool getSelector(const uint64_t Address, StringRef &Selectorname);

        bool isCFString(const uint64_t Address);
        bool isCString(const uint64_t Address);

        bool isConstValue(const uint64_t Address);

        StringRef getString(uint64_t Address);

        std::deque<std::string> getMethodCandidates(StringRef Type, StringRef Selector, bool Meta);
        std::string getFunctionName(uint64_t Address);

        template<typename T> T getRAWData(uint64_t address){
            T r;
            StringRef data = MachO->getData();
            StringRef slice = data.slice(address - 0x100000000, sizeof(T));
            r = *((T*)slice.data());
            return r;
        }

        std::map<uint64_t, ObjectiveC::IVAR> &getIVARs() {return IVARs;};

        ClassMap_t &getClasses() {return Classes;}
        ClassMap_t &getMetaClasses() {return Metaclasses;}
        ProtocolMap_t &getProtocols() {return protocolMap;}
        object::section_iterator getSectionIterator(StringRef Name);
        std::string getSectionName(uint64_t address);
    private:
        llvm::object::OwningBinary<llvm::object::ObjectFile> ObjectFile;
        llvm::object::MachOObjectFile *MachO;

        object::DataRefImpl SelRefsDataRef;
        object::DataRefImpl MethnameDataRef;


        void loadSections();
        void loadClasses();
        void doBinding();

        void parseClass(uint64_t DataAddress, bool MetaClass = false);
        std::vector<ObjectiveC::Method> parseMethods(uint64_t MethodsPtr, uint64_t SignatureStartPtr = 0, uint64_t *signaturesIdx = NULL);

        llvm::MachO::segment_command_64 getSegment(uint64_t SegmentNo);

        void addClass(StringRef ClassName, uint64_t Address);



        std::map<uint64_t, StringRef> BindInfo;
        std::map<uint64_t, StringRef> ClassRefs;
        std::map<uint64_t, StringRef> SelRefs;
        std::map<uint64_t, StringRef> ClassNames;
        std::map<uint64_t, ObjectiveC::IVAR> IVARs;
        ClassMap_t Classes;
        ClassMap_t Metaclasses;
        ProtocolMap_t protocolMap;
    };
}

#endif //LLVM_OBJECTIVECBINARY_H
