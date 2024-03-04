#ifndef HOT_TRACKER_H
#define HOT_TRACKER_H

#include <cstdint>

class HotnessTracker {
public:
    HotnessTracker(uint64_t tracking_rd, uint32_t hot_threshold){
        tracking_rd_ = tracking_rd;
        hot_threshold_ = hot_threshold;
    };
    ~HotnessTracker() = default;

    bool CheckHotess(uint64_t global_key) {return false;};
private:
    uint64_t tracking_rd_;
    uint32_t hot_threshold_;
};


#endif