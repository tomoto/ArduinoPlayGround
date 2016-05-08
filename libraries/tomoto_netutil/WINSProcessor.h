#ifndef WINSProcessor_h
#define WINSProcessor_h

#include <WINSMessage.h>
#include <WINSMessageReader.h>

namespace tomoto {

class WINSProcessor {
private:
  bool m_verbose;
  WINSMessageReader m_reader;
  dns_header_t m_header;
  int m_questionIndex;
  question_t m_question;

public:
  WINSProcessor(bool verbose);
  void process(const uint8_t* message, size_t messageLength);
  
  const dns_header_t* h() { return &m_header; }
  bool moveToNextQuestion();
  const question_t* q() { return &m_question; }

private:
  void parseHeader();
  void parseQuestion();
};

}

#endif
