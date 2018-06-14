#include <PointsTo/PointsTo.h>
#include <llvm/IR/Dominators.h>
#include <llvm/ADT/StringExtras.h>
#include <sstream>
#include "Path.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"


using namespace llvm;
using namespace slicing;

cl::opt<bool> SameUseDef("same-usedef-only", cl::desc("Paths with same origin and location of use are considered as equal."), cl::init(false), cl::Hidden);

bool PathElement::setNext(PathElementBase *n) {

//    return PathElementBase::setNext(n);

    if (n->getType() != InstructionElement) {
        return PathElementBase::setNext(n);
    }

    PathElement *nElem = (PathElement*)n;

    if (inst->getParent()->getParent() != nElem->inst->getParent()->getParent() && nElem->inst->getOpcode() != Instruction::Ret) {
        if (nElem->inst->getOpcode() == Instruction::Call) {
            assert(true);
        }

//        if (inst->getName().find("init") != StringRef::npos) {
//            if (nElem->getInstruction()->getOpcode() != Instruction::Store) {
//                return false;
//            }
//        }


        if (parent->hasCall()) {
            const CallInst *c = parent->getLatestCall();
            //TODO: check for a correct call...
            parent->popCall();

            if (n->getElement() == c) {
                assert(true);
            } else {
                return false;
            }

            std::string from = nElem->inst->getParent()->getParent()->getName().str();
            std::string to = c->getParent()->getParent()->getName().str();
            if (from != to && !ptr::getSimpleCallGraph().hasPath(from, to)) {
//                errs() << "No call from " << from << " to " << to << "\n";
                assert(true);
                return false;
            }

//            if (c->getParent()->getParent() != nElem->getInstruction()->getParent()->getParent()) {
//                return false;
//            }
//
//            if (const StoreInst *store = dyn_cast<const StoreInst>(nElem->getInstruction())) {
//                if (const GetElementPtrInst *getElementPtrInst = dyn_cast<const GetElementPtrInst>(store->getOperand(1))) {
//                    const Instruction *current = getSuccInBlock(c);
//                    while (current) {
//                        if (const StoreInst *cmp = dyn_cast<const StoreInst>(current)) {
//                            if (cmp->getOperand(1) == getElementPtrInst) {
//                                if (cmp == store) {
//                                    break;
//                                } else {
//                                    return false;
//                                }
//                            }
//                        }
//                        current = current->getPrevNode();
//                    }
//                }
//            }
        }

    }

//    const Function *f = n->getInstruction()->getParent()->getParent();
//    DominatorTree &domTree = ptr::getAndersen()->getAnalysis<DominatorTreeWrapperPass>(*((Function*)f)).getDomTree();
//
//    PathElement *current = this;
//    while (current) {
//        if (current->getInstruction()->getParent()->getParent() == f) {
//            if (!domTree.dominates(n->getInstruction(), inst)) {
//                return false;
//            }
//        }
//        current = current->prev;
//    }

    return PathElementBase::setNext(n);
}

bool PathElementBase::setNext(PathElementBase *n) {
    if (dyn_cast<const GlobalVariable>(n->getElement())) {
        return false;
    }
    n->setParent(parent);
    next = n;
    return true;
}

void PathElement::dump() const {
    print(errs());
}

void PathElement::print(raw_ostream &out) const {
    int64_t address = (int64_t)inst;
    out << utohexstr(address) << " " << inst->getParent()->getParent()->getName() << ": " << *inst << "\n";
    out << "\tRelevant: " << *relevantVariable << "\n";

    if (inst->getOpcode() == Instruction::Call) {
        out << "\tCalled:";
        if (getSuccInBlock(inst))
            out << *getSuccInBlock(inst) << "\n";
        else
            out << "\n";
        SimpleCallGraph::FunctionSet_t &called = ptr::getSimpleCallGraph().getCalled(inst);
        for (auto &c : called) {
            out << "\t\t" << c << "\n";
        }
    }
}

void PathElementBase::dump() const {
    print(errs());
}

void PathElementBase::print(raw_ostream &out) const {
    out << *element << "\n";
    out << "\tRelevant: " << *relevantVariable << "\n";
}

void Path::dump(bool useDefOnly) const {
    print(errs(), useDefOnly);
}

void Path::print(raw_ostream &out, bool useDefOnly) const {
    if (useDefOnly) {
        out << entry << "\n";
        out << getLast() << "\n";
    } else {
        PathElementBase *current = entry;
        while (current) {
//        errs() << current->getInstruction()->getParent()->getParent()->getName() << ": ";
//        current->getInstruction()->print(errs());
//        errs() << "(";
//        current->getRelevantVariable()->print(errs());
//        errs() << ")\n";

            current->print(out);
            current = current->getNext();
        }
    }
}

