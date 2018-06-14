#ifndef LLVM_EXTERNALHANDLER_H
#define LLVM_EXTERNALHANDLER_H

namespace llvm {
    namespace pointsto {
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
void anonymous_1005(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "malloc"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_1008(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSFileHandle fileHandleForReadingAtPath:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSFileHandle");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_1019(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSFileHandle readDataOfLength:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_1030(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[UITextField text]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSString");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_1036(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[UITextView text]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSString");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_1042(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[UILabel text]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSString");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_1048(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSString stringByTrimmingCharactersInSet:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSString");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_1057(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSString stringByAppendingString:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSString");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_106(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSData dataWithContentsOfURL:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_1069(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSString componentsSeparatedByString:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSArray");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
{ //Store operation
DetectParametersPass::UserSet_t Vals = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t Locs = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {
    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);
    if (valIdx == AndersNodeFactory::InvalidIndex)
      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);
    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {
      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);
      if (locIdx == AndersNodeFactory::InvalidIndex)
        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);
      andersen->addConstraint(AndersConstraint::STORE, locIdx, valIdx);
    }
  }
}
}

void anonymous_1081(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[UIView initWithFrame:]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_1088(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[UIViewController initWithNibName:bundle:]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_1095(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSNull null]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSNull");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_1098(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[UIApplication sharedApplication]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "UIApplication");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_1101(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSUserDefaults objectForKey:]"
{ //Load operation
DetectParametersPass::UserSet_t Vals = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t Locs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {
    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);
    if (valIdx == AndersNodeFactory::InvalidIndex)
      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);
    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {
      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);
      if (locIdx == AndersNodeFactory::InvalidIndex)
        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);
      andersen->addConstraint(AndersConstraint::LOAD, valIdx, locIdx);
    }
  }
}
}

void anonymous_1108(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSUserDefaults setObject:forKey:]"
{ //Store operation
DetectParametersPass::UserSet_t Vals = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), CallInst);
DetectParametersPass::UserSet_t Locs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {
    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);
    if (valIdx == AndersNodeFactory::InvalidIndex)
      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);
    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {
      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);
      if (locIdx == AndersNodeFactory::InvalidIndex)
        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);
      andersen->addConstraint(AndersConstraint::STORE, locIdx, valIdx);
    }
  }
}
}

void anonymous_1115(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSKeyedArchiver archivedDataWithRootObject:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_1121(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[UIAlertView textFieldAtIndex:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "UITextField");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_1127(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSDictionary objectForKey:]"
{ //Load operation
DetectParametersPass::UserSet_t Vals = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t Locs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {
    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);
    if (valIdx == AndersNodeFactory::InvalidIndex)
      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);
    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {
      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);
      if (locIdx == AndersNodeFactory::InvalidIndex)
        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);
      andersen->addConstraint(AndersConstraint::LOAD, valIdx, locIdx);
    }
  }
}
}

void anonymous_1131(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSDictionary setObject:forKey:]"
{ //Store operation
DetectParametersPass::UserSet_t Vals = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), CallInst);
DetectParametersPass::UserSet_t Locs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {
    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);
    if (valIdx == AndersNodeFactory::InvalidIndex)
      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);
    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {
      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);
      if (locIdx == AndersNodeFactory::InvalidIndex)
        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);
      andersen->addConstraint(AndersConstraint::STORE, locIdx, valIdx);
    }
  }
}
}

void anonymous_1138(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSDictionary valueForKey:]"
{ //Load operation
DetectParametersPass::UserSet_t Vals = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t Locs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {
    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);
    if (valIdx == AndersNodeFactory::InvalidIndex)
      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);
    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {
      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);
      if (locIdx == AndersNodeFactory::InvalidIndex)
        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);
      andersen->addConstraint(AndersConstraint::LOAD, valIdx, locIdx);
    }
  }
}
}

void anonymous_1142(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSDictionary setValue:forKey:]"
{ //Store operation
DetectParametersPass::UserSet_t Vals = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), CallInst);
DetectParametersPass::UserSet_t Locs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {
    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);
    if (valIdx == AndersNodeFactory::InvalidIndex)
      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);
    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {
      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);
      if (locIdx == AndersNodeFactory::InvalidIndex)
        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);
      andersen->addConstraint(AndersConstraint::STORE, locIdx, valIdx);
    }
  }
}
}

void anonymous_119(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSData dataWithData:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_132(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSData mutableCopy]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_145(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSData isEqual:]"
}

void anonymous_148(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSData writeToFile:options:error:]"
}

