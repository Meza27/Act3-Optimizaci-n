#pragma once

#include <any>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <map>
#include <iostream>
#include <memory>
#include <string>

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/OptimizationLevel.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/OptimizationLevel.h"



#include "CalcBaseVisitor.h"

using namespace antlr4;
using namespace llvm;

class CalcDriver : public CalcBaseVisitor {
private:
  enum ExprType { CONSTANT, VALUE };
  struct ExprValue {
    ExprType type;
    std::any value;
  };
  std::map<std::string, float> symbols;

public:
  LLVMContext context;
  std::unique_ptr<Module> module;
  std::unique_ptr<IRBuilder<>> irBuilder;
  Value *formatStr;
  FunctionCallee printfFunc;

public:
  CalcDriver()
      : module(std::make_unique<Module>("Calc", context)),
        irBuilder(std::make_unique<IRBuilder<>>(context)) {}
  std::any visitProgram(CalcParser::ProgramContext *ctx) override {
    // nuestro programa llvm
    // define i32 @main()
    Function *mainFunc =
        Function::Create(FunctionType::get(Type::getInt32Ty(context), false),
                         Function::ExternalLinkage, "main", *module);

    // begin:
    irBuilder->SetInsertPoint(BasicBlock::Create(context, "begin", mainFunc));

    // FIX: add the right comment for the code generated
    formatStr = irBuilder->CreateGlobalString("Result: %f\n", "str_printf_fmt");
    llvm::FunctionType *printfType = llvm::FunctionType::get(
        Type::getInt32Ty(context),         // Returns int
        {PointerType::getUnqual(context)}, // Arguments vector
        true // IsVariadic = true (crucial for printf)
    );
    printfFunc = module->getOrInsertFunction("printf", printfType);

    visitChildren(ctx);

    // end:
    auto endBB = BasicBlock::Create(context, "end", mainFunc);
    irBuilder->CreateBr(endBB);
    irBuilder->SetInsertPoint(endBB);
    irBuilder->CreateRet(ConstantInt::get(Type::getInt32Ty(context), 0));

    std::error_code EC;
    llvm::raw_fd_ostream out("sinopt.ll", EC, llvm::sys::fs::OpenFlags::OF_None);
    if (EC) {
      errs() << "Error creando sinopt.ll: " << EC.message() << "\n";
    } else {
      module->print(out, nullptr);
      out.close();
    }


    return std::any();
  }
  llvm::Value *getValue(std::any thingy) {
    auto exprValue = std::any_cast<ExprValue>(thingy);

    if (exprValue.type == CONSTANT) {
      return std::any_cast<llvm::Constant *>(exprValue.value);
    } else {
      return std::any_cast<llvm::Value *>(exprValue.value);
    }
  }
  virtual std::any visitStatExpr(CalcParser::StatExprContext *ctx) {
    auto val = getValue(visit(ctx->expr()));

    std::vector<llvm::Value *> printArgs;
    printArgs.push_back(formatStr);
    printArgs.push_back(val);

    irBuilder->CreateCall(printfFunc, printArgs);

    return std::any();
  }
  virtual std::any visitStatAssign(CalcParser::StatAssignContext *ctx) {
    std::string var = ctx->ID()->getText();
    auto exprVal = getValue(visit(ctx->expr()));

    // Convertimos siempre a double
    if (auto *C = dyn_cast<ConstantFP>(exprVal)) {
      symbols[var] = C->getValueAPF().convertToDouble();
    } else {
      // Si viene de una operación, igual lo forzamos a constante
      symbols[var] = 0.0; // seguridad mínima
    }

    return std::any();
  }
  virtual std::any visitExprPar(CalcParser::ExprParContext *ctx) {
    return visit(ctx->expr());
  }
  virtual std::any visitExprAddSub(CalcParser::ExprAddSubContext *ctx) {
    auto lhs = getValue(visit(ctx->expr(0)));
    auto rhs = getValue(visit(ctx->expr(1)));
    auto res = irBuilder->CreateFAdd(lhs, rhs, "addsubres");
    return std::any(ExprValue{VALUE, res});
  }
  virtual std::any visitExprId(CalcParser::ExprIdContext *ctx) override {
    std::string var = ctx->ID()->getText();

    if (symbols.find(var) == symbols.end()) {
      std::cerr << "Error: variable no definida: " << var << std::endl;
      exit(1);
    }

    double val = symbols[var];
    auto *literal = ConstantFP::get(Type::getDoubleTy(context), val);
    return std::any(ExprValue{CONSTANT, literal});
  }
  virtual std::any visitExprNum(CalcParser::ExprNumContext *ctx) {
    auto val = (ctx->SUB() ? -1 : 1) * stof(ctx->NUM()->getText());
    auto literal = ConstantFP::get(Type::getDoubleTy(context), val);
    return std::any(ExprValue{CONSTANT, literal});
  }
  virtual std::any visitExprMulDiv(CalcParser::ExprMulDivContext *ctx) {
    return visitChildren(ctx);
  }

  void optimizeAndWrite(const std::string &outPath) {
  using namespace llvm;

  PassBuilder PB;

  LoopAnalysisManager LAM;
  FunctionAnalysisManager FAM;
  CGSCCAnalysisManager CGAM;
  ModuleAnalysisManager MAM;

  PB.registerModuleAnalyses(MAM);
  PB.registerFunctionAnalyses(FAM);
  PB.registerCGSCCAnalyses(CGAM);
  PB.registerLoopAnalyses(LAM);

  PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

  OptimizationLevel OL = OptimizationLevel::O2;

  ModulePassManager MPM = PB.buildPerModuleDefaultPipeline(OL);

  MPM.run(*module, MAM);

  if (verifyModule(*module, &errs())) {
    errs() << "Error: módulo inválido tras optimización\n";
    return;
  }

  std::error_code EC;
  raw_fd_ostream out(outPath, EC, sys::fs::OpenFlags::OF_None);
  if (EC) {
    errs() << "Error abriendo archivo: " << EC.message() << "\n";
    return;
  }

  module->print(out, nullptr);
}

};
