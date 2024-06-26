/**
 * Software License Agreement (MIT License)
 *
 * @copyright Copyright (c) 2015 DENSO WAVE INCORPORATED
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "denso_robot_core/denso_task.hpp"

namespace denso2
{
DensoTask::DensoTask(DensoBase* parent, Service_Vec& service, Handle_Vec& handle, const std::string& name,
                     const int* mode)
  : DensoBase(parent, service, handle, name, mode)
{
}

DensoTask::~DensoTask()
{
}

HRESULT DensoTask::InitializeBCAP(XMLElement* xmlElem)
{
  return AddVariable(xmlElem);
}

HRESULT DensoTask::StartService()
{
  DensoVariable_Vec::iterator itVar;
  for (itVar = vecVar_.begin(); itVar != vecVar_.end(); itVar++)
  {
    (*itVar)->StartService();
  }

  serving_ = true;

  return S_OK;
}

HRESULT DensoTask::StopService()
{
  mtxSrv_.lock();
  serving_ = false;
  mtxSrv_.unlock();

  DensoVariable_Vec::iterator itVar;
  for (itVar = vecVar_.begin(); itVar != vecVar_.end(); itVar++)
  {
    (*itVar)->StopService();
  }

  return S_OK;
}

bool DensoTask::Update()
{
  boost::mutex::scoped_lock lockSrv(mtxSrv_);
  if (!serving_)
    return false;

  DensoVariable_Vec::iterator itVar;
  for (itVar = vecVar_.begin(); itVar != vecVar_.end(); itVar++)
  {
    (*itVar)->Update();
  }

  return true;
}

HRESULT DensoTask::get_Variable(const std::string& name, DensoVariable_Ptr* var)
{
  if (var == NULL)
  {
    return E_INVALIDARG;
  }

  DensoBase_Vec vecBase;
  vecBase.insert(vecBase.end(), vecVar_.begin(), vecVar_.end());

  DensoBase_Ptr pBase;
  HRESULT hr = DensoBase::get_Object(vecBase, name, &pBase);
  if (SUCCEEDED(hr))
  {
    *var = boost::dynamic_pointer_cast<DensoVariable>(pBase);
  }

  return hr;
}

HRESULT DensoTask::AddVariable(const std::string& name)
{
  return DensoBase::AddVariable(ID_TASK_GETVARIABLE, name, vecVar_);
}

HRESULT DensoTask::AddVariable(XMLElement* xmlElem)
{
  HRESULT hr = S_OK;
  XMLElement* xmlVar;

  for (xmlVar = xmlElem->FirstChildElement(DensoVariable::XML_VARIABLE_NAME); xmlVar != NULL;
       xmlVar = xmlVar->NextSiblingElement(DensoVariable::XML_VARIABLE_NAME))
  {
    hr = DensoBase::AddVariable(ID_TASK_GETVARIABLE, xmlVar, vecVar_);
    if (FAILED(hr))
      break;
  }

  return hr;
}

}  // namespace denso2
