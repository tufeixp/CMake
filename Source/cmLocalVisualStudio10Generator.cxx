/*============================================================================
  CMake - Cross Platform Makefile Generator
  Copyright 2000-2009 Kitware, Inc., Insight Software Consortium

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#include "cmLocalVisualStudio10Generator.h"
#include "cmTarget.h"
#include "cmMakefile.h"
#include "cmVisualStudio10TargetGenerator.h"
#include "cmVisualStudio11MultiPlatformTargetGenerator.h"
#include "cmGlobalVisualStudio10Generator.h"
#include "cmGlobalVisualStudio11Generator.h"
#include <cm_expat.h>
#include "cmXMLParser.h"
#include <cmsys/auto_ptr.hxx>
class cmVS10XMLParser : public cmXMLParser
{
  public:
  virtual void EndElement(const std::string& /* name */)
    {
    }
  virtual void CharacterDataHandler(const char* data, int length)
    {
      if(this->DoGUID )
        {
        this->GUID.assign(data+1, length-2);
        this->DoGUID = false;
        }
    }
  virtual void StartElement(const std::string& name, const char**)
    {
      // once the GUID is found do nothing
      if(!this->GUID.empty())
        {
        return;
        }
      if("ProjectGUID" == name || "ProjectGuid" == name)
        {
        this->DoGUID = true;
        }
    }
  int InitializeParser()
    {
      this->DoGUID = false;
      int ret = cmXMLParser::InitializeParser();
      if(ret == 0)
        {
        return ret;
        }
      // visual studio projects have a strange encoding, but it is
      // really utf-8
      XML_SetEncoding(static_cast<XML_Parser>(this->Parser), "utf-8");
      return 1;
    }
  std::string GUID;
  bool DoGUID;
};


//----------------------------------------------------------------------------
cmLocalVisualStudio10Generator
::cmLocalVisualStudio10Generator(cmGlobalGenerator* gg, cmMakefile* mf):
  cmLocalVisualStudio7Generator(gg, mf)
{
}

cmLocalVisualStudio10Generator::~cmLocalVisualStudio10Generator()
{
}

void cmLocalVisualStudio10Generator::Generate()
{

  cmTargets &tgts = this->Makefile->GetTargets();
  for(cmTargets::iterator l = tgts.begin(); l != tgts.end(); ++l)
    {
    if(l->second.GetType() == cmTarget::INTERFACE_LIBRARY)
      {
      continue;
      }
    if(static_cast<cmGlobalVisualStudioGenerator*>(this->GlobalGenerator)
       ->TargetIsFortranOnly(l->second))
      {
      this->CreateSingleVCProj(l->first.c_str(),l->second);
      }
    else
      {
      cmsys::auto_ptr<cmVisualStudio10TargetGenerator> tg;
      cmGlobalVisualStudio11Generator* vs11Gen =
        dynamic_cast<cmGlobalVisualStudio11Generator*> (this->GlobalGenerator);
      if (vs11Gen != NULL)
        {
        cmsys::auto_ptr<cmVisualStudio11MultiPlatformTargetGenerator>
          tg11 (new cmVisualStudio11MultiPlatformTargetGenerator(
            &l->second, vs11Gen));
        tg = tg11;
        }
      else
        {
        cmGlobalVisualStudio10Generator* vs10Gen =
          static_cast<cmGlobalVisualStudio10Generator*>
          (this->GetGlobalGenerator());
        cmsys::auto_ptr<cmVisualStudio10TargetGenerator>
          tg10 (new cmVisualStudio10TargetGenerator(&l->second, vs10Gen));
        tg = tg10;
        }
      tg->Generate();
      }
    }
  this->WriteStampFiles();
}


void cmLocalVisualStudio10Generator
::ReadAndStoreExternalGUID(const std::string& name,
                           const char* path)
{
  cmVS10XMLParser parser;
  parser.ParseFile(path);

  // if we can not find a GUID then we will generate one later
  if(parser.GUID.empty())
    {
    return;
    }

  std::string guidStoreName = name;
  guidStoreName += "_GUID_CMAKE";
  // save the GUID in the cache
  this->GlobalGenerator->GetCMakeInstance()->
    AddCacheEntry(guidStoreName.c_str(),
                  parser.GUID.c_str(),
                  "Stored GUID",
                  cmState::INTERNAL);
}

//----------------------------------------------------------------------------
const char* cmLocalVisualStudio10Generator::ReportErrorLabel() const
{
  return ":VCEnd";
}
