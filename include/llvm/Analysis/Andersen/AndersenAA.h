#ifndef TCFS_ANDERSEN_AA_H
#define TCFS_ANDERSEN_AA_H

#include "llvm/Analysis/Andersen/Andersen.h"

#include "llvm/Pass.h"
#include "llvm/Analysis/AliasAnalysis.h"

class AndersenAA: public llvm::ModulePass, public llvm::AliasAnalysis
{
private:
	Andersen* anders;

	llvm::AliasResult andersenAlias(const llvm::Value* l1, const llvm::Value* l2);
public:
	static char ID;

	// Interfaces of AliasAnalysis.
	llvm::AliasResult alias(const llvm::MemoryLocation& l1, const llvm::MemoryLocation& l2);
	void deleteValue(llvm::Value* v);
	void copyValue(llvm::Value* from, llvm::Value* to);
	bool pointsToConstantMemory(const llvm::MemoryLocation& loc, bool orLocal);
	//ModRefResult getModRefInfo (llvm::ImmutableCallSite cs, const Location &loc);

	AndersenAA(): ModulePass(ID), anders(nullptr) {}
	bool runOnModule(llvm::Module &M) override;
//	void getAnalysisUsage(llvm::AnalysisUsage &AU) const;
//	void* getAdjustedAnalysisPointer(llvm::AnalysisID PI);
};

#endif
