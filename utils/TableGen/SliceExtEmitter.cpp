#include "llvm/TableGen/TableGenBackend.h"

#include <llvm/TableGen/Record.h>
#include <llvm/Support/raw_ostream.h>


#define PRE_REGS "DetectParametersPass::getRegisterValuesBeforeCall"
#define POST_REGS "DetectParametersPass::getRegisterValuesAfterCall"

#define REGS_TYPE "DetectParametersPass::UserSet_t"
#define REGS_TYPE_IT "DetectParametersPass::UserSet_t::iterator"

namespace llvm {

    typedef std::vector<std::string> string_list_t;
    typedef std::map<std::string, std::unique_ptr<string_list_t>> string_map_t;

    string_map_t FunctionMap;

    string_map_t PtsToFnMap;

    void EmitRegisterTranslation(RecordKeeper &RK, raw_ostream &OS) {

        OS << "namespace {\n"
                      "int translateRegister(std::string RegName) {\n";

        for (const auto &R : RK.getAllDerivedDefinitions("TranslateReg")) {
            OS << "  if (RegName == \"" << R->getValueAsString("Name") << "\") \n    return " << R->getValueAsInt("idx") << ";\n";
        }

        OS << "  llvm_unreachable(\"Unknown Register\");\n}\n}\n";

    }

    void EmitRegister(RecordKeeper &RK, raw_ostream &OS, const Record *Reg, std::string InstName, std::string DstVar) {
        if (Reg->isSubClassOf("Pre")) {
            OS << REGS_TYPE << " " << DstVar << " = ";
            OS << PRE_REGS << "(translateRegister(\"" << Reg->getValueAsString("Name") << "\"), " << InstName << ");\n";
        } else if (Reg->isSubClassOf("PreStore")) {
            OS << REGS_TYPE << " " << DstVar << " = ";
            OS << PRE_REGS << "(translateRegister(\"" << Reg->getValueAsString("Name") << "\"), " << InstName << ", true);\n";
        } else if (Reg->isSubClassOf("Post")) {
            OS << REGS_TYPE << " " << DstVar << " = ";
            OS << POST_REGS << "(translateRegister(\"" << Reg->getValueAsString("Name") << "\"), " << InstName << ");\n";
        } else if (Reg->isSubClassOf("DummyObject")) {
            OS << REGS_TYPE << " " << DstVar << ";\n";
            OS << DstVar << ".insert(new llvm::GlobalVariable(andersen->getModule(), llvm::IntegerType::get(llvm::getGlobalContext(), 1), false, llvm::GlobalVariable::ExternalLinkage, nullptr, \"" << Reg->getValueAsString("name") <<"\"));\n";
            OS << "andersen->addConstraint(AndersConstraint::ADDR_OF, ";
            OS << "andersen->getNodeFactory().createValueNode(*" << DstVar << ".begin()), ";
            OS << "andersen->getNodeFactory().createObjectNode(*" << DstVar << ".begin()));\n";
        } else if (Reg->isSubClassOf("Pts")) {
            OS << REGS_TYPE << " " << DstVar << ";\n";
            OS << "{\n";
            EmitRegister(RK, OS, Reg->getValueAsDef("reg"), InstName, "Src");
            OS <<         "for (auto S : Src) {\n"
                          "  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets\n"
                                  "  for (auto &P : PtsTo) {\n"
                                  "    assert(isa<const User>(P.first));\n"
                                  "    " << DstVar << ".insert((User*)P.first);\n"
                                "  }\n"
                          "}\n";
            OS << "}\n";
        } else if (Reg->isSubClassOf("Call")) {
            OS << REGS_TYPE << " " << DstVar << ";\n";
            OS << "{\n";
            OS << "const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(CallInst->getIns(), PS);//get points to sets\n";
            OS << "for (auto &P : PtsTo) {\n"
                          "assert(isa<const User>(P.first));\n"
                          "    DefRegs.insert((User*)P.first);"
                          "}\n";
            OS << "}\n";
        } else {
            Reg->dump();
            llvm_unreachable("");
        }
    }

