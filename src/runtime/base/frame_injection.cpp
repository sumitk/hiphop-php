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
#include <runtime/base/types.h>
#include <runtime/base/builtin_functions.h>
#include <runtime/base/source_info.h>
#include <runtime/base/class_info.h>
#include <runtime/base/frame_injection.h>

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////

const char *FrameInjection::GetClassName(bool skip /* = false */) {
  FrameInjection *t = ThreadInfo::s_threadInfo->m_top;
  if (t && skip) {
    t = t->m_prev;
  }
  // If we have included a file inside a class method or called a builtin
  // function, we should walk up to find that class
  if (t) {
    while (t->m_prev && !*t->m_class &&
        t->flags & (PseudoMain | BuiltinFunction)) {
      t = t->m_prev;
    }
  }
  if (t && *t->m_class) {
    return t->m_class;
  }
  return "";
}

const char *FrameInjection::GetParentClassName(bool skip /* = false */) {
  String cls = GetClassName(skip);
  if (cls.empty()) return cls;
  const ClassInfo *classInfo = ClassInfo::FindClass(cls);
  if (classInfo) {
    const char *parentClass = classInfo->getParentClass();
    if (parentClass && parentClass[0]) {
      return parentClass;
    }
  }
  return "";
}

Object FrameInjection::GetThis(bool skip /* = false */) {
  FrameInjection *t = ThreadInfo::s_threadInfo->m_top;
  if (t && skip) {
    t = t->m_prev;
  }
  if (t) {
    return t->m_object;
  }
  return Object();
}

String FrameInjection::GetContainingFileName(bool skip /* = false */) {
  FrameInjection *t = ThreadInfo::s_threadInfo->m_top;
  if (t && skip) {
    t = t->m_prev;
  }
  if (t) {
    return t->getFileName();
  }
  return "";
}

Array FrameInjection::GetBacktrace(bool skip /* = false */,
                                   bool withSelf /* = false */) {
  Array bt = Array::Create();
  FrameInjection *t = ThreadInfo::s_threadInfo->m_top;
  if (skip && t) {
    t = t->m_prev;
  }
  // This is used by onError with extended exceptions
  if (withSelf && t) {
    String filename = t->getFileName();
    // If the top frame is not an extension function,
    // add it to the trace
    if (filename != "") {
      Array frame = Array::Create();
      frame.set("file", filename);
      frame.set("line", t->line);
      bt.append(frame);
    }
  }
  while (t) {
    if (t->flags & PseudoMain) {
      // TODO should we generate require_once for pseudo mains?
      t = t->m_prev;
      continue;
    }
    Array frame = Array::Create();

    if (t->m_prev) {
      String file = t->m_prev->getFileName();
      if (!file.empty()) {
        frame.set("file", file);
      }
      frame.set("line", t->m_prev->line);
    }

    const char *c = strstr(t->m_name, "::");
    if (c) {
      frame.set("function", String(c + 2));
      frame.set("class", String(t->m_class));
      if (!t->m_object.isNull()) {
        frame.set("object", t->m_object);
        frame.set("type", "->");
      } else {
        frame.set("type", "::");
      }
    } else {
      frame.set("function", t->m_name);
    }

    Array args = t->getArgs();
    if (!args.isNull()) {
      frame.set("args", args);
    } else {
      frame.set("args", Array::Create());
    }

    bt.append(frame);
    t = t->m_prev;
  }
  return bt;
}

String FrameInjection::getFileName() {
  if (flags & PseudoMain) {
    return m_name + 10;
  }
  const char *c = strstr(m_name, "::");
  const char *f = NULL;
  if (c) {
    f = SourceInfo::TheSourceInfo.getClassDeclaringFile(m_class);
  } else {
    f = SourceInfo::TheSourceInfo.getFunctionDeclaringFile(m_name);
  }
  if (f != NULL) {
    return f;
  }
  return null_string;
}

Array FrameInjection::getArgs() {
  return Array();
}

///////////////////////////////////////////////////////////////////////////////
// static late binding

String FrameInjection::GetStaticClassName(ThreadInfo *info) {
  if (!info) info = ThreadInfo::s_threadInfo.get();
  for (FrameInjection *t = info->m_top; t; t = t->m_prev) {
    if (!t->m_staticClass.empty()) {
      return t->m_staticClass;
    }
    if (!t->m_callingObject.isNull()) {
      return t->m_callingObject->o_getClassName();
    }
    if (!t->m_object.isNull()) {
      return t->m_object->o_getClassName();
    }
  }
  return "";
}

void FrameInjection::SetStaticClassName(ThreadInfo *info, CStrRef cls) {
  if (!info) info = ThreadInfo::s_threadInfo.get();
  FrameInjection *t = info->m_top;
  if (t) t->m_staticClass = cls;
}

void FrameInjection::ResetStaticClassName(ThreadInfo *info) {
  if (!info) info = ThreadInfo::s_threadInfo.get();
  FrameInjection *t = info->m_top;
  if (t) t->m_staticClass.reset();
}

void FrameInjection::SetCallingObject(ThreadInfo* info, ObjectData *obj) {
  if (!info) info = ThreadInfo::s_threadInfo.get();
  FrameInjection *t = info->m_top;
  if (t) t->m_callingObject = obj;
}

FrameInjection::EvalStaticClassNameHelper::EvalStaticClassNameHelper
(CStrRef name, CStrRef resolved, bool sp) : m_set(false) {
  if (!sp && strcasecmp(name.data(), "static")) {
    FrameInjection::SetStaticClassName(NULL, resolved);
    m_set = true;
  }
}

FrameInjection::EvalStaticClassNameHelper::EvalStaticClassNameHelper
(CObjRef obj) {
  FrameInjection::SetCallingObject(NULL, obj.get());
}

FrameInjection::EvalStaticClassNameHelper::~EvalStaticClassNameHelper() {
  if (m_set) {
    FrameInjection::ResetStaticClassName(NULL);
  }
}

///////////////////////////////////////////////////////////////////////////////
}
