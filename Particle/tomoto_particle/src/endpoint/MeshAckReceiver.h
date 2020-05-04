#ifndef MeshAckReceiver_h
#define MeshAckReceiver_h

class MeshAckReceiver {
public:
  void begin(const char* body);
  bool isKeepAwake() const;
};

#endif