    void EmitExtSliceCode(RecordKeeper &RK, raw_ostream &OS) {
        for (const auto &F : RK.getAllDerivedDefinitions("Function")) {

            std::string FunctionName = F->getValueAsString("Name");

            if (FunctionMap.find(FunctionName) == FunctionMap.end()) {
                FunctionMap.insert(std::pair<std::string, std::unique_ptr<string_list_t>>(FunctionName, std::unique_ptr<string_list_t>(new string_list_t())));
            }
            FunctionMap[FunctionName]->push_back(F->getName());

            OS << "void " << F->getName() << "(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {\n";
            OS << "//Handle \"" << FunctionName << "\"\n";

            for (const auto &S : F->getValueAsListOfDefs("SliceOperations")) {
                //Super class 'Operation' and the actual operation are both super classes
                assert(S->getSuperClasses().size() == 2);
                if (S->isSubClassOf("Def")) {
                    OS << "{ //Def\n";
                    const Record *d = S->getValueAsDef("define");
//                    OS <<"  " << REGS_TYPE << " DefRegs" << ++varIdx << " = ";
                    EmitRegister(RK, OS, d, "(Instruction*)CallInst->getIns()", "DefRegs");

                    OS << "  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {\n";
                    OS << "    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));\n";
                    OS << "  }\n";
                    OS << "} //End Def\n";
                } else if (S->isSubClassOf("Ref1")) {
                    OS << "{ //Ref1\n";
                    const Record *d = S->getValueAsDef("reference");
//                    OS <<"  " << REGS_TYPE << " RefRegs" << ++varIdx << " = ";
                    EmitRegister(RK, OS, d, "(Instruction*)CallInst->getIns()", "RefRegs");

                    OS << "  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {\n";
                    OS << "    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);\n";
                    OS << "  }\n";
                    OS << "} //End Ref1\n";
                } else if (S->isSubClassOf("Ref")) {
                    OS << "{ //Ref\n";
                    const Record *d = S->getValueAsDef("reference");
//                    OS <<"  " << REGS_TYPE << " RefRegs" << ++varIdx << " = ";
                    EmitRegister(RK, OS, d, "(Instruction*)CallInst->getIns()", "RefRegs");

                    OS << "  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {\n";
                    OS << "    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));\n";
                    OS << "  }\n";
                    OS << "} //End ref\n";
                } else {
                    S->getSuperClasses().back()->dump();
                    llvm_unreachable("Not handled");
                }
            }

            OS << "}\n\n";
        }
    }

    void EmitCompares(RecordKeeper &RK, raw_ostream &OS, string_map_t &Functions, std::string ParamList, std::string Parameters) {

        OS << "bool handleCall(" << ParamList << ") {\n";

        for (string_map_t::iterator F_it = Functions.begin(); F_it != Functions.end(); ++F_it) {

            OS << "  if (FName == \"" << F_it->first << "\") {\n";

            for (string_list_t::iterator F2_it = F_it->second->begin(); F2_it != F_it->second->end(); ++F2_it) {
                OS << "    " << *F2_it << "("<< Parameters <<");\n";
            }
            OS << "    return true;\n";

            OS << "  }\n";
        }

        OS << "  return false;\n";
        OS << "}\n\n";
    }

    void EmitSliceExt(RecordKeeper &RK, raw_ostream &OS) {

        OS << "#ifndef LLVM_EXTERNALHANDLER_H\n"
                      "#define LLVM_EXTERNALHANDLER_H\n"
                      "\n"
                      "#include \"llvm/Analysis/Andersen/DetectParametersPass.h\"\n"
                      "\n"
                      "namespace llvm {\n\n";

        EmitRegisterTranslation(RK, OS);

        EmitExtSliceCode(RK, OS);

        EmitCompares(RK, OS, FunctionMap, "llvm::slicing::InsInfo *CallInst, std::string FName, const ptr::PointsToSets &PS", "CallInst, PS");

        OS << "}\n"
                      "\n"
                      "\n"
                      "#endif //LLVM_EXTERNALHANDLER_H\n\n";
    }