void anonymous_149(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSMutableData dataWithBytes:length:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_162(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSMutableData data]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_17(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_autoreleaseReturnValue"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_170(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSMutableData dataWithBytesNoCopy:length:freeWhenDone:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_18(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_autorelease"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_183(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSMutableData dataWithContentsOfFile:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_19(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_retainAutoreleasedReturnValue"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_196(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSMutableData dataWithContentsOfFile:options:error:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_20(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_retainAutoreleaseReturnValue"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_209(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSMutableData dataWithContentsOfURL:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_21(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_retainAutorelease"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_22(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSObject retain]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_222(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSMutableData dataWithData:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_23(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSObject autorelease]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_235(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSMutableData dataWithCapacity:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_24(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_release"
}

void anonymous_243(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSMutableData dataWithLength:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_25(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSObject release]"
}

void anonymous_251(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSMutableData mutableBytes]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_26(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSObject dealloc]"
}

void anonymous_260(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSString string]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSString");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_266(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSString stringWithString:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSString");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_27(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSObject respondsToSelector:]"
}

void anonymous_277(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSString stringWithCString:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSString");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_288(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSString stringWithCharacters:length:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSString");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_299(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSString initWithData:encoding:]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_311(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSString initWithString:]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_323(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSString getCString:maxLength:encoding:]"
{ //Check object
DetectParametersPass::UserSet_t Reg = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), CallInst);
  for (auto Post_it = Reg.begin(); Post_it != Reg.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
if (objIndex != AndersNodeFactory::InvalidIndex)
continue;    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNodeDummy(*Post_it, *CallInst->getParent()->getParent()->getParent());
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_33(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSData dataWithBytes:length:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_332(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSString getBytes:maxLength:usedLength:encoding:options:range:remainingRange:]"
{ //Check object
DetectParametersPass::UserSet_t Reg = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), CallInst);
  for (auto Post_it = Reg.begin(); Post_it != Reg.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
if (objIndex != AndersNodeFactory::InvalidIndex)
continue;    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNodeDummy(*Post_it, *CallInst->getParent()->getParent()->getParent());
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_341(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSString cStringUsingEncoding:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_350(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSString lowercaseString]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSString");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_361(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSMutableString stringWithString:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableString");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_372(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSMutableString stringWithCharacters:length:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableString");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_383(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSDictionary dictionaryWithContentsOfFile:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSDictionary");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_389(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSDictionary dictionaryWithObject:forKey:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSDictionary");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
{ //Store operation
DetectParametersPass::UserSet_t Vals = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), CallInst);
DetectParametersPass::UserSet_t Locs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {
    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);
    if (valIdx == AndersNodeFactory::InvalidIndex)
      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);
    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {
      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);
      if (locIdx == AndersNodeFactory::InvalidIndex)
        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);
      andersen->addConstraint(AndersConstraint::STORE, locIdx, valIdx);
    }
  }
}
}

void anonymous_398(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSDictionary dictionaryWithObjectsAndKeys:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSDictionary");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_404(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSDictionary dictionary]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSDictionary");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_410(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSDictionary dictionaryWithDictionary:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableDictionary");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_416(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSDictionary dictionaryWithObjects:forKeys:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSDictionary");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_422(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSDictionary dictionaryWithObjects:forKeys:count:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSDictionary");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_428(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSDictionary initWithObjectsAndKeys:]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_435(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSDictionary objectForKey:]"
{ //Load operation
DetectParametersPass::UserSet_t Vals = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t Locs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {
    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);
    if (valIdx == AndersNodeFactory::InvalidIndex)
      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);
    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {
      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);
      if (locIdx == AndersNodeFactory::InvalidIndex)
        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);
      andersen->addConstraint(AndersConstraint::LOAD, valIdx, locIdx);
    }
  }
}
}

void anonymous_442(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSDictionary valueForKey:]"
{ //Load operation
DetectParametersPass::UserSet_t Vals = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t Locs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {
    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);
    if (valIdx == AndersNodeFactory::InvalidIndex)
      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);
    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {
      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);
      if (locIdx == AndersNodeFactory::InvalidIndex)
        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);
      andersen->addConstraint(AndersConstraint::LOAD, valIdx, locIdx);
    }
  }
}
}

void anonymous_449(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSMutableDictionary dictionaryWithContentsOfFile:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableDictionary");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_455(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSMutableDictionary dictionaryWithObject:forKey:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableDictionary");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
{ //Store operation
DetectParametersPass::UserSet_t Vals = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), CallInst);
DetectParametersPass::UserSet_t Locs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {
    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);
    if (valIdx == AndersNodeFactory::InvalidIndex)
      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);
    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {
      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);
      if (locIdx == AndersNodeFactory::InvalidIndex)
        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);
      andersen->addConstraint(AndersConstraint::STORE, locIdx, valIdx);
    }
  }
}
}

void anonymous_46(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSData dataWithBytesNoCopy:length:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_464(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSMutableDictionary dictionaryWithObjectsAndKeys:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableDictionary");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_470(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSMutableDictionary dictionary]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableDictionary");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_476(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSMutableDictionary dictionaryWithDictionary:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableDictionary");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_482(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSJSONSerialization dataWithJSONObject:options:error:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_488(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSData bytes]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_500(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSData getBytes:range:]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_517(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSData getBytes:length:]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_537(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSData subdataWithRange:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_550(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSMutableData setLength:]"
}

void anonymous_553(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSMutableData appendData:]"
}

void anonymous_565(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSMutableData appendBytes:length:]"
}

void anonymous_577(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSData length]"
}

void anonymous_580(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSString dataUsingEncoding:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_59(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSData data]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_593(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "memcpy"
{ //Check object
DetectParametersPass::UserSet_t Reg = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Reg.begin(); Post_it != Reg.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
if (objIndex != AndersNodeFactory::InvalidIndex)
continue;    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNodeDummy(*Post_it, *CallInst->getParent()->getParent()->getParent());
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_604(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "CCKeyDerivationPBKDF"
{ //Check object
DetectParametersPass::UserSet_t Reg = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X7"), CallInst);
  for (auto Post_it = Reg.begin(); Post_it != Reg.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
if (objIndex != AndersNodeFactory::InvalidIndex)
continue;    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNodeDummy(*Post_it, *CallInst->getParent()->getParent()->getParent());
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_621(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "CCCalibratePBKDF"
}

void anonymous_626(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "CCCryptorCreate"
}

void anonymous_627(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "CCCryptorCreateWithMode"
}

void anonymous_631(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "CCCrypt"
}

void anonymous_632(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "CCCryptorUpdate"
}

void anonymous_645(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_setProperty_nonatomic_copy"
}

void anonymous_654(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_getProperty"
{ //Load operation
DetectParametersPass::UserSet_t Vals = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t Locs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), CallInst);
  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {
    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);
    if (valIdx == AndersNodeFactory::InvalidIndex)
      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);
    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {
      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);
      if (locIdx == AndersNodeFactory::InvalidIndex)
        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);
      andersen->addConstraint(AndersConstraint::LOAD, valIdx, locIdx);
    }
  }
}
}