bool Path::operator < (const Path &b) const
{
    PathElementBase *x = entry;
    PathElementBase *y = b.entry;

    bool same = true;
    while (x && y) {

        if (x->getElement() != y->getElement()) {
            same = false;
            break;
        }

        x = x->getNext();
        y = y->getNext();
    }

    if (same && x == y)
        return false;

    return  x < y;
}

bool Path::operator == (const Path &b) const
{
    if (SameUseDef) {
        return sameUseDef(b);
    }

    PathElementBase *x = entry;
    PathElementBase *y = b.entry;

    bool same = true;
    while (x && y) {

        if (x->getElement() != y->getElement()) {
            same = false;
            break;
        }

        x = x->getNext();
        y = y->getNext();
    }

    if (same && x == y)
        return true;

    return false;


//    if (entry == b.entry && getLast() == b.getLast()) {
//        return true;
//    }
//    return false;
}

uint64_t Path::getShortestLoad(uint64_t currentMin) const {
    PathElementBase *current = entry;
    while (current) {
        if (const LoadInst *inst = dyn_cast<const LoadInst>(current->getElement())) {
            if (inst->getType()->getPrimitiveSizeInBits() < currentMin) {
                currentMin = inst->getType()->getPrimitiveSizeInBits();
            }
        }
        current = current->getNext();
    }
    return currentMin;
}

bool Path::sameUseDef(const Path &other) const {
    if (*entry == *other.entry && *getLast() == *other.getLast()) {
        return true;
    }
    return false;
}


bool Path::isSub(const Path &sub) const {
    PathElementBase *x = entry;
    PathElementBase *y = sub.entry;

    bool same = true;
    while (x && y) {

        if (x->getElement() != y->getElement()) {
            same = false;
            break;
        }

        x = x->getNext();
        y = y->getNext();
    }

    if (same) {
        return true;
    }

    return false;
}

void ConstPathElement::dump() const {
    print(errs());
}

void ConstPathElement::print(raw_ostream &out) const {
    if (const ConstantInt *constant = dyn_cast<const ConstantInt>(element)) {
        uint64_t address = constant->getZExtValue();
        if (ptr::getAndersen()->getMachO().isCFString(address)) {
            out << ptr::getAndersen()->getMachO().getString(address) << "\n";
        } else if (ptr::getAndersen()->getMachO().isCString(address)) {
            out << ptr::getAndersen()->getMachO().getString(address) << "\n";
        } else {
            uint64_t width = getParent()->getShortestLoad(64);
            uint64_t data = 0;
            switch (width) {
                default:
                    llvm_unreachable("");
                case 32: {
                    data = ptr::getAndersen()->getMachO().getRAWData<uint32_t>(address);
                    break;
                }
                case 64: {
                    data = ptr::getAndersen()->getMachO().getRAWData<uint64_t>(address);
                    break;
                }
            }
            out << "Load from: " << address << ": " << data << "\n";
        }
    } else {
        llvm_unreachable("");
    }
}

std::string ConstPathElement::getValue() const {
    if (const ConstantInt *constant = dyn_cast<const ConstantInt>(element)) {
        uint64_t address = constant->getZExtValue();
        if (ptr::getAndersen()->getMachO().isCFString(address)) {
            return "Constant string: \"" + ptr::getAndersen()->getMachO().getString(address).str() + "\"";
        } else if (ptr::getAndersen()->getMachO().isCString(address)) {
            return "Constant string: \"" + ptr::getAndersen()->getMachO().getString(address).str() + "\"";
        } else {

            uint64_t width = getParent()->getShortestLoad(64);
            uint64_t data = 0;
            switch (width) {
                default:
                    llvm_unreachable("");
                case 32: {
                    data = ptr::getAndersen()->getMachO().getRAWData<uint32_t>(address);
                    break;
                }
                case 64: {
                    data = ptr::getAndersen()->getMachO().getRAWData<uint64_t>(address);
                    break;
                }
            }
            std::stringstream ss;
            ss << "Load from 0x" << utohexstr(address) << ": " << data;
            return ss.str();
        }
    }
    llvm_unreachable("");
}

bool ConstPathElement::shouldCreateNewCriterion() const {
    if (const ConstantInt *constant = dyn_cast<const ConstantInt>(element)) {
        uint64_t address = constant->getZExtValue();
        if (ptr::getAndersen()->getMachO().getSectionName(address) == "__objc_ivar" ||
                ptr::getAndersen()->getMachO().getSectionName(address) == "__data") {
            return true;
        }
    }
    return false;
}
