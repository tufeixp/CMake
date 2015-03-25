/*============================================================================
  CMake - Cross Platform Makefile Generator
  Copyright 2000-2009 Kitware, Inc., Insight Software Consortium

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/

#include "cmVisualStudio11MultiPlatformTargetGenerator.h"

cmVisualStudio11MultiPlatformTargetGenerator::
cmVisualStudio11MultiPlatformTargetGenerator(cmTarget* target,
                                  cmGlobalVisualStudio11Generator* gg)
: cmVisualStudio10TargetGenerator(target, gg)
{
  this->target = target;
  this->platforms = gg->GetTargetPlatforms();
}


cmVisualStudio11MultiPlatformTargetGenerator::
~cmVisualStudio11MultiPlatformTargetGenerator(void)
{
}

void cmVisualStudio11MultiPlatformTargetGenerator::
WriteSingleProjectConfiguration(
  const std::string& configuration,
  const std::string& platform)
{
  if (platforms.empty())
    {
    cmVisualStudio10TargetGenerator::WriteSingleProjectConfiguration(
      configuration,
      platform);
    }
  else
    {
    for(auto iter = platforms.begin(); iter != platforms.end(); ++iter)
      {
      cmVisualStudio10TargetGenerator::WriteSingleProjectConfiguration(
        configuration,
        *iter);
      }
    }
}

void cmVisualStudio11MultiPlatformTargetGenerator::
WriteSingleProjectConfigurationValues(
  const std::string& configuration,
  const std::string& platform)
{
  if (platforms.empty())
    {
    cmVisualStudio10TargetGenerator::WriteSingleProjectConfigurationValues(
      configuration,
      platform);
    }
  else
    {
    for(auto iter = platforms.begin(); iter != platforms.end(); ++iter)
      {
      cmVisualStudio10TargetGenerator::WriteSingleProjectConfigurationValues(
        configuration,
        *iter);
      }
    }
}

void cmVisualStudio11MultiPlatformTargetGenerator::
WriteSinglePathAndIncrementalLinkOptions(
  cmTarget::TargetType ttype,
  const std::string& configuration,
  const std::string& platform)
{
  if (platforms.empty())
    {
    cmVisualStudio10TargetGenerator::WriteSinglePathAndIncrementalLinkOptions(
      ttype,
      configuration,
      platform);
    }
  else
    {
    for(auto iter = platforms.begin(); iter != platforms.end(); ++iter)
      {
      cmVisualStudio10TargetGenerator::
        WriteSinglePathAndIncrementalLinkOptions(
        ttype,
        configuration,
        *iter);
      }
    }
}

void cmVisualStudio11MultiPlatformTargetGenerator::
WriteSingleItemDefinitionGroup(
  const std::string& configuration,
  const std::string& platform)
{
  if (platforms.empty())
    {
    cmVisualStudio10TargetGenerator::WriteSingleItemDefinitionGroup(
      configuration,
      platform);
    }
  else
    {
    for(auto iter = platforms.begin(); iter != platforms.end(); ++iter)
      {
      cmVisualStudio10TargetGenerator::WriteSingleItemDefinitionGroup(
        configuration,
        *iter);
      }
    }
  }

void cmVisualStudio11MultiPlatformTargetGenerator::WriteSingleCustomBuild(
    const std::string& configuration,
    const std::string& platform,
    cmSourceFile const* source,
    cmCustomCommand const &command,
    cmLocalVisualStudio7Generator* lg)
{
  if (platforms.empty())
    {
    cmVisualStudio10TargetGenerator::WriteSingleCustomBuild(
      configuration,
      platform,
      source,
      command,
      lg);
    }
  else
    {
    for(auto iter = platforms.begin(); iter != platforms.end(); ++iter)
      {
      cmVisualStudio10TargetGenerator::WriteSingleCustomBuild(
        configuration,
        *iter,
        source,
        command,
        lg);
      }
    }
}

void cmVisualStudio11MultiPlatformTargetGenerator::
WriteSingleDeploymentConfigurationValue(
  const std::string& configuration,
  const std::string& platform)
{
  if (platforms.empty())
    {
    cmVisualStudio10TargetGenerator::WriteSingleDeploymentConfigurationValue(
      configuration,
      platform);
    }
  else
    {
    for(auto iter = platforms.begin(); iter != platforms.end(); ++iter)
      {
      cmVisualStudio10TargetGenerator::WriteSingleDeploymentConfigurationValue(
        configuration,
        *iter);
      }
    }
}

void cmVisualStudio11MultiPlatformTargetGenerator::
WriteSingleExcludedConfigurationValue(
  const std::string& configuration,
  const std::string& platform)
{
  if (platforms.empty())
    {
    cmVisualStudio10TargetGenerator::WriteSingleExcludedConfigurationValue(
      configuration,
      platform);
    }
  else
    {
    for(auto iter = platforms.begin(); iter != platforms.end(); ++iter)
      {
      cmVisualStudio10TargetGenerator::WriteSingleExcludedConfigurationValue(
        configuration,
        *iter);
      }
    }
}

void cmVisualStudio11MultiPlatformTargetGenerator::WriteSingleEmbeddedResource(
  const std::string& config,
  const std::string& platform)
{
  if (platforms.empty())
    {
    cmVisualStudio10TargetGenerator::WriteSingleEmbeddedResource(
      config,
      platform);
    }
  else
    {
    for(auto iter = platforms.begin(); iter != platforms.end(); ++iter)
      {
      cmVisualStudio10TargetGenerator::WriteSingleEmbeddedResource(
        config,
        *iter);
      }
    }
}

void cmVisualStudio11MultiPlatformTargetGenerator::
WriteSingleSourceSpecificFlag(
  std::string const& flags,
  const char* compileAs,
  bool noWinRT,
  std::string const& configDefines,
  std::string const& lang,
  std::string const& configuration,
  std::string const& platform)
{
  if (platforms.empty())
    {
    cmVisualStudio10TargetGenerator::WriteSingleSourceSpecificFlag(
      flags,
      compileAs,
      noWinRT,
      configDefines,
      lang,
      configuration,
      platform);
    }
  else
    {
    for(auto iter = platforms.begin(); iter != platforms.end(); ++iter)
      {
      cmVisualStudio10TargetGenerator::WriteSingleSourceSpecificFlag(
        flags,
        compileAs,
        noWinRT,
        configDefines,
        lang,
        configuration,
        *iter);
      }
    }
}
