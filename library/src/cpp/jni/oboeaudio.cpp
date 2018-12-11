#include "oboeaudio.hpp"
#include "../utility/log.hpp"
#include "../audio/audioengine.hpp"
#include "../utility/var.hpp"

OBOEAUDIO_METHOD(void, init) (JNIEnv* env, jobject self, jobject asset_manager) {
    // set audioEngine in OboeAudio class
    AAssetManager* mgr = AAssetManager_fromJava(env, asset_manager);
    set_var_as(env, self, "audioEngine", new audio_engine(2, mgr));
}

OBOEAUDIO_METHOD(jlong, createSoundpool) (JNIEnv* env, jobject self, jstring path) {
    const char *cpp_path = env->GetStringUTFChars(path, NULL);
    auto soundpool = get_var_as<audio_engine>(env, self, "audioEngine")->new_soundpool(cpp_path);
    env->ReleaseStringUTFChars(path, cpp_path);
    return reinterpret_cast<jlong>(soundpool);
}

OBOEAUDIO_METHOD(void, dispose) (JNIEnv* env, jobject self) {
    delete get_var_as<audio_engine>(env, self, "audioEngine");
}

OBOEAUDIO_METHOD(void, resume) (JNIEnv* env, jobject self) {
    get_var_as<audio_engine>(env, self, "audioEngine")->resume();
}

OBOEAUDIO_METHOD(void, stop) (JNIEnv* env, jobject self) {
    get_var_as<audio_engine>(env, self, "audioEngine")->stop();
}