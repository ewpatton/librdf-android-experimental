/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_librdf_rasqal_RasqalWorld */

#ifndef _Included_org_librdf_rasqal_RasqalWorld
#define _Included_org_librdf_rasqal_RasqalWorld
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_librdf_rasqal_RasqalWorld
 * Method:    newWorld
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_org_librdf_rasqal_RasqalWorld_newWorld
  (JNIEnv *, jclass);

/*
 * Class:     org_librdf_rasqal_RasqalWorld
 * Method:    freeWorld
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_librdf_rasqal_RasqalWorld_freeWorld
  (JNIEnv *, jobject);

/*
 * Class:     org_librdf_rasqal_RasqalWorld
 * Method:    testLibCurl
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_librdf_rasqal_RasqalWorld_testLibCurl
  (JNIEnv *env, jclass clazz);

#ifdef __cplusplus
}
#endif
#endif
