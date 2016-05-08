#include "BuildCop.h"
#include <HTMLUtil.h>

using namespace tomoto;

const HTMLUtil::OptionsDef<int, 4> durationOptions ={ { 5, 10, 30, 60 }, 1 };
const HTMLUtil::OptionsDef<int, 6> powerOptions = { { 100, 75, 50, 25, 10, 0 }, 0 };

String renderRootView()
{
  String html;
  
  forEachPin([&html](int i, int p) {
    html = html +
      "<h1>Light #" + i + "</h1>" +
      "<form action='/light/" + i + "/on'>" +
      "Turn on for <select name='duration'>" + HTMLUtil::renderOptions(durationOptions) + "</select>" +
      " seconds on <select name='power'>" + HTMLUtil::renderOptions(powerOptions) + "</select>% power" +
      "<input type='submit'>" +
      "</form>";
  });

  return html;
}


