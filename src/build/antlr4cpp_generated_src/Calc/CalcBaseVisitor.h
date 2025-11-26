
// Generated from /mnt/c/Users/User/Desktop/Archivos Meza/Documentos UPC/Teoria de Compiladores/expr_calc/src/Calc.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "CalcVisitor.h"


/**
 * This class provides an empty implementation of CalcVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  CalcBaseVisitor : public CalcVisitor {
public:

  virtual std::any visitProgram(CalcParser::ProgramContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatExpr(CalcParser::StatExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatAssign(CalcParser::StatAssignContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprPar(CalcParser::ExprParContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprAddSub(CalcParser::ExprAddSubContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprId(CalcParser::ExprIdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprNum(CalcParser::ExprNumContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprMulDiv(CalcParser::ExprMulDivContext *ctx) override {
    return visitChildren(ctx);
  }


};

