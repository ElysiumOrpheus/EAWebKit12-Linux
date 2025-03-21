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

#include "config.h"
#include "JSUIEvent.h"

#include "DOMWindow.h"
#include "ExceptionCode.h"
#include "JSDOMBinding.h"
#include "JSDOMWindow.h"
#include "UIEvent.h"
#include <runtime/Error.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSUIEvent);

/* Hash table */
#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

static const HashTableValue JSUIEventTableValues[] =
{
    { "view", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsUIEventView), (intptr_t)0 THUNK_GENERATOR(0) },
    { "detail", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsUIEventDetail), (intptr_t)0 THUNK_GENERATOR(0) },
    { "keyCode", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsUIEventKeyCode), (intptr_t)0 THUNK_GENERATOR(0) },
    { "charCode", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsUIEventCharCode), (intptr_t)0 THUNK_GENERATOR(0) },
    { "layerX", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsUIEventLayerX), (intptr_t)0 THUNK_GENERATOR(0) },
    { "layerY", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsUIEventLayerY), (intptr_t)0 THUNK_GENERATOR(0) },
    { "pageX", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsUIEventPageX), (intptr_t)0 THUNK_GENERATOR(0) },
    { "pageY", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsUIEventPageY), (intptr_t)0 THUNK_GENERATOR(0) },
    { "which", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsUIEventWhich), (intptr_t)0 THUNK_GENERATOR(0) },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsUIEventConstructor), (intptr_t)0 THUNK_GENERATOR(0) },
    { 0, 0, 0, 0 THUNK_GENERATOR(0) }
};

#undef THUNK_GENERATOR
static JSC_CONST_HASHTABLE HashTable JSUIEventTable = { 34, 31, JSUIEventTableValues, 0 };
/* Hash table for constructor */
#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

static const HashTableValue JSUIEventConstructorTableValues[] =
{
    { 0, 0, 0, 0 THUNK_GENERATOR(0) }
};

#undef THUNK_GENERATOR
static JSC_CONST_HASHTABLE HashTable JSUIEventConstructorTable = { 1, 0, JSUIEventConstructorTableValues, 0 };
class JSUIEventConstructor : public DOMConstructorObject {
private:
    JSUIEventConstructor(JSC::Structure*, JSDOMGlobalObject*);
    void finishCreation(JSC::ExecState*, JSDOMGlobalObject*);

public:
    typedef DOMConstructorObject Base;
    static JSUIEventConstructor* create(JSC::ExecState* exec, JSC::Structure* structure, JSDOMGlobalObject* globalObject)
    {
        JSUIEventConstructor* ptr = new (JSC::allocateCell<JSUIEventConstructor>(*exec->heap())) JSUIEventConstructor(structure, globalObject);
        ptr->finishCreation(exec, globalObject);
        return ptr;
    }

    virtual bool getOwnPropertySlot(JSC::ExecState*, const JSC::Identifier&, JSC::PropertySlot&);
    virtual bool getOwnPropertyDescriptor(JSC::ExecState*, const JSC::Identifier&, JSC::PropertyDescriptor&);
    static const JSC::ClassInfo s_info;
    static JSC::Structure* createStructure(JSC::JSGlobalData& globalData, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(globalData, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), &s_info);
    }
protected:
    static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::ImplementsHasInstance | DOMConstructorObject::StructureFlags;
};

const ClassInfo JSUIEventConstructor::s_info = { "UIEventConstructor", &DOMConstructorObject::s_info, &JSUIEventConstructorTable, 0 };

JSUIEventConstructor::JSUIEventConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSUIEventConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSUIEventPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}

bool JSUIEventConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSUIEventConstructor, JSDOMWrapper>(exec, &JSUIEventConstructorTable, this, propertyName, slot);
}

bool JSUIEventConstructor::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSUIEventConstructor, JSDOMWrapper>(exec, &JSUIEventConstructorTable, this, propertyName, descriptor);
}

/* Hash table for prototype */
#if ENABLE(JIT)
#define THUNK_GENERATOR(generator) , generator
#else
#define THUNK_GENERATOR(generator)
#endif

static const HashTableValue JSUIEventPrototypeTableValues[] =
{
    { "initUIEvent", DontDelete | Function, (intptr_t)static_cast<NativeFunction>(jsUIEventPrototypeFunctionInitUIEvent), (intptr_t)5 THUNK_GENERATOR(0) },
    { 0, 0, 0, 0 THUNK_GENERATOR(0) }
};

#undef THUNK_GENERATOR
static JSC_CONST_HASHTABLE HashTable JSUIEventPrototypeTable = { 2, 1, JSUIEventPrototypeTableValues, 0 };
const ClassInfo JSUIEventPrototype::s_info = { "UIEventPrototype", &JSC::JSNonFinalObject::s_info, &JSUIEventPrototypeTable, 0 };

