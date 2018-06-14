#include <llvm/ADT/StringExtras.h>
#include <llvm/Object/SymbolicFile.h>
#include "llvm/Analysis/Andersen/ObjectiveCBinary.h"

#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/LEB128.h"
#include "llvm/Support/Debug.h"
#include <sstream>


using namespace llvm;

#define DEBUG_TYPE "OBJC_BINARY"

#define SEC_SELREFS "__objc_selrefs"
#define SEC_CLASSREFS "__objc_classrefs"
#define SEC_METHNAME "__objc_methname"
#define SEC_OBJC_DATA "__objc_data"
#define SEC_DATA "__data"
#define SEC_CONST "__objc_const"
#define SEC_CLASSNAME "__objc_classname"
#define SEC_CLASSLIST "__objc_classlist"
#define SEC_SUPERREF "__objc_superrefs"
#define SEC_IVAR "__objc_ivar"
#define SEC_TEXT "__text"
#define SEC_CFSTRING "__cfstring"
#define SEC_CSTRING "__cstring"
#define SEC_GOT "__got"
#define SEC_METHTYPE "__objc_methtype"

#define ISA_OFFSET 0
#define SUPER_OFFSET 8
#define DATA_OFFSET 32

#define NAME_OFFSET 24
#define METHOD_OFFSET 32
#define PROTOCOL_OFFSET 40
#define IVAR_OFFSET 48
#define PROP_OFFSET 64

#define OBJC_CLASS_ID "_OBJC_CLASS_$_"
#define OBJC_METACLASS_ID "_OBJC_METACLASS_$_"

#define ADDSUBSUP(sub, sup) \
  if (!Classes[sub])\
    Classes[sub] = BaseClassPtr_t(new ObjectiveC::DummyClass(sub));\
  Classes[sub]->setSuperclass(sup);\
  if (!Metaclasses[sub])\
    Metaclasses[sub] = BaseClassPtr_t(new ObjectiveC::DummyClass(sub));\
  Metaclasses[sub]->setSuperclass(sup);


ObjectiveCBinary::ObjectiveCBinary(llvm::StringRef Path) {
    auto O = object::MachOObjectFile::createObjectFile(Path);
    ObjectFile = std::move(O.get());
    MachO = dyn_cast<object::MachOObjectFile>(ObjectFile.getBinary());

    loadSections();
    doBinding();
    loadClasses();


//    Classes["NSMutableData"]->setSuperclass("NSData");
//    Metaclasses["NSMutableData"]->setSuperclass("NSData");
    ADDSUBSUP("NSMutableData", "NSData")
    ADDSUBSUP("NSData", "NSObject")
    ADDSUBSUP("NSString", "NSObject")
    ADDSUBSUP("NSMutableString", "NSString")
    ADDSUBSUP("UIButton", "UIControl")
    ADDSUBSUP("UIControl", "UIView")
    ADDSUBSUP("NSMutableDictionary", "NSDictionary")
}

bool ObjectiveCBinary::isValidAddress(uint64_t Address) {
    return false;
}

bool ObjectiveCBinary::isSelectorRef(uint64_t Address) {
    return isAddressInSection(Address, getSectionIterator(SEC_SELREFS));
}

bool ObjectiveCBinary::isClassRef(uint64_t Address) {
    return isAddressInSection(Address, getSectionIterator(SEC_CLASSREFS));
}

bool ObjectiveCBinary::isMethname(uint64_t Address){
    return isAddressInSection(Address, getSectionIterator(SEC_METHNAME));
}

bool ObjectiveCBinary::isData(uint64_t Address) {
    return isAddressInSection(Address, getSectionIterator(SEC_OBJC_DATA));
}

bool ObjectiveCBinary::isConst(uint64_t Address) {
    return isAddressInSection(Address, getSectionIterator(SEC_CONST));
}

bool ObjectiveCBinary::isIVAR(uint64_t Address) {
    return isAddressInSection(Address, getSectionIterator(SEC_IVAR));
}

bool ObjectiveCBinary::isAddressInSection(uint64_t Address, object::section_iterator Section) {
    if (Section == MachO->section_end())
        return false;
    return Section->getAddress() <= Address &&
           Address < (Section->getAddress() + Section->getSize());
}

