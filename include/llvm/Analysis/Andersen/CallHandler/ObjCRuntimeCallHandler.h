#ifndef LLVM_OBJCRUNTIMECALLHANDLER_H
#define LLVM_OBJCRUNTIMECALLHANDLER_H

#include "llvm/Analysis/Andersen/ObjCCallHandler.h"

#include <map>
#include <set>

class Andersen;

namespace llvm {
    namespace ObjectiveC {

        class objcMsgSend: public CallHandlerBase {
        public:
            objcMsgSend() {}
            virtual bool shouldHandleCall(std::string &F) {return F == "objc_msgSend";};
            virtual bool run(const Instruction *CallInst, std::string &F, Andersen *andersen);
            virtual int64_t getPriority() const {return 1;};

        private:
            void handleCall(StringRef ClassName, StringRef MethodName, bool Meta, Instruction *CallInst, Value *X0, Value *X1, Andersen *andersen);
        };

        class objcInit: public CallHandlerBase {
        public:
            virtual bool shouldHandleCall(std::string &F);
            virtual bool run(const Instruction *CallInst, std::string &F, Andersen *andersen);
            virtual int64_t getPriority() const {return -1;};
        };

        class objcARC: public CallHandlerBase {
        public:
            virtual bool shouldHandleCall(std::string &F);
            virtual bool run(const Instruction *CallInst, std::string &F, Andersen *andersen);
            virtual int64_t getPriority() const {return 1;};
        };

        class objcPreserveX0: public CallHandlerBase {
        public:
            virtual bool shouldHandleCall(std::string &F);
            virtual bool run(const Instruction *CallInst, std::string &F, Andersen *andersen);
            virtual int64_t getPriority() const {return 1;};
        };

        class objcPreserveNone : public CallHandlerBase {
        public:
            virtual bool shouldHandleCall(std::string &F);
            virtual bool run(const Instruction *CallInst, std::string &F, Andersen *andersen);
            virtual int64_t getPriority() const {return 1;};
        };

        class dispatchBlock: public CallHandlerBase {
        public:
            virtual bool shouldHandleCall(std::string &F);
            virtual bool run(const Instruction *CallInst, std::string &F, Andersen *andersen);
            virtual int64_t getPriority() const {return 1;};
        private:
            std::mutex handlerLock;
        };

        class retainBlock: public CallHandlerBase {
        public:
            virtual bool shouldHandleCall(std::string &F);
            virtual bool run(const Instruction *CallInst, std::string &F, Andersen *andersen);
            virtual int64_t getPriority() const {return 1;};
        private:
            std::set<const llvm::Instruction*> Preserved;
        };

        class specialAllocs: public CallHandlerBase {
        public:
            virtual bool shouldHandleCall(std::string &F);
            virtual bool run(const Instruction *CallInst, std::string &F, Andersen *andersen);
            virtual int64_t getPriority() const {return 1;};
        };

        class ClassHandler: public CallHandlerBase {
        public:
            virtual bool shouldHandleCall(std::string &F);
            virtual bool run(const Instruction *CallInst, std::string &F, Andersen *andersen);
            virtual int64_t getPriority() const {return 1;};
        };

        class MsgSendSuper: public CallHandlerBase {
        public:
            virtual bool shouldHandleCall(std::string &F);
            virtual bool run(const Instruction *CallInst, std::string &F, Andersen *andersen);
            virtual int64_t getPriority() const {return 1;};
        };

        class CopyProperty: public CallHandlerBase {
        public:
            virtual bool shouldHandleCall(std::string &F);
            virtual bool run(const Instruction *CallInst, std::string &F, Andersen *andersen);
            virtual int64_t getPriority() const {return 1;};
        };

        class ExternalHandler: public CallHandlerBase {
        public:
            virtual bool shouldHandleCall(std::string &F);
            virtual bool run(const Instruction *CallInst, std::string &F, Andersen *andersen);
            virtual int64_t getPriority() const {return 1;};
        };

        class NSArray: public CallHandlerBase {
        public:
            virtual bool shouldHandleCall(std::string &F);
            virtual bool run(const Instruction *CallInst, std::string &F, Andersen *andersen);
            virtual int64_t getPriority() const {return 1;};

        private:
            void handleFastEnum(const Instruction *CallInst, Andersen *andersen);
        };

        class UIControlTarget: public CallHandlerBase {
        public:
            virtual bool shouldHandleCall(std::string &F);
            virtual bool run(const Instruction *CallInst, std::string &F, Andersen *andersen);
            virtual int64_t getPriority() const {return 1;};

        };

        class UIAppDelegate: public CallHandlerBase {
        public:
            virtual bool shouldHandleCall(std::string &F);
            virtual bool run(const Instruction *CallInst, std::string &F, Andersen *andersen);
            virtual int64_t getPriority() const {return 1;};

        };

        class NSUserDefaults: public CallHandlerBase {
        public:
            virtual bool shouldHandleCall(std::string &F);
            virtual bool run(const Instruction *CallInst, std::string &F, Andersen *andersen);
            virtual int64_t getPriority() const {return 1;};
        private:
            Value *singleton = nullptr;
        };

        class SecItemCopyAdd : public CallHandlerBase {
        public:
            virtual bool shouldHandleCall(std::string &F);
            virtual bool run(const Instruction *CallInst, std::string &F, Andersen *andersen);
            virtual int64_t getPriority() const {return 1;};
        };
        

//        static RegisterCallHandler<objcMsgSend> X2 = RegisterCallHandler<objcMsgSend>();
//
//        static RegisterCallHandler<DummyHandler> X3 = RegisterCallHandler<DummyHandler>();
//        static RegisterCallHandler<objcInit> X4 = RegisterCallHandler<objcInit>();
//        static RegisterCallHandler<objcARC> X5 = RegisterCallHandler<objcARC>();
    }
}


#endif //LLVM_OBJCRUNTIMECALLHANDLER_H
