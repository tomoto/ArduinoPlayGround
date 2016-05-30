#include "PopsParserBase.h"
#include "StreamProcessUtil.h"

using namespace tomoto;

template<int N>
int getAndParsePops(PopsParserBase<N>& parser, const char* uri, TinyVector<int>& result)
{
  WiFiHTTPClient httpClient;
  
  int status = httpClient.get(uri, [&](int c){
    parser.next(c);
    
    if (parser.found()) {
      result.resize(parser.numberOfPops());
      for (int i = 0; i < parser.numberOfPops(); i++) {
        result[i] = parser.pop(i);
        Serial.printf("%d ", parser.pop(i));
      }
      Serial.println();
      return false;
    }
    return true;
  });

  return status;
}

int getShortForecast(const char* uri, TinyVector<int>& result)
{
  class ShortForecastParser : public PopsParserBase<8>
  {
  private:
    StringFinder m_markerFinder;
    IntParser m_intParser;
  
  public:
    ShortForecastParser() : m_markerFinder("class=\"rain\"") {}
  
  protected:
    virtual void nextImpl(char c)
    {
      if (!m_markerFinder.found()) {
        m_markerFinder.next(c);
      } else {
        m_intParser.next(c);
        if (c == '%') addPop(m_intParser.found() ? m_intParser.value() : -1);
      }
    }
  } parser;
  
  return getAndParsePops(parser, uri, result);
}

int getWeeklyForecast(const char* uri, TinyVector<int>& result)
{
  class WeeklyForecastParser : public PopsParserBase<10>
  {
  private:
    StringFinder m_markerFinder;
    IntParser m_intParser;
  
  public:
    WeeklyForecastParser() : m_markerFinder("class=\"pop\"") {}
  
  protected:
    virtual void nextImpl(char c)
    {
      if (!m_markerFinder.found()) {
        m_markerFinder.next(c);
      } else {
        m_intParser.next(c);
        if (m_intParser.found()) addPop(m_intParser.value());
      }
    }
  } parser;
  
  return getAndParsePops(parser, uri, result);
}

