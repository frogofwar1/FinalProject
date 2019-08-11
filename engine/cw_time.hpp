/*
 * =====================================================================================
 *
 *         A simple game engine for ITGT533
 *
 *         Author:  Chatchai Wangwiwattana
 *
 * =====================================================================================
 */

#ifndef cw_time_h
#define cw_time_h

#include <chrono>

namespace cw
{
    
    using Milli = std::chrono::milliseconds;
    using TimePoint = std::chrono::high_resolution_clock::time_point;
    using Clock = std::chrono::high_resolution_clock;
    
    class CWTime final
    {
    private:

        TimePoint start;
        
    public:

        CWTime()
        {
            reset();
        }
        
        inline void reset()
        {
            start = Clock::now();
        }
        
        inline float deltaTimeSecond() const
        {
            auto end = Clock::now();
            return (std::chrono::duration_cast<Milli>(end - start).count()) / 1000.0f;
        }
        
    };
}
#endif /* cw_time_h */