void ObjectiveCBinary::loadSections() {
    for (object::section_iterator S_it = MachO->section_begin(); S_it != MachO->section_end(); ++S_it) {
        StringRef SectionName;
        if (S_it->getName(SectionName)) {
            continue;
        }
        if (SectionName == SEC_SELREFS) {
            const object::SectionRef &S = *S_it;
            SelRefsDataRef = S.getRawDataRefImpl();
        } else if (SectionName == SEC_METHNAME) {
            const object::SectionRef &S = *S_it;
            MethnameDataRef = S.getRawDataRefImpl();
        }
    }
}

void ObjectiveCBinary::loadClasses() {
    auto getInt8ArrayRef = [] (object::section_iterator Section) {
        StringRef Content;
        Section->getContents(Content);
        return ArrayRef<uint8_t>((uint8_t*) Content.data(), Content.size());
    };

    object::section_iterator DataSection = getSectionIterator(SEC_OBJC_DATA);
    ArrayRef<uint8_t> Data = getInt8ArrayRef(DataSection);

    object::section_iterator ClassList = getSectionIterator(SEC_CLASSLIST);
    ArrayRef<uint8_t> ClassListData = getInt8ArrayRef(ClassList);
    //Iterate over the classes that are defined in this file
    for (unsigned Idx = 0; Idx < ClassListData.size(); Idx += 8) {
        uint64_t DataAddress = *(uint64_t*)ClassListData.slice(Idx).data();
        if (!isAddressInSection(DataAddress, DataSection)) {
            llvm_unreachable("Class does not point to objc_data");
            continue;
        }
        parseClass(DataAddress);
    }


    StringRef Content;

    object::section_iterator ClassRefsSection = getSectionIterator(SEC_CLASSREFS);
    ClassRefsSection->getContents(Content);
    ArrayRef<uint8_t> ClassRefs((uint8_t*) Content.data(), Content.size());

    object::section_iterator ConstSection = getSectionIterator(SEC_CONST);
    ConstSection->getContents(Content);
    ArrayRef<uint8_t> Const((uint8_t*) Content.data(), Content.size());

    object::section_iterator ClassnameSection = getSectionIterator(SEC_CLASSNAME);
    ClassnameSection->getContents(Content);
    ArrayRef<uint8_t> Classnames((uint8_t*) Content.data(), Content.size());

    for (unsigned Idx = 0; Idx < ClassRefs.size(); Idx += 8) {
        uint64_t ObjcDataAddress = *(uint64_t*)ClassRefs.slice(Idx).data();
        uint64_t ClassRefAddress = ClassRefsSection->getAddress() + Idx;
        if (!ObjcDataAddress) {
            if (BindInfo[ClassRefAddress].size()) {
                StringRef BindClassname = BindInfo[ClassRefAddress];
                //TODO: do meta-classes have to be handled here?
                this->ClassRefs[ClassRefAddress] = BindClassname.startswith_lower(OBJC_CLASS_ID) ? BindClassname.substr(strlen(OBJC_CLASS_ID)) : BindClassname.substr(strlen(OBJC_METACLASS_ID));
            }
            continue;
        }
        if (!isData(ObjcDataAddress))
            continue;
        uint64_t ConstAddress = *(uint64_t*)Data.slice(ObjcDataAddress - DataSection->getAddress() + 32).data();
        if (!isConst(ConstAddress)) {
            llvm_unreachable("this should not happen...");
        }
        uint64_t ClassNameAddress = *(uint64_t*)Const.slice(ConstAddress - ConstSection->getAddress() + 24).data();
        if (!isAddressInSection(ClassNameAddress, getSectionIterator(SEC_CLASSNAME))) {
            llvm_unreachable("no classname?");
        }
        StringRef Classname = (const char*)Classnames.slice(ClassNameAddress - ClassnameSection->getAddress()).data();
        this->ClassRefs[ClassNameAddress] = Classname;
        this->ClassRefs[ClassRefsSection->getAddress() + Idx] = Classname;
    }
}

