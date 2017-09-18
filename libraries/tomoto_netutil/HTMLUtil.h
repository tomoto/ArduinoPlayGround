#ifndef HTMLUtil_h
#define HTMLUtil_h

#include <WString.h>

namespace tomoto {

class HTMLUtil
{
public:
  template<typename T, int L>
  struct OptionsDef
  {
    T values[L];
    int selectedIndex;
  };
  
  template<typename T, int L>
  static String renderOptions(const OptionsDef<T, L>& optionsDef)
  {
    String result;
    
    for (int i = 0; i < L; i++) {
      const T& value = optionsDef.values[i];
      const char* selected = (i == optionsDef.selectedIndex) ? " selected" : "";
      result = result + "<option value='" + value + "'" + selected + ">" + value + "</option>";
    }
    
    return result;
  }
  
  static const char* renderChecked(bool value)
  {
    return value ? "checked='checked'" : "";
  }
};

}

#endif
