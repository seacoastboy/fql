#include <stdio.h>

#include "parser.h"
#include "expr.h"

#define PREPARE_PARAM  \
  if (left_) left = left_->Evaluate(row); \
  if (right_) right = right_->Evaluate(row);

#define BINARY_OP(OP) do { \
  PREPARE_PARAM \
  if (left && right) { \
    ret = left->OP(right); \
  }\
} while (0)

#define LOGIC_OP(OP) do { \
  PREPARE_PARAM \
  if (left && right) { \
    ret = new Bool(left->OP(right)); \
  }\
} while (0)

#define COMPARE_OP(OP) do {\
  PREPARE_PARAM \
  if (left && right) { \
    ret = new Bool(left->Compare(right) OP 0); \
  }\
} while (0)

Variant *BinaryExpr::Evaluate(Row *row) {
  Variant *ret = NULL;
  Variant *left = NULL;
  Variant *right = NULL;

  switch (op_) {
    case TK_PLUS:
      BINARY_OP(Plus);
      break;
    case TK_MINUS:
      BINARY_OP(Minus);
      break;
    case TK_DIV:
      BINARY_OP(Div);
      break;
    case TK_MOD:
      BINARY_OP(Mod);
      break;
    case TK_STAR:
      BINARY_OP(Mult);
      break;
    case TK_OR:
      LOGIC_OP(Or);
      break;
    case TK_AND:
      LOGIC_OP(And);
      break;
    case TK_GT:
      COMPARE_OP(>);
      break;
    case TK_LT:
      COMPARE_OP(<);
      break;
    case TK_EQ:
      COMPARE_OP(==);
      break;
    case TK_GE:
      COMPARE_OP(>=);
      break;
    case TK_LE:
      COMPARE_OP(<=);
      break;
    case TK_NE:
      COMPARE_OP(<=);
      break;
    case TK_LIKE:
      PREPARE_PARAM;
      if (right) {
        ret = new Bool(!right->c_bool());
      }
      break;
    default:
      break;
  }
  return ret;
}

void BinaryExpr::Print(int indent) {
  for (int i = 0; i < indent; i++) printf(" ");
  printf("(%2d,\n", op_);
  if (left_) left_->Print(indent + 2);
  for (int i = 0; i < indent + 2; i++) printf(" ");
  printf(", ");
  if (right_) right_->Print(indent + 2);
  for (int i = 0; i < indent; i++) printf(" ");
  printf(")\n");
}

Variant *UnaryExpr::Evaluate(Row *row) {
  Variant *ret = NULL;
  Variant *right = NULL;

  switch (op_) {
    case TK_NOT:
      if (right_) right = right_->Evaluate(row);
      if (right) {
        ret = new Bool(!right->c_bool());
      }
      break;
    default:
      break;
  }
  return ret;
}