void ObjectiveCBinary::doBinding() {
    ArrayRef<uint8_t> BindOpcodes = MachO->getDyldInfoBindOpcodes();

    uint64_t SegmentNo = 0;
    int64_t Offset = 0;
    StringRef SymbolName;

    for (unsigned Idx = 0; Idx < BindOpcodes.size();) {
        unsigned Opcode = *BindOpcodes.slice(Idx).data();

        if ((Opcode & ~0xF) == MachO::BIND_OPCODE_SET_DYLIB_ORDINAL_IMM) {
            unsigned Imm = Opcode & 0xF;
            Idx++;
        } else if ((Opcode & ~0xF) == MachO::BIND_OPCODE_SET_SYMBOL_TRAILING_FLAGS_IMM) {
            Idx++;
            SymbolName = (const char*)BindOpcodes.slice(Idx).data();
            Idx += SymbolName.size() + 1;
        } else if (Opcode == (MachO::BIND_OPCODE_SET_TYPE_IMM | 0x1)) {
            //Set type pointer
            Idx++;
        } else if ((Opcode & ~0xF) == MachO::BIND_OPCODE_SET_SEGMENT_AND_OFFSET_ULEB) {
            SegmentNo = Opcode & 0xF;
            Idx++;
            unsigned n;
            Offset = decodeULEB128(BindOpcodes.slice(Idx).data(), &n);
            Idx += n;
        } else if (Opcode == MachO::BIND_OPCODE_DO_BIND) {
            MachO::segment_command_64 Segment = getSegment(SegmentNo);
            addClass(SymbolName, Segment.vmaddr + Offset);
            //FIXME: is this add correct???
            Offset += 8;
            Idx++;
        } else if (Opcode == MachO::BIND_OPCODE_ADD_ADDR_ULEB) {
            Idx++;
            unsigned n;
            Offset += decodeULEB128(BindOpcodes.slice(Idx).data(), &n);
            Idx += n;
        } else if ((Opcode & ~0xF) == MachO::BIND_OPCODE_DO_BIND_ADD_ADDR_IMM_SCALED) {
            unsigned Scaled = (Opcode & 0xF) * 8;
            addClass(SymbolName, getSegment(SegmentNo).vmaddr + Offset);
            //FIXME: again the add of 8
            Offset += Scaled + 8;
            Idx++;
        } else if (Opcode == MachO::BIND_OPCODE_DO_BIND_ULEB_TIMES_SKIPPING_ULEB) {
            //TODO:
            Idx++;
            unsigned n;
            uint64_t Count = decodeULEB128(BindOpcodes.slice(Idx).data(), &n);
            Idx += n;
            n = 0;
            uint64_t Skip = decodeULEB128(BindOpcodes.slice(Idx).data(), &n);
            for (unsigned i = 0; i < Count; ++i) {
                addClass(SymbolName, getSegment(SegmentNo).vmaddr + Offset);
                Offset += 8 + Skip;
            }
            Idx += n;
        } else if (Opcode == MachO::BIND_OPCODE_DO_BIND_ADD_ADDR_ULEB) {
            addClass(SymbolName, getSegment(SegmentNo).vmaddr + Offset);
            Idx++;
            unsigned n;
            Offset += decodeULEB128(BindOpcodes.slice(Idx).data(), &n) + 8;
            Idx += n;
        } else if (Opcode == MachO::BIND_OPCODE_DONE) {
            break;
        } else if (Opcode == MachO::BIND_OPCODE_SET_DYLIB_ORDINAL_ULEB) {
            Idx++;
            unsigned n;
            uint64_t dylib = decodeULEB128(BindOpcodes.slice(Idx).data(), &n) + 8;
            Idx += n;
        } else if (Opcode == MachO::BIND_OPCODE_SET_ADDEND_SLEB) {
            Idx++;
            unsigned n;
            uint64_t addend = decodeSLEB128(BindOpcodes.slice(Idx).data(), &n);
            Idx += n;
        } else {
            errs() << "0X" << utohexstr(Idx) << "\n";
            errs() << "0x" << utohexstr(Opcode) << "\n";
            assert(false);
        }
    }
}

