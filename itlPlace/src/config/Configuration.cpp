#include "Configuration.h"
#include "Logging.h"

libconfig::ConfigExt gCfg;

namespace libconfig
{
  Setting& ConfigExt::ValueOf(const char* settingName) const
  {
    config_setting_t *s = FindInContext(settingName);
    if (!s) throw SettingNotFoundException(settingName);
    if (m_Replicate)
      m_Replicant->ReplicateSetting(MakeLongName(m_Context.Context(), settingName).c_str(), s);
    return Setting::wrapSetting(s);
  }

  config_setting_t* ConfigExt::FindSetting(const char* path, int segmentsRequired, int depth) const
  {
    if (depth > m_MaxSearchDepth)
    {
      LOGERRORFORMAT(("Too deep (%d) recucsion resolving path. [%s]", depth, path));
      throw SearchStackOverflowException(path);
    }

    string realPath = ExpandVariables(path, depth + 1);
    char* currentPath = (char*)realPath.c_str();
    
    int dotsCount = 0;
    if (segmentsRequired == 0)
      dotsCount = 2 * realPath.length(); //set to big value
    else
      for (string::iterator rpi = realPath.begin(); rpi != realPath.end(); ++rpi)
        if (*rpi == '.')
          ++dotsCount;

    for (;;)
    {
      config_setting_t* setting = StrictFindSetting(this, Root(), currentPath, depth + 1);
      if (setting != 0) return setting;

      if (!IsGlobal())
      {
        setting = StrictFindSetting(this, gCfg.Root(), currentPath, depth + 1);
        if (setting != 0) return setting;
      }

      if (dotsCount >= segmentsRequired)
      {
        dotsCount -= 1;
        char* dot = strchr(currentPath, '.');
        if (dot == 0)
          return 0;
        currentPath = dot + 1;
      }
      else
        return 0;
    }

    return 0;
  }

  config_setting_t* ConfigExt::StrictFindSetting(const ConfigExt* initialCfg, config_setting_t* root, const char* path, int depth)
  {
    string pathContainer(path);
    char* currentPath = (char*)pathContainer.c_str();
    config_setting_t* setting = root;

    while (currentPath != 0)
    {
      char* segment = currentPath;
      char* dot = strchr(currentPath, '.');
      if (dot != 0)
      {
        *dot = 0;
        currentPath = dot + 1;
      }
      else
        currentPath = 0;

      setting = config_setting_get_member(setting, segment);
      if (setting == 0)
        return 0;

      if (config_setting_type(setting) == CONFIG_TYPE_STRING)
      {
        const char *val = config_setting_get_string(setting);
        if (val[0] == '@')
        {
          setting = initialCfg->FindSetting(val + 1, 0, depth + 1);
          if (setting == 0) return 0;
        }
      }
    }
    return setting;
  }

  string ConfigExt::ExpandVariables(const char* path, int depth) const
  {
    const char* start = strchr(path, '$');
    
    if (start == 0)
      return path;

searchStart:
    while (start[0] != '$' && start[1] != '(')
    {
      ++start;
      if (start[1] == 0)
      {
        LOGERRORFORMAT(("Invalid path expression: %s", path));
        throw ConfigException();
      }
    }
    
    int i = 2; //skip "$("
    int def_pos = 0;
    while (start[i] != ')')
    {
      if (start[i] == 0)
      {
        LOGERRORFORMAT(("Invalid path expression: %s", path));
        throw ConfigException();
      }
      if (start[i] == '$' || start[i] == '(')
      {
        start += i;
        goto searchStart;
      }
      if (start[i] == ':')
        if (def_pos == 0)
          def_pos = i;
        else
        {
          LOGERRORFORMAT(("Invalid path expression: %s", path));
          throw ConfigException();
        }
      ++i;
    }

    string varPath(start, 2, def_pos == 0 ? i - 2 : def_pos - 2);
    config_setting_t* setting = FindSetting(varPath.c_str(), 0, depth + 1);
    string newPath(path, 0, start - path);

    if (setting == 0 || config_setting_type(setting) != CONFIG_TYPE_STRING)
      if (def_pos != 0)//default value
        newPath.append(start + def_pos + 1, i - def_pos - 1);
      else
      {
        LOGERRORFORMAT(("Unable to resolve variable: $(%s)", varPath.c_str()));
        throw ConfigException();
      }
    else
      newPath += config_setting_get_string(setting);
    newPath += start + i + 1;

    return ExpandVariables(newPath.c_str(), depth + 1);
  }

