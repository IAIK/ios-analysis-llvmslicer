#include <llvm/Support/ErrorHandling.h>
#include "llvm/Support/raw_ostream.h"
#include <sstream>
#include "llvm/Analysis/Andersen/ObjectiveCClassInfo.h"

using namespace llvm;
using namespace llvm::ObjectiveC;

void Class::addMethod(Method M) {
    assert(!M.Parent);
    M.Parent = this;
    Methods.push_back(M);
}

bool Class::getMethod(StringRef Methodname, Method &M) {
    for (MethodList_t::iterator M_it = Methods.begin(); M_it != Methods.end(); ++M_it) {
        if (M_it->Methodname == Methodname) {
            M = *M_it;
            return true;
        }
    }
    return false;
}

void Class::addIVAR(IVAR ivar) {
    assert(ivar.ParentClass.size() == 0);
    IVARs.push_back(ivar);
    ivar.ParentClass = Classname;
}

bool Class::getIVAR(StringRef IVARName, IVAR &ivar) {
    for (IVARList_t::iterator it = IVARs.begin(); it != IVARs.end(); ++it) {
        if (it->IVARName == IVARName) {
            ivar = *it;
            return true;
        }
    }
    return false;
}

bool Class::getIVAR(uint64_t IVAROffset, IVAR &ivar) {
    for (IVARList_t::iterator it = IVARs.begin(); it != IVARs.end(); ++it) {
        if (it->OffsetPtr == IVAROffset) {
            ivar = *it;
            return true;
        }
    }
    return false;
}


//Method

void Method::parseType() {
    if (!type.size())
        return;

    const char *type_it = type.data();
    bool isReturnType = true;

    unsigned currentGPR = 5;
    unsigned currentFPR = -1U;

    while (*type_it) {
        switch (*type_it) {
            default:
                llvm_unreachable("");
            case 'r':
            case 'n':
            case 'N':
            case 'o':
            case 'O':
            case 'V': {
                ++type_it;
                break;
            }
            case 'v': {
                
                ++type_it;
                break;
            }
            case 'i':
            case 's':
            case 'l':
            case 'I':
            case 'S':
            case 'L':
            case 'C':
            case 'c':
            case 'B':
            case '*':
            case ':':
            case '#':
            {
                ++currentGPR;
                ++type_it;
                break;
            }
            case 'q':
            case 'Q':
            {
                return;
                ++type_it;
                break;
            }
            case 'f': {
                
                ++type_it;
                break;
            }
            case 'd': {
                
                ++type_it;
                break;
            }
            case '@': {
                unsigned reg = currentGPR++;
                if (*(type_it + 1) == '\"') {
                    type_it += 2;
                    const char *end_it = strchr(type_it, '\"');
                    unsigned length = end_it - type_it;
                    std::string paramType(type_it);
                    paramType = paramType.substr(0, length);
                    type_it = end_it;
                    regTypes.push_back(RegType_t(reg, paramType));
                } else {

                }
                ++type_it;
                break;
            }
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            {
                ++type_it;
                break;
            }
            case '[':
            case '{':
            case '(':
            case 'b':
            case '?':
            case '^':
            {
                return;
            }
        }
    }
}
