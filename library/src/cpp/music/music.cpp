#include "music.hpp"
#include "../utility/log.hpp"
#include <iterator>
#include <cmath>

music::music(std::unique_ptr<audio_decoder> &&decoder, int8_t channels)
    : m_pan(0)
    , m_looping(false)
    , m_cache_size(16 * 1024 * channels)
    , m_volume(1)
    , m_channels(channels)
    , m_decoder(std::move(decoder))
    , m_current_frame(0)
    , m_buffer_swap(false)
    , m_executor([&]() { fill_second_buffer(); }) {
    m_main_pcm.reserve(m_cache_size);
    stop();
    m_disposed = false;
}

void music::fill_second_buffer() {
    if(m_disposed) return;

    m_decoder->decode(m_cache_size);
}

void music::swabuffers() {
    if(m_disposed) return;

    m_main_pcm.swap(m_decoder->m_buffer);
    m_eof = m_decoder->m_eof;
    m_current_frame = 0;
}

void music::play() {
    if(m_disposed) return;

    while(m_buffer_swap.test_and_set(std::memory_order_acquire));

    if(m_eof) stop();

    m_playing = true;

    m_buffer_swap.clear(std::memory_order_release);
}

void music::pause() {
    if(m_disposed) return;

    m_playing = false;
}

void music::stop() {
    if(m_disposed) return;

    while(m_buffer_swap.test_and_set(std::memory_order_acquire));

    m_playing = false;
    m_eof = false;
    position(0);

    m_buffer_swap.clear(std::memory_order_release);
}

void music::dispose() {
    if(m_disposed) return;

    m_disposed = true;
}

bool music::is_playing() {
    if(m_disposed) return false;

    return m_playing;
}

bool music::is_disposed() {
    return m_disposed;
}

bool music::is_rendering() {
    if(m_disposed) return false;

    return m_playing;
}

void music::position(float position) {
    if(m_disposed) return;

    while(m_buffer_swap.test_and_set(std::memory_order_acquire));
    m_executor.wait();
    m_decoder->seek(position);
    m_position = position;
    fill_second_buffer();
    swabuffers();
    m_executor.queue();
    m_buffer_swap.clear(std::memory_order_release);
}

float music::position() {
    if(m_disposed) return 0;

    return m_position;
}

void music::volume(float volume) {
    if(m_disposed) return;

    m_volume = std::min(std::max(0.0f, volume), 1.0f);
}

float music::volume() {
    if(m_disposed) return 1;

    return m_volume;
}

bool music::is_looping() {
    if(m_disposed) return false;

    return m_looping;
}

void music::is_looping(bool loop) {
    if(m_disposed) return;

    m_looping = loop;
}

void music::on_complete(std::function<void()> callback) {
    if(m_disposed) return;

    m_on_complete = callback;
}

void music::pan(float pan) {
    if(m_disposed) return;

    m_pan.pan(pan);
}

void music::raw_render(int16_t* stream, int32_t frames) {
    if(!m_playing) return;
    if(m_disposed) return;

    auto iter = std::next(m_main_pcm.begin(), m_current_frame * m_channels);
    int size = frames * m_channels;
    for(int sample = 0; sample < size; ++sample, std::advance(iter, 1)) {
        stream[sample] += *iter * m_volume * m_pan.modulation(sample % m_channels);
    }

    // TODO remove hard-coded stuff
    m_position += frames / 44100.0f;
    m_current_frame += frames;
}

void music::render(int16_t* stream, int32_t frames) {
    if(!m_playing) return;
    if(m_disposed) return;

    while(m_buffer_swap.test_and_set(std::memory_order_acquire));

    int32_t frames_in_pcm = m_main_pcm.size() / m_channels;
    int32_t frames_to_process = std::min(frames, frames_in_pcm - m_current_frame);

    raw_render(stream, frames_to_process);
    m_buffer_swap.clear(std::memory_order_release);

    if (frames_to_process < frames) {
        bool end = m_eof && m_current_frame >= frames_in_pcm;
        if (end) {
            m_playing = m_looping;
            m_position = 0;
            if (m_on_complete && !m_looping) m_on_complete();
        }

        // wait for buffer in case there was no position reset
        m_executor.wait();
        swabuffers();
        if(m_playing) {
            if (m_looping && m_decoder->m_eof) {
                m_decoder->seek(0);
            }
            m_executor.queue();
        }

        // render additional pcm to fill full stream
        while(m_buffer_swap.test_and_set(std::memory_order_acquire));
        int16_t remain = frames - frames_to_process;
        raw_render(stream + frames_to_process * m_channels, remain);
        m_buffer_swap.clear(std::memory_order_release);
    }
}