  string GetFullName(config_setting_t* s)
  {
    if (s == 0) return "";
    string result = config_setting_name(s);
    for (s = config_setting_parent(s); !config_setting_is_root(s); s = config_setting_parent(s))
      result = config_setting_name(s) + ("." + result);
    return result;
  }

  config_setting_t* ConfigExt::FindInContext(const char* path) const
  {
    int startDotsCount = 0;
    while (path[startDotsCount] == '.') ++startDotsCount;

    int dotsCount = startDotsCount;
    for (int i = startDotsCount; path[i] != 0; ++i)
      if (path[i] == '.') ++dotsCount;
    
    if (m_Trace)
    {
      ALERTFORMAT(("cfgtrace - Context: %s   Path: %s", m_Context.Context().c_str(), path));
    }
    config_setting_t* result = FindSetting((m_Context.Context() + (path + startDotsCount)).c_str(), dotsCount, 0);
    if (m_Trace)
      if (result == 0)
      {
        ALERTFORMAT(("cfgtrace - NotFound %s", path));
      }
      else
      {
        ALERTFORMAT(("cfgtrace - Found %s at %s", path, GetFullName(result).c_str()));
      }

    return result;
  }

  void ConfigExt::LoadConfiguration(const char* file)
  {
    try
    {
      m_FileName = file;
      FILE *fp = fopen(file, "r");

      if (!fp)
      {
        LOGCRITICALFORMAT(("Unable to open %s\n", file));
      }

      try
      {
        this->read(fp);
      }
      catch(...)
      {
        fclose(fp);
        throw;
      }
      fclose(fp);
      this->setAutoConvert(true);

      m_Trace = ValueOf("Config.Trace", m_Trace);
      m_MaxSearchDepth = ValueOf("Config.MaxSearchDepth", m_MaxSearchDepth);
      {//must go together and before any warning keys
        m_Replicate = ValueOf("Config.Replicate", m_Replicate);
        if (m_Replicate && m_Replicant == 0)
          m_Replicant = new ConfigExt();
      }
      m_WarnNondefaultOptions = ValueOf("Config.WarnNondefaultOptions", m_WarnNondefaultOptions);
      m_WarnMissingOptions = ValueOf("Config.WarnMissingOptions", m_WarnMissingOptions);
      

      

      return;
    }
    catch(ParseException& ex)
    {
      printf("Configuration error on line %d: %s\n", ex.getLine(), ex.getError());
    }
    catch(SettingNotFoundException nfex)
    {
      printf("Configuration setting not found: %s\n", nfex.getPath());
    }
    catch(ConfigException& cex)
    {
      printf("Unknown configuration exception!\n");
    }
    catch(std::exception ex)
    {
       printf("Unable to read %s\n", file);
    }
    exit(1);
  }

  void ConfigExt::SetArguments(int argc, char** argv)
  {
    for(int i = 0; i < argc; i++)
    {
      std::string name(argv[i]);
      if (name.size() < 4 || name[0] != '-' || name[1] != '-') continue;

      size_t pos = name.find('=', 2);
      if(pos == -1) continue;

      SetCfgValue(name.substr(2, pos - 2), name.substr(pos + 1));
    }
  }

  bool ParseBool(const char* str, bool* val)
  {
    if (str == 0) return false;

    if ( (str[0] == 'T' || str[0] == 't')
      && (str[1] == 'R' || str[1] == 'r')
      && (str[2] == 'U' || str[2] == 'u')
      && (str[3] == 'E' || str[3] == 'e')
      && str[4] == 0)
    {
      *val = true;
      return true;
    }

    if ( (str[0] == 'F' || str[0] == 'f')
      && (str[1] == 'A' || str[1] == 'a')
      && (str[2] == 'L' || str[2] == 'l')
      && (str[3] == 'S' || str[3] == 's')
      && (str[4] == 'E' || str[4] == 'e')
      && str[5] == 0)
    {
      *val = false;
      return true;
    }

    return false;
  }