void ObjectiveCBinary::parseClass(uint64_t DataAddress, bool MetaClass) {
    object::section_iterator ConstSection = getSectionIterator(SEC_CONST);
    object::section_iterator ObjcDataSection = getSectionIterator(SEC_OBJC_DATA);
    object::section_iterator ClassnameSection = getSectionIterator(SEC_CLASSNAME);
    object::section_iterator DataSection = getSectionIterator(SEC_DATA);

    StringRef Content;

    ObjcDataSection->getContents(Content);
    ArrayRef<uint8_t> ObjcDataData((uint8_t*) Content.data(), Content.size());

    ConstSection->getContents(Content);
    ArrayRef<uint8_t> ConstData((uint8_t*) Content.data(), Content.size());

    ClassnameSection->getContents(Content);
    ArrayRef<uint8_t> ClassnameData((uint8_t*) Content.data(), Content.size());

    DataSection->getContents(Content);
    ArrayRef<uint8_t> DataData((uint8_t*) Content.data(), Content.size());

    uint64_t ISA = *(uint64_t*) ObjcDataData.slice(DataAddress - ObjcDataSection->getAddress() + ISA_OFFSET).data();
    uint64_t Super = *(uint64_t*) ObjcDataData.slice(DataAddress - ObjcDataSection->getAddress() + SUPER_OFFSET).data();
    uint64_t Data = *(uint64_t*) ObjcDataData.slice(DataAddress - ObjcDataSection->getAddress() + DATA_OFFSET).data();

    assert(isAddressInSection(Data, ConstSection));

    uint64_t NamePtr = *(uint64_t*)ConstData.slice(Data - ConstSection->getAddress() + NAME_OFFSET).data();
    assert(isAddressInSection(NamePtr, ClassnameSection));
    uint64_t MethodsPtr = *(uint64_t*)ConstData.slice(Data - ConstSection->getAddress() + METHOD_OFFSET).data();
    assert(!MethodsPtr || isAddressInSection(MethodsPtr, ConstSection));
    uint64_t IVARsPtr = *(uint64_t*)ConstData.slice(Data - ConstSection->getAddress() + IVAR_OFFSET).data();
    assert(!IVARsPtr || isAddressInSection(IVARsPtr, ConstSection));
    uint64_t ProtocolsListPtr = *(uint64_t*)ConstData.slice(Data - ConstSection->getAddress() + PROTOCOL_OFFSET).data();
    assert(!ProtocolsListPtr || isAddressInSection(ProtocolsListPtr, ConstSection));

    StringRef Classname = (const char*)ClassnameData.slice(NamePtr - ClassnameSection->getAddress()).data();

    ClassNames[DataAddress] = Classname;

    BaseClassPtr_t BaseClassPtr = MetaClass ? Metaclasses[Classname] : Classes[Classname];
    if (!BaseClassPtr) {
        BaseClassPtr = BaseClassPtr_t(new ObjectiveC::Class(Classname));
        BaseClassPtr->setAddress(DataAddress);
        if (MetaClass)
            Metaclasses[Classname] = BaseClassPtr;
        else
            Classes[Classname] = BaseClassPtr;
    }
    assert(BaseClassPtr->getType() != ObjectiveC::Dummy);
    ClassPtr_t ClassPtr = std::static_pointer_cast<ObjectiveC::Class>(BaseClassPtr);


    if (!Super) {
        StringRef Name = BindInfo[DataAddress + SUPER_OFFSET];
        if (Name.size()) {
            assert(!(MetaClass ^ Name.startswith_lower(OBJC_METACLASS_ID)));
            ClassPtr->setSuperclass(Name.substr(MetaClass ? strlen(OBJC_METACLASS_ID) : strlen(OBJC_CLASS_ID)));
        } else {
            assert(false);
        }
    } else {
        uint64_t SuperData = *(uint64_t*) ObjcDataData.slice(Super - ObjcDataSection->getAddress() + DATA_OFFSET).data();
        assert(isAddressInSection(SuperData, ConstSection));
        uint64_t SuperNamePtr = *(uint64_t*)ConstData.slice(SuperData - ConstSection->getAddress() + NAME_OFFSET).data();
        assert(isAddressInSection(NamePtr, ClassnameSection));
        StringRef SuperClassname = (const char*)ClassnameData.slice(SuperNamePtr - ClassnameSection->getAddress()).data();
        ClassPtr->setSuperclass(SuperClassname);
    }

    if (MethodsPtr) {
        auto methods = parseMethods(MethodsPtr);
        for (auto &M : methods) {
            ClassPtr->addMethod(M);
        }
    }

    if (Classname == "AddAccountViewController") {
        assert(true);
    }

    if (IVARsPtr) {
        uint32_t IVAREntrySize = *(uint32_t *) ConstData.slice(IVARsPtr - ConstSection->getAddress()).data();
        uint32_t IVAREntryCount = *(uint32_t *) ConstData.slice(IVARsPtr - ConstSection->getAddress() + 4).data();

        uint64_t CurrentIVAROffest = 8;
        for (unsigned i = 0; i < IVAREntryCount; ++i) {
            uint64_t OffsetPtr = *(uint64_t *) ConstData.slice(
                    IVARsPtr - ConstSection->getAddress() + CurrentIVAROffest).data();
            uint64_t IVARNamePtr = *(uint64_t *) ConstData.slice(
                    IVARsPtr - ConstSection->getAddress() + CurrentIVAROffest + 8).data();
            StringRef IVARName = getString(IVARNamePtr);

            uint64_t IVARTypePtr = *(uint64_t *) ConstData.slice(
                    IVARsPtr - ConstSection->getAddress() + CurrentIVAROffest + 16).data();
            StringRef IVARType = getString(IVARTypePtr);
            if (IVARType.find("@") == 0 && IVARType != "@?") {
                if (IVARType == "@") {
                    //FIXME: we can't obtain any more information!?
                    IVARType = "NSObject";
                } else {
                    assert(IVARType.find("\"") != StringRef::npos);
                    IVARType = IVARType.substr(IVARType.find("\"") + 1);
                    assert(IVARType.find("\"") != StringRef::npos);
                    IVARType = IVARType.substr(0, IVARType.find("\""));
                }
            } else {
                IVARType = "";
            }

            ObjectiveC::IVAR ivar(IVARName, OffsetPtr, IVARType);
            ClassPtr->addIVAR(ivar);
            assert(IVARs.find(OffsetPtr) == IVARs.end());
//            IVARs[OffsetPtr] = ivar;
            IVARs.insert(std::pair<uint64_t, ObjectiveC::IVAR>(OffsetPtr, ivar));

            CurrentIVAROffest += IVAREntrySize;
        }
    }

    auto parseProtocol = [&](uint64_t ProtocolPtr) {
        uint64_t ProtocolNamePtr = *(uint64_t *) DataData.slice(ProtocolPtr - DataSection->getAddress() + 8).data();
        StringRef ProtocolName = getString(ProtocolNamePtr);


        ObjectiveC::Protocol aProtocol(ProtocolName);
        uint64_t ProtocolsListPtr = *(uint64_t *) DataData.slice(ProtocolPtr - DataSection->getAddress() + 16).data();
        uint64_t InstanceMethodsPtr = *(uint64_t *) DataData.slice(ProtocolPtr - DataSection->getAddress() + 24).data();
        uint64_t ClassMethodsPtr = *(uint64_t *) DataData.slice(ProtocolPtr - DataSection->getAddress() + 32).data();
        uint64_t OptInstanceMethodsPtr = *(uint64_t *) DataData.slice(ProtocolPtr - DataSection->getAddress() + 40).data();
        uint64_t OptClassMethodsPtr = *(uint64_t *) DataData.slice(ProtocolPtr - DataSection->getAddress() + 48).data();
        uint64_t SignatureStartPtr = *(uint64_t *) DataData.slice(ProtocolPtr - DataSection->getAddress() + 72).data();
        ClassPtr->addProtocol(ProtocolName.str());

        uint64_t signaturesIdx = 0;


        if (InstanceMethodsPtr) {
            std::vector<ObjectiveC::Method> methods = parseMethods(InstanceMethodsPtr, SignatureStartPtr, &signaturesIdx);
            for (unsigned i = 0; i < methods.size(); ++i) {
                aProtocol.addInstanceMethod(methods[i]);
            }
        }
        if (OptInstanceMethodsPtr) {
            auto methods = parseMethods(OptInstanceMethodsPtr, SignatureStartPtr, &signaturesIdx);
            for (auto m : methods) {
                aProtocol.addInstanceMethod(m);
            }
        }
        if (ClassMethodsPtr) {
            auto methods = parseMethods(ClassMethodsPtr, SignatureStartPtr, &signaturesIdx);
            for (auto m : methods) {
                aProtocol.addClassMethod(m);
            }
        }
        if (OptClassMethodsPtr) {
            auto methods = parseMethods(OptClassMethodsPtr, SignatureStartPtr, &signaturesIdx);
            for (auto m : methods) {
                aProtocol.addClassMethod(m);
            }
        }
        protocolMap[ProtocolName] = aProtocol;
    };

    if (ProtocolsListPtr) {
        uint64_t ProtocolsCount = *(uint64_t *) ConstData.slice(ProtocolsListPtr - ConstSection->getAddress() ).data();
        uint64_t index = 8;
        for (unsigned i = 0; i < ProtocolsCount; ++i) {
            uint64_t ProtocolPtr = *(uint64_t *) ConstData.slice(ProtocolsListPtr - ConstSection->getAddress() + index).data();
            parseProtocol(ProtocolPtr);
            index += 8;
        }
    }


    assert((MetaClass ^ ISA) && "A metaclass can't have an ISA pointer!?");
    if (ISA && isAddressInSection(ISA, ObjcDataSection)) {
        parseClass(ISA, true);
    }
}

