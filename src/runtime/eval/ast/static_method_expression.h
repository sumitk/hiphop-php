/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010 Facebook, Inc. (http://www.facebook.com)          |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#ifndef __EVAL_STATIC_METHOD_EXPRESSION_H__
#define __EVAL_STATIC_METHOD_EXPRESSION_H__

#include <runtime/eval/ast/simple_function_call_expression.h>

namespace HPHP {
namespace Eval {
///////////////////////////////////////////////////////////////////////////////

DECLARE_AST_PTR(StaticMethodExpression);
DECLARE_AST_PTR(Name);

class StaticMethodExpression : public SimpleFunctionCallExpression {
public:
  StaticMethodExpression(EXPRESSION_ARGS, const NamePtr &cname, bool sp,
      const NamePtr &name, const std::vector<ExpressionPtr> &params);
  virtual Variant eval(VariableEnvironment &env) const;
  virtual void dump() const;
protected:
  NamePtr m_cname;
  bool m_sp; // it was "self" or "parent"
  bool m_construct;
};

///////////////////////////////////////////////////////////////////////////////
}
}

#endif /* __EVAL_STATIC_METHOD_EXPRESSION_H__ */
