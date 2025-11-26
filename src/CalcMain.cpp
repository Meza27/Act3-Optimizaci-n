#include <iostream>

#include "CalcDriver.h"
#include "CalcLexer.h"
#include "CalcParser.h"

using namespace antlr4;
using namespace std;

int main(int argc, const char *argv[]) {
  ifstream ifile;
  if (argc > 1) {
    ifile.open(argv[1]);
    if (!ifile.is_open()) {
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

  if (argc > 2) {

    std::string outname = "conopt.ll";
    driver->optimizeAndWrite(outname);
  }

  delete driver;
  return 0;
}
