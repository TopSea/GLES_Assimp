#include <jni.h>
#include <string>
#include "gl_running_model.h"

extern "C"
JNIEXPORT void JNICALL
Java_top_topsea_gles_1assimp_vws_ModelRenderer_surfaceChangedNative(JNIEnv *env, jobject thiz,
                                                                    jint width, jint height) {
    setViewPortSize(width, height);
}
extern "C"
JNIEXPORT void JNICALL
Java_top_topsea_gles_1assimp_vws_ModelRenderer_surfaceCreatedNative(JNIEnv *env, jobject thiz) {
    initGL();
}
extern "C"
JNIEXPORT void JNICALL
Java_top_topsea_gles_1assimp_vws_ModelRenderer_drawFrameNative(JNIEnv *env, jobject thiz) {
    draw();
}
extern "C"
JNIEXPORT void JNICALL
Java_top_topsea_gles_1assimp_MainActivity_createObjectNative(JNIEnv *env, jobject thiz,
                                                             jobject asset_manager,
                                                             jstring path_to_internal_dir) {
    // TODO: implement createObjectNative()
}
extern "C"
JNIEXPORT void JNICALL
Java_top_topsea_gles_1assimp_MainActivity_deleteObjectNative(JNIEnv *env, jobject thiz) {
    // TODO: implement deleteObjectNative()
}