    void EmitExtPtsToCode(RecordKeeper &RK, raw_ostream &OS) {
        for (const auto &F : RK.getAllDerivedDefinitions("Function")) {

            std::string FunctionName = F->getValueAsString("Name");

            if (PtsToFnMap.find(FunctionName) == PtsToFnMap.end()) {
                PtsToFnMap.insert(std::pair<std::string, std::unique_ptr<string_list_t>>(FunctionName, std::unique_ptr<string_list_t>(new string_list_t())));
            }
            PtsToFnMap[FunctionName]->push_back(F->getName());

            OS << "void " << F->getName() << "(llvm::Instruction *CallInst, Andersen *andersen) {\n";
            OS << "//Handle \"" << FunctionName << "\"\n";

            int varIdx = 0;
            for (const auto &S : F->getValueAsListOfDefs("PtsToOperations")) {

                if (S->isSubClassOf("Load")) {
                    OS << "{ //Load operation\n";
                    const Record *val = S->getValueAsDef("val");
                    const Record *loc = S->getValueAsDef("loc");

                    EmitRegister(RK, OS, val, "CallInst", "Vals");
                    EmitRegister(RK, OS, loc, "CallInst", "Locs");


                    OS << "  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {\n";

                    OS << "    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);\n";
                    OS << "    if (valIdx == AndersNodeFactory::InvalidIndex)\n";
                    OS << "      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);\n";

                    OS << "    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {\n";

                    OS << "      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);\n";
                    OS << "      if (locIdx == AndersNodeFactory::InvalidIndex)\n";
                    OS << "        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);\n";

                    OS << "      andersen->addConstraint(AndersConstraint::LOAD, valIdx, locIdx);\n";

                    OS << "    }\n";

                    OS << "  }\n";

                    OS << "}\n";
                } else if (S->isSubClassOf("Store")) {
                    OS << "{ //Store operation\n";
                    const Record *val = S->getValueAsDef("val");
                    const Record *loc = S->getValueAsDef("loc");

                    EmitRegister(RK, OS, val, "CallInst", "Vals");
                    EmitRegister(RK, OS, loc, "CallInst", "Locs");


                    OS << "  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {\n";

                    OS << "    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);\n";
                    OS << "    if (valIdx == AndersNodeFactory::InvalidIndex)\n";
                    OS << "      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);\n";

                    OS << "    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {\n";

                    OS << "      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);\n";
                    OS << "      if (locIdx == AndersNodeFactory::InvalidIndex)\n";
                    OS << "        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);\n";

                    OS << "      andersen->addConstraint(AndersConstraint::STORE, locIdx, valIdx);\n";

                    OS << "    }\n";

                    OS << "  }\n";

                    OS << "}\n";

                } else if (S->isSubClassOf("Alloc")) {
                    OS << "{ //Alloc operation\n";
                    const Record *reg = S->getValueAsDef("reg");
                    std::string type = S->getValueAsString("type");
                    EmitRegister(RK, OS, reg, "CallInst", "Post");

                    OS << "  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {\n";

                    OS << "    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);\n"
                          "    if (valIndex == AndersNodeFactory::InvalidIndex)\n"
                          "      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);\n"
                          "    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);\n"
                          "    if (objIndex == AndersNodeFactory::InvalidIndex)\n"
                          "      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);\n"
                          "    andersen->setType(*Post_it, \"" << type << "\");\n"
                          "    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);\n";

                    OS << "  }\n";

                    OS << "}\n";
                } else if (S->isSubClassOf("Copy")) {
                    OS << "{ //Copy operation\n";

                    const Record *from = S->getValueAsDef("from");
                    const Record *to = S->getValueAsDef("to");

                    EmitRegister(RK, OS, from, "CallInst", "From");
                    EmitRegister(RK, OS, to, "CallInst", "To");


                    OS << "  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {\n";

                    OS << "    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);\n";
                    OS << "    if (srcIdx == AndersNodeFactory::InvalidIndex)\n";
                    OS << "      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);\n";

                    OS << "    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {\n";

                    OS << "      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);\n";
                    OS << "      if (dstIdx == AndersNodeFactory::InvalidIndex)\n";
                    OS << "        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);\n";

                    OS << "      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);\n";

                    OS << "    }\n";

                    OS << "  }\n";

                    OS << "} //end copy\n";
                } else if (S->isSubClassOf("Loc")) {
                    OS << "{ //Check object\n";
                    const Record *reg = S->getValueAsDef("reg");
                    EmitRegister(RK, OS, reg, "CallInst", "Reg");

                    OS << "  for (auto Post_it = Reg.begin(); Post_it != Reg.end(); ++Post_it) {\n";

                    OS << "    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);\n"
                            "    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);\n"
                                  "if (objIndex != AndersNodeFactory::InvalidIndex)\n"
                                  "continue;"
                            "    if (objIndex == AndersNodeFactory::InvalidIndex)\n"
                            "      objIndex = andersen->getNodeFactory().createObjectNodeDummy(*Post_it, *CallInst->getParent()->getParent()->getParent());\n"
                              "    if (valIndex == AndersNodeFactory::InvalidIndex)\n"
                              "      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);\n"
                            "    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);\n";

                    OS << "  }\n";

                    OS << "}\n";
                } else {
                    llvm_unreachable("Not handled");
                }
            }

            OS << "}\n\n";
        }
    }

    void EmitHelperPtsTo(RecordKeeper &RK, raw_ostream &OS) {

        OS << "bool canHandleCall(const std::string &FName) {\n";
        for (auto &F : PtsToFnMap) {
            OS << "  if (FName == \"" << F.first << "\")\n"
                  "    return true;\n";
        }
        OS << "  return false;\n"
                      "}\n\n";
    }

