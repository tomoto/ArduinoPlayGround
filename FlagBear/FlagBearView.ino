const char* handDisplayStrings[] = { "Right Hand", "Left Hand" };
const char* actionDisplayStrings[] = { "Up", "Wave", "Down" };
const char* speedDisplayStrings[] = { "(moderate)", "(slow)", "(fast)" };
const char* handURLStrings[] = { "right", "left" };
const char* actionURLStrings[] = { "up", "wave", "down" };
const char* speedURLStrings[] = { "moderate", "slow", "fast" };

String rootView()
{
  String html;
  
  html +=
    "<html>"
    "<head>"
    "<title>Kuma - Menu</title>"
    "</head>"
    "<body>";

  for (int i = 0; i < 2; i++) {
    html += "<h1>";
    html += handDisplayStrings[i];
    html += "</h1>";
    for (int a = 0; a < 3; a++) {
      html += "<font size='150%'>";
      html += "<a href='";
      html += getCommandURL(i, a, 0);
      html += "'>";
      html += actionDisplayStrings[a];
      html += " ";
      html += speedDisplayStrings[0];
      html += "</a>";
      html += "</font> ";
      for (int s = 1; s < 3; s++) {
        html += "<a href='";
        html += getCommandURL(i, a, s);
        html += "'>";
        html += speedDisplayStrings[s];
        html += "</a> ";
      }
      html += "<br>";
    }
  }
  
  html +=
    "</body>"
    "</html>";

  return html;
}

String responseView()
{
  String html;
  
  html +=
    "<html>"
    "<head>"
    "<title>Kuma - Response</title>"
    "</head>"
    "<body>"
    "<h1>Accepted</h1><a href='/'>Back</a>"
    "</body>";

  return html;
}

String getCommandURL(int i, int a, int s)
{
  String url("/");
  url += handURLStrings[i];
  url += "/";
  url += actionURLStrings[a];
  url += "?speed=";
  url += speedURLStrings[s];
  return url;
}