void anonymous_668(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_setProperty_nonatomic"
{ //Store operation
DetectParametersPass::UserSet_t Vals = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X3"), CallInst);
DetectParametersPass::UserSet_t Locs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), CallInst);
  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {
    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);
    if (valIdx == AndersNodeFactory::InvalidIndex)
      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);
    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {
      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);
      if (locIdx == AndersNodeFactory::InvalidIndex)
        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);
      andersen->addConstraint(AndersConstraint::STORE, locIdx, valIdx);
    }
  }
}
}

void anonymous_67(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSData dataWithBytesNoCopy:length:freeWhenDone:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_680(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_setProperty_atomic"
{ //Store operation
DetectParametersPass::UserSet_t Vals = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X3"), CallInst);
DetectParametersPass::UserSet_t Locs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), CallInst);
  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {
    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);
    if (valIdx == AndersNodeFactory::InvalidIndex)
      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);
    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {
      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);
      if (locIdx == AndersNodeFactory::InvalidIndex)
        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);
      andersen->addConstraint(AndersConstraint::STORE, locIdx, valIdx);
    }
  }
}
}

void anonymous_692(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_setProperty"
{ //Store operation
DetectParametersPass::UserSet_t Vals = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X3"), CallInst);
DetectParametersPass::UserSet_t Locs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), CallInst);
  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {
    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);
    if (valIdx == AndersNodeFactory::InvalidIndex)
      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);
    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {
      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);
      if (locIdx == AndersNodeFactory::InvalidIndex)
        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);
      andersen->addConstraint(AndersConstraint::STORE, locIdx, valIdx);
    }
  }
}
}

void anonymous_704(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_loadWeakRetained"
{ //Load operation
DetectParametersPass::UserSet_t Vals = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t Locs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {
    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);
    if (valIdx == AndersNodeFactory::InvalidIndex)
      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);
    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {
      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);
      if (locIdx == AndersNodeFactory::InvalidIndex)
        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);
      andersen->addConstraint(AndersConstraint::LOAD, valIdx, locIdx);
    }
  }
}
}

void anonymous_718(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSString UTF8String]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "char");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_731(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSArray arrayWithArray:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSArray");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_734(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSArray arrayWithObject:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSArray");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_735(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSArray arrayWithObjects:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSArray");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_736(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSArray array]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSArray");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_737(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSArray objectAtIndex:]"
{ //Load operation
DetectParametersPass::UserSet_t Vals = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t Locs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {
    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);
    if (valIdx == AndersNodeFactory::InvalidIndex)
      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);
    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {
      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);
      if (locIdx == AndersNodeFactory::InvalidIndex)
        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);
      andersen->addConstraint(AndersConstraint::LOAD, valIdx, locIdx);
    }
  }
}
}

void anonymous_747(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSArray objectAtIndexedSubscript:]"
{ //Load operation
DetectParametersPass::UserSet_t Vals = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t Locs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {
    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);
    if (valIdx == AndersNodeFactory::InvalidIndex)
      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);
    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {
      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);
      if (locIdx == AndersNodeFactory::InvalidIndex)
        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);
      andersen->addConstraint(AndersConstraint::LOAD, valIdx, locIdx);
    }
  }
}
}

void anonymous_757(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSMutableString stringWithCapacity:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableString");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_763(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSMutableString stringWithUTF8String:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableString");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_764(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSMutableString string]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableString");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_765(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSString stringWithCString:encoding:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSString");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_771(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSString stringWithContentsOfFile:encoding:error:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSString");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_772(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSString stringWithFormat:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSString");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_773(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSString stringWithUTF8String:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSString");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_774(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSString stringWithCString:encoding:]"
}

void anonymous_781(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSObject init]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_788(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSMutableData init]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_789(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSData initWithBytes:length:]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_790(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSData initWithBase64EncodedData:options:]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_791(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSData initWithContentsOfFile:]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_792(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSData initWithData:]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_793(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSData init]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_794(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSData initWithBase64EncodedString:options:]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_795(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSString initWithBytes:length:encoding:]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_796(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSString initWithData:encoding:]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_797(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSString initWithFormat:]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_798(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSMutableDictionary init]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_799(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSDictionary init]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_8(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_retain"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_80(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSData dataWithContentsOfFile:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_800(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[UITableViewController init]"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_801(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSData initWithBytes:length:]"
}

void anonymous_810(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSData initWithBase64EncodedString:options:]"
}

void anonymous_816(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSString copy]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSString");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_825(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSMutableString copy]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSMutableString");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_834(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSData copy]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_843(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "CC_SHA256_Init"
}

void anonymous_847(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "CC_SHA256_Update"
}

void anonymous_857(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "CC_SHA256_Final"
}

void anonymous_864(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "arc4random"
}

void anonymous_870(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "SecRandomCopyBytes"
{ //Check object
DetectParametersPass::UserSet_t Reg = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X2"), CallInst);
  for (auto Post_it = Reg.begin(); Post_it != Reg.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
if (objIndex != AndersNodeFactory::InvalidIndex)
continue;    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNodeDummy(*Post_it, *CallInst->getParent()->getParent()->getParent());
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_878(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_storeStrong"
{ //Store operation
DetectParametersPass::UserSet_t Vals = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X1"), CallInst);
DetectParametersPass::UserSet_t Locs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {
    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);
    if (valIdx == AndersNodeFactory::InvalidIndex)
      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);
    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {
      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);
      if (locIdx == AndersNodeFactory::InvalidIndex)
        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);
      andersen->addConstraint(AndersConstraint::STORE, locIdx, valIdx);
    }
  }
}
}

