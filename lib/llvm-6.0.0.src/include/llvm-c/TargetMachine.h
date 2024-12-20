/*===-- llvm-c/TargetMachine.h - Target Machine Library C Interface - C++ -*-=*\
|*                                                                            *|
|*                     The LLVM Compiler Infrastructure                       *|
|*                                                                            *|
|* This file is distributed under the University of Illinois Open Source      *|
|* License. See LICENSE.TXT for details.                                      *|
|*                                                                            *|
|*===----------------------------------------------------------------------===*|
|*                                                                            *|
|* This header declares the C interface to the Target and TargetMachine       *|
|* classes, which can be used to generate assembly or object files.           *|
|*                                                                            *|
|* Many exotic languages can interoperate with C code but have a harder time  *|
|* with C++ due to name mangling. So in addition to C, this interface enables *|
|* tools written in such languages.                                           *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

#ifndef LLVM_C_TARGETMACHINE_H
#define LLVM_C_TARGETMACHINE_H

#include "llvm-c/Target.h"
#include "llvm-c/Types.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct LLVMOpaqueTargetMachine *LLVMTargetMachineRef;
typedef struct LLVMTarget *LLVMTargetRef;

typedef enum {
    LLVMCodeGenLevelNone,
    LLVMCodeGenLevelLess,
    LLVMCodeGenLevelDefault,
    LLVMCodeGenLevelAggressive
} LLVMCodeGenOptLevel;

typedef enum {
    LLVMRelocDefault,
    LLVMRelocStatic,
    LLVMRelocPIC,
    LLVMRelocDynamicNoPic
} LLVMRelocMode;

typedef enum {
    LLVMCodeModelDefault,
    LLVMCodeModelJITDefault,
    LLVMCodeModelSmall,
    LLVMCodeModelKernel,
    LLVMCodeModelMedium,
    LLVMCodeModelLarge
} LLVMCodeModel;

typedef enum {
    LLVMAssemblyFile,
    LLVMObjectFile
} LLVMCodeGenFileType;

/** Returns the first llvm::Target in the registered targets list. */
LLVMTargetRef LLVM_STDCALL LLVMGetFirstTarget(void);
/** Returns the next llvm::Target given a previous one (or null if there's none) */
LLVMTargetRef LLVM_STDCALL LLVMGetNextTarget(LLVMTargetRef T);

/*===-- Target ------------------------------------------------------------===*/
/** Finds the target corresponding to the given name and stores it in \p T.
  Returns 0 on success. */
LLVMTargetRef LLVM_STDCALL LLVMGetTargetFromName(const char *Name);

/** Finds the target corresponding to the given triple and stores it in \p T.
  Returns 0 on success. Optionally returns any error in ErrorMessage.
  Use LLVMDisposeMessage to dispose the message. */
LLVMBool LLVM_STDCALL LLVMGetTargetFromTriple(const char* Triple, LLVMTargetRef *T,
                                 char **ErrorMessage);

/** Returns the name of a target. See llvm::Target::getName */
const char *LLVM_STDCALL LLVMGetTargetName(LLVMTargetRef T);

/** Returns the description  of a target. See llvm::Target::getDescription */
const char *LLVM_STDCALL LLVMGetTargetDescription(LLVMTargetRef T);

/** Returns if the target has a JIT */
LLVMBool LLVM_STDCALL LLVMTargetHasJIT(LLVMTargetRef T);

/** Returns if the target has a TargetMachine associated */
LLVMBool LLVM_STDCALL LLVMTargetHasTargetMachine(LLVMTargetRef T);

/** Returns if the target as an ASM backend (required for emitting output) */
LLVMBool LLVM_STDCALL LLVMTargetHasAsmBackend(LLVMTargetRef T);

/*===-- Target Machine ----------------------------------------------------===*/
/** Creates a new llvm::TargetMachine. See llvm::Target::createTargetMachine */
LLVMTargetMachineRef LLVM_STDCALL LLVMCreateTargetMachine(LLVMTargetRef T,
  const char *Triple, const char *CPU, const char *Features,
  LLVMCodeGenOptLevel Level, LLVMRelocMode Reloc, LLVMCodeModel CodeModel);

/** Dispose the LLVMTargetMachineRef instance generated by
  LLVMCreateTargetMachine. */
void LLVM_STDCALL LLVMDisposeTargetMachine(LLVMTargetMachineRef T);

/** Returns the Target used in a TargetMachine */
LLVMTargetRef LLVM_STDCALL LLVMGetTargetMachineTarget(LLVMTargetMachineRef T);

/** Returns the triple used creating this target machine. See
  llvm::TargetMachine::getTriple. The result needs to be disposed with
  LLVMDisposeMessage. */
char *LLVM_STDCALL LLVMGetTargetMachineTriple(LLVMTargetMachineRef T);

/** Returns the cpu used creating this target machine. See
  llvm::TargetMachine::getCPU. The result needs to be disposed with
  LLVMDisposeMessage. */
char *LLVM_STDCALL LLVMGetTargetMachineCPU(LLVMTargetMachineRef T);

/** Returns the feature string used creating this target machine. See
  llvm::TargetMachine::getFeatureString. The result needs to be disposed with
  LLVMDisposeMessage. */
char *LLVM_STDCALL LLVMGetTargetMachineFeatureString(LLVMTargetMachineRef T);

/** Create a DataLayout based on the targetMachine. */
LLVMTargetDataRef LLVM_STDCALL LLVMCreateTargetDataLayout(LLVMTargetMachineRef T);

/** Set the target machine's ASM verbosity. */
void LLVM_STDCALL LLVMSetTargetMachineAsmVerbosity(LLVMTargetMachineRef T,
                                      LLVMBool VerboseAsm);

/** Emits an asm or object file for the given module to the filename. This
  wraps several c++ only classes (among them a file stream). Returns any
  error in ErrorMessage. Use LLVMDisposeMessage to dispose the message. */
LLVMBool LLVM_STDCALL LLVMTargetMachineEmitToFile(LLVMTargetMachineRef T, LLVMModuleRef M,
  char *Filename, LLVMCodeGenFileType codegen, char **ErrorMessage);

/** Compile the LLVM IR stored in \p M and store the result in \p OutMemBuf. */
LLVMBool LLVM_STDCALL LLVMTargetMachineEmitToMemoryBuffer(LLVMTargetMachineRef T, LLVMModuleRef M,
  LLVMCodeGenFileType codegen, char** ErrorMessage, LLVMMemoryBufferRef *OutMemBuf);

/*===-- Triple ------------------------------------------------------------===*/
/** Get a triple for the host machine as a string. The result needs to be
  disposed with LLVMDisposeMessage. */
char* LLVM_STDCALL LLVMGetDefaultTargetTriple(void);

/** Normalize a target triple. The result needs to be disposed with
  LLVMDisposeMessage. */
char* LLVM_STDCALL LLVMNormalizeTargetTriple(const char* triple);

/** Get the host CPU as a string. The result needs to be disposed with
  LLVMDisposeMessage. */
char* LLVM_STDCALL LLVMGetHostCPUName(void);

/** Get the host CPU's features as a string. The result needs to be disposed
  with LLVMDisposeMessage. */
char* LLVM_STDCALL LLVMGetHostCPUFeatures(void);

/** Adds the target-specific analysis passes to the pass manager. */
void LLVM_STDCALL LLVMAddAnalysisPasses(LLVMTargetMachineRef T, LLVMPassManagerRef PM);

#ifdef __cplusplus
}
#endif

#endif
