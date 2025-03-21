/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef JSSQLError_h
#define JSSQLError_h

#if ENABLE(DATABASE)

#include "JSDOMBinding.h"
#include "SQLError.h"
#include <runtime/JSGlobalObject.h>
#include <runtime/JSObject.h>
#include <runtime/ObjectPrototype.h>

namespace WebCore {

class JSSQLError : public JSDOMWrapper {
public:
    typedef JSDOMWrapper Base;
    static JSSQLError* create(JSC::Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<SQLError> impl)
    {
        JSSQLError* ptr = new (JSC::allocateCell<JSSQLError>(globalObject->globalData().heap)) JSSQLError(structure, globalObject, impl);
        ptr->finishCreation(globalObject->globalData());
        return ptr;
    }

    static JSC::JSObject* createPrototype(JSC::ExecState*, JSC::JSGlobalObject*);
    virtual bool getOwnPropertySlot(JSC::ExecState*, const JSC::Identifier& propertyName, JSC::PropertySlot&);
    virtual bool getOwnPropertyDescriptor(JSC::ExecState*, const JSC::Identifier& propertyName, JSC::PropertyDescriptor&);
    static const JSC::ClassInfo s_info;

    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

    SQLError* impl() const { return m_impl.get(); }

private:
    RefPtr<SQLError> m_impl;
protected:
    JSSQLError(JSC::Structure*, JSDOMGlobalObject*, PassRefPtr<SQLError>);
    void finishCreation(JSC::JSGlobalData&);
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | Base::StructureFlags;
};

JSC::JSValue toJS(JSC::ExecState*, JSDOMGlobalObject*, SQLError*);
SQLError* toSQLError(JSC::JSValue);

class JSSQLErrorPrototype : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;
    static JSC::JSObject* self(JSC::ExecState*, JSC::JSGlobalObject*);
    static JSSQLErrorPrototype* create(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSSQLErrorPrototype* ptr = new (JSC::allocateCell<JSSQLErrorPrototype>(globalData.heap)) JSSQLErrorPrototype(globalData, globalObject, structure);
        ptr->finishCreation(globalData);
        return ptr;
    }

    static const JSC::ClassInfo s_info;
    virtual bool getOwnPropertySlot(JSC::ExecState*, const JSC::Identifier&, JSC::PropertySlot&);
    virtual bool getOwnPropertyDescriptor(JSC::ExecState*, const JSC::Identifier&, JSC::PropertyDescriptor&);
    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

private:
    JSSQLErrorPrototype(JSC::JSGlobalData& globalData, JSC::JSGlobalObject*, JSC::Structure* structure) : JSC::JSNonFinalObject(globalData, structure) { }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | Base::StructureFlags;
};

// Attributes

JSC::JSValue jsSQLErrorCode(JSC::ExecState*, JSC::JSValue, const JSC::Identifier&);
JSC::JSValue jsSQLErrorMessage(JSC::ExecState*, JSC::JSValue, const JSC::Identifier&);
// Constants

JSC::JSValue jsSQLErrorUNKNOWN_ERR(JSC::ExecState*, JSC::JSValue, const JSC::Identifier&);
JSC::JSValue jsSQLErrorDATABASE_ERR(JSC::ExecState*, JSC::JSValue, const JSC::Identifier&);
JSC::JSValue jsSQLErrorVERSION_ERR(JSC::ExecState*, JSC::JSValue, const JSC::Identifier&);
JSC::JSValue jsSQLErrorTOO_LARGE_ERR(JSC::ExecState*, JSC::JSValue, const JSC::Identifier&);
JSC::JSValue jsSQLErrorQUOTA_ERR(JSC::ExecState*, JSC::JSValue, const JSC::Identifier&);
JSC::JSValue jsSQLErrorSYNTAX_ERR(JSC::ExecState*, JSC::JSValue, const JSC::Identifier&);
JSC::JSValue jsSQLErrorCONSTRAINT_ERR(JSC::ExecState*, JSC::JSValue, const JSC::Identifier&);
JSC::JSValue jsSQLErrorTIMEOUT_ERR(JSC::ExecState*, JSC::JSValue, const JSC::Identifier&);

} // namespace WebCore

#endif // ENABLE(DATABASE)

#endif