void anonymous_888(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_storeWeak"
{ //Store operation
DetectParametersPass::UserSet_t Vals = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X1"), CallInst);
DetectParametersPass::UserSet_t Locs = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
  for (auto Vals_it = Vals.begin(); Vals_it != Vals.end(); ++Vals_it) {
    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(*Vals_it);
    if (valIdx == AndersNodeFactory::InvalidIndex)
      valIdx = andersen->getNodeFactory().createValueNode(*Vals_it);
    for (auto Locs_it = Locs.begin(); Locs_it != Locs.end(); ++Locs_it) {
      NodeIndex locIdx = andersen->getNodeFactory().getValueNodeFor(*Locs_it);
      if (locIdx == AndersNodeFactory::InvalidIndex)
        locIdx = andersen->getNodeFactory().createValueNode(*Locs_it);
      andersen->addConstraint(AndersConstraint::STORE, locIdx, valIdx);
    }
  }
}
}

void anonymous_898(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_getClass"
{ //Copy operation
DetectParametersPass::UserSet_t From = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
DetectParametersPass::UserSet_t To = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto From_it = From.begin(); From_it != From.end(); ++From_it) {
    NodeIndex srcIdx = andersen->getNodeFactory().getValueNodeFor(*From_it);
    if (srcIdx == AndersNodeFactory::InvalidIndex)
      srcIdx = andersen->getNodeFactory().createValueNode(*From_it);
    for (auto To_it = To.begin(); To_it != To.end(); ++To_it) {
      NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(*To_it);
      if (dstIdx == AndersNodeFactory::InvalidIndex)
        dstIdx = andersen->getNodeFactory().createValueNode(*To_it);
      andersen->addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
    }
  }
} //end copy
}

void anonymous_907(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "bzero"
{ //Check object
DetectParametersPass::UserSet_t Reg = DetectParametersPass::getRegisterValuesBeforeCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Reg.begin(); Post_it != Reg.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
if (objIndex != AndersNodeFactory::InvalidIndex)
continue;    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNodeDummy(*Post_it, *CallInst->getParent()->getParent()->getParent());
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_915(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSBundle bundleWithPath:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSBundle");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_923(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSBundle bundleForClass:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSBundle");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_924(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSBundle mainBundle]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSBundle");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_925(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "NSLog"
}

void anonymous_926(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "__stack_chk_fail"
}

void anonymous_927(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_enumerationMutation"
}

void anonymous_928(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_terminate"
}

void anonymous_929(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_sync_exit"
}

void anonymous_93(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSData dataWithContentsOfFile:options:error:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSData");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_930(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_sync_enter"
}

void anonymous_931(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_exception_rethrow"
}

void anonymous_932(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_exception_throw"
}

void anonymous_933(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_destroyWeak"
}

void anonymous_934(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_end_catch"
}

void anonymous_935(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "objc_begin_catch"
}

void anonymous_936(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "_Znam"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_941(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "_Znwm"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_946(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "_ZnwmRKSt9nothrow_t"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_951(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "CC_MD5"
}

void anonymous_958(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "CC_SHA256"
}

void anonymous_965(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "read"
}

void anonymous_971(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "open"
}

void anonymous_977(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "+[NSNumber numberWithInt:]"
{ //Alloc operation
DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(translateRegister("X0"), CallInst);
  for (auto Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*Post_it);
    if (valIndex == AndersNodeFactory::InvalidIndex)
      valIndex = andersen->getNodeFactory().createValueNode(*Post_it);
    NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*Post_it);
    if (objIndex == AndersNodeFactory::InvalidIndex)
      objIndex = andersen->getNodeFactory().createObjectNode(*Post_it);
    andersen->setType(*Post_it, "NSNumber");
    andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
  }
}
}

void anonymous_987(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSNumber integerValue]"
}

void anonymous_993(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSString intValue]"
}

void anonymous_999(llvm::Instruction *CallInst, Andersen *andersen) {
//Handle "-[NSString integerValue]"
}

