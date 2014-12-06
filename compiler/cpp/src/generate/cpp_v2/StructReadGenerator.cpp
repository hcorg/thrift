/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "StructReadGenerator.h"

#include <cctype>
#include <algorithm>

#include <parse/t_base_type.h>
#include <parse/t_field.h>
#include <parse/t_list.h>
#include <parse/t_map.h>
#include <parse/t_set.h>
#include <parse/t_struct.h>
#include <parse/t_type.h>

#include <generate/ScopeGuard.h>
#include <generate/IndentGuard.h>

#include "TypeGenerator.h"
#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler;

StructReadGenerator::StructReadGenerator(OutputGenerator& parent, const t_struct* tstruct)
  : MethodGenerator(parent, tstruct->get_name()), tStruct_(tstruct) {
}

void StructReadGenerator::generateReturnType() {
  out() << "std::uint32_t";
}

void StructReadGenerator::generateDeclarationWithoutReturn() {
  out() << "read(::apache::thrift::protocol::TProtocol* iprot)";
}

void StructReadGenerator::generateDefinitionBody() {
  generateVariables();
  generateStructClear();
  generateStructBegin();
  generateWhileLoop();
  generateStructEnd();
  generateDataValidation();
  generateReturnStatement();
}

void StructReadGenerator::generateVariables() {
  indented() << "std::uint32_t xfer = 0;" << endl;
  indented() << "std::string fieldName;" << endl;
  indented() << "::apache::thrift::protocol::TType fieldType;" << endl;
  indented() << "std::int16_t fieldId;" << endl;
  out() << endl;
  generateVariablesForFields();
}

void StructReadGenerator::generateStructClear() {
  if (isAnyFieldSpecial() || tStruct_->get_members().empty())
    return;
  indented() << "clear();" << endl;
  out() << endl;
}

void StructReadGenerator::generateStructBegin() {
  indented() << "xfer += iprot->readStructBegin(fieldName);" << endl;
  out() << endl;
}

void StructReadGenerator::generateStructEnd() {
  out() << endl;
  indented() << "xfer += iprot->readStructEnd();" << endl;
  out() << endl;
}

void StructReadGenerator::generateWhileLoop() {
  indented() << "while (true)";
  ScopeGuard g(*this);
  generateWhileLoopBody();
}

void StructReadGenerator::generateReturnStatement() {
  indented() << "return xfer;" << endl;
}

void StructReadGenerator::generateWhileLoopBody() {
  generateReadFieldBegin();
  generateSwitchByFieldId();
  generateReadFieldEnd();
}

void StructReadGenerator::generateReadFieldBegin() {
  indented() << "xfer += iprot->readFieldBegin(fieldName, fieldType, fieldId);" << endl;
  indented() << "if (fieldType == ::apache::thrift::protocol::T_STOP)" << endl;
  IndentGuard g(*this);
  indented() << "break;" << endl;
}

void StructReadGenerator::generateSkipUnknownField() {
  indented() << "xfer += iprot->skip(fieldType);" << endl;
}

void StructReadGenerator::generateReadFieldEnd() {
  indented() << "xfer += iprot->readFieldEnd();" << endl;
}

void StructReadGenerator::generateSwitchByFieldId() {
  if (tStruct_->get_members().empty())
    return generateSkipUnknownField();
  out() << endl;
  indented() << "switch (fieldId)";
  generateSwitchBody();
  out() << endl;
}

void StructReadGenerator::generateSwitchBody() {
  ScopeGuard g(*this);
  for (auto f : tStruct_->get_members())
    generateFieldCase(f);
  generateDefaultCase();
}

void StructReadGenerator::generateDefaultCase() {
  indented() << "default:" << endl;
  IndentGuard g(*this);
  generateSkipUnknownField();
  indented() << "break;" << endl;
}

void StructReadGenerator::generateFieldCase(const t_field* f) {
  indented() << "case " << f->get_key() << ":" << endl;
  IndentGuard g(*this);
  indented() << "if (fieldType == " << getTTypeString(getTrueType(f)) << ") {" << endl;
  generateFieldCaseBody(f);
  indented() << "} else {" << endl;
  generateFieldCaseElse();
  indented() << "}" << endl;
  indented() << "break;" << endl;
}

void StructReadGenerator::generateFieldCaseElse() {
  IndentGuard g(*this);
  generateSkipUnknownField();
}

void StructReadGenerator::generateFieldCaseBody(const t_field* f) {
  IndentGuard g(*this);
  generateFieldRead(f);
  if (isRequired(f) || isResultSpecialField(f))
    indented() << getVariableName(f) << " = true;" << endl;
}

void StructReadGenerator::generateFieldRead(const t_field* f) {
  const auto type = getTrueType(f);
  const auto fieldName = getInitializedFieldName(f);

  if (type->is_struct() || type->is_xception())
    return generateStructFieldRead(fieldName, f->get_req() == t_field::T_OPTIONAL);
  generateReadByType(type, fieldName);
}

