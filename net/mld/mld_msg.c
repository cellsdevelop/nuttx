/****************************************************************************
 * net/mld/mld_msg.c
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of CITEL Technologies Ltd nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY CITEL TECHNOLOGIES AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL CITEL TECHNOLOGIES OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <assert.h>
#include <debug.h>

#include <nuttx/net/netconfig.h>
#include <nuttx/net/net.h>
#include <nuttx/net/mld.h>

#include "devif/devif.h"
#include "netdev/netdev.h"
#include "mld/mld.h"

#ifdef CONFIG_NET_MLD

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mld_schedmsg
 *
 * Description:
 *   Schedule a message to be send at the next driver polling interval.
 *
 * Assumptions:
 *   The network is locked
 *
 ****************************************************************************/

int mld_schedmsg(FAR struct mld_group_s *group, uint8_t msgtype)
{
  FAR struct net_driver_s *dev;

  DEBUGASSERT(group != NULL && !IS_MLD_SCHEDMSG(group->flags));
  DEBUGASSERT(group->ifindex > 0);

  /* Get the device instance associated with the interface index of the group */

  dev = netdev_findbyindex(group->ifindex);
  if (dev == NULL)
    {
      nerr("ERROR: No device for this interface index: %u\n",
           group->ifindex);
      return -ENODEV;
    }

  group->msgtype = msgtype;
  SET_MLD_SCHEDMSG(group->flags);

  /* Notify the device that we have a packet to send */

  netdev_txnotify_dev(dev);
  return OK;
}

/****************************************************************************
 * Name: mld_waitmsg
 *
 * Description:
 *   Schedule a message to be send at the next driver polling interval and
 *   block, waiting for the message to be sent.
 *
 * Assumptions:
 *   The network is locked
 *
 ****************************************************************************/

int mld_waitmsg(FAR struct mld_group_s *group, uint8_t msgtype)
{
  int ret;

  /* Schedule to send the message */

  DEBUGASSERT(!IS_MLD_WAITMSG(group->flags));
  SET_MLD_WAITMSG(group->flags);

  ret = mld_schedmsg(group, msgtype);
  if (ret < 0)
    {
      nerr("ERROR: Failed to schedule the message: %d\n", ret);
      goto errout;
    }

  /* Then wait for the message to be sent */

  while (IS_MLD_SCHEDMSG(group->flags))
    {
      /* Wait for the semaphore to be posted */

      while ((ret = net_lockedwait(&group->sem)) < 0)
        {
          /* The only error that should occur from net_lockedwait() is if
           * the wait is awakened by a signal or, perhaps, canceled.
           */

          DEBUGASSERT(ret == -EINTR || ret == -ECANCELED);
          if (ret != -EINTR && ret != -ECANCELED)
            {
              break;
            }

          ret = OK;
        }
    }

  /* The message has been sent and we are no longer waiting */

errout:
  CLR_MLD_WAITMSG(group->flags);
  return ret;
}

#endif /* CONFIG_NET_MLD */