    void EmitPtsToExt(RecordKeeper &RK, raw_ostream &OS) {

        OS << "#ifndef LLVM_EXTERNALHANDLER_H\n"
                      "#define LLVM_EXTERNALHANDLER_H\n\n";

        OS << "namespace llvm {\n"
                      "    namespace pointsto {\n";

        EmitRegisterTranslation(RK, OS);

        EmitExtPtsToCode

                (RK, OS);

        EmitHelperPtsTo(RK, OS);

        EmitCompares(RK, OS, PtsToFnMap, "llvm::Instruction *CallInst, Andersen *andersen, const std::string &FName", "CallInst, andersen");

        OS << "    }\n"
                      "}\n"
                      "\n"
                      "#endif //LLVM_EXTERNALHANDLER_H\n\n";
    }


    void EmitExtModifiesCode(RecordKeeper &RK, raw_ostream &OS) {
        for (const auto &F : RK.getAllDerivedDefinitions("Function")) {

            std::string FunctionName = F->getValueAsString("Name");

            if (FunctionMap.find(FunctionName) == FunctionMap.end()) {
                FunctionMap.insert(std::pair<std::string, std::unique_ptr<string_list_t>>(FunctionName, std::unique_ptr<string_list_t>(new string_list_t())));
            }
            FunctionMap[FunctionName]->push_back(F->getName());

            OS << "void " << F->getName() << "(llvm::Instruction *CallInst, const ptr::PointsToSets &PS, llvm::mods::ProgramStructure::Commands &commands, std::mutex &lock) {\n";
            OS << "//Handle \"" << FunctionName << "\"\n";

            for (const auto &S : F->getValueAsListOfDefs("SliceOperations")) {
                //Super class 'Operation' and the actual operation are both super classes
                assert(S->getSuperClasses().size() == 2);
                if (S->isSubClassOf("DefMod")) {
                    const Record *d = S->getValueAsDef("define");
                    if (!d->isSubClassOf("Pts")) {
                        continue;
                    }
                    OS << "{ //Def\n";
//                    OS <<"  " << REGS_TYPE << " DefRegs" << ++varIdx << " = ";
                    EmitRegister(RK, OS, d, "(Instruction*)CallInst", "DefRegs");

                    OS << "  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {\n";
                    OS << "    lock.lock();\n";
                    OS << "    commands.push_back(llvm::mods::ProgramStructure::Command(llvm::mods::CMD_FRC_DEF, *DefRegs_it));\n";
                    OS << "    lock.unlock();\n";
                    OS << "  }\n";
                    OS << "} //End Def\n";
                } else if (S->isSubClassOf("Def")) {
                    const Record *d = S->getValueAsDef("define");
                    if (!d->isSubClassOf("Pts")) {
                        continue;
                    }
                    OS << "{ //Def\n";
//                    OS <<"  " << REGS_TYPE << " DefRegs" << ++varIdx << " = ";
                    EmitRegister(RK, OS, d, "(Instruction*)CallInst", "DefRegs");

                    OS << "  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {\n";
                    OS << "    lock.lock();\n";
                    OS << "    commands.push_back(llvm::mods::ProgramStructure::Command(llvm::mods::CMD_DEF, *DefRegs_it));\n";
                    OS << "    lock.unlock();\n";
                    OS << "  }\n";
                    OS << "} //End Def\n";
                } else {
                    if (S->isSubClassOf("Ref1") || S->isSubClassOf("Ref")) {
                        //do nothing...
                    } else {
                        S->getSuperClasses().back()->dump();
                        llvm_unreachable("Not handled");
                    }
                }
            }

            OS << "}\n\n";
        }

    }

    void EmitMODExt(RecordKeeper &RK, raw_ostream &OS) {
        OS << "#ifndef LLVM_EXTERNALMODHANDLER_H\n"
                "#define LLVM_EXTERNALMODHANDLER_H\n"
                "\n"
                "#include \"llvm/Analysis/Andersen/DetectParametersPass.h\"\n"
                "\n"
                "namespace llvm {\n\n";


        EmitRegisterTranslation(RK, OS);
        EmitExtModifiesCode(RK, OS);
        EmitCompares(RK, OS, FunctionMap, "llvm::Instruction *CallInst, const ptr::PointsToSets &PS, const std::string &FName, llvm::mods::ProgramStructure::Commands &commands, std::mutex &lock", "CallInst, PS, commands, lock");

        OS << "}\n"
                      "#endif\n";
    }

}