void StructReadGenerator::generateReadByType(const t_type* type, const std::string& fieldName) {
  if (type->is_base_type())
    return generateBaseTypeFieldRead(type, fieldName);
  if (type->is_struct() || type->is_xception())
    return generateStructFieldRead(fieldName);
  if (type->is_enum())
    return generateEnumFieldRead(type, fieldName);
  if (type->is_list())
    return generateListFieldRead(type, fieldName);
  if (type->is_set())
    return generateSetFieldRead(type, fieldName);
  if (type->is_map())
    return generateMapFieldRead(type, fieldName);
}

void StructReadGenerator::generateVariablesForFields() {
  if (!isAnyFieldRequired() && !isAnyFieldSpecial())
    return;
  for (auto f : tStruct_->get_members())
    generateVariableForField(f);
  out() << endl;
}

void StructReadGenerator::generateVariableForField(const t_field* f) {
  if (isRequired(f) || isResultSpecialField(f))
    indented() << "bool " << getVariableName(f) << " = false;" << endl;
}

bool StructReadGenerator::isAnyFieldRequired() const {
  return std::any_of(tStruct_->get_members().begin(), tStruct_->get_members().end(), isRequired);
}

bool StructReadGenerator::isAnyFieldSpecial() const {
  return std::any_of(tStruct_->get_members().begin(),
                     tStruct_->get_members().end(),
                     isResultSpecialField);
}

bool StructReadGenerator::isRequired(const t_field* f) {
  return f->get_req() == t_field::T_REQUIRED;
}

std::string StructReadGenerator::getVariableName(const t_field* f) const {
  return f->get_name() + "WasRead";
}

void StructReadGenerator::generateDataValidation() {
  generateRequiredFieldsValidationCheck();
  generateSpecialFieldsValidationCheck();
}

void StructReadGenerator::generateRequiredFieldsValidationCheck() {
  if (!isAnyFieldRequired())
    return;
  indented() << "using ::apache::thrift::protocol::TProtocolException;" << endl;
  indented() << "if (";
  generateDataValidationCheck();
  out() << ")" << endl;
  IndentGuard g(*this);
  indented() << "throw TProtocolException(TProtocolException::INVALID_DATA);" << endl;
  out() << endl;
}

void StructReadGenerator::generateSpecialFieldsValidationCheck() {
  if (!isAnyFieldSpecial())
    return;
  indented() << "if (!successWasRead)" << endl;
  IndentGuard g(*this);
  indented() << "this->success = boost::none;" << endl;
  out() << endl;
}

void StructReadGenerator::generateDataValidationCheck() {
  bool added = false;
  for (auto f : tStruct_->get_members()) {
    if (!isRequired(f))
      continue;

    if (added) {
      out() << endl << indent() << " || !";
    } else {
      added = true;
      out() << "!";
    }

    out() << getVariableName(f);
  }
}

const t_type* StructReadGenerator::getTrueType(const t_field* f) const {
  return f->get_type()->get_true_type();
}

std::string StructReadGenerator::getPrefixedEnumName(const t_type* t) const {
  t = t->get_true_type();
  return namespacePrefix(t) + t->get_name();
}

std::string StructReadGenerator::getInitializedFieldName(const t_field* f) const {
  const auto name = "this->" + f->get_name();
  if (f->get_req() == t_field::T_OPTIONAL)
    return "*::apache::thrift::initializedOptional(" + name + ")";
  return name;
}

void StructReadGenerator::generateBaseTypeFieldRead(const t_type* t, const std::string& fieldName) {
  indented() << "xfer += iprot->read" << getReadWriteSuffix(static_cast<const t_base_type*>(t))
             << "(" << fieldName << ");" << endl;
}

void StructReadGenerator::generateStructFieldRead(const std::string& fieldName, bool optional) {
  auto f = fieldName;
  if (optional)
    f = "(" + fieldName + ")";
  indented() << "xfer += " << f << ".read(iprot);" << endl;
}

void StructReadGenerator::generateEnumFieldRead(const t_type* t, const std::string& fieldName) {
  indented() << "std::int32_t val;" << endl;
  indented() << "xfer += iprot->readI32(val);" << endl;
  indented() << fieldName << " = "
             << "static_cast<" << getPrefixedEnumName(t) << ">(val);" << endl;
}

class StructReadGenerator::ContainerReadGenerator : public OutputGenerator {
protected:
  ContainerReadGenerator(StructReadGenerator& parent,
                         const std::string& containerNameTempl,
                         const t_type* t)
    : OutputGenerator(parent),
      containerNameVar_(makeUniqueName(containerNameTempl)),
      containerSizeVar_(makeUniqueName(containerNameTempl + "Size")),
      indexVarName_(makeUniqueName("idx")),
      parent_(parent),
      currentNamespace_(getCppNamespace(t)) {}

public:
  void generate(const std::string& fieldName) {
    indented() << "auto& " << containerNameVar_ << " = " << fieldName << ";" << endl;
    generateClearAndSizeVar();
    generateReadBegin();
    generateResizeAndReadLoop();
    generateReadEnd();
  }

protected:
  virtual void generateReadBegin() = 0;
  virtual void generateReadEnd() = 0;
  virtual void generateReadLoopContents() = 0;