std::vector<ObjectiveC::Method> ObjectiveCBinary::parseMethods(uint64_t MethodsPtr, uint64_t SignatureStartPtr, uint64_t *signaturesIdx) {
    object::section_iterator ConstSection = getSectionIterator(SEC_CONST);
    StringRef Content;

    ConstSection->getContents(Content);
    ArrayRef<uint8_t> ConstData((uint8_t*) Content.data(), Content.size());


    std::vector<ObjectiveC::Method> methods;
    uint32_t MethodEntrySize = *(uint32_t *) ConstData.slice(MethodsPtr - ConstSection->getAddress()).data();
    uint32_t MethodEntryCount = *(uint32_t *) ConstData.slice(MethodsPtr - ConstSection->getAddress() + 4).data();

    uint64_t CurrentMethodOffest = 8;
    for (unsigned i = 0; i < MethodEntryCount; ++i) {
        uint64_t MethodNamePtr = *(uint64_t *) ConstData.slice(
                MethodsPtr - ConstSection->getAddress() + CurrentMethodOffest).data();
        uint64_t IMPPtr = *(uint64_t *) ConstData.slice(
                MethodsPtr - ConstSection->getAddress() + CurrentMethodOffest + 16).data();
        uint64_t MethTypePtr = *(uint64_t *) ConstData.slice(
                MethodsPtr - ConstSection->getAddress() + CurrentMethodOffest + 8).data();
        StringRef MethodName = getString(MethodNamePtr);
        StringRef MethodType = getString(MethTypePtr);
        if (SignatureStartPtr && signaturesIdx) {
            uint64_t MethType2Ptr = SignatureStartPtr + 8*(*signaturesIdx);
            (*signaturesIdx)++;
            MethType2Ptr = *(uint64_t *) ConstData.slice(
                    MethType2Ptr - ConstSection->getAddress()).data();
            MethodType = getString(MethType2Ptr);
        }

        methods.push_back(ObjectiveC::Method(MethodName, IMPPtr, MethodType));

        CurrentMethodOffest += MethodEntrySize;
    }
    return methods;
}

