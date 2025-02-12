package barsoosayque.libgdxoboe

import com.badlogic.gdx.audio.AudioRecorder

/** [AudioRecorder] implementation using the same oboe stream. */
class OboeAudioRecorder(private val audioEngine: NativeAudioEngine) : AudioRecorder {
    external override fun dispose()
    external override fun read(samples: ShortArray, offset: Int, numSamples: Int)
}