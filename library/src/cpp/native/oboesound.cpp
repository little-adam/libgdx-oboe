#include "oboesound.hpp"
#include "../sound/soundpool.hpp"
#include "../utility/var.hpp"

OBOESOUND_METHOD(jlong, play) (JNIEnv* env, jobject self, jfloat volume, jfloat pitch, jfloat pan) {
    return shared_ptr_var<soundpool>(env, self, "soundpool")->play(volume, pitch, pan, false);
}

OBOESOUND_METHOD(jlong, loop) (JNIEnv* env, jobject self, jfloat volume, jfloat pitch, jfloat pan) {
    return shared_ptr_var<soundpool>(env, self, "soundpool")->play(volume, pitch, pan, true);
}

OBOESOUND_METHOD(void, pause__) (JNIEnv* env, jobject self) {
    shared_ptr_var<soundpool>(env, self, "soundpool")->pause();
}

OBOESOUND_METHOD(void, pause__J) (JNIEnv* env, jobject self, jlong id) {
    shared_ptr_var<soundpool>(env, self, "soundpool")->pause(id);
}

OBOESOUND_METHOD(void, stop__) (JNIEnv* env, jobject self) {
    shared_ptr_var<soundpool>(env, self, "soundpool")->stop();
}

OBOESOUND_METHOD(void, stop__J) (JNIEnv* env, jobject self, jlong id) {
    shared_ptr_var<soundpool>(env, self, "soundpool")->stop(id);
}

OBOESOUND_METHOD(void, resume__) (JNIEnv* env, jobject self) {
    shared_ptr_var<soundpool>(env, self, "soundpool")->resume();
}

OBOESOUND_METHOD(void, resume__J) (JNIEnv* env, jobject self, jlong id) {
    shared_ptr_var<soundpool>(env, self, "soundpool")->resume(id);
}

OBOESOUND_METHOD(void, setLooping) (JNIEnv* env, jobject self, jlong id, bool loop) {
    shared_ptr_var<soundpool>(env, self, "soundpool")->looping(id, loop);
}

OBOESOUND_METHOD(void, setVolume) (JNIEnv* env, jobject self, jlong id, jfloat volume) {
    shared_ptr_var<soundpool>(env, self, "soundpool")->volume(id, volume);
}

OBOESOUND_METHOD(void, dispose) (JNIEnv* env, jobject self) {
    delete get_var_as<std::shared_ptr<soundpool>>(env, self, "soundpool");
}

OBOESOUND_METHOD(void, setPitch) (JNIEnv* env, jobject self, jlong id, jfloat pitch) {
    shared_ptr_var<soundpool>(env, self, "soundpool")->speed(id, pitch);
}

OBOESOUND_METHOD(void, setPan) (JNIEnv* env, jobject self, jlong id, jfloat pan, jfloat volume) {
    auto instance = shared_ptr_var<soundpool>(env, self, "soundpool");
    instance->pan(id, pan);
    instance->volume(id, volume);
}
