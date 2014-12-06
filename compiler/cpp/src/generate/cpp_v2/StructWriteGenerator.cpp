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

#include "StructWriteGenerator.h"

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

#include "Utils.h"

using namespace apache::thrift::compiler::generate::cpp_v2;
using namespace apache::thrift::compiler;

StructWriteGenerator::StructWriteGenerator(OutputGenerator& parent, const t_struct* tstruct)
  : MethodGenerator(parent, tstruct->get_name()), tStruct_(tstruct) {
}

void StructWriteGenerator::generateReturnType() {
  out() << "std::uint32_t";
}

void StructWriteGenerator::generateDeclarationWithoutReturn() {
  out() << "write(::apache::thrift::protocol::TProtocol* oprot) const";
}

void StructWriteGenerator::generateDefinitionBody() {
  generateWriteHeader();
  generateWriteFields();
  generateWriteFooter();
}

void StructWriteGenerator::generateWriteHeader() {
  indented() << "std::uint32_t xfer = 0;" << endl;
  indented() << "oprot->incrementRecursionDepth();" << endl;
  indented() << "xfer += oprot->writeStructBegin(\"" << tStruct_->get_name() << "\");" << endl;
  out() << endl;
}

void StructWriteGenerator::generateWriteFooter() {
  indented() << "xfer += oprot->writeFieldStop();" << endl;
  indented() << "xfer += oprot->writeStructEnd();" << endl;
  indented() << "oprot->decrementRecursionDepth();" << endl;
  indented() << "return xfer;" << endl;
}

void StructWriteGenerator::generateWriteFields() {
  for (auto f : tStruct_->get_members()) {
    generateFieldWriteSequenceDependingOnReq(f);
    out() << endl;
  }
}

void StructWriteGenerator::generateFieldWriteSequence(const t_field* f) {
  generateFieldWriteBegin(f);
  generateFieldWrite(f);
  generateFieldWriteEnd();
}

void StructWriteGenerator::generateFieldWriteSequenceDependingOnReq(const t_field* f) {
  if (f->get_req() == t_field::T_OPTIONAL)
    return generateOptionalFieldWriteSequence(f);
  return generateFieldWriteSequence(f);
}

void StructWriteGenerator::generateOptionalFieldWriteSequence(const t_field* f) {
  indented() << "if (this->" << f->get_name() << ")";
  ScopeGuard g(*this);
  generateFieldWriteSequence(f);
}

void StructWriteGenerator::generateFieldWriteBegin(const t_field* f) {
  indented() << "xfer += oprot->writeFieldBegin(\"" << f->get_name() << "\", "
             << getTTypeString(getTrueType(f)) << ", " << f->get_key() << ");" << endl;
}

void StructWriteGenerator::generateFieldWriteEnd() {
  indented() << "xfer += oprot->writeFieldEnd();" << endl;
}

void StructWriteGenerator::generateFieldWrite(const t_field* f) {
  const auto type = getTrueType(f);
  const auto fieldName = getFieldName(f);

  if (type->is_struct() || type->is_xception())
    return generateStructFieldWrite(fieldName, f->get_req() == t_field::T_OPTIONAL);
  generateWriteByType(type, fieldName);
}

void StructWriteGenerator::generateWriteByType(const t_type* type, const std::string& fieldName) {
  if (type->is_base_type())
    return generateBaseTypeFieldWrite(type, fieldName);
  if (type->is_struct() || type->is_xception())
    return generateStructFieldWrite(fieldName);
  if (type->is_enum())
    return generateEnumFieldWrite(fieldName);
  if (type->is_list())
    return generateListFieldWrite(type, fieldName);
  if (type->is_set())
    return generateSetFieldWrite(type, fieldName);
  if (type->is_map())
    return generateMapFieldWrite(type, fieldName);
}

void StructWriteGenerator::generateBaseTypeFieldWrite(const t_type* t,
                                                      const std::string& fieldName) {
  indented() << "xfer += oprot->write" << getReadWriteSuffix(static_cast<const t_base_type*>(t))
             << "(" << fieldName << ");" << endl;
}

const t_type* StructWriteGenerator::getTrueType(const t_field* f) const {
  return f->get_type()->get_true_type();
}

void StructWriteGenerator::generateStructFieldWrite(const std::string& fieldName, bool optional) {
  auto f = fieldName;
  if (optional)
    f = "(" + f + ")";
  indented() << "xfer += " << f << ".write(oprot);" << endl;
}

void StructWriteGenerator::generateEnumFieldWrite(const std::string& fieldName) {
  indented() << "xfer += oprot->writeI32(static_cast<std::int32_t>(" << fieldName << "));" << endl;
}

