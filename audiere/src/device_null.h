#ifndef DEVICE_NULL_HPP
#define DEVICE_NULL_HPP


#include <set>
#include "audiere.h"
#include "internal.h"
#include "repeatable.h"
#include "threads.h"
#include "types.h"


namespace audiere {

  class NullOutputStream;

  class NullAudioDevice
    : public RefCountedImplementation<AudioDevice>
    , public Synchronized
  {
  public:
    NullAudioDevice();
    ~NullAudioDevice();

    bool initialize(const char* parameters);
    bool supportsStreaming();
    void update();
    OutputStream* openStream(SampleSource* source);

  private:
    void removeStream(NullOutputStream* stream);

    typedef std::set<NullOutputStream*> StreamList;
    StreamList m_streams;

    friend class NullOutputStream;
  };

  class NullOutputStream : public DLLImplementation<OutputStream> {
  private:
    NullOutputStream(NullAudioDevice* device, SampleSource* source);
    ~NullOutputStream();

  public:
    void play();
    void stop();
    void reset();
    bool isPlaying();

    void setRepeat(bool repeat);
    bool getRepeat();

    void  setVolume(float volume);
    float getVolume();

    void  setPan(float pan);
    float getPan();

    bool isSeekable();
    int  getLength();
    void setPosition(int position);
    int  getPosition();

  private:
    void resetTimer();
    void update();
    int dummyRead(int samples_to_read);

    NullAudioDevice* m_device;
                          
    RepeatableStream* m_source;
    int m_channel_count;           //
    int m_sample_rate;             // cached stream format
    SampleFormat m_sample_format;  //

    bool m_is_playing;
    float m_volume;
    float m_pan;

    u64 m_last_update;

    friend class NullAudioDevice;
  };

}


#endif
