#include "BufferingUtils.h"
#include "BufferPositions.h"
namespace Utils
{
  void PrintVariants(TemplateTypes<VGVariantsListElement>::list* vGList)
  {
    int ind = 0;
    for (TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->begin(); i != vGList->end(); ++i)
    {
      PrintVariantsNode(&(*i), ind);
      ind++;
    } 
  }

  void PrintVariantsNode(VGVariantsListElement* vGE, int i)
  {
    char* sBuf = new char [256]; 
    string s;
    s = "(";
    sprintf(sBuf, "%f", vGE->GetRAT());
    s += string(sBuf);
    s += ",\t";
    sprintf(sBuf, "%f", vGE->GetC());
    s += string(sBuf);
    s += ",\t{";
    for (TemplateTypes<BufferPositions>::list::iterator pos = vGE->GetBufferPosition()->begin(); pos != vGE->GetBufferPosition()->end(); ++pos)
    {
      sprintf(sBuf, "%d", pos->GetPosition()->index);
      s += string(sBuf);
      s += ",\t";
    }
    s += "})";
    ALERT("variant: %d\t\t%s",i, s.c_str());
    delete [] sBuf;
  }
}