bool ObjectiveCBinary::getValue(const uint64_t Address, uint64_t &Value) {
    if (isSelectorRef(Address)) {
        object::SectionRef Section(SelRefsDataRef, MachO);
        StringRef Contents;
        Section.getContents(Contents);
        ArrayRef<uint8_t> D((uint8_t*) Contents.data(), Section.getSize());
        Value = *((uint64_t*) D.slice(Address - Section.getAddress()).data());
        return true;
    } else if (isClassRef(Address)) {
        object::section_iterator ClassRefSection = getSectionIterator(SEC_CLASSREFS);
        StringRef Contents;
        ClassRefSection->getContents(Contents);
        ArrayRef<uint8_t> D((uint8_t*) Contents.data(), ClassRefSection->getSize());
        Value = *((uint64_t*) D.slice(Address - ClassRefSection->getAddress()).data());
        return true;
    }
    return false;
}

bool ObjectiveCBinary::getData(const uint64_t Address, StringRef &Content) {
    if (isMethname(Address)) {
        object::SectionRef Section(MethnameDataRef, MachO);
        StringRef Contents;
        Section.getContents(Contents);
        ArrayRef<uint8_t> D((uint8_t*) Contents.data(), Section.getSize());
        const char *Name = ((const char*) D.slice(Address - Section.getAddress()).data());
        Content = StringRef(Name);
        return true;
    }
    return false;
}

object::section_iterator ObjectiveCBinary::getSectionIterator(StringRef Name) {
    for (object::section_iterator S_it = MachO->section_begin(); S_it != MachO->section_end(); ++S_it) {
        StringRef SectionName;
        if (S_it->getName(SectionName)) {
            continue;
        }
        if (SectionName == Name) {
            return S_it;
        }
    }
    DEBUG(errs() << "Can't find section: " << Name << "\n");
    return MachO->section_end();
}

std::string ObjectiveCBinary::getSectionName(uint64_t address) {
    for (object::section_iterator S_it = MachO->section_begin(); S_it != MachO->section_end(); ++S_it) {
        if (S_it->getAddress() <= address && address < (S_it->getAddress() + S_it->getSize()))
        {
            StringRef name;
            S_it->getName(name);
            return name.str();
        }
    }
    return "";
}

