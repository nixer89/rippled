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

#ifndef RIPPLE_APP_PATHS_PATHREQUESTS_H_INCLUDED
#define RIPPLE_APP_PATHS_PATHREQUESTS_H_INCLUDED

#include <xrpl/app/main/Application.h>
#include <xrpl/app/paths/PathRequest.h>
#include <xrpl/app/paths/RippleLineCache.h>
#include <xrpl/core/Job.h>
#include <atomic>
#include <mutex>
#include <vector>

namespace xrpl {

class PathRequests
{
public:
    /** A collection of all PathRequest instances. */
    PathRequests(
        Application& app,
        beast::Journal journal,
        beast::insight::Collector::ptr const& collector)
        : app_(app), mJournal(journal), mLastIdentifier(0)
    {
        mFast = collector->make_event("pathfind_fast");
        mFull = collector->make_event("pathfind_full");
    }

    /** Update all of the contained PathRequest instances.

        @param ledger Ledger we are pathfinding in.
        @param shouldCancel Invocable that returns whether to cancel.
     */
    void
    updateAll(
        std::shared_ptr<ReadView const> const& ledger,
        Job::CancelCallback shouldCancel);

    std::shared_ptr<RippleLineCache>
    getLineCache(
        std::shared_ptr<ReadView const> const& ledger,
        bool authoritative);

    // Create a new-style path request that pushes
    // updates to a subscriber
    Json::Value
    makePathRequest(
        std::shared_ptr<InfoSub> const& subscriber,
        std::shared_ptr<ReadView const> const& ledger,
        Json::Value const& request);

    // Create an old-style path request that is
    // managed by a coroutine and updated by
    // the path engine
    Json::Value
    makeLegacyPathRequest(
        PathRequest::pointer& req,
        std::function<void(void)> completion,
        Resource::Consumer& consumer,
        std::shared_ptr<ReadView const> const& inLedger,
        Json::Value const& request);

    // Execute an old-style path request immediately
    // with the ledger specified by the caller
    Json::Value
    doLegacyPathRequest(
        Resource::Consumer& consumer,
        std::shared_ptr<ReadView const> const& inLedger,
        Json::Value const& request);

    void
    reportFast(std::chrono::milliseconds ms)
    {
        mFast.notify(ms);
    }

    void
    reportFull(std::chrono::milliseconds ms)
    {
        mFull.notify(ms);
    }

private:
    void
    insertPathRequest(PathRequest::pointer const&);

    Application& app_;
    beast::Journal mJournal;

    beast::insight::Event mFast;
    beast::insight::Event mFull;

    // Track all requests
    std::vector<PathRequest::wptr> requests_;

    // Use a RippleLineCache
    std::shared_ptr<RippleLineCache> mLineCache;

    std::atomic<int> mLastIdentifier;

    std::recursive_mutex mLock;
};

}  // namespace xrpl

#endif