  void ConfigExt::SetCfgValue(const std::string& path, const std::string& value)
  {
    if (path.length() == 0)
    {
      LOGERROR("Config path should not be empty.");
      return;
    }

    //parse value
    config_value_t val;
    short type = CONFIG_TYPE_NONE;
    short format = CONFIG_FORMAT_DEFAULT;
    int n;
    int len = (int)value.length();
    if (1 == sscanf(value.c_str(), "%I64dL%n", &val, &n) && n == len)
      type = CONFIG_TYPE_INT64;
    else if ((1 == sscanf(value.c_str(), "%I64XL%n", &val, &n) && n == len)
      || (1 == sscanf(value.c_str(), "%I64xL%n", &val, &n) && n == len))
    {
      type = CONFIG_TYPE_INT64;
      format = CONFIG_FORMAT_HEX;
    }
    else if (1 == sscanf(value.c_str(), "%ld%n", &val, &n) && n == len)
      type = CONFIG_TYPE_INT;
    else if ((1 == sscanf(value.c_str(), "%lX%n", &val, &n) && n == len)
      || (1 == sscanf(value.c_str(), "%lx%n", &val, &n) && n == len))
    {
      type = CONFIG_TYPE_INT;
      format = CONFIG_FORMAT_HEX;
    }
    else if (1 == sscanf(value.c_str(), "%g%n", &val, &n) && n == len)
      type = CONFIG_TYPE_FLOAT;
    else if (ParseBool(value.c_str(), (bool*)&val))
      type = CONFIG_TYPE_BOOL;
    else
      type = CONFIG_TYPE_STRING;

    //find/create setting
    config_setting_t* setting = FindOrCreate(path, type);
    
    if (setting == 0) return;

    switch (type)
    {
    case CONFIG_TYPE_INT:
    case CONFIG_TYPE_BOOL:
      config_setting_set_int(setting, val.ival);
      break;
    case CONFIG_TYPE_INT64:
      config_setting_set_int64(setting, val.llval);
      break;
    case CONFIG_TYPE_FLOAT:
      config_setting_set_float(setting, val.fval);
      break;
    case CONFIG_TYPE_STRING:
      config_setting_set_string(setting, value.c_str());
      break;
    default:
      LOGCRITICAL("Oops!");
    };
  }

  config_setting_t* ConfigExt::FindOrCreate( const std::string &path, short type )
  {
    string currentPath(path);
    size_t pos = 0;
    size_t startPos = 0;
    config_setting_t* nextSetting = 0;
    config_setting_t* setting = Root();
    do
    {
      startPos = pos;
      pos = currentPath.find('.', startPos);
      if (pos != string::npos)
      {
        currentPath[pos] = 0;
        pos++;
      }

      nextSetting = StrictFindSetting(this, setting, currentPath.c_str() + startPos, 0);
      if (nextSetting == 0) break;
      setting = nextSetting;
    } while(pos != string::npos);

    if (nextSetting == 0)
    {
      if (config_setting_type(setting) != CONFIG_TYPE_GROUP)
      {
        LOGERRORFORMAT(("Unable to override non-group setting: [%s]", GetFullName(setting).c_str()));
        return 0;
      }

      while (pos != string::npos)
      {
        nextSetting = config_setting_add(setting, currentPath.c_str() + startPos, CONFIG_TYPE_GROUP);
        if (nextSetting == 0)
        {
          LOGERRORFORMAT(("Unable to create setting [%s.%s]", GetFullName(setting).c_str(), currentPath.c_str() + startPos));
          return 0;
        }
        setting = nextSetting;

        startPos = pos;
        pos = currentPath.find('.', startPos);
        if (pos != string::npos)
        {
          currentPath[pos] = 0;
          pos++;
        }
      }

      //create last tier with correct type
      nextSetting = config_setting_add(setting, currentPath.c_str() + startPos, type);
      if (nextSetting == 0)
      {
        LOGERRORFORMAT(("Unable to create setting [%s.%s]", GetFullName(setting).c_str(), currentPath.c_str() + startPos));
        return 0;
      }
      setting = nextSetting;
    }

    if (config_setting_is_aggregate(setting) && type != config_setting_type(setting))
    {
      LOGERRORFORMAT(("Overriding aggregate setting with value is not allowed. Path: %s", path.c_str()));
      return 0;
    }

    if (config_setting_type(setting) != type)
    {
      config_setting_t* parent = config_setting_parent(setting);
      string settingName = config_setting_name(setting);
      config_setting_remove(parent, settingName.c_str());
      setting = config_setting_add(parent, settingName.c_str(), type);
    }

    return setting;
  }

