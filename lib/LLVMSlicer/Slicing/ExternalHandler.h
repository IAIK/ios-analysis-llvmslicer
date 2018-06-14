#ifndef LLVM_EXTERNALHANDLER_H
#define LLVM_EXTERNALHANDLER_H

#include "llvm/Analysis/Andersen/DetectParametersPass.h"

namespace llvm {

namespace {
int translateRegister(std::string RegName) {
  if (RegName == "X0") 
    return 5;
  if (RegName == "X1") 
    return 6;
  if (RegName == "X2") 
    return 7;
  if (RegName == "X3") 
    return 8;
  if (RegName == "X4") 
    return 9;
  if (RegName == "X5") 
    return 10;
  if (RegName == "X6") 
    return 11;
  if (RegName == "X7") 
    return 12;
  llvm_unreachable("Unknown Register");
}
}
void anonymous_1005(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "malloc"
}

void anonymous_1008(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSFileHandle fileHandleForReadingAtPath:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_1019(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSFileHandle readDataOfLength:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_1030(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[UITextField text]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_1036(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[UITextView text]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_1042(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[UILabel text]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_1048(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSString stringByTrimmingCharactersInSet:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_1057(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSString stringByAppendingString:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_106(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSData dataWithContentsOfURL:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
}

void anonymous_1069(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSString componentsSeparatedByString:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_1081(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[UIView initWithFrame:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_1088(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[UIViewController initWithNibName:bundle:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_1095(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSNull null]"
}

void anonymous_1098(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[UIApplication sharedApplication]"
}

void anonymous_1101(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSUserDefaults objectForKey:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_1108(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSUserDefaults setObject:forKey:]"
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_1115(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSKeyedArchiver archivedDataWithRootObject:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_1121(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[UIAlertView textFieldAtIndex:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_1127(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSDictionary objectForKey:]"
}

void anonymous_1131(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSDictionary setObject:forKey:]"
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_1138(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSDictionary valueForKey:]"
}

void anonymous_1142(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSDictionary setValue:forKey:]"
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_119(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSData dataWithData:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
}

void anonymous_132(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSData mutableCopy]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_145(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSData isEqual:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_148(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSData writeToFile:options:error:]"
}

void anonymous_149(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSMutableData dataWithBytes:length:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
}

void anonymous_162(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSMutableData data]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_17(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_autoreleaseReturnValue"
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_170(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSMutableData dataWithBytesNoCopy:length:freeWhenDone:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
}

void anonymous_18(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_autorelease"
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_183(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSMutableData dataWithContentsOfFile:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
}

void anonymous_19(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_retainAutoreleasedReturnValue"
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_196(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSMutableData dataWithContentsOfFile:options:error:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
}

void anonymous_20(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_retainAutoreleaseReturnValue"
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_209(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSMutableData dataWithContentsOfURL:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
}

void anonymous_21(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_retainAutorelease"
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_22(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSObject retain]"
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_222(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSMutableData dataWithData:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
}

void anonymous_23(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSObject autorelease]"
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_235(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSMutableData dataWithCapacity:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_24(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_release"
}

void anonymous_243(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSMutableData dataWithLength:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_25(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSObject release]"
}

void anonymous_251(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSMutableData mutableBytes]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_26(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSObject dealloc]"
}

void anonymous_260(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSString string]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_266(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSString stringWithString:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_27(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSObject respondsToSelector:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_277(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSString stringWithCString:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_288(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSString stringWithCharacters:length:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_299(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSString initWithData:encoding:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_311(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSString initWithString:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_323(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSString getCString:maxLength:encoding:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_33(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSData dataWithBytes:length:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
}

void anonymous_332(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSString getBytes:maxLength:usedLength:encoding:options:range:remainingRange:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_341(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSString cStringUsingEncoding:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_350(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSString lowercaseString]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_361(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSMutableString stringWithString:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_372(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSMutableString stringWithCharacters:length:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_383(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSDictionary dictionaryWithContentsOfFile:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_389(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSDictionary dictionaryWithObject:forKey:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_398(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSDictionary dictionaryWithObjectsAndKeys:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_404(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSDictionary dictionary]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_410(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSDictionary dictionaryWithDictionary:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_416(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSDictionary dictionaryWithObjects:forKeys:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_422(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSDictionary dictionaryWithObjects:forKeys:count:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_428(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSDictionary initWithObjectsAndKeys:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_435(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSDictionary objectForKey:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_442(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSDictionary valueForKey:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_449(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSMutableDictionary dictionaryWithContentsOfFile:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_455(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSMutableDictionary dictionaryWithObject:forKey:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_46(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSData dataWithBytesNoCopy:length:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
}

void anonymous_464(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSMutableDictionary dictionaryWithObjectsAndKeys:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_470(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSMutableDictionary dictionary]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_476(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSMutableDictionary dictionaryWithDictionary:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_482(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSJSONSerialization dataWithJSONObject:options:error:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_488(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSData bytes]"
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_500(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSData getBytes:range:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_517(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSData getBytes:length:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_537(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSData subdataWithRange:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_550(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSMutableData setLength:]"
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
}

void anonymous_553(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSMutableData appendData:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_565(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSMutableData appendBytes:length:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_577(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSData length]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_580(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSString dataUsingEncoding:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_59(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSData data]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_593(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "memcpy"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X1"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
}

void anonymous_604(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "CCKeyDerivationPBKDF"
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns(), true);
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X1"), (Instruction*)CallInst->getIns(), true);
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X7"), (Instruction*)CallInst->getIns(), true);
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X7"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X1"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X1"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
}

void anonymous_621(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "CCCalibratePBKDF"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X5"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_626(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "CCCryptorCreate"
}

void anonymous_627(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "CCCryptorCreateWithMode"
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X4"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_631(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "CCCrypt"
}

void anonymous_632(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "CCCryptorUpdate"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X3"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X1"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_645(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_setProperty_nonatomic_copy"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X3"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_654(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_getProperty"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_668(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_setProperty_nonatomic"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X3"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_67(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSData dataWithBytesNoCopy:length:freeWhenDone:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
}

void anonymous_680(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_setProperty_atomic"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X3"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_692(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_setProperty"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X3"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_704(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_loadWeakRetained"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_718(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSString UTF8String]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_731(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSArray arrayWithArray:]"
}

void anonymous_734(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSArray arrayWithObject:]"
}

void anonymous_735(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSArray arrayWithObjects:]"
}

void anonymous_736(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSArray array]"
}

void anonymous_737(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSArray objectAtIndex:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_747(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSArray objectAtIndexedSubscript:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_757(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSMutableString stringWithCapacity:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_763(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSMutableString stringWithUTF8String:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_764(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSMutableString string]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_765(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSString stringWithCString:encoding:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_771(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSString stringWithContentsOfFile:encoding:error:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_772(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSString stringWithFormat:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_773(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSString stringWithUTF8String:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_774(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSString stringWithCString:encoding:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_781(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSObject init]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_788(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSMutableData init]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_789(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSData initWithBytes:length:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_790(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSData initWithBase64EncodedData:options:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_791(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSData initWithContentsOfFile:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_792(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSData initWithData:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_793(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSData init]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_794(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSData initWithBase64EncodedString:options:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_795(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSString initWithBytes:length:encoding:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_796(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSString initWithData:encoding:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_797(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSString initWithFormat:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_798(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSMutableDictionary init]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_799(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSDictionary init]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_8(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_retain"
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_80(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSData dataWithContentsOfFile:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
}

void anonymous_800(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[UITableViewController init]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_801(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSData initWithBytes:length:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
}

void anonymous_810(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSData initWithBase64EncodedString:options:]"
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
}

void anonymous_816(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSString copy]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_825(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSMutableString copy]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_834(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSData copy]"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_843(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "CC_SHA256_Init"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_847(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "CC_SHA256_Update"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X1"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_857(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "CC_SHA256_Final"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X1"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_864(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "arc4random"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_870(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "SecRandomCopyBytes"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_878(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_storeStrong"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X1"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_888(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_storeWeak"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X1"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_898(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_getClass"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_907(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "bzero"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_915(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSBundle bundleWithPath:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_923(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSBundle bundleForClass:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_924(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSBundle mainBundle]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_925(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "NSLog"
}

void anonymous_926(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "__stack_chk_fail"
}

void anonymous_927(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_enumerationMutation"
}

void anonymous_928(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_terminate"
}

void anonymous_929(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_sync_exit"
}

void anonymous_93(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSData dataWithContentsOfFile:options:error:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
{ //Ref
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1));
  }
} //End ref
}

void anonymous_930(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_sync_enter"
}

void anonymous_931(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_exception_rethrow"
}

void anonymous_932(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_exception_throw"
}

void anonymous_933(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_destroyWeak"
}

void anonymous_934(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_end_catch"
}

void anonymous_935(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "objc_begin_catch"
}

void anonymous_936(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "_Znam"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_941(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "_Znwm"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_946(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "_ZnwmRKSt9nothrow_t"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
}

void anonymous_951(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "CC_MD5"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_958(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "CC_SHA256"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_965(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "read"
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X1"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_971(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "open"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_977(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "+[NSNumber numberWithInt:]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Def
DetectParametersPass::UserSet_t DefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    DefRegs.insert((User*)P.first);
  }
}
}
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), (Instruction*)CallInst->getIns());
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_987(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSNumber integerValue]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_993(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSString intValue]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

void anonymous_999(llvm::slicing::InsInfo *CallInst, const ptr::PointsToSets &PS) {
//Handle "-[NSString integerValue]"
{ //Def
DetectParametersPass::UserSet_t DefRegs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
  for (auto DefRegs_it = DefRegs.begin(); DefRegs_it != DefRegs.end(); ++DefRegs_it) {
    CallInst->addDEF(llvm::ptr::PointsToSets::Pointee(*DefRegs_it, -1));
  }
} //End Def
{ //Ref1
DetectParametersPass::UserSet_t RefRegs;
{
DetectParametersPass::UserSet_t Src = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), (Instruction*)CallInst->getIns());
for (auto S : Src) {
  const ptr::PointsToSets::PointsToSet &PtsTo = ptr::getPointsToSet(S, PS);//get points to sets
  for (auto &P : PtsTo) {
    assert(isa<const User>(P.first));
    RefRegs.insert((User*)P.first);
  }
}
}
  for (auto RefRegs_it = RefRegs.begin(); RefRegs_it != RefRegs.end(); ++RefRegs_it) {
    CallInst->addREF(llvm::ptr::PointsToSets::Pointee(*RefRegs_it, -1), 1.0);
  }
} //End Ref1
}

bool handleCall(llvm::slicing::InsInfo *CallInst, std::string FName, const ptr::PointsToSets &PS) {
  if (FName == "+[NSArray arrayWithArray:]") {
    anonymous_731(CallInst, PS);
    return true;
  }
  if (FName == "+[NSArray arrayWithObject:]") {
    anonymous_734(CallInst, PS);
    return true;
  }
  if (FName == "+[NSArray arrayWithObjects:]") {
    anonymous_735(CallInst, PS);
    return true;
  }
  if (FName == "+[NSArray array]") {
    anonymous_736(CallInst, PS);
    return true;
  }
  if (FName == "+[NSBundle bundleForClass:]") {
    anonymous_923(CallInst, PS);
    return true;
  }
  if (FName == "+[NSBundle bundleWithPath:]") {
    anonymous_915(CallInst, PS);
    return true;
  }
  if (FName == "+[NSBundle mainBundle]") {
    anonymous_924(CallInst, PS);
    return true;
  }
  if (FName == "+[NSData dataWithBytes:length:]") {
    anonymous_33(CallInst, PS);
    return true;
  }
  if (FName == "+[NSData dataWithBytesNoCopy:length:]") {
    anonymous_46(CallInst, PS);
    return true;
  }
  if (FName == "+[NSData dataWithBytesNoCopy:length:freeWhenDone:]") {
    anonymous_67(CallInst, PS);
    return true;
  }
  if (FName == "+[NSData dataWithContentsOfFile:]") {
    anonymous_80(CallInst, PS);
    return true;
  }
  if (FName == "+[NSData dataWithContentsOfFile:options:error:]") {
    anonymous_93(CallInst, PS);
    return true;
  }
  if (FName == "+[NSData dataWithContentsOfURL:]") {
    anonymous_106(CallInst, PS);
    return true;
  }
  if (FName == "+[NSData dataWithData:]") {
    anonymous_119(CallInst, PS);
    return true;
  }
  if (FName == "+[NSData data]") {
    anonymous_59(CallInst, PS);
    return true;
  }
  if (FName == "+[NSDictionary dictionaryWithContentsOfFile:]") {
    anonymous_383(CallInst, PS);
    return true;
  }
  if (FName == "+[NSDictionary dictionaryWithDictionary:]") {
    anonymous_410(CallInst, PS);
    return true;
  }
  if (FName == "+[NSDictionary dictionaryWithObject:forKey:]") {
    anonymous_389(CallInst, PS);
    return true;
  }
  if (FName == "+[NSDictionary dictionaryWithObjects:forKeys:]") {
    anonymous_416(CallInst, PS);
    return true;
  }
  if (FName == "+[NSDictionary dictionaryWithObjects:forKeys:count:]") {
    anonymous_422(CallInst, PS);
    return true;
  }
  if (FName == "+[NSDictionary dictionaryWithObjectsAndKeys:]") {
    anonymous_398(CallInst, PS);
    return true;
  }
  if (FName == "+[NSDictionary dictionary]") {
    anonymous_404(CallInst, PS);
    return true;
  }
  if (FName == "+[NSFileHandle fileHandleForReadingAtPath:]") {
    anonymous_1008(CallInst, PS);
    return true;
  }
  if (FName == "+[NSJSONSerialization dataWithJSONObject:options:error:]") {
    anonymous_482(CallInst, PS);
    return true;
  }
  if (FName == "+[NSKeyedArchiver archivedDataWithRootObject:]") {
    anonymous_1115(CallInst, PS);
    return true;
  }
  if (FName == "+[NSMutableData dataWithBytes:length:]") {
    anonymous_149(CallInst, PS);
    return true;
  }
  if (FName == "+[NSMutableData dataWithBytesNoCopy:length:freeWhenDone:]") {
    anonymous_170(CallInst, PS);
    return true;
  }
  if (FName == "+[NSMutableData dataWithCapacity:]") {
    anonymous_235(CallInst, PS);
    return true;
  }
  if (FName == "+[NSMutableData dataWithContentsOfFile:]") {
    anonymous_183(CallInst, PS);
    return true;
  }
  if (FName == "+[NSMutableData dataWithContentsOfFile:options:error:]") {
    anonymous_196(CallInst, PS);
    return true;
  }
  if (FName == "+[NSMutableData dataWithContentsOfURL:]") {
    anonymous_209(CallInst, PS);
    return true;
  }
  if (FName == "+[NSMutableData dataWithData:]") {
    anonymous_222(CallInst, PS);
    return true;
  }
  if (FName == "+[NSMutableData dataWithLength:]") {
    anonymous_243(CallInst, PS);
    return true;
  }
  if (FName == "+[NSMutableData data]") {
    anonymous_162(CallInst, PS);
    return true;
  }
  if (FName == "+[NSMutableDictionary dictionaryWithContentsOfFile:]") {
    anonymous_449(CallInst, PS);
    return true;
  }
  if (FName == "+[NSMutableDictionary dictionaryWithDictionary:]") {
    anonymous_476(CallInst, PS);
    return true;
  }
  if (FName == "+[NSMutableDictionary dictionaryWithObject:forKey:]") {
    anonymous_455(CallInst, PS);
    return true;
  }
  if (FName == "+[NSMutableDictionary dictionaryWithObjectsAndKeys:]") {
    anonymous_464(CallInst, PS);
    return true;
  }
  if (FName == "+[NSMutableDictionary dictionary]") {
    anonymous_470(CallInst, PS);
    return true;
  }
  if (FName == "+[NSMutableString stringWithCapacity:]") {
    anonymous_757(CallInst, PS);
    return true;
  }
  if (FName == "+[NSMutableString stringWithCharacters:length:]") {
    anonymous_372(CallInst, PS);
    return true;
  }
  if (FName == "+[NSMutableString stringWithString:]") {
    anonymous_361(CallInst, PS);
    return true;
  }
  if (FName == "+[NSMutableString stringWithUTF8String:]") {
    anonymous_763(CallInst, PS);
    return true;
  }
  if (FName == "+[NSMutableString string]") {
    anonymous_764(CallInst, PS);
    return true;
  }
  if (FName == "+[NSNull null]") {
    anonymous_1095(CallInst, PS);
    return true;
  }
  if (FName == "+[NSNumber numberWithInt:]") {
    anonymous_977(CallInst, PS);
    return true;
  }
  if (FName == "+[NSString stringWithCString:]") {
    anonymous_277(CallInst, PS);
    return true;
  }
  if (FName == "+[NSString stringWithCString:encoding:]") {
    anonymous_765(CallInst, PS);
    anonymous_774(CallInst, PS);
    return true;
  }
  if (FName == "+[NSString stringWithCharacters:length:]") {
    anonymous_288(CallInst, PS);
    return true;
  }
  if (FName == "+[NSString stringWithContentsOfFile:encoding:error:]") {
    anonymous_771(CallInst, PS);
    return true;
  }
  if (FName == "+[NSString stringWithFormat:]") {
    anonymous_772(CallInst, PS);
    return true;
  }
  if (FName == "+[NSString stringWithString:]") {
    anonymous_266(CallInst, PS);
    return true;
  }
  if (FName == "+[NSString stringWithUTF8String:]") {
    anonymous_773(CallInst, PS);
    return true;
  }
  if (FName == "+[NSString string]") {
    anonymous_260(CallInst, PS);
    return true;
  }
  if (FName == "+[UIApplication sharedApplication]") {
    anonymous_1098(CallInst, PS);
    return true;
  }
  if (FName == "-[NSArray objectAtIndex:]") {
    anonymous_737(CallInst, PS);
    return true;
  }
  if (FName == "-[NSArray objectAtIndexedSubscript:]") {
    anonymous_747(CallInst, PS);
    return true;
  }
  if (FName == "-[NSData bytes]") {
    anonymous_488(CallInst, PS);
    return true;
  }
  if (FName == "-[NSData copy]") {
    anonymous_834(CallInst, PS);
    return true;
  }
  if (FName == "-[NSData getBytes:length:]") {
    anonymous_517(CallInst, PS);
    return true;
  }
  if (FName == "-[NSData getBytes:range:]") {
    anonymous_500(CallInst, PS);
    return true;
  }
  if (FName == "-[NSData initWithBase64EncodedData:options:]") {
    anonymous_790(CallInst, PS);
    return true;
  }
  if (FName == "-[NSData initWithBase64EncodedString:options:]") {
    anonymous_794(CallInst, PS);
    anonymous_810(CallInst, PS);
    return true;
  }
  if (FName == "-[NSData initWithBytes:length:]") {
    anonymous_789(CallInst, PS);
    anonymous_801(CallInst, PS);
    return true;
  }
  if (FName == "-[NSData initWithContentsOfFile:]") {
    anonymous_791(CallInst, PS);
    return true;
  }
  if (FName == "-[NSData initWithData:]") {
    anonymous_792(CallInst, PS);
    return true;
  }
  if (FName == "-[NSData init]") {
    anonymous_793(CallInst, PS);
    return true;
  }
  if (FName == "-[NSData isEqual:]") {
    anonymous_145(CallInst, PS);
    return true;
  }
  if (FName == "-[NSData length]") {
    anonymous_577(CallInst, PS);
    return true;
  }
  if (FName == "-[NSData mutableCopy]") {
    anonymous_132(CallInst, PS);
    return true;
  }
  if (FName == "-[NSData subdataWithRange:]") {
    anonymous_537(CallInst, PS);
    return true;
  }
  if (FName == "-[NSData writeToFile:options:error:]") {
    anonymous_148(CallInst, PS);
    return true;
  }
  if (FName == "-[NSDictionary initWithObjectsAndKeys:]") {
    anonymous_428(CallInst, PS);
    return true;
  }
  if (FName == "-[NSDictionary init]") {
    anonymous_799(CallInst, PS);
    return true;
  }
  if (FName == "-[NSDictionary objectForKey:]") {
    anonymous_1127(CallInst, PS);
    anonymous_435(CallInst, PS);
    return true;
  }
  if (FName == "-[NSDictionary setObject:forKey:]") {
    anonymous_1131(CallInst, PS);
    return true;
  }
  if (FName == "-[NSDictionary setValue:forKey:]") {
    anonymous_1142(CallInst, PS);
    return true;
  }
  if (FName == "-[NSDictionary valueForKey:]") {
    anonymous_1138(CallInst, PS);
    anonymous_442(CallInst, PS);
    return true;
  }
  if (FName == "-[NSFileHandle readDataOfLength:]") {
    anonymous_1019(CallInst, PS);
    return true;
  }
  if (FName == "-[NSMutableData appendBytes:length:]") {
    anonymous_565(CallInst, PS);
    return true;
  }
  if (FName == "-[NSMutableData appendData:]") {
    anonymous_553(CallInst, PS);
    return true;
  }
  if (FName == "-[NSMutableData init]") {
    anonymous_788(CallInst, PS);
    return true;
  }
  if (FName == "-[NSMutableData mutableBytes]") {
    anonymous_251(CallInst, PS);
    return true;
  }
  if (FName == "-[NSMutableData setLength:]") {
    anonymous_550(CallInst, PS);
    return true;
  }
  if (FName == "-[NSMutableDictionary init]") {
    anonymous_798(CallInst, PS);
    return true;
  }
  if (FName == "-[NSMutableString copy]") {
    anonymous_825(CallInst, PS);
    return true;
  }
  if (FName == "-[NSNumber integerValue]") {
    anonymous_987(CallInst, PS);
    return true;
  }
  if (FName == "-[NSObject autorelease]") {
    anonymous_23(CallInst, PS);
    return true;
  }
  if (FName == "-[NSObject dealloc]") {
    anonymous_26(CallInst, PS);
    return true;
  }
  if (FName == "-[NSObject init]") {
    anonymous_781(CallInst, PS);
    return true;
  }
  if (FName == "-[NSObject release]") {
    anonymous_25(CallInst, PS);
    return true;
  }
  if (FName == "-[NSObject respondsToSelector:]") {
    anonymous_27(CallInst, PS);
    return true;
  }
  if (FName == "-[NSObject retain]") {
    anonymous_22(CallInst, PS);
    return true;
  }
  if (FName == "-[NSString UTF8String]") {
    anonymous_718(CallInst, PS);
    return true;
  }
  if (FName == "-[NSString cStringUsingEncoding:]") {
    anonymous_341(CallInst, PS);
    return true;
  }
  if (FName == "-[NSString componentsSeparatedByString:]") {
    anonymous_1069(CallInst, PS);
    return true;
  }
  if (FName == "-[NSString copy]") {
    anonymous_816(CallInst, PS);
    return true;
  }
  if (FName == "-[NSString dataUsingEncoding:]") {
    anonymous_580(CallInst, PS);
    return true;
  }
  if (FName == "-[NSString getBytes:maxLength:usedLength:encoding:options:range:remainingRange:]") {
    anonymous_332(CallInst, PS);
    return true;
  }
  if (FName == "-[NSString getCString:maxLength:encoding:]") {
    anonymous_323(CallInst, PS);
    return true;
  }
  if (FName == "-[NSString initWithBytes:length:encoding:]") {
    anonymous_795(CallInst, PS);
    return true;
  }
  if (FName == "-[NSString initWithData:encoding:]") {
    anonymous_299(CallInst, PS);
    anonymous_796(CallInst, PS);
    return true;
  }
  if (FName == "-[NSString initWithFormat:]") {
    anonymous_797(CallInst, PS);
    return true;
  }
  if (FName == "-[NSString initWithString:]") {
    anonymous_311(CallInst, PS);
    return true;
  }
  if (FName == "-[NSString intValue]") {
    anonymous_993(CallInst, PS);
    return true;
  }
  if (FName == "-[NSString integerValue]") {
    anonymous_999(CallInst, PS);
    return true;
  }
  if (FName == "-[NSString lowercaseString]") {
    anonymous_350(CallInst, PS);
    return true;
  }
  if (FName == "-[NSString stringByAppendingString:]") {
    anonymous_1057(CallInst, PS);
    return true;
  }
  if (FName == "-[NSString stringByTrimmingCharactersInSet:]") {
    anonymous_1048(CallInst, PS);
    return true;
  }
  if (FName == "-[NSUserDefaults objectForKey:]") {
    anonymous_1101(CallInst, PS);
    return true;
  }
  if (FName == "-[NSUserDefaults setObject:forKey:]") {
    anonymous_1108(CallInst, PS);
    return true;
  }
  if (FName == "-[UIAlertView textFieldAtIndex:]") {
    anonymous_1121(CallInst, PS);
    return true;
  }
  if (FName == "-[UILabel text]") {
    anonymous_1042(CallInst, PS);
    return true;
  }
  if (FName == "-[UITableViewController init]") {
    anonymous_800(CallInst, PS);
    return true;
  }
  if (FName == "-[UITextField text]") {
    anonymous_1030(CallInst, PS);
    return true;
  }
  if (FName == "-[UITextView text]") {
    anonymous_1036(CallInst, PS);
    return true;
  }
  if (FName == "-[UIView initWithFrame:]") {
    anonymous_1081(CallInst, PS);
    return true;
  }
  if (FName == "-[UIViewController initWithNibName:bundle:]") {
    anonymous_1088(CallInst, PS);
    return true;
  }
  if (FName == "CCCalibratePBKDF") {
    anonymous_621(CallInst, PS);
    return true;
  }
  if (FName == "CCCrypt") {
    anonymous_631(CallInst, PS);
    return true;
  }
  if (FName == "CCCryptorCreate") {
    anonymous_626(CallInst, PS);
    return true;
  }
  if (FName == "CCCryptorCreateWithMode") {
    anonymous_627(CallInst, PS);
    return true;
  }
  if (FName == "CCCryptorUpdate") {
    anonymous_632(CallInst, PS);
    return true;
  }
  if (FName == "CCKeyDerivationPBKDF") {
    anonymous_604(CallInst, PS);
    return true;
  }
  if (FName == "CC_MD5") {
    anonymous_951(CallInst, PS);
    return true;
  }
  if (FName == "CC_SHA256") {
    anonymous_958(CallInst, PS);
    return true;
  }
  if (FName == "CC_SHA256_Final") {
    anonymous_857(CallInst, PS);
    return true;
  }
  if (FName == "CC_SHA256_Init") {
    anonymous_843(CallInst, PS);
    return true;
  }
  if (FName == "CC_SHA256_Update") {
    anonymous_847(CallInst, PS);
    return true;
  }
  if (FName == "NSLog") {
    anonymous_925(CallInst, PS);
    return true;
  }
  if (FName == "SecRandomCopyBytes") {
    anonymous_870(CallInst, PS);
    return true;
  }
  if (FName == "_Znam") {
    anonymous_936(CallInst, PS);
    return true;
  }
  if (FName == "_Znwm") {
    anonymous_941(CallInst, PS);
    return true;
  }
  if (FName == "_ZnwmRKSt9nothrow_t") {
    anonymous_946(CallInst, PS);
    return true;
  }
  if (FName == "__stack_chk_fail") {
    anonymous_926(CallInst, PS);
    return true;
  }
  if (FName == "arc4random") {
    anonymous_864(CallInst, PS);
    return true;
  }
  if (FName == "bzero") {
    anonymous_907(CallInst, PS);
    return true;
  }
  if (FName == "malloc") {
    anonymous_1005(CallInst, PS);
    return true;
  }
  if (FName == "memcpy") {
    anonymous_593(CallInst, PS);
    return true;
  }
  if (FName == "objc_autorelease") {
    anonymous_18(CallInst, PS);
    return true;
  }
  if (FName == "objc_autoreleaseReturnValue") {
    anonymous_17(CallInst, PS);
    return true;
  }
  if (FName == "objc_begin_catch") {
    anonymous_935(CallInst, PS);
    return true;
  }
  if (FName == "objc_destroyWeak") {
    anonymous_933(CallInst, PS);
    return true;
  }
  if (FName == "objc_end_catch") {
    anonymous_934(CallInst, PS);
    return true;
  }
  if (FName == "objc_enumerationMutation") {
    anonymous_927(CallInst, PS);
    return true;
  }
  if (FName == "objc_exception_rethrow") {
    anonymous_931(CallInst, PS);
    return true;
  }
  if (FName == "objc_exception_throw") {
    anonymous_932(CallInst, PS);
    return true;
  }
  if (FName == "objc_getClass") {
    anonymous_898(CallInst, PS);
    return true;
  }
  if (FName == "objc_getProperty") {
    anonymous_654(CallInst, PS);
    return true;
  }
  if (FName == "objc_loadWeakRetained") {
    anonymous_704(CallInst, PS);
    return true;
  }
  if (FName == "objc_release") {
    anonymous_24(CallInst, PS);
    return true;
  }
  if (FName == "objc_retain") {
    anonymous_8(CallInst, PS);
    return true;
  }
  if (FName == "objc_retainAutorelease") {
    anonymous_21(CallInst, PS);
    return true;
  }
  if (FName == "objc_retainAutoreleaseReturnValue") {
    anonymous_20(CallInst, PS);
    return true;
  }
  if (FName == "objc_retainAutoreleasedReturnValue") {
    anonymous_19(CallInst, PS);
    return true;
  }
  if (FName == "objc_setProperty") {
    anonymous_692(CallInst, PS);
    return true;
  }
  if (FName == "objc_setProperty_atomic") {
    anonymous_680(CallInst, PS);
    return true;
  }
  if (FName == "objc_setProperty_nonatomic") {
    anonymous_668(CallInst, PS);
    return true;
  }
  if (FName == "objc_setProperty_nonatomic_copy") {
    anonymous_645(CallInst, PS);
    return true;
  }
  if (FName == "objc_storeStrong") {
    anonymous_878(CallInst, PS);
    return true;
  }
  if (FName == "objc_storeWeak") {
    anonymous_888(CallInst, PS);
    return true;
  }
  if (FName == "objc_sync_enter") {
    anonymous_930(CallInst, PS);
    return true;
  }
  if (FName == "objc_sync_exit") {
    anonymous_929(CallInst, PS);
    return true;
  }
  if (FName == "objc_terminate") {
    anonymous_928(CallInst, PS);
    return true;
  }
  if (FName == "open") {
    anonymous_971(CallInst, PS);
    return true;
  }
  if (FName == "read") {
    anonymous_965(CallInst, PS);
    return true;
  }
  return false;
}

}


#endif //LLVM_EXTERNALHANDLER_H