JSObject* JSUIEventPrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSUIEvent>(exec, globalObject);
}

bool JSUIEventPrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticFunctionSlot<JSObject>(exec, &JSUIEventPrototypeTable, this, propertyName, slot);
}

bool JSUIEventPrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticFunctionDescriptor<JSObject>(exec, &JSUIEventPrototypeTable, this, propertyName, descriptor);
}

const ClassInfo JSUIEvent::s_info = { "UIEvent", &JSEvent::s_info, &JSUIEventTable, 0 };

JSUIEvent::JSUIEvent(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<UIEvent> impl)
    : JSEvent(structure, globalObject, impl)
{
}

void JSUIEvent::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSUIEvent::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSUIEventPrototype::create(exec->globalData(), globalObject, JSUIEventPrototype::createStructure(exec->globalData(), globalObject, JSEventPrototype::self(exec, globalObject)));
}

bool JSUIEvent::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    ASSERT_GC_OBJECT_INHERITS(this, &s_info);
    return getStaticValueSlot<JSUIEvent, Base>(exec, &JSUIEventTable, this, propertyName, slot);
}

bool JSUIEvent::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    ASSERT_GC_OBJECT_INHERITS(this, &s_info);
    return getStaticValueDescriptor<JSUIEvent, Base>(exec, &JSUIEventTable, this, propertyName, descriptor);
}

JSValue jsUIEventView(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSUIEvent* castedThis = static_cast<JSUIEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    UIEvent* imp = static_cast<UIEvent*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(imp->view()));
    return result;
}


JSValue jsUIEventDetail(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSUIEvent* castedThis = static_cast<JSUIEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    UIEvent* imp = static_cast<UIEvent*>(castedThis->impl());
    JSValue result = jsNumber(imp->detail());
    return result;
}


JSValue jsUIEventKeyCode(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSUIEvent* castedThis = static_cast<JSUIEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    UIEvent* imp = static_cast<UIEvent*>(castedThis->impl());
    JSValue result = jsNumber(imp->keyCode());
    return result;
}


JSValue jsUIEventCharCode(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSUIEvent* castedThis = static_cast<JSUIEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    UIEvent* imp = static_cast<UIEvent*>(castedThis->impl());
    JSValue result = jsNumber(imp->charCode());
    return result;
}


JSValue jsUIEventLayerX(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSUIEvent* castedThis = static_cast<JSUIEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    UIEvent* imp = static_cast<UIEvent*>(castedThis->impl());
    JSValue result = jsNumber(imp->layerX());
    return result;
}


JSValue jsUIEventLayerY(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSUIEvent* castedThis = static_cast<JSUIEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    UIEvent* imp = static_cast<UIEvent*>(castedThis->impl());
    JSValue result = jsNumber(imp->layerY());
    return result;
}


JSValue jsUIEventPageX(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSUIEvent* castedThis = static_cast<JSUIEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    UIEvent* imp = static_cast<UIEvent*>(castedThis->impl());
    JSValue result = jsNumber(imp->pageX());
    return result;
}


JSValue jsUIEventPageY(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSUIEvent* castedThis = static_cast<JSUIEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    UIEvent* imp = static_cast<UIEvent*>(castedThis->impl());
    JSValue result = jsNumber(imp->pageY());
    return result;
}


JSValue jsUIEventWhich(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSUIEvent* castedThis = static_cast<JSUIEvent*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    UIEvent* imp = static_cast<UIEvent*>(castedThis->impl());
    JSValue result = jsNumber(imp->which());
    return result;
}


JSValue jsUIEventConstructor(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSUIEvent* domObject = static_cast<JSUIEvent*>(asObject(slotBase));
    return JSUIEvent::getConstructor(exec, domObject->globalObject());
}

JSValue JSUIEvent::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSUIEventConstructor>(exec, static_cast<JSDOMGlobalObject*>(globalObject));
}

EncodedJSValue JSC_HOST_CALL jsUIEventPrototypeFunctionInitUIEvent(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSUIEvent::s_info))
        return throwVMTypeError(exec);
    JSUIEvent* castedThis = static_cast<JSUIEvent*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSUIEvent::s_info);
    UIEvent* imp = static_cast<UIEvent*>(castedThis->impl());
    const String& type(ustringToString(exec->argument(0).toString(exec)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    bool canBubble(exec->argument(1).toBoolean(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    bool cancelable(exec->argument(2).toBoolean(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    DOMWindow* view(toDOMWindow(exec->argument(3)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    int detail(exec->argument(4).toInt32(exec));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    imp->initUIEvent(type, canBubble, cancelable, view, detail);
    return JSValue::encode(jsUndefined());
}


}
