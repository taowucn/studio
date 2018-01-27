/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "libJniCal native.cpp"
#include <utils/Log.h>

#include <stdio.h>

#include "jni.h"
#include "../libcal/Cal.h"

static jint
cal_service_add(JNIEnv *env, jobject thiz, jint a, jint b)
{
    android::Cal cal;
    int result = cal.Add(a, b);
    LOGI("%d + %d = %d", a, b, result);
    return result;
}
static jint
cal_service_sub(JNIEnv *env, jobject thiz, jint a, jint b)
{
    android::Cal cal;
    int result = cal.Sub(a, b);
    LOGI("%d - %d = %d", a, b, result);
    return result;
}
static jint
cal_service_mul(JNIEnv *env, jobject thiz, jint a, jint b)
{
    android::Cal cal;
    int result = cal.Mul(a, b);
    LOGI("%d * %d = %d", a, b, result);
    return result;
}
static jfloat
cal_service_div(JNIEnv *env, jobject thiz, jint a, jint b)
{
    android::Cal cal;
    float result = cal.Div(a, b);
    LOGI("%d / %d = %f", a, b, result);
    return result;
}

static const char *classPathName = "com/example/android/simpleservice/Native";/*应用程序中所对应类的地址*/

static JNINativeMethod methods[] = {
  {"add", "(II)I", (void*)cal_service_add },
  {"sub", "(II)I", (void*)cal_service_sub },
  {"mul", "(II)I", (void*)cal_service_mul },
  {"div", "(II)F", (void*)cal_service_div },
};

/*
 * Register several native methods for one class.
 */
static int registerNativeMethods(JNIEnv* env, const char* className,
    JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;

    clazz = env->FindClass(className);
    if (clazz == NULL) {
        LOGE("Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        LOGE("RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

/*
 * Register native methods for all classes we know about.
 *
 * returns JNI_TRUE on success.
 */
static int registerNatives(JNIEnv* env)
{
  if (!registerNativeMethods(env, classPathName,
                 methods, sizeof(methods) / sizeof(methods[0]))) {
    return JNI_FALSE;
  }

  return JNI_TRUE;
}


// ----------------------------------------------------------------------------

/*
 * This is called by the VM when the shared library is first loaded.
 */

typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    jint result = -1;
    JNIEnv* env = NULL;

    LOGI("JNI_OnLoad");

    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("ERROR: GetEnv failed");
        goto fail;
    }
    env = uenv.env;

    if (registerNatives(env) != JNI_TRUE) {
        LOGE("ERROR: registerNatives failed");
        goto fail;
    }
    LOGI("JNI_OnLoad Done");
    result = JNI_VERSION_1_4;

fail:
    return result;
}
