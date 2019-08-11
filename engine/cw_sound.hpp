/*
 * =====================================================================================
 *
 *         A simple game engine for ITGT533
 *
 *         Author:  Chatchai Wangwiwattana
 *
 * =====================================================================================
 */

#ifndef cw_sound_h
#define cw_sound_h

#define DR_FLAC_IMPLEMENTATION
#include "extras/dr_flac.h"  /* Enables FLAC decoding. */
#define DR_MP3_IMPLEMENTATION
#include "extras/dr_mp3.h"   /* Enables MP3 decoding. */
#define DR_WAV_IMPLEMENTATION
#include "extras/dr_wav.h"   /* Enables WAV decoding. */

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"


namespace cw
{
    void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
    {
        ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
        if (pDecoder == nullptr) {
            return;
        }
        
        ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount);
        
        (void)pInput;
    }
    
    void sound_stop_callback(ma_device* pDevice)
    {
        std::cout << "The Audio has stopped." << std::endl;
    }
    
    
    class Sound final
    {
    public:
        
        ma_result result;
        ma_decoder decoder;
        ma_device_config deviceConfig;
        ma_device device;
        
        Sound( const char* filename)
        {

            result = ma_decoder_init_file(filename, nullptr, &decoder);
            if (result != MA_SUCCESS) {
                return ;
            }
            
            deviceConfig = ma_device_config_init(ma_device_type_playback);
            deviceConfig.playback.format   = decoder.outputFormat;
            deviceConfig.playback.channels = decoder.outputChannels;
            deviceConfig.sampleRate        = decoder.outputSampleRate;
            deviceConfig.dataCallback      = data_callback;
            deviceConfig.pUserData         = &decoder;
            
            if (ma_device_init(nullptr, &deviceConfig, &device) != MA_SUCCESS) {
                printf("Failed to open playback device.\n");
                ma_decoder_uninit(&decoder);
                return ;
            }
            
            ma_device_set_stop_callback(&device, sound_stop_callback);
            
        }
        
        ~Sound()
        {
            releaseDevice();
        }
        
        void releaseDevice()
        {
            
            ma_device_uninit(&device);
            ma_decoder_uninit(&decoder);
        }
        
        void play()
        {
            
            if(!ma_device_is_started(&device))
            {
                if (ma_device_start(&device) != MA_SUCCESS) {
                    printf("Failed to start playback device.\n");
                    releaseDevice();

                }
            }

        }
        
        void stop()
        {
            if(ma_device_is_started(&device))
            {
                ma_decoder_seek_to_pcm_frame(&decoder, 0);
                if (ma_device_stop(&device) != MA_SUCCESS) {
                    printf("Failed to stop playback device.\n");
                    releaseDevice();
                }
            }
        }
        
        void pause()
        {
            if(ma_device_is_started(&device))
            {
                if (ma_device_stop(&device) != MA_SUCCESS) {
                    printf("Failed to pause playback device.\n");
                    releaseDevice();
                }
            }
        }
    };
}

#endif /* cw_sound_h */
