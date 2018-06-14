#ifndef LLVM_PATH_H
#define LLVM_PATH_H

#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"

namespace llvm {
    namespace slicing {

        class Path;
        class PathElement;

        class PathElementBase {
        public:
            enum PathElementType {
                InstructionElement,
                ConstAddressElement
            };

            PathElementBase(const Value *e, const Value *r) : element(e), relevantVariable(r), next(nullptr), prev(
                    nullptr), parent(nullptr) {

            }

            virtual ~PathElementBase() {};

            virtual PathElementType getType() const = 0;

            virtual bool operator < (const PathElementBase &b) const {
                if (getType() != b.getType())
                    return getType() < b.getType();
                return true;
            };
            virtual bool operator == (const PathElementBase &b) const {
                if (element == b.element)
                    return true;
                return false;
            }

            virtual PathElementBase *clone(PathElementBase *prev, Path *parent) = 0;

            virtual bool setNext(PathElementBase *n);
            void setPrev(PathElementBase *p) { prev = p; }

            void removeNext() {
                next = nullptr;
            }

            PathElementBase *getPrev() { return prev; }
            PathElementBase *getNext() { return next; }

            const Value *getElement() const { return element; }
            const Value *getRelevantVariable() { return relevantVariable; }
            Path *getParent() const { return parent; };
            void setParent(Path *p) { parent = p; }

            virtual void dump() const;
            virtual void print(raw_ostream &out) const;

        protected:
            const Value *element;
            const Value *relevantVariable;

            PathElementBase *next;
            PathElementBase *prev;
            Path *parent;
        };

        class PathElement : public PathElementBase {
        public:
            PathElement(const Instruction *i, const Value *r) : PathElementBase(i, r), inst(i) {
                assert(isa<const Instruction>(i));
            }

            const Instruction *getInstruction() {
                assert(isa<const Instruction>(element));
                return dyn_cast<const Instruction>(element);
            }

            virtual void dump() const;
            virtual void print(raw_ostream &out) const;

            virtual PathElementType getType() const {
                return InstructionElement;
            }

            virtual bool setNext(PathElementBase *n);

            virtual PathElementBase *clone(PathElementBase *prev, Path *parent) {
                return new PathElement(*this, prev, parent);
            }
        private:

            PathElement(const PathElement &p, PathElementBase *prev, Path *parent) : PathElementBase(p.element, p.relevantVariable) {
                if (p.next)
                    next = p.next->clone(this, parent);
                else {
                    next = NULL;
                }
                inst = p.inst;
                relevantVariable = p.relevantVariable;

                setPrev(prev);
                setParent(parent);
            }

            const Instruction *inst;

        };

        class ConstPathElement : public PathElementBase {
        public:
            ConstPathElement(const Value *e, const Value *r) : PathElementBase(e, r) {

            };
            virtual PathElementType getType() const {
                return ConstAddressElement;
            };

            virtual PathElementBase *clone(PathElementBase *prev, Path *parent) {
                ConstPathElement *elem = new ConstPathElement(element, relevantVariable);
                elem->setParent(parent);
                elem->setPrev(prev);
                return elem;
            };

            virtual void dump() const;
            virtual void print(raw_ostream &out) const;
            virtual std::string getValue() const;
            virtual bool shouldCreateNewCriterion() const;
        };

        class Path {
        public:
            Path() {}
            Path(const Path &p) {
//                entry = new PathElement(*p.entry, NULL, this);
                entry = p.entry->clone(NULL, this);
                callStack = std::vector<const CallInst*>(p.callStack);
            }

            virtual ~Path() {
                PathElementBase *current = entry;
                PathElementBase *next = nullptr;

                while (current) {
                    next = current->getNext();
                    delete(current);
                    current = next;
                }
            }

            void setEntry(PathElementBase *e) {
                assert(!e->getParent());
                entry = e;
                entry->setParent(this);
            }
            PathElementBase *getLast() const {
                assert(entry);
                PathElementBase *current = entry;
                while (current->getNext())
                    current = current->getNext();
                return current;
            }

            PathElementBase *getEntry() const {
                return entry;
            }

            bool contains(const Instruction *inst, const Value *relevant = nullptr) {
                if (inst->getOpcode() == Instruction::Ret)
                {
                    for (PathElementBase *current = entry; current; current = current->getNext()) {
                        if (current->getElement() == inst) {
                            return current->getRelevantVariable() == relevant;
                        }
                    }
                    return false;
                }
                if (getLatestCall() == inst) {
                    return false;
                }
                for (PathElementBase *current = entry; current; current = current->getNext()) {
                    if (current->getElement() == inst) {
                        return true;
                    }
                }
                return false;
            }

            size_t getLength() const {
                PathElementBase *current = entry;
                size_t l = 0;
                while (current) {
                    l++;
                    current = current->getNext();
                }
                return l;
            }

            void dump(bool useDefOnly = false) const;
            void print(raw_ostream &out, bool useDefOnly = false) const;

            virtual bool operator < (const Path &b) const;
            virtual bool operator == (const Path &b) const;

            void addCall(const CallInst* call) {
                assert(call);
                callStack.push_back(call);
            }

            bool hasCall() {
                return callStack.size() > 0;
            }

            const CallInst *getLatestCall() {
                if (!callStack.size())
                    return nullptr;
                return callStack.back();
            }

            void popCall() {
                callStack.pop_back();
            }

            uint64_t getShortestLoad(uint64_t currentMin = 64) const;

            bool sameUseDef(const Path &other) const;
            bool isSub(const Path &sub) const;
        private:
            PathElementBase *entry;
            std::vector<const CallInst*> callStack;
        };

    }
}

#endif //LLVM_PATH_H
