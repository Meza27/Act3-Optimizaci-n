
// Generated from /mnt/c/Users/User/Desktop/Archivos Meza/Documentos UPC/Teoria de Compiladores/PC2/expr_calc/src/Calc.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "CalcParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by CalcParser.
 */
class  CalcVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by CalcParser.
   */
    virtual std::any visitProgram(CalcParser::ProgramContext *context) = 0;

    virtual std::any visitStatExpr(CalcParser::StatExprContext *context) = 0;

    virtual std::any visitStatAssign(CalcParser::StatAssignContext *context) = 0;

    virtual std::any visitExprPar(CalcParser::ExprParContext *context) = 0;

    virtual std::any visitExprAddSub(CalcParser::ExprAddSubContext *context) = 0;

    virtual std::any visitExprId(CalcParser::ExprIdContext *context) = 0;

    virtual std::any visitExprNum(CalcParser::ExprNumContext *context) = 0;

    virtual std::any visitExprMulDiv(CalcParser::ExprMulDivContext *context) = 0;


};

