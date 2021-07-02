/*******************************************************************************
################################################################################
#   Copyright (c) [2020-2021] [HCL Technologies Ltd.]                          #
#                                                                              #
#   Licensed under the Apache License, Version 2.0 (the "License");            #
#   you may not use this file except in compliance with the License.           #
#   You may obtain a copy of the License at                                    #
#                                                                              #
#       http://www.apache.org/licenses/LICENSE-2.0                             #
#                                                                              #
#   Unless required by applicable law or agreed to in writing, software        #
#   distributed under the License is distributed on an "AS IS" BASIS,          #
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
#   See the License for the specific language governing permissions and        #
#   limitations under the License.                                             #
################################################################################
*******************************************************************************/

/* This file contains functions to support the preparation of Common Header part of VES Event*/


#ifndef __VES_EVENT_HANDLER_HPP__
#define __VES_EVENT_HANDLER_HPP__

#include <iostream>
#include <stdio.h>
#include "VesUtils.hpp"
#include "VesEvent.hpp"

class VesEventHandler
{

   public:
      /* Default constructor/Destructor*/
      VesEventHandler();
      ~VesEventHandler();
      bool prepare(VesEventType evtType);
      bool send();

   private:
      VesEvent *mVesEvent;

};

#endif
/**********************************************************************
  End of file
 **********************************************************************/
