#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#pragma warning( push )
#pragma warning(disable:4290)
#include "libconfig.h++"
#pragma warning( pop )

#include "ConfigContext.h"
#include "Logging.h"

namespace libconfig
{
  class SearchStackOverflowException : public SettingException
  {
  public:
    const char *what() const throw() { return "SearchStackOverflowException"; }
    SearchStackOverflowException(const char* path) : SettingException(path) {}
  };

  class ConfigExt : protected Config
  {
  private:
    ConfigExt(const ConfigExt& other); // not supported
    ConfigExt& operator=(const ConfigExt& other); // not supported

    ConfigContextStack m_Context;
    string m_FileName;
    int m_MaxSearchDepth;

    bool m_Trace;
    bool m_WarnNondefaultOptions;
    bool m_WarnMissingOptions;
    bool m_Replicate;
    ConfigExt* m_Replicant;

    config_setting_t* FindSetting(const char* path, int segmentsRequired, int depth) const;
    string ExpandVariables(const char* path, int depth, const char* originalPath) const;
    static config_setting_t* StrictFindSetting(const ConfigExt* initialCfg, config_setting_t* root, const char* path, int depth);
    config_setting_t* Root() const { return config_root_setting(&_config); }
    config_setting_t* FindOrCreate(const std::string &path, short type);
    static string MakeLongName(const string& context, const char* path);
    static config_setting_t* SetCfgValue(config_setting_t*, const std::string& value);

    void ReplicateSetting(const char* path, config_setting_t* sample);
    void ReplicateSetting(const char* path, const long value);
    void ReplicateSetting(const char* path, const long long value);
    void ReplicateSetting(const char* path, const bool value);
    void ReplicateSetting(const char* path, const double value);
    void ReplicateSetting(const char* path, const char* value);
    void ReplicateSetting(const char* path, const string& value);

    void ReplicateSetting(const char* path, const int value)
    { ReplicateSetting(path, (long)value); }
    void ReplicateSetting(const char* path, const unsigned value)
    { ReplicateSetting(path, (long)value); }

    config_setting_t* FindInContext(const string& path, int depth = 0) const;

  public:
    ConfigExt() : Config()
    {
      m_Trace = false;
      m_WarnMissingOptions = false;
      m_WarnNondefaultOptions = false;
      m_Replicate = false;
      m_Replicant = 0;
      m_MaxSearchDepth = 100;
    }

    ~ConfigExt()
    {
      if (m_Replicate && m_Replicant != 0)
        m_Replicant->writeFile(MakeReplicantName().c_str());

      if (m_Replicant != 0)
      {
        delete m_Replicant;
        m_Replicant = 0;
      }
    }

    string Name() const { return m_FileName; }
    string MakeReplicantName();
    void LoadConfiguration(const char* file);
    void SetArguments(int argc, char** argv);
    void SetCfgValue(const std::string& path, const std::string& value, bool pathIsRooted = false);
    bool IsGlobal() const;

    CfgContextCreationHelper OpenContext(const string& name)
    {
      return CfgContextCreationHelper(m_Context, name);
    }

    Setting& ValueOf(const string& settingName) const;
    template<class T>
    T ValueOf(const string& settingName, const T def) const;

    bool Exists(const string& settingName) const;
    bool Defined(const string& settingName) const;
  };
}

extern libconfig::ConfigExt gCfg;

inline bool libconfig::ConfigExt::IsGlobal() const
{
  return this == &gCfg;
}

template<class T>
inline bool IsValueDifferent(libconfig::Setting& s, T val) { return val != (T)s; }

template<>
inline bool IsValueDifferent(libconfig::Setting& s, const char* val) { return strcmp(val,s) != 0; }


template<class T>
inline T libconfig::ConfigExt::ValueOf(const string& settingName, const T def) const
{
  config_setting_t *s = FindInContext(settingName);
  if(!s || config_setting_is_aggregate(s))
  {
    if (m_WarnMissingOptions)
    {
      GLOGWARNING(LOGINPLACE, "Value for [%s%s] is not found", m_Context.Context().c_str(), settingName.c_str());
    }
    if (m_Replicate)
      m_Replicant->ReplicateSetting(MakeLongName(m_Context.Context(), (settingName + ".**default").c_str()).c_str(), def);
    return def;
  }
  libconfig::Setting& st = libconfig::Setting::wrapSetting(s);
  if (m_WarnNondefaultOptions && IsValueDifferent(st, def))
  {
    GLOGWARNING(LOGINPLACE, "Value for [%s%s] differs from default", m_Context.Context().c_str(), settingName.c_str());
  }
  return st;
}

inline bool libconfig::ConfigExt::Exists(const string& settingName) const
{
  return FindInContext(settingName) != 0;
}

inline bool libconfig::ConfigExt::Defined(const string& settingName) const
{
  config_setting_t *s = FindInContext(settingName);
  return s != 0 && !config_setting_is_aggregate(s);
}

#endif //__CONFIGURATION_H__