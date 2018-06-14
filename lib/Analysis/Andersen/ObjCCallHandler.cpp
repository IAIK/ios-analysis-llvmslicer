#include <memory>
#include <llvm/IR/Value.h>
#include "llvm/Analysis/Andersen/ObjCCallHandler.h"

#include "llvm/Support/ManagedStatic.h"
#include "llvm/IR/CallSite.h"

#include "llvm/Analysis/Andersen/CallHandler/ObjCRuntimeCallHandler.h"
#include "../../LLVMSlicer/Languages/LLVM.h"

using namespace llvm;
using namespace llvm::ObjectiveC;

static std::unique_ptr<CallHandlerManager> GlobalCallHandlerManager(new CallHandlerManager());

std::mutex CallHandlerManager::initLock;
//static llvm::ManagedStatic<CallHandlerManager> GlobalCallHandlerManager;

CallHandlerManager &llvm::ObjectiveC::getGlobalCallHandlerManager() {
    return CallHandlerManager::getInstance();
}

CallHandlerManager &CallHandlerManager::getInstance() {
    initLock.lock();
    static CallHandlerManager *Instance = nullptr;
    if (!Instance) {
        Instance = new CallHandlerManager();
        Instance->registerCallHandler<objcMsgSend>();
        Instance->registerCallHandler<MsgSendSuper>();
        Instance->registerCallHandler<dispatchBlock>();
        Instance->registerCallHandler<CopyProperty>();
                Instance->registerCallHandler<retainBlock>();
//        Instance->registerCallHandler<objcARC>();
//        Instance->registerCallHandler<objcPreserveX0>();
//        Instance->registerCallHandler<objcPreserveNone>();
//        Instance->registerCallHandler<specialAllocs>();
        Instance->registerCallHandler<ClassHandler>();
        Instance->registerCallHandler<ExternalHandler>();
        Instance->registerCallHandler<NSArray>();
        Instance->registerCallHandler<UIControlTarget>();
        Instance->registerCallHandler<UIAppDelegate>();
        Instance->registerCallHandler<NSUserDefaults>();
        Instance->registerCallHandler<SecItemCopyAdd>();

//        Instance->registerCallHandler<Dummy>();

//        Instance->registerCallHandler<objcInit>();
//        Instance->registerCallHandler<DummyHandler>();
    }
    initLock.unlock();
    return *Instance;
}

bool CallHandlerManager::handleFunctionCall(const Instruction *CallInst, std::string &F, Andersen *andersen) {
    for (CallHandlerList_t::iterator CH_it = CallHandlers.begin(); CH_it != CallHandlers.end(); ++CH_it) {
        if ((*CH_it)->shouldHandleCall(F)) {
            if ((*CH_it)->run(CallInst, F, andersen))
                return true;
        }
    }
    return false;
}

CallHandlerManager::CallHandlerPtr_t CallHandlerManager::getCallHandler(StringRef &FunctionName) {
    for (CallHandlerList_t::iterator CH_it = CallHandlers.begin(); CH_it != CallHandlers.end(); ++CH_it) {
        if ((*CH_it)->getFunctionName() == FunctionName) {
            return *CH_it;
        }
    }
    return nullptr;
}


void CallHandlerManager::addCallHandler(std::shared_ptr<CallHandlerBase> Handler) {
    CallHandlers.push_back(Handler);
}