  void generateReadItem(const t_type* t, const std::string& name) {
    parent_.generateReadByType(t, name);
  }

  std::string generateIgnoredType() {
    const std::string ignoredTypeVarName = makeUniqueName("ignoredType");
    indented() << "::apache::thrift::protocol::TType " << ignoredTypeVarName << "{};" << endl;
    return ignoredTypeVarName;
  }

  void generateType(const t_type* t) {
    TypeGenerator gen(*this);
    gen.setCurrentNamespace(currentNamespace_);
    gen.generate(t);
  }

  const std::string containerNameVar_;
  const std::string containerSizeVar_;
  const std::string indexVarName_;

private:
  void generateClearAndSizeVar() {
    indented() << containerNameVar_ << ".clear();" << endl;
    indented() << "std::uint32_t " << containerSizeVar_ << "{};" << endl;
  }

  void generateResizeAndReadLoop() {
    indented() << "for (std::uint32_t " << indexVarName_ << " = 0; " << indexVarName_ << " < "
               << containerSizeVar_ << "; ++" << indexVarName_ << ")";
    ScopeGuard g(*this);
    IndentGuard p(parent_);
    generateReadLoopContents();
  }

  StructReadGenerator& parent_;
  const std::string currentNamespace_;
};

class StructReadGenerator::ListReadGenerator : public ContainerReadGenerator {
public:
  ListReadGenerator(StructReadGenerator& parent, const t_list* l)
    : ContainerReadGenerator(parent, "list", l), list_(l) {}

private:
  void generateReadBegin() override {
    const auto ignoredTypeVarName = generateIgnoredType();
    indented() << "xfer += iprot->readListBegin(" << ignoredTypeVarName << ", " << containerSizeVar_
               << ");" << endl;
    indented() << containerNameVar_ << ".resize(" << containerSizeVar_ << ");" << endl;
  }

  void generateReadEnd() override { indented() << "xfer += iprot->readListEnd();" << endl; }

  void generateReadLoopContents() override {
    generateReadItem(list_->get_elem_type(), getListElemName());
  }

  std::string getListElemName() { return containerNameVar_ + "[" + indexVarName_ + "]"; }

  const t_list* list_;
};

class StructReadGenerator::SetReadGenerator : public ContainerReadGenerator {
public:
  SetReadGenerator(StructReadGenerator& parent, const t_set* s)
    : ContainerReadGenerator(parent, "set", s), set_(s) {}

private:
  void generateReadBegin() override {
    const auto ignoredTypeVarName = generateIgnoredType();
    indented() << "xfer += iprot->readSetBegin(" << ignoredTypeVarName << ", " << containerSizeVar_
               << ");" << endl;
  }

  void generateReadEnd() override { indented() << "xfer += iprot->readSetEnd();" << endl; }

  void generateReadLoopContents() override {
    const auto keyVarName = makeUniqueName("key");
    out() << indent();
    generateType(set_->get_elem_type());
    out() << " " << keyVarName << "{};" << endl;
    generateReadItem(set_->get_elem_type(), keyVarName);
    indented() << containerNameVar_ << ".insert(std::move(" << keyVarName << "));" << endl;
  }

  const t_set* set_;
};

class StructReadGenerator::MapReadGenerator : public ContainerReadGenerator {
public:
  MapReadGenerator(StructReadGenerator& parent, const t_map* m)
    : ContainerReadGenerator(parent, "map", m), map_(m) {}

private:
  void generateReadBegin() override {
    const auto kIgnoredTypeVarName = generateIgnoredType();
    const auto vIgnoredTypeVarName = generateIgnoredType();
    indented() << "xfer += iprot->readMapBegin(" << kIgnoredTypeVarName << ", "
               << vIgnoredTypeVarName << ", " << containerSizeVar_ << ");" << endl;
  }

  void generateReadEnd() override { indented() << "xfer += iprot->readMapEnd();" << endl; }

  void generateReadLoopContents() override {
    const auto keyVarName = makeUniqueName("key");
    const auto valueVarName = makeUniqueName("val");

    out() << indent();
    generateType(map_->get_key_type());
    out() << " " << keyVarName << "{};" << endl;
    generateReadItem(map_->get_key_type(), keyVarName);

    indented() << "auto& " << valueVarName << " = " << containerNameVar_ << "[" << keyVarName
               << "];" << endl;
    generateReadItem(map_->get_val_type(), valueVarName);
  }

  const t_map* map_;
};

void StructReadGenerator::generateListFieldRead(const t_type* t, const std::string& fieldName) {
  ListReadGenerator gen(*this, static_cast<const t_list*>(t));
  gen.generate(fieldName);
}

void StructReadGenerator::generateSetFieldRead(const t_type* t, const std::string& fieldName) {
  SetReadGenerator gen(*this, static_cast<const t_set*>(t));
  gen.generate(fieldName);
}

void StructReadGenerator::generateMapFieldRead(const t_type* t, const std::string& fieldName) {
  MapReadGenerator gen(*this, static_cast<const t_map*>(t));
  gen.generate(fieldName);
}
