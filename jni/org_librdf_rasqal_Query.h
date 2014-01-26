/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_librdf_rasqal_Query */

#ifndef _Included_org_librdf_rasqal_Query
#define _Included_org_librdf_rasqal_Query
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_librdf_rasqal_Query
 * Method:    newQuery
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_org_librdf_rasqal_Query_newQuery
  (JNIEnv *, jclass, jlong);

/*
 * Class:     org_librdf_rasqal_Query
 * Method:    getMessages
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_org_librdf_rasqal_Query_getMessages
  (JNIEnv *, jclass);

/*
 * Class:     org_librdf_rasqal_Query
 * Method:    freeQuery
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_librdf_rasqal_Query_freeQuery
  (JNIEnv *, jobject);

/*
 * Class:     org_librdf_rasqal_Query
 * Method:    parseQuery
 * Signature: ([B)I
 */
JNIEXPORT jint JNICALL Java_org_librdf_rasqal_Query_parseQuery
  (JNIEnv *, jobject, jbyteArray);

/*
 * Class:     org_librdf_rasqal_Query
 * Method:    getQueryType
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_librdf_rasqal_Query_getQueryType
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