bool canHandleCall(const std::string &FName) {
  if (FName == "+[NSArray arrayWithArray:]")
    return true;
  if (FName == "+[NSArray arrayWithObject:]")
    return true;
  if (FName == "+[NSArray arrayWithObjects:]")
    return true;
  if (FName == "+[NSArray array]")
    return true;
  if (FName == "+[NSBundle bundleForClass:]")
    return true;
  if (FName == "+[NSBundle bundleWithPath:]")
    return true;
  if (FName == "+[NSBundle mainBundle]")
    return true;
  if (FName == "+[NSData dataWithBytes:length:]")
    return true;
  if (FName == "+[NSData dataWithBytesNoCopy:length:]")
    return true;
  if (FName == "+[NSData dataWithBytesNoCopy:length:freeWhenDone:]")
    return true;
  if (FName == "+[NSData dataWithContentsOfFile:]")
    return true;
  if (FName == "+[NSData dataWithContentsOfFile:options:error:]")
    return true;
  if (FName == "+[NSData dataWithContentsOfURL:]")
    return true;
  if (FName == "+[NSData dataWithData:]")
    return true;
  if (FName == "+[NSData data]")
    return true;
  if (FName == "+[NSDictionary dictionaryWithContentsOfFile:]")
    return true;
  if (FName == "+[NSDictionary dictionaryWithDictionary:]")
    return true;
  if (FName == "+[NSDictionary dictionaryWithObject:forKey:]")
    return true;
  if (FName == "+[NSDictionary dictionaryWithObjects:forKeys:]")
    return true;
  if (FName == "+[NSDictionary dictionaryWithObjects:forKeys:count:]")
    return true;
  if (FName == "+[NSDictionary dictionaryWithObjectsAndKeys:]")
    return true;
  if (FName == "+[NSDictionary dictionary]")
    return true;
  if (FName == "+[NSFileHandle fileHandleForReadingAtPath:]")
    return true;
  if (FName == "+[NSJSONSerialization dataWithJSONObject:options:error:]")
    return true;
  if (FName == "+[NSKeyedArchiver archivedDataWithRootObject:]")
    return true;
  if (FName == "+[NSMutableData dataWithBytes:length:]")
    return true;
  if (FName == "+[NSMutableData dataWithBytesNoCopy:length:freeWhenDone:]")
    return true;
  if (FName == "+[NSMutableData dataWithCapacity:]")
    return true;
  if (FName == "+[NSMutableData dataWithContentsOfFile:]")
    return true;
  if (FName == "+[NSMutableData dataWithContentsOfFile:options:error:]")
    return true;
  if (FName == "+[NSMutableData dataWithContentsOfURL:]")
    return true;
  if (FName == "+[NSMutableData dataWithData:]")
    return true;
  if (FName == "+[NSMutableData dataWithLength:]")
    return true;
  if (FName == "+[NSMutableData data]")
    return true;
  if (FName == "+[NSMutableDictionary dictionaryWithContentsOfFile:]")
    return true;
  if (FName == "+[NSMutableDictionary dictionaryWithDictionary:]")
    return true;
  if (FName == "+[NSMutableDictionary dictionaryWithObject:forKey:]")
    return true;
  if (FName == "+[NSMutableDictionary dictionaryWithObjectsAndKeys:]")
    return true;
  if (FName == "+[NSMutableDictionary dictionary]")
    return true;
  if (FName == "+[NSMutableString stringWithCapacity:]")
    return true;
  if (FName == "+[NSMutableString stringWithCharacters:length:]")
    return true;
  if (FName == "+[NSMutableString stringWithString:]")
    return true;
  if (FName == "+[NSMutableString stringWithUTF8String:]")
    return true;
  if (FName == "+[NSMutableString string]")
    return true;
  if (FName == "+[NSNull null]")
    return true;
  if (FName == "+[NSNumber numberWithInt:]")
    return true;
  if (FName == "+[NSString stringWithCString:]")
    return true;
  if (FName == "+[NSString stringWithCString:encoding:]")
    return true;
  if (FName == "+[NSString stringWithCharacters:length:]")
    return true;
  if (FName == "+[NSString stringWithContentsOfFile:encoding:error:]")
    return true;
  if (FName == "+[NSString stringWithFormat:]")
    return true;
  if (FName == "+[NSString stringWithString:]")
    return true;
  if (FName == "+[NSString stringWithUTF8String:]")
    return true;
  if (FName == "+[NSString string]")
    return true;
  if (FName == "+[UIApplication sharedApplication]")
    return true;
  if (FName == "-[NSArray objectAtIndex:]")
    return true;
  if (FName == "-[NSArray objectAtIndexedSubscript:]")
    return true;
  if (FName == "-[NSData bytes]")
    return true;
  if (FName == "-[NSData copy]")
    return true;
  if (FName == "-[NSData getBytes:length:]")
    return true;
  if (FName == "-[NSData getBytes:range:]")
    return true;
  if (FName == "-[NSData initWithBase64EncodedData:options:]")
    return true;
  if (FName == "-[NSData initWithBase64EncodedString:options:]")
    return true;
  if (FName == "-[NSData initWithBytes:length:]")
    return true;
  if (FName == "-[NSData initWithContentsOfFile:]")
    return true;
  if (FName == "-[NSData initWithData:]")
    return true;
  if (FName == "-[NSData init]")
    return true;
  if (FName == "-[NSData isEqual:]")
    return true;
  if (FName == "-[NSData length]")
    return true;
  if (FName == "-[NSData mutableCopy]")
    return true;
  if (FName == "-[NSData subdataWithRange:]")
    return true;
  if (FName == "-[NSData writeToFile:options:error:]")
    return true;
  if (FName == "-[NSDictionary initWithObjectsAndKeys:]")
    return true;
  if (FName == "-[NSDictionary init]")
    return true;
  if (FName == "-[NSDictionary objectForKey:]")
    return true;
  if (FName == "-[NSDictionary setObject:forKey:]")
    return true;
  if (FName == "-[NSDictionary setValue:forKey:]")
    return true;
  if (FName == "-[NSDictionary valueForKey:]")
    return true;
  if (FName == "-[NSFileHandle readDataOfLength:]")
    return true;
  if (FName == "-[NSMutableData appendBytes:length:]")
    return true;
  if (FName == "-[NSMutableData appendData:]")
    return true;
  if (FName == "-[NSMutableData init]")
    return true;
  if (FName == "-[NSMutableData mutableBytes]")
    return true;
  if (FName == "-[NSMutableData setLength:]")
    return true;
  if (FName == "-[NSMutableDictionary init]")
    return true;
  if (FName == "-[NSMutableString copy]")
    return true;
  if (FName == "-[NSNumber integerValue]")
    return true;
  if (FName == "-[NSObject autorelease]")
    return true;
  if (FName == "-[NSObject dealloc]")
    return true;
  if (FName == "-[NSObject init]")
    return true;
  if (FName == "-[NSObject release]")
    return true;
  if (FName == "-[NSObject respondsToSelector:]")
    return true;
  if (FName == "-[NSObject retain]")
    return true;
  if (FName == "-[NSString UTF8String]")
    return true;
  if (FName == "-[NSString cStringUsingEncoding:]")
    return true;
  if (FName == "-[NSString componentsSeparatedByString:]")
    return true;
  if (FName == "-[NSString copy]")
    return true;
  if (FName == "-[NSString dataUsingEncoding:]")
    return true;
  if (FName == "-[NSString getBytes:maxLength:usedLength:encoding:options:range:remainingRange:]")
    return true;
  if (FName == "-[NSString getCString:maxLength:encoding:]")
    return true;
  if (FName == "-[NSString initWithBytes:length:encoding:]")
    return true;
  if (FName == "-[NSString initWithData:encoding:]")
    return true;
  if (FName == "-[NSString initWithFormat:]")
    return true;
  if (FName == "-[NSString initWithString:]")
    return true;
  if (FName == "-[NSString intValue]")
    return true;
  if (FName == "-[NSString integerValue]")
    return true;
  if (FName == "-[NSString lowercaseString]")
    return true;
  if (FName == "-[NSString stringByAppendingString:]")
    return true;
  if (FName == "-[NSString stringByTrimmingCharactersInSet:]")
    return true;
  if (FName == "-[NSUserDefaults objectForKey:]")
    return true;
  if (FName == "-[NSUserDefaults setObject:forKey:]")
    return true;
  if (FName == "-[UIAlertView textFieldAtIndex:]")
    return true;
  if (FName == "-[UILabel text]")
    return true;
  if (FName == "-[UITableViewController init]")
    return true;
  if (FName == "-[UITextField text]")
    return true;
  if (FName == "-[UITextView text]")
    return true;
  if (FName == "-[UIView initWithFrame:]")
    return true;
  if (FName == "-[UIViewController initWithNibName:bundle:]")
    return true;
  if (FName == "CCCalibratePBKDF")
    return true;
  if (FName == "CCCrypt")
    return true;
  if (FName == "CCCryptorCreate")
    return true;
  if (FName == "CCCryptorCreateWithMode")
    return true;
  if (FName == "CCCryptorUpdate")
    return true;
  if (FName == "CCKeyDerivationPBKDF")
    return true;
  if (FName == "CC_MD5")
    return true;
  if (FName == "CC_SHA256")
    return true;
  if (FName == "CC_SHA256_Final")
    return true;
  if (FName == "CC_SHA256_Init")
    return true;
  if (FName == "CC_SHA256_Update")
    return true;
  if (FName == "NSLog")
    return true;
  if (FName == "SecRandomCopyBytes")
    return true;
  if (FName == "_Znam")
    return true;
  if (FName == "_Znwm")
    return true;
  if (FName == "_ZnwmRKSt9nothrow_t")
    return true;
  if (FName == "__stack_chk_fail")
    return true;
  if (FName == "arc4random")
    return true;
  if (FName == "bzero")
    return true;
  if (FName == "malloc")
    return true;
  if (FName == "memcpy")
    return true;
  if (FName == "objc_autorelease")
    return true;
  if (FName == "objc_autoreleaseReturnValue")
    return true;
  if (FName == "objc_begin_catch")
    return true;
  if (FName == "objc_destroyWeak")
    return true;
  if (FName == "objc_end_catch")
    return true;
  if (FName == "objc_enumerationMutation")
    return true;
  if (FName == "objc_exception_rethrow")
    return true;
  if (FName == "objc_exception_throw")
    return true;
  if (FName == "objc_getClass")
    return true;
  if (FName == "objc_getProperty")
    return true;
  if (FName == "objc_loadWeakRetained")
    return true;
  if (FName == "objc_release")
    return true;
  if (FName == "objc_retain")
    return true;
  if (FName == "objc_retainAutorelease")
    return true;
  if (FName == "objc_retainAutoreleaseReturnValue")
    return true;
  if (FName == "objc_retainAutoreleasedReturnValue")
    return true;
  if (FName == "objc_setProperty")
    return true;
  if (FName == "objc_setProperty_atomic")
    return true;
  if (FName == "objc_setProperty_nonatomic")
    return true;
  if (FName == "objc_setProperty_nonatomic_copy")
    return true;
  if (FName == "objc_storeStrong")
    return true;
  if (FName == "objc_storeWeak")
    return true;
  if (FName == "objc_sync_enter")
    return true;
  if (FName == "objc_sync_exit")
    return true;
  if (FName == "objc_terminate")
    return true;
  if (FName == "open")
    return true;
  if (FName == "read")
    return true;
  return false;
}

