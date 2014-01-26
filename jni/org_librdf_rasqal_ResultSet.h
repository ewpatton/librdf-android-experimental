/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_librdf_rasqal_ResultSet */

#ifndef _Included_org_librdf_rasqal_ResultSet
#define _Included_org_librdf_rasqal_ResultSet
#ifdef __cplusplus
extern "C" {
#endif
#undef org_librdf_rasqal_ResultSet_serialVersionUID
#define org_librdf_rasqal_ResultSet_serialVersionUID -2242979285438498493LL
/*
 * Class:     org_librdf_rasqal_ResultSet
 * Method:    isFinished
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_librdf_rasqal_ResultSet_isFinished
  (JNIEnv *, jobject);

/*
 * Class:     org_librdf_rasqal_ResultSet
 * Method:    nextBinding
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_librdf_rasqal_ResultSet_nextBinding
  (JNIEnv *, jobject);

/*
 * Class:     org_librdf_rasqal_ResultSet
 * Method:    getVariables
 * Signature: ()[Lorg/librdf/shared/Variable;
 */
JNIEXPORT jobjectArray JNICALL Java_org_librdf_rasqal_ResultSet_getVariables
  (JNIEnv *, jobject);

/*
 * Class:     org_librdf_rasqal_ResultSet
 * Method:    getTerms
 * Signature: ()[Lorg/librdf/shared/Term;
 */
JNIEXPORT jobjectArray JNICALL Java_org_librdf_rasqal_ResultSet_getTerms
  (JNIEnv *, jobject);

/*
 * Class:     org_librdf_rasqal_ResultSet
 * Method:    freeResultSet
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_librdf_rasqal_ResultSet_freeResultSet
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif