#ifndef __CONFIGCONTEXT_H__
#define __CONFIGCONTEXT_H__

#include "ConfigCache.h"
#include "stdTypes.h"

namespace libconfig
{
class ConfigContext;

class ConfigContextStack
{
private:
    friend class ConfigContext;

    struct CtxLevel
    {
        string Name;
#ifndef NO_CONFIG_CACHE
        mutable ConfigCache settingsCache;
#endif
        CtxLevel(string name) :Name(name)
        {
#if !defined(NO_CONFIG_CACHE)
            settingsCache.set_empty_key(string());
#endif
        }

        CtxLevel& operator=(string name)
        {
#ifndef NO_CONFIG_CACHE
            if (Name != name)
                settingsCache.clear();
#endif
            Name = name;
            return *this;
        }
    };

    typedef std::map<string, CtxLevel*> LevelsMap;
    TemplateTypes<CtxLevel*>::stack m_ContextStack;
    LevelsMap m_Levels;

    CtxLevel* GetLevel(string Name)
    {
        LevelsMap::iterator level = m_Levels.find(Name);
        if (level == m_Levels.end())
        {
            CtxLevel* lvl = new CtxLevel(Name);
            m_Levels[Name] = lvl;
            return lvl;
        }
        return level->second;
    }
    
protected:
    void Push(const string& newLevel)
    {
        if ('.' == *newLevel.rbegin())
            m_ContextStack.push(GetLevel(Context() + newLevel));
        else
            m_ContextStack.push(GetLevel(Context() + newLevel + "."));
    }

public:
    ConfigContextStack()
    {
        m_ContextStack.push(GetLevel(""));
    }

    ~ConfigContextStack()
    {
        for (LevelsMap::iterator lvl = m_Levels.begin(); lvl != m_Levels.end(); ++lvl)
        {
            delete lvl->second;
            lvl->second = 0;
        }
    }

    string Context() const { return m_ContextStack.top()->Name; }

    void Pop()
    {
        if (ContextLevel() > 0)
            m_ContextStack.pop();
    }

    int ContextLevel() const
    {
        return (int)m_ContextStack.size() - 1;
    }

#ifndef NO_CONFIG_CACHE
    ConfigCache& CurrentCache() const {return m_ContextStack.top()->settingsCache;}
#endif
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
    ConfigContext(CfgContextCreationHelper args)
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