bool handleCall(llvm::Instruction *CallInst, Andersen *andersen, const std::string &FName) {
  if (FName == "+[NSArray arrayWithArray:]") {
    anonymous_731(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSArray arrayWithObject:]") {
    anonymous_734(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSArray arrayWithObjects:]") {
    anonymous_735(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSArray array]") {
    anonymous_736(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSBundle bundleForClass:]") {
    anonymous_923(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSBundle bundleWithPath:]") {
    anonymous_915(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSBundle mainBundle]") {
    anonymous_924(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSData dataWithBytes:length:]") {
    anonymous_33(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSData dataWithBytesNoCopy:length:]") {
    anonymous_46(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSData dataWithBytesNoCopy:length:freeWhenDone:]") {
    anonymous_67(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSData dataWithContentsOfFile:]") {
    anonymous_80(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSData dataWithContentsOfFile:options:error:]") {
    anonymous_93(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSData dataWithContentsOfURL:]") {
    anonymous_106(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSData dataWithData:]") {
    anonymous_119(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSData data]") {
    anonymous_59(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSDictionary dictionaryWithContentsOfFile:]") {
    anonymous_383(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSDictionary dictionaryWithDictionary:]") {
    anonymous_410(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSDictionary dictionaryWithObject:forKey:]") {
    anonymous_389(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSDictionary dictionaryWithObjects:forKeys:]") {
    anonymous_416(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSDictionary dictionaryWithObjects:forKeys:count:]") {
    anonymous_422(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSDictionary dictionaryWithObjectsAndKeys:]") {
    anonymous_398(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSDictionary dictionary]") {
    anonymous_404(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSFileHandle fileHandleForReadingAtPath:]") {
    anonymous_1008(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSJSONSerialization dataWithJSONObject:options:error:]") {
    anonymous_482(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSKeyedArchiver archivedDataWithRootObject:]") {
    anonymous_1115(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSMutableData dataWithBytes:length:]") {
    anonymous_149(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSMutableData dataWithBytesNoCopy:length:freeWhenDone:]") {
    anonymous_170(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSMutableData dataWithCapacity:]") {
    anonymous_235(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSMutableData dataWithContentsOfFile:]") {
    anonymous_183(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSMutableData dataWithContentsOfFile:options:error:]") {
    anonymous_196(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSMutableData dataWithContentsOfURL:]") {
    anonymous_209(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSMutableData dataWithData:]") {
    anonymous_222(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSMutableData dataWithLength:]") {
    anonymous_243(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSMutableData data]") {
    anonymous_162(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSMutableDictionary dictionaryWithContentsOfFile:]") {
    anonymous_449(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSMutableDictionary dictionaryWithDictionary:]") {
    anonymous_476(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSMutableDictionary dictionaryWithObject:forKey:]") {
    anonymous_455(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSMutableDictionary dictionaryWithObjectsAndKeys:]") {
    anonymous_464(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSMutableDictionary dictionary]") {
    anonymous_470(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSMutableString stringWithCapacity:]") {
    anonymous_757(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSMutableString stringWithCharacters:length:]") {
    anonymous_372(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSMutableString stringWithString:]") {
    anonymous_361(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSMutableString stringWithUTF8String:]") {
    anonymous_763(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSMutableString string]") {
    anonymous_764(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSNull null]") {
    anonymous_1095(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSNumber numberWithInt:]") {
    anonymous_977(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSString stringWithCString:]") {
    anonymous_277(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSString stringWithCString:encoding:]") {
    anonymous_765(CallInst, andersen);
    anonymous_774(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSString stringWithCharacters:length:]") {
    anonymous_288(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSString stringWithContentsOfFile:encoding:error:]") {
    anonymous_771(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSString stringWithFormat:]") {
    anonymous_772(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSString stringWithString:]") {
    anonymous_266(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSString stringWithUTF8String:]") {
    anonymous_773(CallInst, andersen);
    return true;
  }
  if (FName == "+[NSString string]") {
    anonymous_260(CallInst, andersen);
    return true;
  }
  if (FName == "+[UIApplication sharedApplication]") {
    anonymous_1098(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSArray objectAtIndex:]") {
    anonymous_737(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSArray objectAtIndexedSubscript:]") {
    anonymous_747(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSData bytes]") {
    anonymous_488(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSData copy]") {
    anonymous_834(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSData getBytes:length:]") {
    anonymous_517(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSData getBytes:range:]") {
    anonymous_500(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSData initWithBase64EncodedData:options:]") {
    anonymous_790(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSData initWithBase64EncodedString:options:]") {
    anonymous_794(CallInst, andersen);
    anonymous_810(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSData initWithBytes:length:]") {
    anonymous_789(CallInst, andersen);
    anonymous_801(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSData initWithContentsOfFile:]") {
    anonymous_791(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSData initWithData:]") {
    anonymous_792(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSData init]") {
    anonymous_793(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSData isEqual:]") {
    anonymous_145(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSData length]") {
    anonymous_577(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSData mutableCopy]") {
    anonymous_132(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSData subdataWithRange:]") {
    anonymous_537(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSData writeToFile:options:error:]") {
    anonymous_148(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSDictionary initWithObjectsAndKeys:]") {
    anonymous_428(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSDictionary init]") {
    anonymous_799(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSDictionary objectForKey:]") {
    anonymous_1127(CallInst, andersen);
    anonymous_435(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSDictionary setObject:forKey:]") {
    anonymous_1131(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSDictionary setValue:forKey:]") {
    anonymous_1142(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSDictionary valueForKey:]") {
    anonymous_1138(CallInst, andersen);
    anonymous_442(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSFileHandle readDataOfLength:]") {
    anonymous_1019(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSMutableData appendBytes:length:]") {
    anonymous_565(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSMutableData appendData:]") {
    anonymous_553(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSMutableData init]") {
    anonymous_788(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSMutableData mutableBytes]") {
    anonymous_251(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSMutableData setLength:]") {
    anonymous_550(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSMutableDictionary init]") {
    anonymous_798(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSMutableString copy]") {
    anonymous_825(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSNumber integerValue]") {
    anonymous_987(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSObject autorelease]") {
    anonymous_23(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSObject dealloc]") {
    anonymous_26(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSObject init]") {
    anonymous_781(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSObject release]") {
    anonymous_25(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSObject respondsToSelector:]") {
    anonymous_27(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSObject retain]") {
    anonymous_22(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSString UTF8String]") {
    anonymous_718(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSString cStringUsingEncoding:]") {
    anonymous_341(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSString componentsSeparatedByString:]") {
    anonymous_1069(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSString copy]") {
    anonymous_816(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSString dataUsingEncoding:]") {
    anonymous_580(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSString getBytes:maxLength:usedLength:encoding:options:range:remainingRange:]") {
    anonymous_332(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSString getCString:maxLength:encoding:]") {
    anonymous_323(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSString initWithBytes:length:encoding:]") {
    anonymous_795(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSString initWithData:encoding:]") {
    anonymous_299(CallInst, andersen);
    anonymous_796(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSString initWithFormat:]") {
    anonymous_797(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSString initWithString:]") {
    anonymous_311(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSString intValue]") {
    anonymous_993(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSString integerValue]") {
    anonymous_999(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSString lowercaseString]") {
    anonymous_350(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSString stringByAppendingString:]") {
    anonymous_1057(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSString stringByTrimmingCharactersInSet:]") {
    anonymous_1048(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSUserDefaults objectForKey:]") {
    anonymous_1101(CallInst, andersen);
    return true;
  }
  if (FName == "-[NSUserDefaults setObject:forKey:]") {
    anonymous_1108(CallInst, andersen);
    return true;
  }
  if (FName == "-[UIAlertView textFieldAtIndex:]") {
    anonymous_1121(CallInst, andersen);
    return true;
  }
  if (FName == "-[UILabel text]") {
    anonymous_1042(CallInst, andersen);
    return true;
  }
  if (FName == "-[UITableViewController init]") {
    anonymous_800(CallInst, andersen);
    return true;
  }
  if (FName == "-[UITextField text]") {
    anonymous_1030(CallInst, andersen);
    return true;
  }
  if (FName == "-[UITextView text]") {
    anonymous_1036(CallInst, andersen);
    return true;
  }
  if (FName == "-[UIView initWithFrame:]") {
    anonymous_1081(CallInst, andersen);
    return true;
  }
  if (FName == "-[UIViewController initWithNibName:bundle:]") {
    anonymous_1088(CallInst, andersen);
    return true;
  }
  if (FName == "CCCalibratePBKDF") {
    anonymous_621(CallInst, andersen);
    return true;
  }
  if (FName == "CCCrypt") {
    anonymous_631(CallInst, andersen);
    return true;
  }
  if (FName == "CCCryptorCreate") {
    anonymous_626(CallInst, andersen);
    return true;
  }
  if (FName == "CCCryptorCreateWithMode") {
    anonymous_627(CallInst, andersen);
    return true;
  }
  if (FName == "CCCryptorUpdate") {
    anonymous_632(CallInst, andersen);
    return true;
  }
  if (FName == "CCKeyDerivationPBKDF") {
    anonymous_604(CallInst, andersen);
    return true;
  }
  if (FName == "CC_MD5") {
    anonymous_951(CallInst, andersen);
    return true;
  }
  if (FName == "CC_SHA256") {
    anonymous_958(CallInst, andersen);
    return true;
  }
  if (FName == "CC_SHA256_Final") {
    anonymous_857(CallInst, andersen);
    return true;
  }
  if (FName == "CC_SHA256_Init") {
    anonymous_843(CallInst, andersen);
    return true;
  }
  if (FName == "CC_SHA256_Update") {
    anonymous_847(CallInst, andersen);
    return true;
  }
  if (FName == "NSLog") {
    anonymous_925(CallInst, andersen);
    return true;
  }
  if (FName == "SecRandomCopyBytes") {
    anonymous_870(CallInst, andersen);
    return true;
  }
  if (FName == "_Znam") {
    anonymous_936(CallInst, andersen);
    return true;
  }
  if (FName == "_Znwm") {
    anonymous_941(CallInst, andersen);
    return true;
  }
  if (FName == "_ZnwmRKSt9nothrow_t") {
    anonymous_946(CallInst, andersen);
    return true;
  }
  if (FName == "__stack_chk_fail") {
    anonymous_926(CallInst, andersen);
    return true;
  }
  if (FName == "arc4random") {
    anonymous_864(CallInst, andersen);
    return true;
  }
  if (FName == "bzero") {
    anonymous_907(CallInst, andersen);
    return true;
  }
  if (FName == "malloc") {
    anonymous_1005(CallInst, andersen);
    return true;
  }
  if (FName == "memcpy") {
    anonymous_593(CallInst, andersen);
    return true;
  }
  if (FName == "objc_autorelease") {
    anonymous_18(CallInst, andersen);
    return true;
  }
  if (FName == "objc_autoreleaseReturnValue") {
    anonymous_17(CallInst, andersen);
    return true;
  }
  if (FName == "objc_begin_catch") {
    anonymous_935(CallInst, andersen);
    return true;
  }
  if (FName == "objc_destroyWeak") {
    anonymous_933(CallInst, andersen);
    return true;
  }
  if (FName == "objc_end_catch") {
    anonymous_934(CallInst, andersen);
    return true;
  }
  if (FName == "objc_enumerationMutation") {
    anonymous_927(CallInst, andersen);
    return true;
  }
  if (FName == "objc_exception_rethrow") {
    anonymous_931(CallInst, andersen);
    return true;
  }
  if (FName == "objc_exception_throw") {
    anonymous_932(CallInst, andersen);
    return true;
  }
  if (FName == "objc_getClass") {
    anonymous_898(CallInst, andersen);
    return true;
  }
  if (FName == "objc_getProperty") {
    anonymous_654(CallInst, andersen);
    return true;
  }
  if (FName == "objc_loadWeakRetained") {
    anonymous_704(CallInst, andersen);
    return true;
  }
  if (FName == "objc_release") {
    anonymous_24(CallInst, andersen);
    return true;
  }
  if (FName == "objc_retain") {
    anonymous_8(CallInst, andersen);
    return true;
  }
  if (FName == "objc_retainAutorelease") {
    anonymous_21(CallInst, andersen);
    return true;
  }
  if (FName == "objc_retainAutoreleaseReturnValue") {
    anonymous_20(CallInst, andersen);
    return true;
  }
  if (FName == "objc_retainAutoreleasedReturnValue") {
    anonymous_19(CallInst, andersen);
    return true;
  }
  if (FName == "objc_setProperty") {
    anonymous_692(CallInst, andersen);
    return true;
  }
  if (FName == "objc_setProperty_atomic") {
    anonymous_680(CallInst, andersen);
    return true;
  }
  if (FName == "objc_setProperty_nonatomic") {
    anonymous_668(CallInst, andersen);
    return true;
  }
  if (FName == "objc_setProperty_nonatomic_copy") {
    anonymous_645(CallInst, andersen);
    return true;
  }
  if (FName == "objc_storeStrong") {
    anonymous_878(CallInst, andersen);
    return true;
  }
  if (FName == "objc_storeWeak") {
    anonymous_888(CallInst, andersen);
    return true;
  }
  if (FName == "objc_sync_enter") {
    anonymous_930(CallInst, andersen);
    return true;
  }
  if (FName == "objc_sync_exit") {
    anonymous_929(CallInst, andersen);
    return true;
  }
  if (FName == "objc_terminate") {
    anonymous_928(CallInst, andersen);
    return true;
  }
  if (FName == "open") {
    anonymous_971(CallInst, andersen);
    return true;
  }
  if (FName == "read") {
    anonymous_965(CallInst, andersen);
    return true;
  }
  return false;
}

    }
}

#endif //LLVM_EXTERNALHANDLER_H

