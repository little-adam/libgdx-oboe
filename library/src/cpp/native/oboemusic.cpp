#include "oboemusic.hpp"
#include "../audio/audioengine.hpp"
#include "../music/music.hpp"
#include "../utility/var.hpp"
#include "../jni/jvm_class.hpp"

OBOEMUSIC_METHOD(void, setOnCompletionListener) (JNIEnv* env, jobject self, jobject callback) {
    if (auto old_callback = get_var_as<_jobject>(env, self, "listener")) {
        // remove global reference
        env->DeleteGlobalRef(old_callback);
        old_callback = nullptr;
    }

    if (auto instance = get_var_as<std::shared_ptr<music>>(env, self, "music")) {
        if (callback) {
            // add global reference
            auto new_callback = env->NewGlobalRef(callback);
            auto self_weak = env->NewWeakGlobalRef(self);

            // set listener field
            set_var_as(env, self, "listener", new_callback);

            // set completion listener
            (*instance)->on_complete([self_weak, new_callback] {
                // get thread context
                auto context = jni_context::acquire_thread();

                // call callback.onCompletion()
                auto callback_class = jvm_class(context->GetObjectClass(new_callback));
                callback_class.execute_method<void(Music)>(new_callback, "onCompletion", Music{self_weak});
            });
        }
    }
}

OBOEMUSIC_METHOD(bool, isPlaying) (JNIEnv* env, jobject self) {
    if(auto instance = get_var_as<std::shared_ptr<music>>(env, self, "music")) {
        return (*instance)->is_playing();
    }
    return false;
}

OBOEMUSIC_METHOD(bool, isLooping) (JNIEnv* env, jobject self) {
    if(auto instance = get_var_as<std::shared_ptr<music>>(env, self, "music")) {
        return (*instance)->is_looping();
    }
    return false;
}

OBOEMUSIC_METHOD(void, pause) (JNIEnv* env, jobject self) {
    if(auto instance = get_var_as<std::shared_ptr<music>>(env, self, "music")) {
        (*instance)->pause();
    }
}

OBOEMUSIC_METHOD(void, setPan) (JNIEnv* env, jobject self, jfloat pan, jfloat volume) {
    if(auto instance = get_var_as<std::shared_ptr<music>>(env, self, "music")) {
        (*instance)->pan(pan);
        (*instance)->volume(volume);
    }
}

OBOEMUSIC_METHOD(jfloat, getPosition) (JNIEnv* env, jobject self) {
    if(auto instance = get_var_as<std::shared_ptr<music>>(env, self, "music")) {
        return (*instance)->position();
    }
    return 0;
}

OBOEMUSIC_METHOD(void, setLooping) (JNIEnv* env, jobject self, jboolean loop) {
    if(auto instance = get_var_as<std::shared_ptr<music>>(env, self, "music")) {
        (*instance)->is_looping(loop);
    }
}

OBOEMUSIC_METHOD(jfloat, getVolume) (JNIEnv* env, jobject self) {
    if(auto instance = get_var_as<std::shared_ptr<music>>(env, self, "music")) {
        return (*instance)->volume();
    }
    return 0;
}

OBOEMUSIC_METHOD(void, play) (JNIEnv* env, jobject self) {
    if(auto instance = get_var_as<std::shared_ptr<music>>(env, self, "music")) {
        (*instance)->play();
    }
}

OBOEMUSIC_METHOD(void, stop) (JNIEnv* env, jobject self) {
    if(auto instance = get_var_as<std::shared_ptr<music>>(env, self, "music")) {
        (*instance)->stop();
    }
}

OBOEMUSIC_METHOD(void, setVolume) (JNIEnv* env, jobject self, jfloat volume) {
    if(auto instance = get_var_as<std::shared_ptr<music>>(env, self, "music")) {
        (*instance)->volume(volume);
    }
}

OBOEMUSIC_METHOD(void, setPosition) (JNIEnv* env, jobject self, jfloat position) {
    if(auto instance = get_var_as<std::shared_ptr<music>>(env, self, "music")) {
        (*instance)->position(position);
    }
}

OBOEMUSIC_METHOD(void, dispose) (JNIEnv* env, jobject self) {
    // delete_var<std::shared_ptr<music>>(env, self, "music");
    if (auto instance = get_var_as<std::shared_ptr<music>>(env, self, "music")) {
        // clear completion listener to avoid callback after dispose
        (*instance)->on_complete(nullptr);

        // dispose music to avoid further rendering or modification
        (*instance)->dispose();

        // comment out to prevent gc of music object until OboeMusic is no longer referenced
        // set_var_as<std::shared_ptr<music>>(env, self, "music", nullptr);

        /*
         * Must use delete instance instead of instance->reset()
         */
        // delete native instance
        delete instance;
        instance = nullptr;
    }

    if (auto callback = get_var_as<_jobject>(env, self, "listener")) {
        /*
         * Delete global reference after completion listener is already set to nullptr to avoid usage of gc object
         */
        // remove global reference
        env->DeleteGlobalRef(callback);
        callback = nullptr;

        // comment out to prevent gc of listener object until OboeMusic is no longer referenced
        // set_var_as<_jobject>(env, self, "listener", nullptr);
    }
}

