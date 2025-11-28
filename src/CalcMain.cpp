#include <iostream>

#include "CalcDriver.h"
#include "CalcLexer.h"
#include "CalcParser.h"

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"

using namespace antlr4;
using namespace std;
using namespace llvm;

// Función para ejecutar el módulo con JIT
void runWithJIT(std::unique_ptr<Module> M) {
  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();

  std::string errStr;
  ExecutionEngine *EE =
      EngineBuilder(std::move(M)).setErrorStr(&errStr).create();

  if (!EE) {
    errs() << "Error creando el JIT: " << errStr << "\n";
    return;
  }

  Function *MainFunc = EE->FindFunctionNamed("main");
  if (!MainFunc) {
    errs() << "No se encontró la función main para ejecutar con JIT\n";
    return;
  }

  std::vector<GenericValue> NoArgs;
  EE->runFunction(MainFunc, NoArgs);
}

int main(int argc, const char *argv[]) {
  ifstream ifile;
  if (argc > 1) {
    ifile.open(argv[1]);
    if (!ifile.is_open()) {
      cerr << "No se pudo abrir el archivo\n";
      exit(-1);
    }
  }

  istream &stream = argc > 1 ? ifile : cin;

  ANTLRInputStream input(stream);
  CalcLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  CalcParser parser(&tokens);

  tree::ParseTree *tree = parser.program();

  auto driver = new CalcDriver();
  driver->visit(tree);

  // Generar conopt.ll si se pasa el flag -O
  if (argc > 2 && string(argv[2]) == "-O") {
    driver->optimizeAndWrite("conopt.ll");
  }

  // Generar con jit si se pasa el flag -jit
  if (argc > 2 && string(argv[2]) == "-jit") {
    runWithJIT(std::move(driver->module));
  }

  delete driver;
  return 0;
}