MachO::segment_command_64 ObjectiveCBinary::getSegment(uint64_t SegmentNo) {
    uint64_t Idx = 0;
    for(object::MachOObjectFile::load_command_iterator L_it = MachO->begin_load_commands();
        L_it != MachO->end_load_commands(); ++L_it) {
        if (L_it->C.cmd == MachO::LC_SEGMENT_64) {
            if (Idx == SegmentNo) {
                return MachO->getSegment64LoadCommand(*L_it);
            } else {
                Idx++;
            }
        }
    }
    llvm_unreachable("Cant find segment");
};

void ObjectiveCBinary::addClass(StringRef ClassName, uint64_t Address) {
    if (ClassName.startswith_lower(OBJC_CLASS_ID) || ClassName.startswith_lower(OBJC_METACLASS_ID)) {
        size_t len = ClassName.startswith_lower(OBJC_CLASS_ID) ? strlen(OBJC_CLASS_ID) : strlen(OBJC_METACLASS_ID);
        if (Classes[ClassName.substr(len)] == nullptr)
            Classes[ClassName.substr(len)] = BaseClassPtr_t(new ObjectiveC::DummyClass(ClassName.substr(len)));
        if (Metaclasses[ClassName.substr(len)] == nullptr)
            Metaclasses[ClassName.substr(len)] = BaseClassPtr_t(new ObjectiveC::DummyClass(ClassName.substr(len)));
    } else if (ClassName.startswith("_"))
        //TODO: remove this when symbols are handled with a leading '_'
        ClassName = ClassName.substr(1);
    BindInfo[Address] = ClassName;
}

StringRef ObjectiveCBinary::getString(uint64_t Address) {
    StringRef Contents;

    object::section_iterator MethnameSection = getSectionIterator(SEC_METHNAME);
    object::section_iterator ClassnameSection = getSectionIterator(SEC_CLASSNAME);
    object::section_iterator DataSection = getSectionIterator(SEC_OBJC_DATA);
    object::section_iterator SelRefSection = getSectionIterator(SEC_SELREFS);
    object::section_iterator IVARSection = getSectionIterator(SEC_IVAR);
    object::section_iterator SuperrefSection = getSectionIterator(SEC_SUPERREF);
    object::section_iterator CFStringSection = getSectionIterator(SEC_CFSTRING);
    object::section_iterator CStringSection = getSectionIterator(SEC_CSTRING);
    object::section_iterator MethTypeSection = getSectionIterator(SEC_METHTYPE);

    if (isAddressInSection(Address, CFStringSection)) {
        StringRef Contents;
        CFStringSection->getContents(Contents);
        ArrayRef<uint8_t> Data((uint8_t*)Contents.data(), Contents.size());
        uint64_t stringAddress = *(uint64_t*)Data.slice(Address + 16 - CFStringSection->getAddress()).data();
        StringRef s = getString(stringAddress);
        return s;
    } else if (BindInfo[Address].size()) {
        return BindInfo[Address].startswith_lower(OBJC_CLASS_ID) ? BindInfo[Address].substr(strlen(OBJC_CLASS_ID)) : BindInfo[Address];
    } else if (isAddressInSection(Address, MethnameSection) &&
            !MethnameSection->getContents(Contents)) {
        return (Contents.data() + (Address - MethnameSection->getAddress()));
    }  else  if (isAddressInSection(Address, ClassnameSection) &&
               !ClassnameSection->getContents(Contents)) {
        return (Contents.data() + (Address - ClassnameSection->getAddress()));
    } else if (isAddressInSection(Address, DataSection)) {
        return ClassNames[Address];
    } else if (isAddressInSection(Address, SelRefSection)) {
        StringRef Contents;
        SelRefSection->getContents(Contents);
        ArrayRef<uint8_t> Data((uint8_t*)Contents.data(), Contents.size());
        return getString(*(uint64_t*)Data.slice(Address - SelRefSection->getAddress()).data());
    } else if (isAddressInSection(Address, IVARSection)) {
        std::map<uint64_t, ObjectiveC::IVAR>::iterator it = IVARs.find(Address);
        assert(it != IVARs.end());
        return it->second.getID();
    } else if (isAddressInSection(Address, getSectionIterator(SEC_TEXT))) {
        std::string FName = getFunctionName(Address);
        if (FName.size())
            return FName;
        else {
            //TODO: warn here?
        }
    } else if (isAddressInSection(Address, getSectionIterator(SEC_SUPERREF))) {
        StringRef Contents;
        SuperrefSection->getContents(Contents);
        ArrayRef<uint8_t> Data((uint8_t*)Contents.data(), Contents.size());
        uint64_t ClassAddress = *(uint64_t*)Data.slice(Address - SuperrefSection->getAddress()).data();
        if (!ClassAddress) {
            llvm_unreachable("Class Address is missing");
        }
        StringRef className = getString(ClassAddress);
        if (className.startswith("_OBJC_METACLASS_$_"))
            className = className.substr(strlen("_OBJC_METACLASS_$_"));

        if (className == "NSObject")
            return className;

        ClassMap_t::iterator class_it = Classes.find(className);
        if (class_it == Classes.end()) {
            llvm_unreachable("class not found");
        }
        if (class_it->second->getAddress() != ClassAddress) {
            class_it = Metaclasses.find(className);
        }
        assert(class_it->second->getAddress() == ClassAddress || class_it->second->getAddress() == 0);
        return class_it->second->getSuperclass();
    } else if (isAddressInSection(Address, CStringSection)) {
        StringRef Contents;
        CStringSection->getContents(Contents);
        ArrayRef<uint8_t> Data((uint8_t*)Contents.data(), Contents.size());
        const char *string = (const char*)Data.slice(Address - CStringSection->getAddress()).data();
        return StringRef(string);
    } else if (isAddressInSection(Address, MethTypeSection)) {
        StringRef Contents;
        MethTypeSection->getContents(Contents);
        ArrayRef<uint8_t> Data((uint8_t*)Contents.data(), Contents.size());
        const char *string = (const char*)Data.slice(Address - MethTypeSection->getAddress()).data();
        return StringRef(string);
    }
    return "";
}

