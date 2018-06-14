#ifndef LLVM_OBJCCALLHANDLER_H
#define LLVM_OBJCCALLHANDLER_H

#include <map>
#include <mutex>
#include <string>
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/CallSite.h"
#include "llvm/Analysis/Andersen/SimpleCallGraph.h"

class Andersen;

namespace llvm {
    namespace ObjectiveC {

        class CallHandlerManager;
        class objcARC;
        class objcInit;
        class objcMsgSend;

        class CallHandlerBase {
        public:
            virtual ~CallHandlerBase() {};
            virtual StringRef &getFunctionName() {return *FunctionName;};

            //TODO: make this class abstract so subclasses have to do this
            virtual bool run(const Instruction *CallInst, std::string &F, Andersen *andersen) {return true;}
            virtual bool shouldHandleCall(std::string &F) = 0;

            bool operator<(CallHandlerBase &other) const
            {
                return getPriority() > other.getPriority();
            }

            static bool isObjectiveCMethod(StringRef F);
            static StringRef getClassname(StringRef F);
            static StringRef getMethodname(StringRef F);

        protected:
            StringRef *FunctionName;

            bool isSelf(Instruction *v, Andersen *andersen);


            virtual int64_t getPriority() const = 0;
        };

        CallHandlerManager &getGlobalCallHandlerManager();


        class CallHandlerManager {
        public:
            CallHandlerManager() {
                assert(true);
            };

            static CallHandlerManager &getInstance();

            template<class T>
            void registerCallHandler() {

//                CallHandlers.insert(std::make_pair(FunctionName, NULL));
//                CallHandlers.push_back(std::unique_ptr<CallHandlerBase>(new T()));
                addCallHandler(std::shared_ptr<T>(new T()));
            }

            void addCallHandler(std::shared_ptr<CallHandlerBase> Handler);
            bool handleFunctionCall(const Instruction *CallInst, std::string &F, Andersen *andersen);

        private:
            typedef std::shared_ptr<CallHandlerBase> CallHandlerPtr_t;
            typedef std::map<StringRef, std::shared_ptr<CallHandlerBase>> CallHandlerMap_t;
            typedef std::vector<CallHandlerPtr_t> CallHandlerList_t;
            CallHandlerList_t CallHandlers;
            CallHandlerPtr_t getCallHandler(StringRef &FunctionName);

            static std::mutex initLock;
        };


        template <class T>
        class RegisterCallHandler {
        public:
            RegisterCallHandler() {
                getGlobalCallHandlerManager().registerCallHandler<T>();
            }
        };

    }
}

#endif //LLVM_OBJCCALLHANDLER_H
