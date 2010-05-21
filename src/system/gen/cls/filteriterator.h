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
// @generated by HipHop Compiler

#ifndef __GENERATED_cls_filteriterator_h__
#define __GENERATED_cls_filteriterator_h__

#include <cls/iterator.h>
#include <cls/outeriterator.h>

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////

/* SRC: classes/iterator.php line 268 */
class c_filteriterator : public ExtObjectData {
  BEGIN_CLASS_MAP(filteriterator)
    PARENT_CLASS(iterator)
    PARENT_CLASS(outeriterator)
    PARENT_CLASS(traversable)
  END_CLASS_MAP(filteriterator)
  DECLARE_CLASS(filteriterator, FilterIterator, ObjectData)
  DECLARE_INVOKES_FROM_EVAL
  void init();
};

///////////////////////////////////////////////////////////////////////////////
}

#endif // __GENERATED_cls_filteriterator_h__