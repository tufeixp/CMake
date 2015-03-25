/*============================================================================
  CMake - Cross Platform Makefile Generator
  Copyright 2000-2009 Kitware, Inc., Insight Software Consortium

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#ifndef cmVisualStudio11MultiPlatformTargetGenerator_h
#define cmVisualStudio11MultiPlatformTargetGenerator_h

#include "cmStandardIncludes.h"
#include "cmvisualstudio10targetgenerator.h"
#include "cmGlobalVisualStudio11Generator.h"
class cmVisualStudio11MultiPlatformTargetGenerator :
    public cmVisualStudio10TargetGenerator
{
public:
    cmVisualStudio11MultiPlatformTargetGenerator(cmTarget* target,
                                  cmGlobalVisualStudio11Generator* gg);
    virtual ~cmVisualStudio11MultiPlatformTargetGenerator(void);

protected:
    virtual void WriteSingleProjectConfiguration(
      const std::string& configuration,
      const std::string& platform);
    virtual void WriteSingleProjectConfigurationValues(
      const std::string& configuration,
      const std::string& platform);
    virtual void WriteSinglePathAndIncrementalLinkOptions(
      cmTarget::TargetType ttype,
      const std::string& configuration,
      const std::string& platform);
    virtual void WriteSingleItemDefinitionGroup(
      const std::string& configuration,
      const std::string& platform);
    virtual void WriteSingleDeploymentConfigurationValue(
      const std::string& configuration,
      const std::string& platform);
    virtual void WriteSingleExcludedConfigurationValue(
      const std::string& configuration,
      const std::string& platform);
    virtual void WriteSingleEmbeddedResource(
      const std::string& config,
      const std::string& platform);
    virtual void WriteSingleSourceSpecificFlag(
      std::string const& flags,
      const char* compileAs,
      bool noWinRT,
      std::string const& configDefines,
      std::string const& lang,
      std::string const& configuration,
      std::string const& platform);
    virtual void WriteSingleCustomBuild(
      const std::string& configuration,
      const std::string& platform,
      cmSourceFile const *source,
      cmCustomCommand const &command,
      cmLocalVisualStudio7Generator* lg);
private:
    cmTarget* target;
    std::vector<std::string> platforms;
};

#endif
