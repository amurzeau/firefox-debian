/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * vim: set ts=8 sw=4 et tw=99:
 *
 * ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is SpiderMonkey JSON.
 *
 * The Initial Developer of the Original Code is
 * Mozilla Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998-1999
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Robert Sayre <sayrer@gmail.com>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either of the GNU General Public License Version 2 or later (the "GPL"),
 * or the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include <string.h>     /* memset */
#include "jsapi.h"
#include "jsarena.h"
#include "jsarray.h"
#include "jsatom.h"
#include "jsbool.h"
#include "jscntxt.h"
#include "jsdtoa.h"
#include "jsfun.h"
#include "jsinterp.h"
#include "jsiter.h"
#include "jsnum.h"
#include "jsobj.h"
#include "jsprf.h"
#include "jsscan.h"
#include "jsstr.h"
#include "jstypes.h"
#include "jsutil.h"
#include "jsxml.h"

#include "json.h"

JSClass js_JSONClass = {
    js_JSON_str,
    JSCLASS_HAS_CACHED_PROTO(JSProto_JSON),
    JS_PropertyStub,  JS_PropertyStub,  JS_PropertyStub,  JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub,   JS_ConvertStub,   JS_FinalizeStub,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

JSBool
js_json_parse(JSContext *cx, uintN argc, jsval *vp)
{
    JSString *s = NULL;
    jsval *argv = vp + 2;
    jsval reviver = JSVAL_NULL;
    JSAutoTempValueRooter tvr(cx, 1, &reviver);
    
    if (!JS_ConvertArguments(cx, argc, argv, "S / v", &s, &reviver))
        return JS_FALSE;

    JSONParser *jp = js_BeginJSONParse(cx, vp);
    JSBool ok = jp != NULL;
    if (ok) {
        ok = js_ConsumeJSONText(cx, jp, JS_GetStringChars(s), JS_GetStringLength(s));
        ok &= js_FinishJSONParse(cx, jp, reviver);
    }

    return ok;
}

class WriterContext
{
public:
    WriterContext(JSContext *cx) : cx(cx), didWrite(JS_FALSE)
    {
        js_InitStringBuffer(&sb);
    }

    ~WriterContext()
    {
        js_FinishStringBuffer(&sb);
    }

    JSStringBuffer sb;
    JSContext *cx;
    JSBool didWrite;
};

static JSBool
WriteCallback(const jschar *buf, uint32 len, void *data)
{
    WriterContext *wc = static_cast<WriterContext*>(data);
    wc->didWrite = JS_TRUE;

    js_AppendUCString(&wc->sb, buf, len);
    if (!STRING_BUFFER_OK(&wc->sb)) {
        JS_ReportOutOfMemory(wc->cx);
        return JS_FALSE;
    }

    return JS_TRUE;
}

JSBool
js_json_stringify(JSContext *cx, uintN argc, jsval *vp)
{
    jsval *argv = vp + 2;
    JSObject *replacer = NULL;
    jsval space = JSVAL_NULL;
    JSAutoTempValueRooter tvr(cx, replacer);
    JSAutoTempValueRooter tvr2(cx, 1, &space);

    // Must throw an Error if there isn't a first arg
    if (!JS_ConvertArguments(cx, argc, argv, "v / o v", vp, &replacer, &space))
        return JS_FALSE;

    WriterContext wc(cx);

    if (!js_Stringify(cx, vp, replacer, space, &WriteCallback, &wc))
        return JS_FALSE;

    // XXX This can never happen to nsJSON.cpp, but the JSON object
    // needs to support returning undefined. So this is a little awkward
    // for the API, because we want to support streaming writers.
    if (wc.didWrite) {
        JSStringBuffer *sb = &wc.sb;
        JSString *s = JS_NewUCStringCopyN(cx, sb->base, STRING_BUFFER_OFFSET(sb));
        if (!s)
            return JS_FALSE;
        *vp = STRING_TO_JSVAL(s);
    } else {
        *vp = JSVAL_VOID;
    }

    return JS_TRUE;
}

JSBool
js_TryJSON(JSContext *cx, jsval *vp)
{
    // Checks whether the return value implements toJSON()
    JSBool ok = JS_TRUE;

    if (!JSVAL_IS_PRIMITIVE(*vp)) {
        JSObject *obj = JSVAL_TO_OBJECT(*vp);
        ok = js_TryMethod(cx, obj, cx->runtime->atomState.toJSONAtom, 0, NULL, vp);
    }

    return ok;
}


static const jschar quote = jschar('"');
static const jschar backslash = jschar('\\');
static const jschar unicodeEscape[] = {'\\', 'u', '0', '0'};
static const jschar null_ucstr[] = {'n', 'u', 'l', 'l'};
static const jschar true_ucstr[] = {'t', 'r', 'u', 'e'};
static const jschar false_ucstr[] = {'f', 'a', 'l', 's', 'e'};

static JSBool
write_string(JSContext *cx, JSONWriteCallback callback, void *data, const jschar *buf, uint32 len)
{
    if (!callback(&quote, 1, data))
        return JS_FALSE;

    uint32 mark = 0;
    uint32 i;
    for (i = 0; i < len; ++i) {
        if (buf[i] == quote || buf[i] == backslash) {
            if (!callback(&buf[mark], i - mark, data) || !callback(&backslash, 1, data) ||
                !callback(&buf[i], 1, data)) {
                return JS_FALSE;
            }
            mark = i + 1;
        } else if (buf[i] <= 31 || buf[i] == 127) {
            if (!callback(&buf[mark], i - mark, data) || !callback(unicodeEscape, 4, data))
                return JS_FALSE;
            char ubuf[3];
            size_t len = JS_snprintf(ubuf, sizeof(ubuf), "%.2x", buf[i]);
            JS_ASSERT(len == 2);
            jschar wbuf[3];
            size_t wbufSize = JS_ARRAY_LENGTH(wbuf);
            if (!js_InflateStringToBuffer(cx, ubuf, len, wbuf, &wbufSize) ||
                !callback(wbuf, wbufSize, data)) {
                return JS_FALSE;
            }
            mark = i + 1;
        }
    }

    if (mark < len && !callback(&buf[mark], len - mark, data))
        return JS_FALSE;

    return callback(&quote, 1, data);
}

class StringifyContext
{
public:
    StringifyContext(JSONWriteCallback callback, JSObject *replacer, void *data)
    : callback(callback), replacer(replacer), data(data), depth(0)
    {
        js_InitStringBuffer(&gap);
    }

    ~StringifyContext()
    {
        if (STRING_BUFFER_OK(&gap))
            js_FinishStringBuffer(&gap);
    }

    JSONWriteCallback callback;
    JSStringBuffer gap;
    JSObject *replacer;
    void *data;
    uint32 depth;
};

static JSBool CallReplacerFunction(JSContext *cx, jsid id, JSObject *holder,
                                   StringifyContext *scx, jsval *vp);
static JSBool Str(JSContext *cx, jsid id, JSObject *holder,
                  StringifyContext *scx, jsval *vp, bool callReplacer = true);

static JSBool
WriteIndent(JSContext *cx, StringifyContext *scx, uint32 limit)
{
    if (STRING_BUFFER_OFFSET(&scx->gap) > 0) {
        jschar c = jschar('\n');
        if (!scx->callback(&c, 1, scx->data))
            return JS_FALSE;
        for (uint32 i = 0; i < limit; i++) {
            if (!scx->callback(scx->gap.base, STRING_BUFFER_OFFSET(&scx->gap), scx->data))
                return JS_FALSE;
        }
    }

    return JS_TRUE;
}

static JSBool
JO(JSContext *cx, jsval *vp, StringifyContext *scx)
{
    JSObject *obj = JSVAL_TO_OBJECT(*vp);

    jschar c = jschar('{');
    if (!scx->callback(&c, 1, scx->data))
        return JS_FALSE;

    jsval vec[3] = {JSVAL_NULL, JSVAL_NULL, JSVAL_NULL};
    JSAutoTempValueRooter tvr(cx, 3, vec);
    jsval& key = vec[0];
    jsval& outputValue = vec[1];

    JSObject *iterObj = NULL;
    jsval *keySource = vp;
    bool usingWhitelist = false;

    // if the replacer is an array, we use the keys from it
    if (scx->replacer && JS_IsArrayObject(cx, scx->replacer)) {
        usingWhitelist = true;
        vec[2] = OBJECT_TO_JSVAL(scx->replacer);
        keySource = &vec[2];
    }

    if (!js_ValueToIterator(cx, JSITER_ENUMERATE, keySource))
        return JS_FALSE;
    iterObj = JSVAL_TO_OBJECT(*keySource);

    JSBool memberWritten = JS_FALSE;
    JSBool ok;

    do {
        outputValue = JSVAL_VOID;
        ok = js_CallIteratorNext(cx, iterObj, &key);
        if (!ok)
            break;
        if (key == JSVAL_HOLE)
            break;

        jsuint index = 0;
        if (usingWhitelist) {
            // skip non-index properties
            if (!js_IdIsIndex(key, &index))
                continue;

            jsval newKey;
            if (!OBJ_GET_PROPERTY(cx, scx->replacer, key, &newKey))
                return JS_FALSE;
            key = newKey;
        }

        JSString *ks;
        if (JSVAL_IS_STRING(key)) {
            ks = JSVAL_TO_STRING(key);
        } else {
            ks = js_ValueToString(cx, key);
            if (!ks) {
                ok = JS_FALSE;
                break;
            }
        }
        JSAutoTempValueRooter keyStringRoot(cx, ks);

        // Don't include prototype properties, since this operation is
        // supposed to be implemented as if by ES3.1 Object.keys()
        jsid id;
        jsval v = JS_FALSE;
        if (!js_ValueToStringId(cx, STRING_TO_JSVAL(ks), &id) ||
            !js_HasOwnProperty(cx, obj->map->ops->lookupProperty, obj, id, &v)) {
            ok = JS_FALSE;
            break;
        }

        if (v != JSVAL_TRUE)
            continue;

        ok = JS_GetPropertyById(cx, obj, id, &outputValue);
        if (!ok)
            break;

        if (JSVAL_IS_OBJECT(outputValue)) {
            ok = js_TryJSON(cx, &outputValue);
            if (!ok)
                break;
        }

        // call this here, so we don't write out keys if the replacer function
        // wants to elide the value.
        if (!CallReplacerFunction(cx, id, obj, scx, &outputValue))
            return JS_FALSE;

        JSType type = JS_TypeOfValue(cx, outputValue);

        // elide undefined values and functions and XML
        if (outputValue == JSVAL_VOID || type == JSTYPE_FUNCTION || type == JSTYPE_XML)
            continue;

        // output a comma unless this is the first member to write
        if (memberWritten) {
            c = jschar(',');
            ok = scx->callback(&c, 1, scx->data);
            if (!ok)
                break;
        }
        memberWritten = JS_TRUE;

        if (!WriteIndent(cx, scx, scx->depth))
            return JS_FALSE;

        // Be careful below, this string is weakly rooted
        JSString *s = js_ValueToString(cx, key);
        if (!s) {
            ok = JS_FALSE;
            break;
        }

        ok = write_string(cx, scx->callback, scx->data, JS_GetStringChars(s), JS_GetStringLength(s));
        if (!ok)
            break;

        c = jschar(':');
        ok = scx->callback(&c, 1, scx->data);
        if (!ok)
            break;

        ok = Str(cx, id, obj, scx, &outputValue, false);
        if (!ok)
            break;

    } while (ok);

    if (iterObj) {
        // Always close the iterator, but make sure not to stomp on OK
        JS_ASSERT(OBJECT_TO_JSVAL(iterObj) == *keySource);
        ok &= js_CloseIterator(cx, *keySource);
    }

    if (!ok)
        return JS_FALSE;

    if (memberWritten && !WriteIndent(cx, scx, scx->depth - 1))
        return JS_FALSE;

    c = jschar('}');

    return scx->callback(&c, 1, scx->data);
}

static JSBool
JA(JSContext *cx, jsval *vp, StringifyContext *scx)
{
    JSObject *obj = JSVAL_TO_OBJECT(*vp);

    jschar c = jschar('[');
    if (!scx->callback(&c, 1, scx->data))
        return JS_FALSE;

    jsuint length;
    if (!js_GetLengthProperty(cx, obj, &length))
        return JS_FALSE;

    jsval outputValue = JSVAL_NULL;
    JSAutoTempValueRooter tvr(cx, 1, &outputValue);

    jsid id;
    jsuint i;
    for (i = 0; i < length; i++) {
        id = INT_TO_JSID(i);

        if (!OBJ_GET_PROPERTY(cx, obj, id, &outputValue))
            return JS_FALSE;

        if (!Str(cx, id, obj, scx, &outputValue))
            return JS_FALSE;

        if (outputValue == JSVAL_VOID) {
            if (!scx->callback(null_ucstr, JS_ARRAY_LENGTH(null_ucstr), scx->data))
                return JS_FALSE;
        }

        if (i < length - 1) {
            c = jschar(',');
            if (!scx->callback(&c, 1, scx->data))
                return JS_FALSE;
            if (!WriteIndent(cx, scx, scx->depth))
                return JS_FALSE;
        }
    }

    if (length != 0 && !WriteIndent(cx, scx, scx->depth - 1))
        return JS_FALSE;

    c = jschar(']');

    return scx->callback(&c, 1, scx->data);
}

static JSBool
CallReplacerFunction(JSContext *cx, jsid id, JSObject *holder, StringifyContext *scx, jsval *vp)
{
    if (scx->replacer && js_IsCallable(scx->replacer, cx)) {
        jsval vec[2] = {ID_TO_VALUE(id), *vp};
        if (!JS_CallFunctionValue(cx, holder, OBJECT_TO_JSVAL(scx->replacer), 2, vec, vp))
            return JS_FALSE;
    }

    return JS_TRUE;
}

static JSBool
Str(JSContext *cx, jsid id, JSObject *holder, StringifyContext *scx, jsval *vp, bool callReplacer)
{
    JS_CHECK_RECURSION(cx, return JS_FALSE);

    if (!OBJ_GET_PROPERTY(cx, holder, id, vp))
        return JS_FALSE;

    if (!JSVAL_IS_PRIMITIVE(*vp) && !js_TryJSON(cx, vp))
        return JS_FALSE;

    if (callReplacer && !CallReplacerFunction(cx, id, holder, scx, vp))
        return JS_FALSE;

    // catches string and number objects with no toJSON
    if (!JSVAL_IS_PRIMITIVE(*vp)) {
        JSClass *clasp = OBJ_GET_CLASS(cx, JSVAL_TO_OBJECT(*vp));
        if (clasp == &js_StringClass || clasp == &js_NumberClass)
            *vp = JSVAL_TO_OBJECT(*vp)->fslots[JSSLOT_PRIVATE];
    }

    if (JSVAL_IS_STRING(*vp)) {
        JSString *s = JSVAL_TO_STRING(*vp);
        return write_string(cx, scx->callback, scx->data, JS_GetStringChars(s), JS_GetStringLength(s));
    }

    if (JSVAL_IS_NULL(*vp)) {
        return scx->callback(null_ucstr, JS_ARRAY_LENGTH(null_ucstr), scx->data);
    }

    if (JSVAL_IS_BOOLEAN(*vp)) {
        uint32 len = JS_ARRAY_LENGTH(true_ucstr);
        const jschar *chars = true_ucstr;
        JSBool b = JSVAL_TO_BOOLEAN(*vp);

        if (!b) {
            chars = false_ucstr;
            len = JS_ARRAY_LENGTH(false_ucstr);
        }

        return scx->callback(chars, len, scx->data);
    }

    if (JSVAL_IS_NUMBER(*vp)) {
        if (JSVAL_IS_DOUBLE(*vp)) {
            jsdouble d = *JSVAL_TO_DOUBLE(*vp);
            if (!JSDOUBLE_IS_FINITE(d))
                return  scx->callback(null_ucstr, JS_ARRAY_LENGTH(null_ucstr), scx->data);
        }

        char numBuf[DTOSTR_STANDARD_BUFFER_SIZE], *numStr;
        jsdouble d = JSVAL_IS_INT(*vp) ? jsdouble(JSVAL_TO_INT(*vp)) : *JSVAL_TO_DOUBLE(*vp);
        numStr = JS_dtostr(numBuf, sizeof numBuf, DTOSTR_STANDARD, 0, d);        
        if (!numStr) {
            JS_ReportOutOfMemory(cx);
            return JS_FALSE;
        }

        jschar dstr[DTOSTR_STANDARD_BUFFER_SIZE];
        size_t dbufSize = DTOSTR_STANDARD_BUFFER_SIZE;
        if (!js_InflateStringToBuffer(cx, numStr, strlen(numStr), dstr, &dbufSize))
            return JS_FALSE;

        return scx->callback(dstr, dbufSize, scx->data);
    }

    if (JSVAL_IS_OBJECT(*vp) && !VALUE_IS_FUNCTION(cx, *vp) && !VALUE_IS_XML(cx, *vp)) {
        JSBool ok;

        scx->depth++;
        ok = (JS_IsArrayObject(cx, JSVAL_TO_OBJECT(*vp)) ? JA : JO)(cx, vp, scx);
        scx->depth--;

        return ok;
    }
    
    *vp = JSVAL_VOID;
    return JS_TRUE;
}

static JSBool
WriteStringGap(JSContext *cx, jsval space, JSStringBuffer *sb)
{
    JSString *s = js_ValueToString(cx, space);
    if (!s)
        return JS_FALSE;

    js_AppendUCString(sb, JS_GetStringChars(s), JS_GetStringLength(s));
    if (!STRING_BUFFER_OK(sb)) {
        JS_ReportOutOfMemory(cx);
        return JS_FALSE;
    }

    return JS_TRUE;
}

static JSBool
InitializeGap(JSContext *cx, jsval space, JSStringBuffer *sb)
{
    if (!JSVAL_IS_PRIMITIVE(space)) {
        JSClass *clasp = OBJ_GET_CLASS(cx, JSVAL_TO_OBJECT(space));
        if (clasp == &js_StringClass || clasp == &js_NumberClass)
            return WriteStringGap(cx, space, sb);
    }

    if (JSVAL_IS_STRING(space))
        return WriteStringGap(cx, space, sb);

    if (JSVAL_IS_NUMBER(space)) {
        uint32 i;
        if (!JS_ValueToECMAUint32(cx, space, &i))
            return JS_FALSE;

        js_RepeatChar(sb, jschar(' '), i);

        if (!STRING_BUFFER_OK(sb)) {
            JS_ReportOutOfMemory(cx);
            return JS_FALSE;
        }
    }

    return JS_TRUE;
}

JSBool
js_Stringify(JSContext *cx, jsval *vp, JSObject *replacer, jsval space,
             JSONWriteCallback callback, void *data)
{
    // XXX stack
    JSObject *stack = JS_NewArrayObject(cx, 0, NULL);
    if (!stack)
        return JS_FALSE;

    StringifyContext scx(callback, replacer, data);
    if (!InitializeGap(cx, space, &scx.gap))
        return JS_FALSE;

    JSObject *obj = js_NewObject(cx, &js_ObjectClass, NULL, NULL, 0);
    if (!obj)
        return JS_FALSE;

    if (!OBJ_DEFINE_PROPERTY(cx, obj, ATOM_TO_JSID(cx->runtime->atomState.emptyAtom),
                             *vp, NULL, NULL, JSPROP_ENUMERATE, NULL)) {
        return JS_FALSE;
    }

    return Str(cx, ATOM_TO_JSID(cx->runtime->atomState.emptyAtom), obj, &scx, vp);
}

// helper to determine whether a character could be part of a number
static JSBool IsNumChar(jschar c)
{
    return ((c <= '9' && c >= '0') || c == '.' || c == '-' || c == '+' || c == 'e' || c == 'E');
}

static JSBool HandleData(JSContext *cx, JSONParser *jp, JSONDataType type);
static JSBool PopState(JSContext *cx, JSONParser *jp);

static JSBool
DestroyIdArrayOnError(JSContext *cx, JSIdArray *ida) {
    JS_DestroyIdArray(cx, ida);
    return JS_FALSE;
}

static JSBool
Walk(JSContext *cx, jsid id, JSObject *holder, jsval reviver, jsval *vp)
{
    JS_CHECK_RECURSION(cx, return JS_FALSE);
    
    if (!OBJ_GET_PROPERTY(cx, holder, id, vp))
        return JS_FALSE;

    JSObject *obj;

    if (!JSVAL_IS_PRIMITIVE(*vp) && !js_IsCallable(obj = JSVAL_TO_OBJECT(*vp), cx)) {
        jsval propValue = JSVAL_NULL;
        JSAutoTempValueRooter tvr(cx, 1, &propValue);
        
        if(OBJ_IS_ARRAY(cx, obj)) {
            jsuint length = 0;
            if (!js_GetLengthProperty(cx, obj, &length))
                return JS_FALSE;

            for (jsuint i = 0; i < length; i++) {
                jsid index;
                if (!js_IndexToId(cx, i, &index))
                    return JS_FALSE;

                if (!Walk(cx, index, obj, reviver, &propValue))
                    return JS_FALSE;

                if (!OBJ_DEFINE_PROPERTY(cx, obj, index, propValue,
                                         NULL, NULL, JSPROP_ENUMERATE, NULL)) {
                    return JS_FALSE;
                }
            }
        } else {
            JSIdArray *ida = JS_Enumerate(cx, obj);
            if (!ida)
                return JS_FALSE;

            JSAutoTempValueRooter idaroot(cx, JS_ARRAY_LENGTH(ida), (jsval*)ida);

            for(jsint i = 0; i < ida->length; i++) {
                jsid idName = ida->vector[i];
                if (!Walk(cx, idName, obj, reviver, &propValue))
                    return DestroyIdArrayOnError(cx, ida);
                if (propValue == JSVAL_VOID) {
                    if (!js_DeleteProperty(cx, obj, idName, &propValue))
                        return DestroyIdArrayOnError(cx, ida);
                } else {
                    if (!OBJ_DEFINE_PROPERTY(cx, obj, idName, propValue,
                                             NULL, NULL, JSPROP_ENUMERATE, NULL)) {
                        return DestroyIdArrayOnError(cx, ida);
                    }
                }
            }

            JS_DestroyIdArray(cx, ida);
        }
    }

    // return reviver.call(holder, key, value);
    jsval value = *vp;
    JSString *key = js_ValueToString(cx, ID_TO_VALUE(id));
    if (!key)
        return JS_FALSE;

    jsval vec[2] = {STRING_TO_JSVAL(key), value};
    jsval reviverResult;
    if (!JS_CallFunctionValue(cx, holder, reviver, 2, vec, &reviverResult))
        return JS_FALSE;

    *vp = reviverResult;

    return JS_TRUE;
}

static JSBool
Revive(JSContext *cx, jsval reviver, jsval *vp)
{
    
    JSObject *obj = js_NewObject(cx, &js_ObjectClass, NULL, NULL, 0);
    if (!obj)
        return JS_FALSE;

    jsval v = OBJECT_TO_JSVAL(obj);
    JSAutoTempValueRooter tvr(cx, 1, &v);
    if (!OBJ_DEFINE_PROPERTY(cx, obj, ATOM_TO_JSID(cx->runtime->atomState.emptyAtom),
                             *vp, NULL, NULL, JSPROP_ENUMERATE, NULL)) {
        return JS_FALSE;
    }

    return Walk(cx, ATOM_TO_JSID(cx->runtime->atomState.emptyAtom), obj, reviver, vp);
}

#define JSON_INITIAL_BUFSIZE    256

JSONParser *
js_BeginJSONParse(JSContext *cx, jsval *rootVal)
{
    if (!cx)
        return NULL;

    JSObject *arr = js_NewArrayObject(cx, 0, NULL);
    if (!arr)
        return NULL;

    JSONParser *jp = (JSONParser*) JS_malloc(cx, sizeof(JSONParser));
    if (!jp)
        return NULL;
    memset(jp, 0, sizeof *jp);

    jp->objectStack = arr;
    if (!js_AddRoot(cx, &jp->objectStack, "JSON parse stack"))
        goto bad;

    jp->statep = jp->stateStack;
    *jp->statep = JSON_PARSE_STATE_INIT;
    jp->rootVal = rootVal;

    js_InitStringBuffer(&jp->objectKey);
    js_InitStringBuffer(&jp->buffer);

    if (!jp->buffer.grow(&jp->buffer, JSON_INITIAL_BUFSIZE)) {
        JS_ReportOutOfMemory(cx);
        goto bad;
    }
    return jp;

bad:
    js_FinishJSONParse(cx, jp, JSVAL_NULL);
    return NULL;
}

JSBool
js_FinishJSONParse(JSContext *cx, JSONParser *jp, jsval reviver)
{
    if (!jp)
        return JS_TRUE;

    JSBool early_ok = JS_TRUE;

    // Check for unprocessed primitives at the root. This doesn't happen for
    // strings because a closing quote triggers value processing.
    if ((jp->statep - jp->stateStack) == 1) {
        if (*jp->statep == JSON_PARSE_STATE_KEYWORD) {
            early_ok = HandleData(cx, jp, JSON_DATA_KEYWORD);
            if (early_ok)
                PopState(cx, jp);
        } else if (*jp->statep == JSON_PARSE_STATE_NUMBER) {
            early_ok = HandleData(cx, jp, JSON_DATA_NUMBER);
            if (early_ok)
                PopState(cx, jp);
        }
    }

    js_FinishStringBuffer(&jp->objectKey);
    js_FinishStringBuffer(&jp->buffer);

    /* This internal API is infallible, in spite of its JSBool return type. */
    js_RemoveRoot(cx->runtime, &jp->objectStack);

    JSBool ok = *jp->statep == JSON_PARSE_STATE_FINISHED;
    jsval *vp = jp->rootVal;
    JS_free(cx, jp);

    if (!early_ok)
        return JS_FALSE;

    if (!ok) {
        JS_ReportErrorNumber(cx, js_GetErrorMessage, NULL, JSMSG_JSON_BAD_PARSE);
        return JS_FALSE;
    }

    if (!JSVAL_IS_PRIMITIVE(reviver) && js_IsCallable(JSVAL_TO_OBJECT(reviver), cx))
        ok = Revive(cx, reviver, vp);

    return ok;
}

static JSBool
PushState(JSContext *cx, JSONParser *jp, JSONParserState state)
{
    if (*jp->statep == JSON_PARSE_STATE_FINISHED) {
        // extra input
        JS_ReportErrorNumber(cx, js_GetErrorMessage, NULL, JSMSG_JSON_BAD_PARSE);
        return JS_FALSE; 
    }

    jp->statep++;
    if ((uint32)(jp->statep - jp->stateStack) >= JS_ARRAY_LENGTH(jp->stateStack)) {
        // too deep
        JS_ReportErrorNumber(cx, js_GetErrorMessage, NULL, JSMSG_JSON_BAD_PARSE);
        return JS_FALSE;
    }

    *jp->statep = state;

    return JS_TRUE;
}

static JSBool
PopState(JSContext *cx, JSONParser *jp)
{
    jp->statep--;
    if (jp->statep < jp->stateStack) {
        jp->statep = jp->stateStack;
        JS_ReportErrorNumber(cx, js_GetErrorMessage, NULL, JSMSG_JSON_BAD_PARSE);
        return JS_FALSE;
    }

    if (*jp->statep == JSON_PARSE_STATE_INIT)
        *jp->statep = JSON_PARSE_STATE_FINISHED;

    return JS_TRUE;
}

static JSBool
PushValue(JSContext *cx, JSONParser *jp, JSObject *parent, jsval value)
{
    JSBool ok;
    if (OBJ_IS_ARRAY(cx, parent)) {
        jsuint len;
        ok = js_GetLengthProperty(cx, parent, &len);
        if (ok) {
            jsid index;
            if (!js_IndexToId(cx, len, &index))
                return JS_FALSE;
            ok = OBJ_DEFINE_PROPERTY(cx, parent, index, value,
                                     NULL, NULL, JSPROP_ENUMERATE, NULL);
        }
    } else {
        ok = JS_DefineUCProperty(cx, parent, jp->objectKey.base,
                                 STRING_BUFFER_OFFSET(&jp->objectKey), value,
                                 NULL, NULL, JSPROP_ENUMERATE);
        js_RewindStringBuffer(&jp->objectKey);
    }

    return ok;
}

static JSBool
PushObject(JSContext *cx, JSONParser *jp, JSObject *obj)
{
    jsuint len;
    if (!js_GetLengthProperty(cx, jp->objectStack, &len))
        return JS_FALSE;
    if (len >= JSON_MAX_DEPTH) {
        JS_ReportErrorNumber(cx, js_GetErrorMessage, NULL, JSMSG_JSON_BAD_PARSE);
        return JS_FALSE;
    }

    jsval v = OBJECT_TO_JSVAL(obj);
    JSAutoTempValueRooter tvr(cx, v);

    // Check if this is the root object
    if (len == 0) {
        *jp->rootVal = v;
        // This property must be enumerable to keep the array dense
        if (!OBJ_DEFINE_PROPERTY(cx, jp->objectStack, INT_TO_JSID(0), *jp->rootVal,
                                 NULL, NULL, JSPROP_ENUMERATE, NULL)) {
            return JS_FALSE;
        }
        return JS_TRUE;
    }

    jsval p;
    if (!OBJ_GET_PROPERTY(cx, jp->objectStack, INT_TO_JSID(len - 1), &p))
        return JS_FALSE;

    JS_ASSERT(JSVAL_IS_OBJECT(p));
    JSObject *parent = JSVAL_TO_OBJECT(p);
    if (!PushValue(cx, jp, parent, v))
        return JS_FALSE;

    // This property must be enumerable to keep the array dense
    if (!OBJ_DEFINE_PROPERTY(cx, jp->objectStack, INT_TO_JSID(len), v,
                             NULL, NULL, JSPROP_ENUMERATE, NULL)) {
        return JS_FALSE;
    }

    return JS_TRUE;
}

static JSBool
OpenObject(JSContext *cx, JSONParser *jp)
{
    JSObject *obj = js_NewObject(cx, &js_ObjectClass, NULL, NULL, 0);
    if (!obj)
        return JS_FALSE;

    return PushObject(cx, jp, obj);
}

static JSBool
OpenArray(JSContext *cx, JSONParser *jp)
{
    // Add an array to an existing array or object
    JSObject *arr = js_NewArrayObject(cx, 0, NULL);
    if (!arr)
        return JS_FALSE;

    return PushObject(cx, jp, arr);
}

static JSBool
CloseObject(JSContext *cx, JSONParser *jp)
{
    jsuint len;
    if (!js_GetLengthProperty(cx, jp->objectStack, &len))
        return JS_FALSE;
    if (!js_SetLengthProperty(cx, jp->objectStack, len - 1))
        return JS_FALSE;

    return JS_TRUE;
}

static JSBool
CloseArray(JSContext *cx, JSONParser *jp)
{
    return CloseObject(cx, jp);
}

static JSBool
PushPrimitive(JSContext *cx, JSONParser *jp, jsval value)
{
    JSAutoTempValueRooter tvr(cx, 1, &value);

    jsuint len;
    if (!js_GetLengthProperty(cx, jp->objectStack, &len))
        return JS_FALSE;

    if (len > 0) {
        jsval o;
        if (!OBJ_GET_PROPERTY(cx, jp->objectStack, INT_TO_JSID(len - 1), &o))
            return JS_FALSE;

        JS_ASSERT(!JSVAL_IS_PRIMITIVE(o));
        return PushValue(cx, jp, JSVAL_TO_OBJECT(o), value);
    }

    // root value must be primitive
    *jp->rootVal = value;
    return JS_TRUE;
}

static JSBool
HandleNumber(JSContext *cx, JSONParser *jp, const jschar *buf, uint32 len)
{
    const jschar *ep;
    double val;
    if (!js_strtod(cx, buf, buf + len, &ep, &val))
        return JS_FALSE;
    if (ep != buf + len) {
        // bad number input
        JS_ReportErrorNumber(cx, js_GetErrorMessage, NULL, JSMSG_JSON_BAD_PARSE);
        return JS_FALSE;
    }

    jsval numVal;        
    if (!JS_NewNumberValue(cx, val, &numVal))
        return JS_FALSE;
        
    return PushPrimitive(cx, jp, numVal);
}

static JSBool
HandleString(JSContext *cx, JSONParser *jp, const jschar *buf, uint32 len)
{
    JSString *str = js_NewStringCopyN(cx, buf, len);
    if (!str)
        return JS_FALSE;

    return PushPrimitive(cx, jp, STRING_TO_JSVAL(str));
}

static JSBool
HandleKeyword(JSContext *cx, JSONParser *jp, const jschar *buf, uint32 len)
{
    jsval keyword;
    JSTokenType tt = js_CheckKeyword(buf, len);
    if (tt != TOK_PRIMARY) {
        // bad keyword
        JS_ReportErrorNumber(cx, js_GetErrorMessage, NULL, JSMSG_JSON_BAD_PARSE);
        return JS_FALSE;
    }

    if (buf[0] == 'n') {
        keyword = JSVAL_NULL;
    } else if (buf[0] == 't') {
        keyword = JSVAL_TRUE;
    } else if (buf[0] == 'f') {
        keyword = JSVAL_FALSE;
    } else {
        JS_ReportErrorNumber(cx, js_GetErrorMessage, NULL, JSMSG_JSON_BAD_PARSE);
        return JS_FALSE;
    }

    return PushPrimitive(cx, jp, keyword);
}

static JSBool
HandleData(JSContext *cx, JSONParser *jp, JSONDataType type)
{
    JSBool ok;

    switch (type) {
      case JSON_DATA_STRING:
        ok = HandleString(cx, jp, jp->buffer.base, STRING_BUFFER_OFFSET(&jp->buffer));
        break;

      case JSON_DATA_KEYSTRING:
        js_AppendUCString(&jp->objectKey, jp->buffer.base, STRING_BUFFER_OFFSET(&jp->buffer));
        ok = STRING_BUFFER_OK(&jp->objectKey);
        if (!ok)
            JS_ReportOutOfMemory(cx);
        break;

      case JSON_DATA_NUMBER:
        ok = HandleNumber(cx, jp, jp->buffer.base, STRING_BUFFER_OFFSET(&jp->buffer));
        break;

      default:
        JS_ASSERT(type == JSON_DATA_KEYWORD);
        ok = HandleKeyword(cx, jp, jp->buffer.base, STRING_BUFFER_OFFSET(&jp->buffer));
        break;
    }

    if (ok) {
        ok = STRING_BUFFER_OK(&jp->buffer);
        if (ok)
            js_RewindStringBuffer(&jp->buffer);
        else
            JS_ReportOutOfMemory(cx);
    }
    return ok;
}

JSBool
js_ConsumeJSONText(JSContext *cx, JSONParser *jp, const jschar *data, uint32 len)
{
    uint32 i;

    if (*jp->statep == JSON_PARSE_STATE_INIT) {
        PushState(cx, jp, JSON_PARSE_STATE_VALUE);
    }

    for (i = 0; i < len; i++) {
        jschar c = data[i];
        switch (*jp->statep) {
          case JSON_PARSE_STATE_VALUE:
            if (c == ']') {
                // empty array
                if (!PopState(cx, jp))
                    return JS_FALSE;

                if (*jp->statep != JSON_PARSE_STATE_ARRAY) {
                    JS_ReportErrorNumber(cx, js_GetErrorMessage, NULL, JSMSG_JSON_BAD_PARSE);
                    return JS_FALSE;
                }

                if (!CloseArray(cx, jp) || !PopState(cx, jp))
                    return JS_FALSE;

                break;
            }

            if (c == '}') {
                // we should only find these in OBJECT_KEY state
                JS_ReportErrorNumber(cx, js_GetErrorMessage, NULL, JSMSG_JSON_BAD_PARSE);
                return JS_FALSE;
            }

            if (c == '"') {
                *jp->statep = JSON_PARSE_STATE_STRING;
                break;
            }

            if (IsNumChar(c)) {
                *jp->statep = JSON_PARSE_STATE_NUMBER;
                js_FastAppendChar(&jp->buffer, c);
                break;
            }

            if (JS7_ISLET(c)) {
                *jp->statep = JSON_PARSE_STATE_KEYWORD;
                js_FastAppendChar(&jp->buffer, c);
                break;
            }

          // fall through in case the value is an object or array
          case JSON_PARSE_STATE_OBJECT_VALUE:
            if (c == '{') {
                *jp->statep = JSON_PARSE_STATE_OBJECT;
                if (!OpenObject(cx, jp) || !PushState(cx, jp, JSON_PARSE_STATE_OBJECT_PAIR))
                    return JS_FALSE;
            } else if (c == '[') {
                *jp->statep = JSON_PARSE_STATE_ARRAY;
                if (!OpenArray(cx, jp) || !PushState(cx, jp, JSON_PARSE_STATE_VALUE))
                    return JS_FALSE;
            } else if (!JS_ISXMLSPACE(c)) {
                JS_ReportErrorNumber(cx, js_GetErrorMessage, NULL, JSMSG_JSON_BAD_PARSE);
                return JS_FALSE;
            }
            break;

          case JSON_PARSE_STATE_OBJECT:
            if (c == '}') {
                if (!CloseObject(cx, jp) || !PopState(cx, jp))
                    return JS_FALSE;
            } else if (c == ',') {
                if (!PushState(cx, jp, JSON_PARSE_STATE_OBJECT_PAIR))
                    return JS_FALSE;
            } else if (c == ']' || !JS_ISXMLSPACE(c)) {
                JS_ReportErrorNumber(cx, js_GetErrorMessage, NULL, JSMSG_JSON_BAD_PARSE);
                return JS_FALSE;
            }
            break;

          case JSON_PARSE_STATE_ARRAY :
            if (c == ']') {
                if (!CloseArray(cx, jp) || !PopState(cx, jp))
                    return JS_FALSE;
            } else if (c == ',') {
                if (!PushState(cx, jp, JSON_PARSE_STATE_VALUE))
                    return JS_FALSE;
            } else if (!JS_ISXMLSPACE(c)) {
                JS_ReportErrorNumber(cx, js_GetErrorMessage, NULL, JSMSG_JSON_BAD_PARSE);
                return JS_FALSE;
            }
            break;

          case JSON_PARSE_STATE_OBJECT_PAIR :
            if (c == '"') {
                // we want to be waiting for a : when the string has been read
                *jp->statep = JSON_PARSE_STATE_OBJECT_IN_PAIR;
                if (!PushState(cx, jp, JSON_PARSE_STATE_STRING))
                    return JS_FALSE;
            } else if (c == '}') {
                // pop off the object pair state and the object state
                if (!CloseObject(cx, jp) || !PopState(cx, jp) || !PopState(cx, jp))
                    return JS_FALSE;
            } else if (c == ']' || !JS_ISXMLSPACE(c)) {
              JS_ReportErrorNumber(cx, js_GetErrorMessage, NULL, JSMSG_JSON_BAD_PARSE);
              return JS_FALSE;
            }
            break;

          case JSON_PARSE_STATE_OBJECT_IN_PAIR:
            if (c == ':') {
                *jp->statep = JSON_PARSE_STATE_VALUE;
            } else if (!JS_ISXMLSPACE(c)) {
                JS_ReportErrorNumber(cx, js_GetErrorMessage, NULL, JSMSG_JSON_BAD_PARSE);
                return JS_FALSE;
            }
            break;

          case JSON_PARSE_STATE_STRING:
            if (c == '"') {
                if (!PopState(cx, jp))
                    return JS_FALSE;
                JSONDataType jdt;
                if (*jp->statep == JSON_PARSE_STATE_OBJECT_IN_PAIR) {
                    jdt = JSON_DATA_KEYSTRING;
                } else {
                    jdt = JSON_DATA_STRING;
                }
                if (!HandleData(cx, jp, jdt))
                    return JS_FALSE;
            } else if (c == '\\') {
                *jp->statep = JSON_PARSE_STATE_STRING_ESCAPE;
            } else {
                js_FastAppendChar(&jp->buffer, c);
            }
            break;

          case JSON_PARSE_STATE_STRING_ESCAPE:
            switch (c) {
              case '"':
              case '\\':
              case '/':
                break;
              case 'b' : c = '\b'; break;
              case 'f' : c = '\f'; break;
              case 'n' : c = '\n'; break;
              case 'r' : c = '\r'; break;
              case 't' : c = '\t'; break;
              default :
                if (c == 'u') {
                    jp->numHex = 0;
                    jp->hexChar = 0;
                    *jp->statep = JSON_PARSE_STATE_STRING_HEX;
                    continue;
                } else {
                    JS_ReportErrorNumber(cx, js_GetErrorMessage, NULL, JSMSG_JSON_BAD_PARSE);
                    return JS_FALSE;
                }
            }

            js_FastAppendChar(&jp->buffer, c);
            *jp->statep = JSON_PARSE_STATE_STRING;
            break;

          case JSON_PARSE_STATE_STRING_HEX:
            if (('0' <= c) && (c <= '9')) {
                jp->hexChar = (jp->hexChar << 4) | (c - '0');
            } else if (('a' <= c) && (c <= 'f')) {
                jp->hexChar = (jp->hexChar << 4) | (c - 'a' + 0x0a);
            } else if (('A' <= c) && (c <= 'F')) {
                jp->hexChar = (jp->hexChar << 4) | (c - 'A' + 0x0a);
            } else {
                JS_ReportErrorNumber(cx, js_GetErrorMessage, NULL, JSMSG_JSON_BAD_PARSE);
                return JS_FALSE;
            }
            
            if (++(jp->numHex) == 4) {
                js_FastAppendChar(&jp->buffer, jp->hexChar);
                jp->hexChar = 0;
                jp->numHex = 0;
                *jp->statep = JSON_PARSE_STATE_STRING;
            }
            break;

          case JSON_PARSE_STATE_KEYWORD:
            if (JS7_ISLET(c)) {
                js_FastAppendChar(&jp->buffer, c);
            } else {
                // this character isn't part of the keyword, process it again
                i--;
                if (!PopState(cx, jp))
                    return JS_FALSE;
            
                if (!HandleData(cx, jp, JSON_DATA_KEYWORD))
                    return JS_FALSE;
            }
            break;

          case JSON_PARSE_STATE_NUMBER:
            if (IsNumChar(c)) {
                js_FastAppendChar(&jp->buffer, c);
            } else {
                // this character isn't part of the number, process it again
                i--;
                if (!PopState(cx, jp))
                    return JS_FALSE;
                if (!HandleData(cx, jp, JSON_DATA_NUMBER))
                    return JS_FALSE;
            }
            break;

          case JSON_PARSE_STATE_FINISHED:
            if (!JS_ISXMLSPACE(c)) {
                // extra input
                JS_ReportErrorNumber(cx, js_GetErrorMessage, NULL, JSMSG_JSON_BAD_PARSE);
                return JS_FALSE;
            }
            break;

          default:
            JS_NOT_REACHED("Invalid JSON parser state");
        }

        if (!STRING_BUFFER_OK(&jp->buffer)) {
            JS_ReportOutOfMemory(cx);
            return JS_FALSE;
        }
    }

    *jp->buffer.ptr = 0;
    return JS_TRUE;
}

#if JS_HAS_TOSOURCE
static JSBool
json_toSource(JSContext *cx, uintN argc, jsval *vp)
{
    *vp = ATOM_KEY(CLASS_ATOM(cx, JSON));
    return JS_TRUE;
}
#endif

static JSFunctionSpec json_static_methods[] = {
#if JS_HAS_TOSOURCE
    JS_FN(js_toSource_str,  json_toSource,      0, 0),
#endif
    JS_FN("parse",          js_json_parse,      1, 0),
    JS_FN("stringify",      js_json_stringify,  1, 0),
    JS_FS_END
};

JSObject *
js_InitJSONClass(JSContext *cx, JSObject *obj)
{
    JSObject *JSON;

    JSON = JS_NewObject(cx, &js_JSONClass, NULL, obj);
    if (!JSON)
        return NULL;
    if (!JS_DefineProperty(cx, obj, js_JSON_str, OBJECT_TO_JSVAL(JSON),
                           JS_PropertyStub, JS_PropertyStub, 0))
        return NULL;

    if (!JS_DefineFunctions(cx, JSON, json_static_methods))
        return NULL;

    return JSON;
}
