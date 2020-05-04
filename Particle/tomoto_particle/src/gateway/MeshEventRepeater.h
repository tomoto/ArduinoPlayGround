#ifndef MeshEventRepeater_h
#define MeshEventRepeater_h

class MeshEventRepeater {
public:
  void begin();
  void setAck(const char* ack); // caller needs to keep the buffer
  const char* getAck() const;
};

#endif
