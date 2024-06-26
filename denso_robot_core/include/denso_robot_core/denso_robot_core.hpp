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

#ifndef DENSO_ROBOT_CORE_HPP__
#define DENSO_ROBOT_CORE_HPP__

#include "denso_robot_core/denso_controller.hpp"

namespace denso2
{
class DensoRobotCore
{
public:
  DensoRobotCore();
  virtual ~DensoRobotCore();

  HRESULT Initialize(int ctrl_type, std::string addr, int port, std::string file_name);

  void Start();
  void Stop();

  HRESULT ChangeMode(int mode, bool service = false);
  int get_Mode() const
  {
    return mode_;
  }

  const DensoController_Ptr& get_Controller() const
  {
    return ctrl_;
  }

private:
  DensoController_Ptr ctrl_;
  int ctrlType_, mode_;
  volatile bool quit_;
};

typedef boost::shared_ptr<DensoRobotCore> DensoRobotCore_Ptr;

}  // namespace denso2

#endif  // DENSO_ROBOT_CORE_H
