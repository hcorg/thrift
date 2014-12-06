#ifndef _THRIFT_COMPILER_TESTS_GENERATE_CPP_V2_CONTENTSGENERATORSTUB_H_
#define _THRIFT_COMPILER_TESTS_GENERATE_CPP_V2_CONTENTSGENERATORSTUB_H_

#include "../ContentsGenerator.h"

class ContentsGeneratorStub : public apache::thrift::compiler::generate::cpp_v2::ContentsGenerator {
public:
  ContentsGeneratorStub(apache::thrift::compiler::generate::OutputGenerator& parent,
                        const std::string& text,
                        const std::string& include,
                        const std::string& footer)
    : ContentsGenerator(parent), text_(text), include_(include), footer_(footer) {}

  void generateContents() override { out() << text_ << endl; }

  void generateHeader() override { out() << "#include <" << include_ << ">" << endl; }

  void generateFooter() override { out() << footer_ << endl; }

private:
  const std::string text_;
  const std::string include_;
  const std::string footer_;
};

#endif // _THRIFT_COMPILER_TESTS_GENERATE_CPP_V2_CONTENTSGENERATORSTUB_H_
