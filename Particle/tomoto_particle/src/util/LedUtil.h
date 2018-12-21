#ifndef LedUtil_h
#define LedUtil_h

class LedUtil {
public:
    static void sendToCloudBegin();
    static bool sendToCloudEnd(bool result);
    
    static void sendToMeshBegin();
    static bool sendToMeshEnd(bool result);
};

#endif