bool ObjectiveCBinary::getClass(const uint64_t Address, StringRef &Classname) {
    Classname = ClassRefs[Address];
    return Classname.size() > 0;
}

bool ObjectiveCBinary::getSelector(const uint64_t Address, StringRef &Selectorname) {
    if (isSelectorRef(Address)) {
        Selectorname = getString(Address);
        return true;
    }
    return false;
}

std::deque<std::string> ObjectiveCBinary::getMethodCandidates(StringRef Type, StringRef Selector, bool Meta) {
    BaseClassPtr_t Base = Meta ? Metaclasses[Type] : Classes[Type];
    if (!Base) {
//        for (ClassMap_t::iterator C_it = (Meta ? Metaclasses.begin() : Classes.begin()); C_it != (Meta ? Metaclasses.end(): Classes.end()); ++C_it) {
//            errs() << C_it->first << " " << (uint64_t)C_it->second.get() << "\n";
//        }
//        errs() << "NOT FOUND:\n";
//        errs() << Type << " " << Selector << "\n";
        std::stringstream ss;
        ss << (Meta ? "+[" : "-[") << Type.str() + " " + Selector.str() + "]";
        std::deque<std::string> M;
        M.push_front(ss.str());
        return M;
    }
    assert(Base);
    if (Base->getSuperclass().size()) {
//        ClassPtr_t Class = std::static_pointer_cast<ObjectiveC::Class>(Base);
        std::deque<std::string> M = getMethodCandidates(Base->getSuperclass(), Selector, Meta);
        std::stringstream ss;
        ss << (Meta ? "+[" : "-[") << Base->getClassName().str() + " " + Selector.str() + "]";
        M.push_front(ss.str());
        return M;
    } else {
        std::stringstream ss;
        ss << (Meta ? "+[" : "-[") << Base->getClassName().str() + " " + Selector.str() + "]";
        std::deque<std::string> M;
        M.push_front(ss.str());
        return M;
    }
    llvm_unreachable("");
}

std::string ObjectiveCBinary::getFunctionName(uint64_t Address) {
    return "fn_" + utohexstr(Address);
}

bool ObjectiveCBinary::isCFString(const uint64_t Address) {
    if (isAddressInSection(Address, getSectionIterator(SEC_CFSTRING)))
        return true;
    return false;
}

bool ObjectiveCBinary::isCString(const uint64_t Address) {
    if (isAddressInSection(Address, getSectionIterator(SEC_CSTRING)))
        return true;
    return false;
}

bool ObjectiveCBinary::isConstValue(const uint64_t Address) {
    if (isClassRef(Address) || isSelectorRef(Address) || isAddressInSection(Address, getSectionIterator(SEC_GOT)))
        return true;
    return false;
}
