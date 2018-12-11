#include "oboesound.hpp"
#include "../sound/soundpool.hpp"
#include "../utility/var.hpp"

OBOESOUND_METHOD(void, pause__) (JNIEnv* env, jobject self) {

}

OBOESOUND_METHOD(void, pause__J) (JNIEnv* env, jobject self, jlong id) {

}

OBOESOUND_METHOD(jlong, play__) (JNIEnv* env, jobject self) {
    return get_var_as<soundpool>(env, self, "soundpool")->play();
}

OBOESOUND_METHOD(jlong, play__F) (JNIEnv* env, jobject self, jfloat volume) {
    return 0;
}

OBOESOUND_METHOD(void, stop__) (JNIEnv* env, jobject self) {

}

OBOESOUND_METHOD(void, stop__J) (JNIEnv* env, jobject self, jlong id) {

}

OBOESOUND_METHOD(void, resume__) (JNIEnv* env, jobject self) {

}

OBOESOUND_METHOD(void, resume__J) (JNIEnv* env, jobject self, jlong id) {

}

OBOESOUND_METHOD(void, setVolume) (JNIEnv* env, jobject self, jlong id, jfloat volume) {

}

OBOESOUND_METHOD(void, dispose) (JNIEnv* env, jobject self) {

}