  void ConfigExt::ReplicateSetting(const char* path, config_setting_t* sample)
  {
    config_setting_t* setting = FindOrCreate(path, config_setting_type(sample));

    switch (config_setting_type(sample))
    {
    case CONFIG_TYPE_INT:
      config_setting_set_int(setting, config_setting_get_int(sample));
      config_setting_set_format(setting, config_setting_get_format(sample));
      break;
    case CONFIG_TYPE_BOOL:
      config_setting_set_bool(setting, config_setting_get_bool(sample));
      break;
    case CONFIG_TYPE_INT64:
      config_setting_set_int64(setting, config_setting_get_int64(sample));
      config_setting_set_format(setting, config_setting_get_format(sample));
      break;
    case CONFIG_TYPE_FLOAT:
      config_setting_set_float(setting, config_setting_get_float(sample));
      break;
    case CONFIG_TYPE_STRING:
      config_setting_set_string(setting, config_setting_get_string(sample));
      break;
    default:
      break;//do nothing
    };
    //config_setting_set_format()
  }

  string ConfigExt::MakeLongName(const string& context, const char* path)
  {
    if (path == 0) return context;
    while (path[0] == '.') ++path;
    return context + path;
  }

  void ConfigExt::ReplicateSetting(const char* path, const long value)
  {
    config_setting_t* setting = FindOrCreate(path, CONFIG_TYPE_INT);
    config_setting_set_int(setting, value);
  }

  void ConfigExt::ReplicateSetting(const char* path, const long long value)
  {
    config_setting_t* setting = FindOrCreate(path, CONFIG_TYPE_INT64);
    config_setting_set_int64(setting, value);
  }

  void ConfigExt::ReplicateSetting(const char* path, const bool value)
  {
    config_setting_t* setting = FindOrCreate(path, CONFIG_TYPE_BOOL);
    config_setting_set_bool(setting, (int)value);
  }

  void ConfigExt::ReplicateSetting(const char* path, const double value)
  {
    config_setting_t* setting = FindOrCreate(path, CONFIG_TYPE_FLOAT);
    config_setting_set_float(setting, value);
  }

  void ConfigExt::ReplicateSetting(const char* path, const char* value)
  {
    config_setting_t* setting = FindOrCreate(path, CONFIG_TYPE_STRING);
    config_setting_set_string(setting, value);
  }

  void ConfigExt::ReplicateSetting(const char* path, const string& value)
  {
    config_setting_t* setting = FindOrCreate(path, CONFIG_TYPE_STRING);
    config_setting_set_string(setting, value.c_str());
  }

  bool ConfigExt::HasValue(const char* settingName, const char* value, bool returnTrueIfNotDefined) const
  {
    config_setting_t *s = FindInContext(settingName);
    if (s == 0)
    {
      if (m_Replicate)
        m_Replicant->ReplicateSetting(MakeLongName(m_Context.Context(), settingName).c_str(), value);
      return returnTrueIfNotDefined;
    }
    else if (m_Replicate)
      m_Replicant->ReplicateSetting(MakeLongName(m_Context.Context(), settingName).c_str(), s);
    return strcmp((const char*)libconfig::Setting::wrapSetting(s), value) == 0;
  }
}
