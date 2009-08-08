#ifndef __CONFIGCONTEXT_H__
#define __CONFIGCONTEXT_H__

#include "stdTypes.h"

namespace libconfig
{
  class ConfigContext;

  class ConfigContextStack
  {
  private:
    friend class ConfigContext;
    TemplateTypes<string>::stack m_ContextStack;
    
  protected:
    void Push(const string& newLevel)
    {
      if ('.' == *newLevel.rbegin())
        m_ContextStack.push(Context() + newLevel);
      else
        m_ContextStack.push(Context() + newLevel + ".");
    }

  public:
    ConfigContextStack()
    {
      m_ContextStack.push("");
    }

    string Context() const { return m_ContextStack.top(); }

    void Pop()
    {
      if (ContextLevel() > 0)
        m_ContextStack.pop();
    }

    int ContextLevel() const
    {
      return (int)m_ContextStack.size() - 1;
    }
  };

  class CfgContextCreationHelper
  {
  private:
    friend class ConfigContext;
    ConfigContextStack& m_Stack;
    const string& m_Name;

  public:
    CfgContextCreationHelper(ConfigContextStack& stack, const string& name)
      : m_Stack(stack), m_Name(name)
    {}
  };

  class ConfigContext
  {
  private:
    ConfigContextStack& m_ParentStack;
    int m_Level;
    bool m_Closed;

    ConfigContext(const ConfigContext&);
    ConfigContext& operator=(const ConfigContext&);

  public:
    ConfigContext(CfgContextCreationHelper& args)
      : m_ParentStack(args.m_Stack),
      m_Level(args.m_Stack.ContextLevel()),
      m_Closed(false)
    {
      m_ParentStack.Push(args.m_Name);
    }

    ~ConfigContext()
    {
      Close();
    }

    operator string() const
    {
      return m_ParentStack.Context();
    }

    void Close()
    {
      if (!m_Closed)
      {
        while (m_Level < m_ParentStack.ContextLevel())
          m_ParentStack.Pop();
        m_Closed = true;
      }
    }
  };
}

using libconfig::ConfigContext;

#endif //__CONFIGCONTEXT_H__
