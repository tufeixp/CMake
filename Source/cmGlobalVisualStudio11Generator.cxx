/*============================================================================
  CMake - Cross Platform Makefile Generator
  Copyright 2000-2011 Kitware, Inc., Insight Software Consortium

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#include "cmGlobalVisualStudio11Generator.h"
#include "cmLocalVisualStudio10Generator.h"
#include "cmMakefile.h"

static const char vs11generatorName[] = "Visual Studio 11 2012";

// Map generator name without year to name with year.
static const char* cmVS11GenName(const std::string& name, std::string& genName)
{
  if(strncmp(name.c_str(), vs11generatorName,
             sizeof(vs11generatorName)-6) != 0)
    {
    return 0;
    }
  const char* p = name.c_str() + sizeof(vs11generatorName) - 6;
  if(cmHasLiteralPrefix(p, " 2012"))
    {
    p += 5;
    }
  genName = std::string(vs11generatorName) + p;
  return p;
}

class cmGlobalVisualStudio11Generator::Factory
  : public cmGlobalGeneratorFactory
{
public:
  virtual cmGlobalGenerator* CreateGlobalGenerator(
                                                const std::string& name) const
    {
    std::string genName;
    const char* p = cmVS11GenName(name, genName);
    if(!p)
      { return 0; }
    if(!*p)
      {
      return new cmGlobalVisualStudio11Generator(
        genName, "");
      }
    if(*p++ != ' ')
      { return 0; }
    if(strcmp(p, "Win64") == 0)
      {
      return new cmGlobalVisualStudio11Generator(
        genName, "x64");
      }
    if(strcmp(p, "ARM") == 0)
      {
      return new cmGlobalVisualStudio11Generator(
        genName, "ARM");
      }

    std::set<std::string> installedSDKs =
      cmGlobalVisualStudio11Generator::GetInstalledWindowsCESDKs();

    if(installedSDKs.find(p) == installedSDKs.end())
      {
      return 0;
      }

    cmGlobalVisualStudio11Generator* ret =
      new cmGlobalVisualStudio11Generator(name, p);
    ret->WindowsCEVersion = "8.00";
    return ret;
    }

  virtual void GetDocumentation(cmDocumentationEntry& entry) const
    {
    entry.Name = vs11generatorName;
    entry.Brief = "Generates Visual Studio 11 (VS 2012) project files.";
    }

  virtual void GetGenerators(std::vector<std::string>& names) const
    {
    names.push_back(vs11generatorName);
    names.push_back(vs11generatorName + std::string(" ARM"));
    names.push_back(vs11generatorName + std::string(" Win64"));

    std::set<std::string> installedSDKs =
      cmGlobalVisualStudio11Generator::GetInstalledWindowsCESDKs();
    for(std::set<std::string>::const_iterator i =
        installedSDKs.begin(); i != installedSDKs.end(); ++i)
      {
      names.push_back(std::string(vs11generatorName) + " " + *i);
      }
    }
};

//----------------------------------------------------------------------------
cmGlobalGeneratorFactory* cmGlobalVisualStudio11Generator::NewFactory()
{
  return new Factory;
}

//----------------------------------------------------------------------------
cmGlobalVisualStudio11Generator::cmGlobalVisualStudio11Generator(
  const std::string& name, const std::string& platformName)
  : cmGlobalVisualStudio10Generator(name, platformName)
{
  std::string vc11Express;
  this->ExpressEdition = cmSystemTools::ReadRegistryValue(
    "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\VCExpress\\11.0\\Setup\\VC;"
    "ProductDir", vc11Express, cmSystemTools::KeyWOW64_32);
  this->DefaultPlatformToolset = "v110";
}

//----------------------------------------------------------------------------
bool
cmGlobalVisualStudio11Generator::MatchesGeneratorName(
                                                const std::string& name) const
{
  std::string genName;
  if(cmVS11GenName(name, genName))
    {
    return genName == this->GetName();
    }
  return false;
}

//----------------------------------------------------------------------------
bool cmGlobalVisualStudio11Generator::InitializeWindowsPhone(cmMakefile* mf)
{
  if(!this->SelectWindowsPhoneToolset(this->DefaultPlatformToolset))
    {
    cmOStringStream e;
    if(this->DefaultPlatformToolset.empty())
      {
      e << this->GetName() << " supports Windows Phone '8.0', but not '"
        << this->SystemVersion << "'.  Check CMAKE_SYSTEM_VERSION.";
      }
    else
      {
      e << "A Windows Phone component with CMake requires both the Windows "
        << "Desktop SDK as well as the Windows Phone '" << this->SystemVersion
        << "' SDK. Please make sure that you have both installed";
      }
    mf->IssueMessage(cmake::FATAL_ERROR, e.str());
    return false;
    }
  return true;
}

//----------------------------------------------------------------------------
bool cmGlobalVisualStudio11Generator::InitializeWindowsStore(cmMakefile* mf)
{
  if(!this->SelectWindowsStoreToolset(this->DefaultPlatformToolset))
    {
    cmOStringStream e;
    if(this->DefaultPlatformToolset.empty())
      {
      e << this->GetName() << " supports Windows Store '8.0', but not '"
        << this->SystemVersion << "'.  Check CMAKE_SYSTEM_VERSION.";
      }
    else
      {
      e << "A Windows Store component with CMake requires both the Windows "
        << "Desktop SDK as well as the Windows Store '" << this->SystemVersion
        << "' SDK. Please make sure that you have both installed";
      }
    mf->IssueMessage(cmake::FATAL_ERROR, e.str());
    return false;
    }

  return true;
}

//----------------------------------------------------------------------------
bool cmGlobalVisualStudio11Generator
::InitializeWindowsPhonePlatforms(cmMakefile* mf)
{
  this->targetPlatforms.clear();
  cmGeneratorExpression::Split(this->SystemPlatforms, this->targetPlatforms);
  for(std::vector<std::string>::iterator i = this->targetPlatforms.begin();
      i != this->targetPlatforms.end(); ++i)
    {
    if(*i != "Win32" && *i != "ARM")
      {
      cmOStringStream e;
      e << this->GetName() << " does not support " << *i
        << " as a platform for Windows Phone.";
        mf->IssueMessage(cmake::FATAL_ERROR, e.str());
        return false;
      }
    }

  return true;
}

//----------------------------------------------------------------------------
bool cmGlobalVisualStudio11Generator
::InitializeWindowsStorePlatforms(cmMakefile* mf)
{
  this->targetPlatforms.clear();
  cmGeneratorExpression::Split(this->SystemPlatforms, this->targetPlatforms);
  for(std::vector<std::string>::iterator i = this->targetPlatforms.begin();
      i != this->targetPlatforms.end(); ++i)
    {
    if(*i != "Win32" && *i != "x64" && *i != "ARM")
      {
      cmOStringStream e;
      e << this->GetName() << " does not support " << *i
        << " as a platform for Windows Store.";
      mf->IssueMessage(cmake::FATAL_ERROR, e.str());
      return false;
      }
    }

  return true;
}

//----------------------------------------------------------------------------
bool
cmGlobalVisualStudio11Generator::SelectWindowsPhoneToolset(
  std::string& toolset) const
{
  if(this->SystemVersion == "8.0")
    {
    if (this->IsWindowsPhoneToolsetInstalled() &&
        this->IsWindowsDesktopToolsetInstalled())
      {
      toolset = "v110_wp80";
      return true;
      }
    else
      {
      return false;
      }
    }
  return
    this->cmGlobalVisualStudio10Generator::SelectWindowsPhoneToolset(toolset);
}

//----------------------------------------------------------------------------
bool
cmGlobalVisualStudio11Generator::SelectWindowsStoreToolset(
  std::string& toolset) const
{
  if(this->SystemVersion == "8.0")
    {
    if(this->IsWindowsStoreToolsetInstalled() &&
       this->IsWindowsDesktopToolsetInstalled())
      {
      toolset = "v110";
      return true;
      }
    else
      {
      return false;
      }
    }
  return
    this->cmGlobalVisualStudio10Generator::SelectWindowsStoreToolset(toolset);
}

//----------------------------------------------------------------------------
void cmGlobalVisualStudio11Generator::WriteSLNHeader(std::ostream& fout)
{
  fout << "Microsoft Visual Studio Solution File, Format Version 12.00\n";
  if (this->ExpressEdition)
    {
    fout << "# Visual Studio Express 2012 for Windows Desktop\n";
    }
  else
    {
    fout << "# Visual Studio 2012\n";
    }
}

//----------------------------------------------------------------------------
void
cmGlobalVisualStudio11Generator
::WriteSolutionConfigurations(std::ostream& fout)
  {
  bool hasTargetProcessors = false;
  fout << "\tGlobalSection(SolutionConfigurationPlatforms) = preSolution\n";
  for(std::vector<std::string>::iterator i = this->Configurations.begin();
    i != this->Configurations.end(); ++i)
    {
    if(this->GetTargetPlatforms().empty())
      {
      fout << "\t\t" << *i << "|" << this->GetPlatformName()
        << " = " << *i << "|" << this->GetPlatformName() << "\n";
      }
    else
      {
      for(std::vector<std::string>::iterator j =
            this->GetTargetPlatforms().begin();
          j != this->GetTargetPlatforms().end(); ++j)
        {
        hasTargetProcessors = true;
        fout << "\t\t" << *i << "|" << *j
          << " = " << *i << "|" << *j << "\n";
        }
      }
    }
  fout << "\tEndGlobalSection\n";
  }

//----------------------------------------------------------------------------
void
cmGlobalVisualStudio11Generator::WriteProjectConfigurations(
  std::ostream& fout, const std::string& name, cmTarget::TargetType type,
  const std::set<std::string>& configsPartOfDefaultBuild,
  const std::string& platform,
  const std::string& platformMapping)
  {
  if(this->GetTargetPlatforms().empty())
    {
    cmGlobalVisualStudio10Generator::WriteProjectConfigurations(
      fout, name, type, configsPartOfDefaultBuild,
      this->GetPlatformName(), platformMapping);
    }
  else
    {
    // Iterate through the platforms and write each of them in turn
    for(std::vector<std::string>::iterator j =
        this->GetTargetPlatforms().begin();
      j != this->GetTargetPlatforms().end(); ++j)
      {
      cmGlobalVisualStudio10Generator::WriteProjectConfigurations(
        fout, name, type, configsPartOfDefaultBuild, *j, platformMapping);
      }
    }
}

//----------------------------------------------------------------------------
cmLocalGenerator *cmGlobalVisualStudio11Generator::CreateLocalGenerator()
{
  cmLocalVisualStudio10Generator* lg =
    new cmLocalVisualStudio10Generator(cmLocalVisualStudioGenerator::VS11);
  lg->SetGlobalGenerator(this);
  return lg;
}

//----------------------------------------------------------------------------
bool cmGlobalVisualStudio11Generator::UseFolderProperty()
{
  // Intentionally skip over the parent class implementation and call the
  // grand-parent class's implementation. Folders are not supported by the
  // Express editions in VS10 and earlier, but they are in VS11 Express.
  return cmGlobalVisualStudio8Generator::UseFolderProperty();
}

//----------------------------------------------------------------------------
std::set<std::string>
cmGlobalVisualStudio11Generator::GetInstalledWindowsCESDKs()
{
  const char sdksKey[] = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\"
                         "Windows CE Tools\\SDKs";

  std::vector<std::string> subkeys;
  cmSystemTools::GetRegistrySubKeys(sdksKey, subkeys,
                                    cmSystemTools::KeyWOW64_32);

  std::set<std::string> ret;
  for(std::vector<std::string>::const_iterator i =
      subkeys.begin(); i != subkeys.end(); ++i)
    {
    std::string key = sdksKey;
    key += '\\';
    key += *i;
    key += ';';

    std::string path;
    if(cmSystemTools::ReadRegistryValue(key.c_str(),
                                        path,
                                        cmSystemTools::KeyWOW64_32) &&
        !path.empty())
      {
      ret.insert(*i);
      }
    }

  return ret;
}

//----------------------------------------------------------------------------
bool
cmGlobalVisualStudio11Generator::NeedsDeploy(cmTarget::TargetType type) const
{
  if((type == cmTarget::EXECUTABLE ||
      type == cmTarget::SHARED_LIBRARY) &&
     (this->SystemIsWindowsPhone ||
      this->SystemIsWindowsStore))
    {
    return true;
    }
  return cmGlobalVisualStudio10Generator::NeedsDeploy(type);
}

//----------------------------------------------------------------------------
bool
cmGlobalVisualStudio11Generator::IsWindowsDesktopToolsetInstalled() const
{
  const char desktop80Key[] =
    "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\"
    "VisualStudio\\11.0\\VC\\Libraries\\Extended";
  const char VS2012DesktopExpressKey[] =
    "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\"
    "WDExpress\\11.0;InstallDir";

  std::vector<std::string> subkeys;
  std::string path;
  return cmSystemTools::ReadRegistryValue(VS2012DesktopExpressKey,
                                          path,
                                          cmSystemTools::KeyWOW64_32) ||
         cmSystemTools::GetRegistrySubKeys(desktop80Key,
                                           subkeys,
                                           cmSystemTools::KeyWOW64_32);
}

//----------------------------------------------------------------------------
bool
cmGlobalVisualStudio11Generator::IsWindowsPhoneToolsetInstalled() const
{
  const char wp80Key[] =
    "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\"
    "Microsoft SDKs\\WindowsPhone\\v8.0\\"
    "Install Path;Install Path";

  std::string path;
  cmSystemTools::ReadRegistryValue(wp80Key,
                                   path,
                                   cmSystemTools::KeyWOW64_32);
  return !path.empty();
}

//----------------------------------------------------------------------------
bool
cmGlobalVisualStudio11Generator::IsWindowsStoreToolsetInstalled() const
{
  const char win80Key[] =
    "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\"
    "VisualStudio\\11.0\\VC\\Libraries\\Core\\Arm";

  std::vector<std::string> subkeys;
  return cmSystemTools::GetRegistrySubKeys(win80Key,
                                           subkeys,
                                           cmSystemTools::KeyWOW64_32);
}
