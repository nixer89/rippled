//------------------------------------------------------------------------------
/*
    This file is part of xrpld: https://github.com/xrplf/xrpld
    Copyright (c) 2021 XRP Ledger Foundation

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#include <xrpl/app/main/NodeStoreScheduler.h>
#include <cassert>

namespace xrpl {

NodeStoreScheduler::NodeStoreScheduler(JobQueue& jobQueue) : jobQueue_(jobQueue)
{
}

void
NodeStoreScheduler::scheduleTask(NodeStore::Task& task)
{
    if (jobQueue_.isStopped())
        return;

    if (!jobQueue_.addJob(jtWRITE, "NodeObject::store", [&task](Job&) {
            task.performScheduledTask();
        }))
    {
        // Job not added, presumably because we're shutting down.
        // Recover by executing the task synchronously.
        task.performScheduledTask();
    }
}

void
NodeStoreScheduler::onFetch(NodeStore::FetchReport const& report)
{
    if (jobQueue_.isStopped())
        return;

    jobQueue_.addLoadEvents(
        report.fetchType == NodeStore::FetchType::async ? jtNS_ASYNC_READ
                                                        : jtNS_SYNC_READ,
        1,
        report.elapsed);
}

void
NodeStoreScheduler::onBatchWrite(NodeStore::BatchWriteReport const& report)
{
    if (jobQueue_.isStopped())
        return;

    jobQueue_.addLoadEvents(jtNS_WRITE, report.writeCount, report.elapsed);
}

}  // namespace xrpl
