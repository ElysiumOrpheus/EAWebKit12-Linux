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

#ifndef JSValidityState_h
#define JSValidityState_h

#include "JSDOMBinding.h"
#include "ValidityState.h"
#include <runtime/JSGlobalObject.h>
#include <runtime/JSObject.h>
#include <runtime/ObjectPrototype.h>

namespace WebCore {

class JSValidityState : public JSDOMWrapper {
public:
    typedef JSDOMWrapper Base;
    static JSValidityState* create(JSC::Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<ValidityState> impl)
    {
        JSValidityState* ptr = new (JSC::allocateCell<JSValidityState>(globalObject->globalData().heap)) JSValidityState(structure, globalObject, impl);
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

    ValidityState* impl() const { return m_impl.get(); }

private:
    RefPtr<ValidityState> m_impl;
protected:
    JSValidityState(JSC::Structure*, JSDOMGlobalObject*, PassRefPtr<ValidityState>);
    void finishCreation(JSC::JSGlobalData&);
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | Base::StructureFlags;
};

JSC::JSValue toJS(JSC::ExecState*, JSDOMGlobalObject*, ValidityState*);
ValidityState* toValidityState(JSC::JSValue);

class JSValidityStatePrototype : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;
    static JSC::JSObject* self(JSC::ExecState*, JSC::JSGlobalObject*);
    static JSValidityStatePrototype* create(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSValidityStatePrototype* ptr = new (JSC::allocateCell<JSValidityStatePrototype>(globalData.heap)) JSValidityStatePrototype(globalData, globalObject, structure);
        ptr->finishCreation(globalData);
        return ptr;
    }

    static const JSC::ClassInfo s_info;
    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }

private:
    JSValidityStatePrototype(JSC::JSGlobalData& globalData, JSC::JSGlobalObject*, JSC::Structure* structure) : JSC::JSNonFinalObject(globalData, structure) { }
protected:
    static const unsigned StructureFlags = Base::StructureFlags;
};

// Attributes

JSC::JSValue jsValidityStateValueMissing(JSC::ExecState*, JSC::JSValue, const JSC::Identifier&);
JSC::JSValue jsValidityStateTypeMismatch(JSC::ExecState*, JSC::JSValue, const JSC::Identifier&);
JSC::JSValue jsValidityStatePatternMismatch(JSC::ExecState*, JSC::JSValue, const JSC::Identifier&);
JSC::JSValue jsValidityStateTooLong(JSC::ExecState*, JSC::JSValue, const JSC::Identifier&);
JSC::JSValue jsValidityStateRangeUnderflow(JSC::ExecState*, JSC::JSValue, const JSC::Identifier&);
JSC::JSValue jsValidityStateRangeOverflow(JSC::ExecState*, JSC::JSValue, const JSC::Identifier&);
JSC::JSValue jsValidityStateStepMismatch(JSC::ExecState*, JSC::JSValue, const JSC::Identifier&);
JSC::JSValue jsValidityStateCustomError(JSC::ExecState*, JSC::JSValue, const JSC::Identifier&);
JSC::JSValue jsValidityStateValid(JSC::ExecState*, JSC::JSValue, const JSC::Identifier&);

} // namespace WebCore

#endif