class StructWriteGenerator::ContainerWriteGenerator : public OutputGenerator {
protected:
  ContainerWriteGenerator(StructWriteGenerator& parent, const std::string& containerNameTempl)
    : OutputGenerator(parent),
      containerItemVar_(makeUniqueName(containerNameTempl + "Item")),
      parent_(parent) {}

public:
  void generate(const std::string& fieldName) {
    generateWriteBegin(fieldName);
    generateWriteLoop(fieldName);
    generateWriteEnd();
  }

protected:
  virtual void generateWriteBegin(const std::string& fieldName) = 0;
  virtual void generateWriteEnd() = 0;
  virtual void generateWriteLoopContents() = 0;

  void generateWriteItem(const t_type* t, const std::string& name) {
    parent_.generateWriteByType(t, name);
  }

  const std::string containerItemVar_;

private:
  void generateWriteLoop(const std::string& fieldName) {
    indented() << "for (const auto& " << containerItemVar_ << " : " << fieldName << ")";
    ScopeGuard g(*this);
    IndentGuard p(parent_);
    generateWriteLoopContents();
  }

  StructWriteGenerator& parent_;
};

class StructWriteGenerator::ListWriteGenerator : public ContainerWriteGenerator {
public:
  ListWriteGenerator(StructWriteGenerator& parent, const t_list* l)
    : ContainerWriteGenerator(parent, "list"), list_(l) {}

private:
  void generateWriteBegin(const std::string& fieldName) override {
    indented() << "xfer += oprot->writeListBegin(" << getTTypeString(list_->get_elem_type())
               << ", static_cast<std::uint32_t>(" << fieldName << ".size()));" << endl;
  }

  void generateWriteEnd() override { indented() << "xfer += oprot->writeListEnd();" << endl; }

  void generateWriteLoopContents() override {
    generateWriteItem(list_->get_elem_type(), containerItemVar_);
  }

  const t_list* list_;
};

class StructWriteGenerator::SetWriteGenerator : public ContainerWriteGenerator {
public:
  SetWriteGenerator(StructWriteGenerator& parent, const t_set* s)
    : ContainerWriteGenerator(parent, "set"), set_(s) {}

private:
  void generateWriteBegin(const std::string& fieldName) override {
    indented() << "xfer += oprot->writeSetBegin(" << getTTypeString(set_->get_elem_type())
               << ", static_cast<std::uint32_t>(" << fieldName << ".size()));" << endl;
  }

  void generateWriteEnd() override { indented() << "xfer += oprot->writeSetEnd();" << endl; }

  void generateWriteLoopContents() override {
    generateWriteItem(set_->get_elem_type(), containerItemVar_);
  }

  const t_set* set_;
};

class StructWriteGenerator::MapWriteGenerator : public ContainerWriteGenerator {
public:
  MapWriteGenerator(StructWriteGenerator& parent, const t_map* m)
    : ContainerWriteGenerator(parent, "map"), map_(m) {}

private:
  void generateWriteBegin(const std::string& fieldName) override {
    indented() << "xfer += oprot->writeMapBegin(" << getTTypeString(map_->get_key_type()) << ", "
               << getTTypeString(map_->get_val_type()) << ", static_cast<std::uint32_t>("
               << fieldName << ".size()));" << endl;
  }

  void generateWriteEnd() override { indented() << "xfer += oprot->writeMapEnd();" << endl; }

  void generateWriteLoopContents() override {
    generateWriteItem(map_->get_key_type(), containerItemVar_ + ".first");
    generateWriteItem(map_->get_val_type(), containerItemVar_ + ".second");
  }

  const t_map* map_;
};

void StructWriteGenerator::generateListFieldWrite(const t_type* t, const std::string& fieldName) {
  ListWriteGenerator gen(*this, static_cast<const t_list*>(t));
  gen.generate(fieldName);
}

void StructWriteGenerator::generateSetFieldWrite(const t_type* t, const std::string& fieldName) {
  SetWriteGenerator gen(*this, static_cast<const t_set*>(t));
  gen.generate(fieldName);
}

void StructWriteGenerator::generateMapFieldWrite(const t_type* t, const std::string& fieldName) {
  MapWriteGenerator gen(*this, static_cast<const t_map*>(t));
  gen.generate(fieldName);
}

std::string StructWriteGenerator::getFieldName(const t_field* f) const {
  const auto name = "this->" + f->get_name();
  if (f->get_req() == t_field::T_OPTIONAL)
    return "*(" + name + ")";
  return name;
}
