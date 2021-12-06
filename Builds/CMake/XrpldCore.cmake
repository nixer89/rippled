#[===================================================================[
   xrpl_core
   core functionality, useable by some client software perhaps
#]===================================================================]

file (GLOB_RECURSE rb_headers
  src/xrpl/beast/*.h
  src/xrpl/beast/*.hpp)

add_library (xrpl_core
  ${rb_headers}) ## headers added here for benefit of IDEs
if (unity)
  set_target_properties(xrpl_core PROPERTIES UNITY_BUILD ON)
endif ()


#[===============================[
    beast/legacy FILES:
    TODO: review these sources for removal or replacement
#]===============================]
target_sources (xrpl_core PRIVATE
  src/xrpl/beast/clock/basic_seconds_clock.cpp
  src/xrpl/beast/core/CurrentThreadName.cpp
  src/xrpl/beast/core/SemanticVersion.cpp
  src/xrpl/beast/hash/impl/xxhash.cpp
  src/xrpl/beast/insight/impl/Collector.cpp
  src/xrpl/beast/insight/impl/Groups.cpp
  src/xrpl/beast/insight/impl/Hook.cpp
  src/xrpl/beast/insight/impl/Metric.cpp
  src/xrpl/beast/insight/impl/NullCollector.cpp
  src/xrpl/beast/insight/impl/StatsDCollector.cpp
  src/xrpl/beast/net/impl/IPAddressConversion.cpp
  src/xrpl/beast/net/impl/IPAddressV4.cpp
  src/xrpl/beast/net/impl/IPAddressV6.cpp
  src/xrpl/beast/net/impl/IPEndpoint.cpp
  src/xrpl/beast/utility/src/beast_Journal.cpp
  src/xrpl/beast/utility/src/beast_PropertyStream.cpp)

#[===============================[
    core sources
#]===============================]
target_sources (xrpl_core PRIVATE
  #[===============================[
    main sources:
      subdir: basics (partial)
  #]===============================]
  src/xrpl/basics/impl/base64.cpp
  src/xrpl/basics/impl/contract.cpp
  src/xrpl/basics/impl/CountedObject.cpp
  src/xrpl/basics/impl/FileUtilities.cpp
  src/xrpl/basics/impl/IOUAmount.cpp
  src/xrpl/basics/impl/Log.cpp
  src/xrpl/basics/impl/strHex.cpp
  src/xrpl/basics/impl/StringUtilities.cpp
  #[===============================[
    main sources:
      subdir: json
  #]===============================]
  src/xrpl/json/impl/JsonPropertyStream.cpp
  src/xrpl/json/impl/Object.cpp
  src/xrpl/json/impl/Output.cpp
  src/xrpl/json/impl/Writer.cpp
  src/xrpl/json/impl/json_reader.cpp
  src/xrpl/json/impl/json_value.cpp
  src/xrpl/json/impl/json_valueiterator.cpp
  src/xrpl/json/impl/json_writer.cpp
  src/xrpl/json/impl/to_string.cpp
  #[===============================[
    main sources:
      subdir: protocol
  #]===============================]
  src/xrpl/protocol/impl/AccountID.cpp
  src/xrpl/protocol/impl/Book.cpp
  src/xrpl/protocol/impl/BuildInfo.cpp
  src/xrpl/protocol/impl/ErrorCodes.cpp
  src/xrpl/protocol/impl/Feature.cpp
  src/xrpl/protocol/impl/Indexes.cpp
  src/xrpl/protocol/impl/InnerObjectFormats.cpp
  src/xrpl/protocol/impl/Issue.cpp
  src/xrpl/protocol/impl/Keylet.cpp
  src/xrpl/protocol/impl/LedgerFormats.cpp
  src/xrpl/protocol/impl/PublicKey.cpp
  src/xrpl/protocol/impl/Quality.cpp
  src/xrpl/protocol/impl/Rate2.cpp
  src/xrpl/protocol/impl/SField.cpp
  src/xrpl/protocol/impl/SOTemplate.cpp
  src/xrpl/protocol/impl/STAccount.cpp
  src/xrpl/protocol/impl/STAmount.cpp
  src/xrpl/protocol/impl/STArray.cpp
  src/xrpl/protocol/impl/STBase.cpp
  src/xrpl/protocol/impl/STBlob.cpp
  src/xrpl/protocol/impl/STInteger.cpp
  src/xrpl/protocol/impl/STLedgerEntry.cpp
  src/xrpl/protocol/impl/STObject.cpp
  src/xrpl/protocol/impl/STParsedJSON.cpp
  src/xrpl/protocol/impl/STPathSet.cpp
  src/xrpl/protocol/impl/STTx.cpp
  src/xrpl/protocol/impl/STValidation.cpp
  src/xrpl/protocol/impl/STVar.cpp
  src/xrpl/protocol/impl/STVector256.cpp
  src/xrpl/protocol/impl/SecretKey.cpp
  src/xrpl/protocol/impl/Seed.cpp
  src/xrpl/protocol/impl/Serializer.cpp
  src/xrpl/protocol/impl/Sign.cpp
  src/xrpl/protocol/impl/TER.cpp
  src/xrpl/protocol/impl/TxFormats.cpp
  src/xrpl/protocol/impl/TxMeta.cpp
  src/xrpl/protocol/impl/UintTypes.cpp
  src/xrpl/protocol/impl/digest.cpp
  src/xrpl/protocol/impl/tokens.cpp
  #[===============================[
    main sources:
      subdir: crypto
  #]===============================]
  src/xrpl/crypto/impl/RFC1751.cpp
  src/xrpl/crypto/impl/csprng.cpp
  src/xrpl/crypto/impl/secure_erase.cpp)

add_library (Xrpl::xrpl_core ALIAS xrpl_core)
target_include_directories (xrpl_core
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/xrpl>
    # this one is for beast/legacy files:
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/beast/extras>
    $<INSTALL_INTERFACE:include>)


target_compile_definitions(xrpl_core
  PUBLIC
    BOOST_ASIO_USE_TS_EXECUTOR_AS_DEFAULT
    BOOST_CONTAINER_FWD_BAD_DEQUE
    HAS_UNCAUGHT_EXCEPTIONS=1)
target_compile_options (xrpl_core
  PUBLIC
    $<$<BOOL:${is_gcc}>:-Wno-maybe-uninitialized>)
target_link_libraries (xrpl_core
  PUBLIC
    OpenSSL::Crypto
    Xrpl::boost
    Xrpl::syslibs
    NIH::secp256k1
    NIH::ed25519-donna
    date::date
    Xrpl::opts)
#[=================================[
   main/core headers installation
#]=================================]
install (
  FILES
    src/xrpl/basics/base64.h
    src/xrpl/basics/Blob.h
    src/xrpl/basics/Buffer.h
    src/xrpl/basics/CountedObject.h
    src/xrpl/basics/FileUtilities.h
    src/xrpl/basics/IOUAmount.h
    src/xrpl/basics/LocalValue.h
    src/xrpl/basics/Log.h
    src/xrpl/basics/MathUtilities.h
    src/xrpl/basics/safe_cast.h
    src/xrpl/basics/Slice.h
    src/xrpl/basics/StringUtilities.h
    src/xrpl/basics/ToString.h
    src/xrpl/basics/UnorderedContainers.h
    src/xrpl/basics/XRPAmount.h
    src/xrpl/basics/algorithm.h
    src/xrpl/basics/base_uint.h
    src/xrpl/basics/chrono.h
    src/xrpl/basics/contract.h
    src/xrpl/basics/FeeUnits.h
    src/xrpl/basics/hardened_hash.h
    src/xrpl/basics/strHex.h
  DESTINATION include/xrpl/basics)
install (
  FILES
    src/xrpl/crypto/RFC1751.h
    src/xrpl/crypto/csprng.h
    src/xrpl/crypto/secure_erase.h
  DESTINATION include/xrpl/crypto)
install (
  FILES
    src/xrpl/json/JsonPropertyStream.h
    src/xrpl/json/Object.h
    src/xrpl/json/Output.h
    src/xrpl/json/Writer.h
    src/xrpl/json/json_forwards.h
    src/xrpl/json/json_reader.h
    src/xrpl/json/json_value.h
    src/xrpl/json/json_writer.h
    src/xrpl/json/to_string.h
  DESTINATION include/xrpl/json)
install (
  FILES
    src/xrpl/json/impl/json_assert.h
  DESTINATION include/xrpl/json/impl)
install (
  FILES
    src/xrpl/protocol/AccountID.h
    src/xrpl/protocol/AmountConversions.h
    src/xrpl/protocol/Book.h
    src/xrpl/protocol/BuildInfo.h
    src/xrpl/protocol/ErrorCodes.h
    src/xrpl/protocol/Feature.h
    src/xrpl/protocol/HashPrefix.h
    src/xrpl/protocol/Indexes.h
    src/xrpl/protocol/InnerObjectFormats.h
    src/xrpl/protocol/Issue.h
    src/xrpl/protocol/KeyType.h
    src/xrpl/protocol/Keylet.h
    src/xrpl/protocol/KnownFormats.h
    src/xrpl/protocol/LedgerFormats.h
    src/xrpl/protocol/Protocol.h
    src/xrpl/protocol/PublicKey.h
    src/xrpl/protocol/Quality.h
    src/xrpl/protocol/Rate.h
    src/xrpl/protocol/SField.h
    src/xrpl/protocol/SOTemplate.h
    src/xrpl/protocol/STAccount.h
    src/xrpl/protocol/STAmount.h
    src/xrpl/protocol/STArray.h
    src/xrpl/protocol/STBase.h
    src/xrpl/protocol/STBitString.h
    src/xrpl/protocol/STBlob.h
    src/xrpl/protocol/STExchange.h
    src/xrpl/protocol/STInteger.h
    src/xrpl/protocol/STLedgerEntry.h
    src/xrpl/protocol/STObject.h
    src/xrpl/protocol/STParsedJSON.h
    src/xrpl/protocol/STPathSet.h
    src/xrpl/protocol/STTx.h
    src/xrpl/protocol/STValidation.h
    src/xrpl/protocol/STVector256.h
    src/xrpl/protocol/SecretKey.h
    src/xrpl/protocol/Seed.h
    src/xrpl/protocol/SeqProxy.h
    src/xrpl/protocol/Serializer.h
    src/xrpl/protocol/Sign.h
    src/xrpl/protocol/SystemParameters.h
    src/xrpl/protocol/TER.h
    src/xrpl/protocol/TxFlags.h
    src/xrpl/protocol/TxFormats.h
    src/xrpl/protocol/TxMeta.h
    src/xrpl/protocol/UintTypes.h
    src/xrpl/protocol/digest.h
    src/xrpl/protocol/jss.h
    src/xrpl/protocol/tokens.h
  DESTINATION include/xrpl/protocol)
install (
  FILES
    src/xrpl/protocol/impl/STVar.h
    src/xrpl/protocol/impl/secp256k1.h
  DESTINATION include/xrpl/protocol/impl)

#[===================================[
   beast/legacy headers installation
#]===================================]
install (
  FILES
    src/xrpl/beast/clock/abstract_clock.h
    src/xrpl/beast/clock/basic_seconds_clock.h
    src/xrpl/beast/clock/manual_clock.h
  DESTINATION include/xrpl/beast/clock)
install (
  FILES
    src/xrpl/beast/core/LexicalCast.h
    src/xrpl/beast/core/List.h
    src/xrpl/beast/core/SemanticVersion.h
  DESTINATION include/xrpl/beast/core)
install (
  FILES
    src/xrpl/beast/hash/hash_append.h
    src/xrpl/beast/hash/uhash.h
    src/xrpl/beast/hash/xxhasher.h
  DESTINATION include/xrpl/beast/hash)
install (
  FILES src/xrpl/beast/hash/impl/xxhash.h
  DESTINATION include/xrpl/beast/hash/impl)
install (
  FILES
    src/xrpl/beast/rfc2616.h
    src/xrpl/beast/type_name.h
    src/xrpl/beast/unit_test.h
    src/xrpl/beast/xor_shift_engine.h
  DESTINATION include/xrpl/beast)
install (
  FILES
    src/xrpl/beast/utility/Journal.h
    src/xrpl/beast/utility/PropertyStream.h
    src/xrpl/beast/utility/Zero.h
    src/xrpl/beast/utility/rngfill.h
  DESTINATION include/xrpl/beast/utility)
# WARNING!! -- horrible levelization ahead
# (these files should be isolated or moved...but
#  unfortunately unit_test.h above creates this dependency)
if (tests)
  install (
    FILES
      src/beast/extras/beast/unit_test/amount.hpp
      src/beast/extras/beast/unit_test/dstream.hpp
      src/beast/extras/beast/unit_test/global_suites.hpp
      src/beast/extras/beast/unit_test/match.hpp
      src/beast/extras/beast/unit_test/recorder.hpp
      src/beast/extras/beast/unit_test/reporter.hpp
      src/beast/extras/beast/unit_test/results.hpp
      src/beast/extras/beast/unit_test/runner.hpp
      src/beast/extras/beast/unit_test/suite.hpp
      src/beast/extras/beast/unit_test/suite_info.hpp
      src/beast/extras/beast/unit_test/suite_list.hpp
      src/beast/extras/beast/unit_test/thread.hpp
    DESTINATION include/beast/unit_test)
  install (
    FILES
      src/beast/extras/beast/unit_test/detail/const_container.hpp
    DESTINATION include/beast/unit_test/detail)
endif () #tests
#[===================================================================[
   xrpld executable
#]===================================================================]

#[=========================================================[
   this one header is added as source just to keep older
   versions of cmake happy. cmake 3.10+ allows
   add_executable with no sources
#]=========================================================]
add_executable (xrpld src/xrpl/app/main/Application.h)
if (unity)
  set_target_properties(xrpld PROPERTIES UNITY_BUILD ON)
endif ()
if (tests)
    target_compile_definitions(xrpld PUBLIC ENABLE_TESTS)
endif()
target_sources (xrpld PRIVATE
  #[===============================[
     main sources:
       subdir: app
  #]===============================]
  src/xrpl/app/consensus/RCLConsensus.cpp
  src/xrpl/app/consensus/RCLCxPeerPos.cpp
  src/xrpl/app/consensus/RCLValidations.cpp
  src/xrpl/app/ledger/AcceptedLedger.cpp
  src/xrpl/app/ledger/AcceptedLedgerTx.cpp
  src/xrpl/app/ledger/AccountStateSF.cpp
  src/xrpl/app/ledger/BookListeners.cpp
  src/xrpl/app/ledger/ConsensusTransSetSF.cpp
  src/xrpl/app/ledger/Ledger.cpp
  src/xrpl/app/ledger/LedgerHistory.cpp
  src/xrpl/app/ledger/OrderBookDB.cpp
  src/xrpl/app/ledger/TransactionStateSF.cpp
  src/xrpl/app/ledger/impl/BuildLedger.cpp
  src/xrpl/app/ledger/impl/InboundLedger.cpp
  src/xrpl/app/ledger/impl/InboundLedgers.cpp
  src/xrpl/app/ledger/impl/InboundTransactions.cpp
  src/xrpl/app/ledger/impl/LedgerCleaner.cpp
  src/xrpl/app/ledger/impl/LedgerDeltaAcquire.cpp
  src/xrpl/app/ledger/impl/LedgerMaster.cpp
  src/xrpl/app/ledger/impl/LedgerReplay.cpp
  src/xrpl/app/ledger/impl/LedgerReplayer.cpp
  src/xrpl/app/ledger/impl/LedgerReplayMsgHandler.cpp
  src/xrpl/app/ledger/impl/LedgerReplayTask.cpp
  src/xrpl/app/ledger/impl/LedgerToJson.cpp
  src/xrpl/app/ledger/impl/LocalTxs.cpp
  src/xrpl/app/ledger/impl/OpenLedger.cpp
  src/xrpl/app/ledger/impl/SkipListAcquire.cpp
  src/xrpl/app/ledger/impl/TimeoutCounter.cpp
  src/xrpl/app/ledger/impl/TransactionAcquire.cpp
  src/xrpl/app/ledger/impl/TransactionMaster.cpp
  src/xrpl/app/main/Application.cpp
  src/xrpl/app/main/BasicApp.cpp
  src/xrpl/app/main/CollectorManager.cpp
  src/xrpl/app/main/GRPCServer.cpp
  src/xrpl/app/main/LoadManager.cpp
  src/xrpl/app/main/Main.cpp
  src/xrpl/app/main/NodeIdentity.cpp
  src/xrpl/app/main/NodeStoreScheduler.cpp
  src/xrpl/app/reporting/ReportingETL.cpp
  src/xrpl/app/reporting/ETLSource.cpp
  src/xrpl/app/reporting/P2pProxy.cpp
  src/xrpl/app/misc/CanonicalTXSet.cpp
  src/xrpl/app/misc/FeeVoteImpl.cpp
  src/xrpl/app/misc/HashRouter.cpp
  src/xrpl/app/misc/NegativeUNLVote.cpp
  src/xrpl/app/misc/NetworkOPs.cpp
  src/xrpl/app/misc/SHAMapStoreImp.cpp
  src/xrpl/app/misc/detail/impl/WorkSSL.cpp
  src/xrpl/app/misc/impl/AccountTxPaging.cpp
  src/xrpl/app/misc/impl/AmendmentTable.cpp
  src/xrpl/app/misc/impl/LoadFeeTrack.cpp
  src/xrpl/app/misc/impl/Manifest.cpp
  src/xrpl/app/misc/impl/Transaction.cpp
  src/xrpl/app/misc/impl/TxQ.cpp
  src/xrpl/app/misc/impl/ValidatorKeys.cpp
  src/xrpl/app/misc/impl/ValidatorList.cpp
  src/xrpl/app/misc/impl/ValidatorSite.cpp
  src/xrpl/app/paths/AccountCurrencies.cpp
  src/xrpl/app/paths/Credit.cpp
  src/xrpl/app/paths/Flow.cpp
  src/xrpl/app/paths/PathRequest.cpp
  src/xrpl/app/paths/PathRequests.cpp
  src/xrpl/app/paths/Pathfinder.cpp
  src/xrpl/app/paths/RippleCalc.cpp
  src/xrpl/app/paths/RippleLineCache.cpp
  src/xrpl/app/paths/RippleState.cpp
  src/xrpl/app/paths/impl/BookStep.cpp
  src/xrpl/app/paths/impl/DirectStep.cpp
  src/xrpl/app/paths/impl/PaySteps.cpp
  src/xrpl/app/paths/impl/XRPEndpointStep.cpp
  src/xrpl/app/rdb/backend/RelationalDBInterfacePostgres.cpp
  src/xrpl/app/rdb/backend/RelationalDBInterfaceSqlite.cpp
  src/xrpl/app/rdb/impl/RelationalDBInterface.cpp
  src/xrpl/app/rdb/impl/RelationalDBInterface_global.cpp
  src/xrpl/app/rdb/impl/RelationalDBInterface_nodes.cpp
  src/xrpl/app/rdb/impl/RelationalDBInterface_postgres.cpp
  src/xrpl/app/rdb/impl/RelationalDBInterface_shards.cpp
  src/xrpl/app/tx/impl/ApplyContext.cpp
  src/xrpl/app/tx/impl/BookTip.cpp
  src/xrpl/app/tx/impl/CancelCheck.cpp
  src/xrpl/app/tx/impl/CancelOffer.cpp
  src/xrpl/app/tx/impl/CashCheck.cpp
  src/xrpl/app/tx/impl/Change.cpp
  src/xrpl/app/tx/impl/CreateCheck.cpp
  src/xrpl/app/tx/impl/CreateOffer.cpp
  src/xrpl/app/tx/impl/CreateTicket.cpp
  src/xrpl/app/tx/impl/DeleteAccount.cpp
  src/xrpl/app/tx/impl/DepositPreauth.cpp
  src/xrpl/app/tx/impl/Escrow.cpp
  src/xrpl/app/tx/impl/InvariantCheck.cpp
  src/xrpl/app/tx/impl/OfferStream.cpp
  src/xrpl/app/tx/impl/PayChan.cpp
  src/xrpl/app/tx/impl/Payment.cpp
  src/xrpl/app/tx/impl/SetAccount.cpp
  src/xrpl/app/tx/impl/SetRegularKey.cpp
  src/xrpl/app/tx/impl/SetSignerList.cpp
  src/xrpl/app/tx/impl/SetTrust.cpp
  src/xrpl/app/tx/impl/SignerEntries.cpp
  src/xrpl/app/tx/impl/Taker.cpp
  src/xrpl/app/tx/impl/Transactor.cpp
  src/xrpl/app/tx/impl/apply.cpp
  src/xrpl/app/tx/impl/applySteps.cpp
  #[===============================[
     main sources:
       subdir: basics (partial)
  #]===============================]
  src/xrpl/basics/impl/Archive.cpp
  src/xrpl/basics/impl/BasicConfig.cpp
  src/xrpl/basics/impl/PerfLogImp.cpp
  src/xrpl/basics/impl/ResolverAsio.cpp
  src/xrpl/basics/impl/UptimeClock.cpp
  src/xrpl/basics/impl/make_SSLContext.cpp
  src/xrpl/basics/impl/mulDiv.cpp
  src/xrpl/basics/impl/partitioned_unordered_map.cpp
  #[===============================[
     main sources:
       subdir: conditions
  #]===============================]
  src/xrpl/conditions/impl/Condition.cpp
  src/xrpl/conditions/impl/Fulfillment.cpp
  src/xrpl/conditions/impl/error.cpp
  #[===============================[
     main sources:
       subdir: core
  #]===============================]
  src/xrpl/core/impl/Config.cpp
  src/xrpl/core/impl/DatabaseCon.cpp
  src/xrpl/core/impl/Job.cpp
  src/xrpl/core/impl/JobQueue.cpp
  src/xrpl/core/impl/LoadEvent.cpp
  src/xrpl/core/impl/LoadMonitor.cpp
  src/xrpl/core/impl/SNTPClock.cpp
  src/xrpl/core/impl/SociDB.cpp
  src/xrpl/core/impl/TimeKeeper.cpp
  src/xrpl/core/impl/Workers.cpp
  src/xrpl/core/Pg.cpp
  #[===============================[
     main sources:
       subdir: consensus
  #]===============================]
  src/xrpl/consensus/Consensus.cpp
  #[===============================[
     main sources:
       subdir: ledger
  #]===============================]
  src/xrpl/ledger/impl/ApplyStateTable.cpp
  src/xrpl/ledger/impl/ApplyView.cpp
  src/xrpl/ledger/impl/ApplyViewBase.cpp
  src/xrpl/ledger/impl/ApplyViewImpl.cpp
  src/xrpl/ledger/impl/BookDirs.cpp
  src/xrpl/ledger/impl/CachedView.cpp
  src/xrpl/ledger/impl/Directory.cpp
  src/xrpl/ledger/impl/OpenView.cpp
  src/xrpl/ledger/impl/PaymentSandbox.cpp
  src/xrpl/ledger/impl/RawStateTable.cpp
  src/xrpl/ledger/impl/ReadView.cpp
  src/xrpl/ledger/impl/View.cpp
  #[===============================[
     main sources:
       subdir: net
  #]===============================]
  src/xrpl/net/impl/DatabaseDownloader.cpp
  src/xrpl/net/impl/HTTPClient.cpp
  src/xrpl/net/impl/HTTPDownloader.cpp
  src/xrpl/net/impl/HTTPStream.cpp
  src/xrpl/net/impl/InfoSub.cpp
  src/xrpl/net/impl/RPCCall.cpp
  src/xrpl/net/impl/RPCErr.cpp
  src/xrpl/net/impl/RPCSub.cpp
  src/xrpl/net/impl/RegisterSSLCerts.cpp
  #[===============================[
     main sources:
       subdir: nodestore
  #]===============================]
  src/xrpl/nodestore/backend/CassandraFactory.cpp
  src/xrpl/nodestore/backend/MemoryFactory.cpp
  src/xrpl/nodestore/backend/NuDBFactory.cpp
  src/xrpl/nodestore/backend/NullFactory.cpp
  src/xrpl/nodestore/backend/RocksDBFactory.cpp
  src/xrpl/nodestore/impl/BatchWriter.cpp
  src/xrpl/nodestore/impl/Database.cpp
  src/xrpl/nodestore/impl/DatabaseNodeImp.cpp
  src/xrpl/nodestore/impl/DatabaseRotatingImp.cpp
  src/xrpl/nodestore/impl/DatabaseShardImp.cpp
  src/xrpl/nodestore/impl/DeterministicShard.cpp
  src/xrpl/nodestore/impl/DecodedBlob.cpp
  src/xrpl/nodestore/impl/DummyScheduler.cpp
  src/xrpl/nodestore/impl/EncodedBlob.cpp
  src/xrpl/nodestore/impl/ManagerImp.cpp
  src/xrpl/nodestore/impl/NodeObject.cpp
  src/xrpl/nodestore/impl/Shard.cpp
  src/xrpl/nodestore/impl/ShardInfo.cpp
  src/xrpl/nodestore/impl/TaskQueue.cpp
  #[===============================[
     main sources:
       subdir: overlay
  #]===============================]
  src/xrpl/overlay/impl/Cluster.cpp
  src/xrpl/overlay/impl/ConnectAttempt.cpp
  src/xrpl/overlay/impl/Handshake.cpp
  src/xrpl/overlay/impl/Message.cpp
  src/xrpl/overlay/impl/OverlayImpl.cpp
  src/xrpl/overlay/impl/PeerImp.cpp
  src/xrpl/overlay/impl/PeerReservationTable.cpp
  src/xrpl/overlay/impl/PeerSet.cpp
  src/xrpl/overlay/impl/ProtocolVersion.cpp
  src/xrpl/overlay/impl/TrafficCount.cpp
  src/xrpl/overlay/impl/TxMetrics.cpp
  #[===============================[
     main sources:
       subdir: peerfinder
  #]===============================]
  src/xrpl/peerfinder/impl/Bootcache.cpp
  src/xrpl/peerfinder/impl/Endpoint.cpp
  src/xrpl/peerfinder/impl/PeerfinderConfig.cpp
  src/xrpl/peerfinder/impl/PeerfinderManager.cpp
  src/xrpl/peerfinder/impl/SlotImp.cpp
  src/xrpl/peerfinder/impl/SourceStrings.cpp
  #[===============================[
     main sources:
       subdir: resource
  #]===============================]
  src/xrpl/resource/impl/Charge.cpp
  src/xrpl/resource/impl/Consumer.cpp
  src/xrpl/resource/impl/Fees.cpp
  src/xrpl/resource/impl/ResourceManager.cpp
  #[===============================[
     main sources:
       subdir: rpc
  #]===============================]
  src/xrpl/rpc/handlers/AccountChannels.cpp
  src/xrpl/rpc/handlers/AccountCurrenciesHandler.cpp
  src/xrpl/rpc/handlers/AccountInfo.cpp
  src/xrpl/rpc/handlers/AccountLines.cpp
  src/xrpl/rpc/handlers/AccountObjects.cpp
  src/xrpl/rpc/handlers/AccountOffers.cpp
  src/xrpl/rpc/handlers/AccountTx.cpp
  src/xrpl/rpc/handlers/AccountTxOld.cpp
  src/xrpl/rpc/handlers/BlackList.cpp
  src/xrpl/rpc/handlers/BookOffers.cpp
  src/xrpl/rpc/handlers/CanDelete.cpp
  src/xrpl/rpc/handlers/Connect.cpp
  src/xrpl/rpc/handlers/ConsensusInfo.cpp
  src/xrpl/rpc/handlers/CrawlShards.cpp
  src/xrpl/rpc/handlers/DepositAuthorized.cpp
  src/xrpl/rpc/handlers/DownloadShard.cpp
  src/xrpl/rpc/handlers/Feature1.cpp
  src/xrpl/rpc/handlers/Fee1.cpp
  src/xrpl/rpc/handlers/FetchInfo.cpp
  src/xrpl/rpc/handlers/GatewayBalances.cpp
  src/xrpl/rpc/handlers/GetCounts.cpp
  src/xrpl/rpc/handlers/LedgerAccept.cpp
  src/xrpl/rpc/handlers/LedgerCleanerHandler.cpp
  src/xrpl/rpc/handlers/LedgerClosed.cpp
  src/xrpl/rpc/handlers/LedgerCurrent.cpp
  src/xrpl/rpc/handlers/LedgerData.cpp
  src/xrpl/rpc/handlers/LedgerDiff.cpp
  src/xrpl/rpc/handlers/LedgerEntry.cpp
  src/xrpl/rpc/handlers/LedgerHandler.cpp
  src/xrpl/rpc/handlers/LedgerHeader.cpp
  src/xrpl/rpc/handlers/LedgerRequest.cpp
  src/xrpl/rpc/handlers/LogLevel.cpp
  src/xrpl/rpc/handlers/LogRotate.cpp
  src/xrpl/rpc/handlers/Manifest.cpp
  src/xrpl/rpc/handlers/NodeToShard.cpp
  src/xrpl/rpc/handlers/NoRippleCheck.cpp
  src/xrpl/rpc/handlers/OwnerInfo.cpp
  src/xrpl/rpc/handlers/PathFind.cpp
  src/xrpl/rpc/handlers/PayChanClaim.cpp
  src/xrpl/rpc/handlers/Peers.cpp
  src/xrpl/rpc/handlers/Ping.cpp
  src/xrpl/rpc/handlers/Print.cpp
  src/xrpl/rpc/handlers/Random.cpp
  src/xrpl/rpc/handlers/Reservations.cpp
  src/xrpl/rpc/handlers/RipplePathFind.cpp
  src/xrpl/rpc/handlers/ServerInfo.cpp
  src/xrpl/rpc/handlers/ServerState.cpp
  src/xrpl/rpc/handlers/SignFor.cpp
  src/xrpl/rpc/handlers/SignHandler.cpp
  src/xrpl/rpc/handlers/Stop.cpp
  src/xrpl/rpc/handlers/Submit.cpp
  src/xrpl/rpc/handlers/SubmitMultiSigned.cpp
  src/xrpl/rpc/handlers/Subscribe.cpp
  src/xrpl/rpc/handlers/TransactionEntry.cpp
  src/xrpl/rpc/handlers/Tx.cpp
  src/xrpl/rpc/handlers/TxHistory.cpp
  src/xrpl/rpc/handlers/TxReduceRelay.cpp
  src/xrpl/rpc/handlers/UnlList.cpp
  src/xrpl/rpc/handlers/Unsubscribe.cpp
  src/xrpl/rpc/handlers/ValidationCreate.cpp
  src/xrpl/rpc/handlers/ValidatorInfo.cpp
  src/xrpl/rpc/handlers/ValidatorListSites.cpp
  src/xrpl/rpc/handlers/Validators.cpp
  src/xrpl/rpc/handlers/WalletPropose.cpp
  src/xrpl/rpc/impl/DeliveredAmount.cpp
  src/xrpl/rpc/impl/Handler.cpp
  src/xrpl/rpc/impl/GRPCHelpers.cpp
  src/xrpl/rpc/impl/LegacyPathFind.cpp
  src/xrpl/rpc/impl/RPCHandler.cpp
  src/xrpl/rpc/impl/RPCHelpers.cpp
  src/xrpl/rpc/impl/Role.cpp
  src/xrpl/rpc/impl/ServerHandlerImp.cpp
  src/xrpl/rpc/impl/ShardArchiveHandler.cpp
  src/xrpl/rpc/impl/ShardVerificationScheduler.cpp
  src/xrpl/rpc/impl/Status.cpp
  src/xrpl/rpc/impl/TransactionSign.cpp

  #[===============================[
     main sources:
       subdir: server
  #]===============================]
  src/xrpl/server/impl/JSONRPCUtil.cpp
  src/xrpl/server/impl/Port.cpp
  #[===============================[
     main sources:
       subdir: shamap
  #]===============================]
  src/xrpl/shamap/impl/NodeFamily.cpp
  src/xrpl/shamap/impl/SHAMap.cpp
  src/xrpl/shamap/impl/SHAMapDelta.cpp
  src/xrpl/shamap/impl/SHAMapInnerNode.cpp
  src/xrpl/shamap/impl/SHAMapLeafNode.cpp
  src/xrpl/shamap/impl/SHAMapNodeID.cpp
  src/xrpl/shamap/impl/SHAMapSync.cpp
  src/xrpl/shamap/impl/SHAMapTreeNode.cpp
  src/xrpl/shamap/impl/ShardFamily.cpp)

  #[===============================[
     test sources:
       subdir: app
  #]===============================]
if (tests)
  target_sources (xrpld PRIVATE
    src/test/app/AccountDelete_test.cpp
    src/test/app/AccountTxPaging_test.cpp
    src/test/app/AmendmentTable_test.cpp
    src/test/app/Check_test.cpp
    src/test/app/CrossingLimits_test.cpp
    src/test/app/DeliverMin_test.cpp
    src/test/app/DepositAuth_test.cpp
    src/test/app/Discrepancy_test.cpp
    src/test/app/DNS_test.cpp
    src/test/app/Escrow_test.cpp
    src/test/app/FeeVote_test.cpp
    src/test/app/Flow_test.cpp
    src/test/app/Freeze_test.cpp
    src/test/app/HashRouter_test.cpp
    src/test/app/LedgerHistory_test.cpp
    src/test/app/LedgerLoad_test.cpp
    src/test/app/LedgerReplay_test.cpp
    src/test/app/LoadFeeTrack_test.cpp
    src/test/app/Manifest_test.cpp
    src/test/app/MultiSign_test.cpp
    src/test/app/OfferStream_test.cpp
    src/test/app/Offer_test.cpp
    src/test/app/OversizeMeta_test.cpp
    src/test/app/Path_test.cpp
    src/test/app/PayChan_test.cpp
    src/test/app/PayStrand_test.cpp
    src/test/app/PseudoTx_test.cpp
    src/test/app/RCLCensorshipDetector_test.cpp
    src/test/app/RCLValidations_test.cpp
    src/test/app/Regression_test.cpp
    src/test/app/SHAMapStore_test.cpp
    src/test/app/SetAuth_test.cpp
    src/test/app/SetRegularKey_test.cpp
    src/test/app/SetTrust_test.cpp
    src/test/app/Taker_test.cpp
    src/test/app/TheoreticalQuality_test.cpp
    src/test/app/Ticket_test.cpp
    src/test/app/Transaction_ordering_test.cpp
    src/test/app/TrustAndBalance_test.cpp
    src/test/app/TxQ_test.cpp
    src/test/app/ValidatorKeys_test.cpp
    src/test/app/ValidatorList_test.cpp
    src/test/app/ValidatorSite_test.cpp
    src/test/app/tx/apply_test.cpp
    #[===============================[
       test sources:
         subdir: basics
    #]===============================]
    src/test/basics/Buffer_test.cpp
    src/test/basics/DetectCrash_test.cpp
    src/test/basics/Expected_test.cpp
    src/test/basics/FileUtilities_test.cpp
    src/test/basics/IOUAmount_test.cpp
    src/test/basics/KeyCache_test.cpp
    src/test/basics/PerfLog_test.cpp
    src/test/basics/RangeSet_test.cpp
    src/test/basics/scope_test.cpp
    src/test/basics/Slice_test.cpp
    src/test/basics/StringUtilities_test.cpp
    src/test/basics/TaggedCache_test.cpp
    src/test/basics/XRPAmount_test.cpp
    src/test/basics/base64_test.cpp
    src/test/basics/base_uint_test.cpp
    src/test/basics/contract_test.cpp
    src/test/basics/FeeUnits_test.cpp
    src/test/basics/hardened_hash_test.cpp
    src/test/basics/mulDiv_test.cpp
    src/test/basics/tagged_integer_test.cpp
    #[===============================[
       test sources:
         subdir: beast
    #]===============================]
    src/test/beast/IPEndpoint_test.cpp
    src/test/beast/LexicalCast_test.cpp
    src/test/beast/SemanticVersion_test.cpp
    src/test/beast/aged_associative_container_test.cpp
    src/test/beast/beast_CurrentThreadName_test.cpp
    src/test/beast/beast_Journal_test.cpp
    src/test/beast/beast_PropertyStream_test.cpp
    src/test/beast/beast_Zero_test.cpp
    src/test/beast/beast_abstract_clock_test.cpp
    src/test/beast/beast_basic_seconds_clock_test.cpp
    src/test/beast/beast_io_latency_probe_test.cpp
    src/test/beast/define_print.cpp
    #[===============================[
       test sources:
         subdir: conditions
    #]===============================]
    src/test/conditions/PreimageSha256_test.cpp
    #[===============================[
       test sources:
         subdir: consensus
    #]===============================]
    src/test/consensus/ByzantineFailureSim_test.cpp
    src/test/consensus/Consensus_test.cpp
    src/test/consensus/DistributedValidatorsSim_test.cpp
    src/test/consensus/LedgerTiming_test.cpp
    src/test/consensus/LedgerTrie_test.cpp
    src/test/consensus/NegativeUNL_test.cpp
    src/test/consensus/ScaleFreeSim_test.cpp
    src/test/consensus/Validations_test.cpp
    #[===============================[
       test sources:
         subdir: core
    #]===============================]
    src/test/core/ClosureCounter_test.cpp
    src/test/core/Config_test.cpp
    src/test/core/Coroutine_test.cpp
    src/test/core/CryptoPRNG_test.cpp
    src/test/core/JobQueue_test.cpp
    src/test/core/SociDB_test.cpp
    src/test/core/Workers_test.cpp
    #[===============================[
       test sources:
         subdir: csf
    #]===============================]
    src/test/csf/BasicNetwork_test.cpp
    src/test/csf/Digraph_test.cpp
    src/test/csf/Histogram_test.cpp
    src/test/csf/Scheduler_test.cpp
    src/test/csf/impl/Sim.cpp
    src/test/csf/impl/ledgers.cpp
    #[===============================[
       test sources:
         subdir: json
    #]===============================]
    src/test/json/Object_test.cpp
    src/test/json/Output_test.cpp
    src/test/json/Writer_test.cpp
    src/test/json/json_value_test.cpp
    #[===============================[
       test sources:
         subdir: jtx
    #]===============================]
    src/test/jtx/Env_test.cpp
    src/test/jtx/WSClient_test.cpp
    src/test/jtx/impl/Account.cpp
    src/test/jtx/impl/Env.cpp
    src/test/jtx/impl/JSONRPCClient.cpp
    src/test/jtx/impl/ManualTimeKeeper.cpp
    src/test/jtx/impl/WSClient.cpp
    src/test/jtx/impl/acctdelete.cpp
    src/test/jtx/impl/account_txn_id.cpp
    src/test/jtx/impl/amount.cpp
    src/test/jtx/impl/balance.cpp
    src/test/jtx/impl/check.cpp
    src/test/jtx/impl/delivermin.cpp
    src/test/jtx/impl/deposit.cpp
    src/test/jtx/impl/envconfig.cpp
    src/test/jtx/impl/fee.cpp
    src/test/jtx/impl/flags.cpp
    src/test/jtx/impl/invoice_id.cpp
    src/test/jtx/impl/jtx_json.cpp
    src/test/jtx/impl/last_ledger_sequence.cpp
    src/test/jtx/impl/memo.cpp
    src/test/jtx/impl/multisign.cpp
    src/test/jtx/impl/offer.cpp
    src/test/jtx/impl/owners.cpp
    src/test/jtx/impl/paths.cpp
    src/test/jtx/impl/pay.cpp
    src/test/jtx/impl/quality2.cpp
    src/test/jtx/impl/rate.cpp
    src/test/jtx/impl/regkey.cpp
    src/test/jtx/impl/sendmax.cpp
    src/test/jtx/impl/seq.cpp
    src/test/jtx/impl/sig.cpp
    src/test/jtx/impl/tag.cpp
    src/test/jtx/impl/ticket.cpp
    src/test/jtx/impl/trust.cpp
    src/test/jtx/impl/txflags.cpp
    src/test/jtx/impl/utility.cpp

    #[===============================[
       test sources:
         subdir: ledger
    #]===============================]
    src/test/ledger/BookDirs_test.cpp
    src/test/ledger/Directory_test.cpp
    src/test/ledger/Invariants_test.cpp
    src/test/ledger/PaymentSandbox_test.cpp
    src/test/ledger/PendingSaves_test.cpp
    src/test/ledger/SkipList_test.cpp
    src/test/ledger/View_test.cpp
    #[===============================[
       test sources:
         subdir: net
    #]===============================]
    src/test/net/DatabaseDownloader_test.cpp
    #[===============================[
       test sources:
         subdir: nodestore
    #]===============================]
    src/test/nodestore/Backend_test.cpp
    src/test/nodestore/Basics_test.cpp
    src/test/nodestore/DatabaseShard_test.cpp
    src/test/nodestore/Database_test.cpp
    src/test/nodestore/Timing_test.cpp
    src/test/nodestore/import_test.cpp
    src/test/nodestore/varint_test.cpp
    #[===============================[
       test sources:
         subdir: overlay
    #]===============================]
    src/test/overlay/ProtocolVersion_test.cpp
    src/test/overlay/cluster_test.cpp
    src/test/overlay/short_read_test.cpp
    src/test/overlay/compression_test.cpp
    src/test/overlay/reduce_relay_test.cpp
    src/test/overlay/handshake_test.cpp
    src/test/overlay/tx_reduce_relay_test.cpp
    #[===============================[
       test sources:
         subdir: peerfinder
    #]===============================]
    src/test/peerfinder/Livecache_test.cpp
    src/test/peerfinder/PeerFinder_test.cpp
    #[===============================[
       test sources:
         subdir: protocol
    #]===============================]
    src/test/protocol/BuildInfo_test.cpp
    src/test/protocol/InnerObjectFormats_test.cpp
    src/test/protocol/Issue_test.cpp
    src/test/protocol/KnownFormatToGRPC_test.cpp
    src/test/protocol/PublicKey_test.cpp
    src/test/protocol/Quality_test.cpp
    src/test/protocol/STAccount_test.cpp
    src/test/protocol/STAmount_test.cpp
    src/test/protocol/STObject_test.cpp
    src/test/protocol/STTx_test.cpp
    src/test/protocol/STValidation_test.cpp
    src/test/protocol/SecretKey_test.cpp
    src/test/protocol/Seed_test.cpp
    src/test/protocol/SeqProxy_test.cpp
    src/test/protocol/TER_test.cpp
    src/test/protocol/types_test.cpp
    #[===============================[
       test sources:
         subdir: resource
    #]===============================]
    src/test/resource/Logic_test.cpp
    #[===============================[
       test sources:
         subdir: rpc
    #]===============================]
    src/test/rpc/AccountCurrencies_test.cpp
    src/test/rpc/AccountInfo_test.cpp
    src/test/rpc/AccountLinesRPC_test.cpp
    src/test/rpc/AccountObjects_test.cpp
    src/test/rpc/AccountOffers_test.cpp
    src/test/rpc/AccountSet_test.cpp
    src/test/rpc/AccountTx_test.cpp
    src/test/rpc/AmendmentBlocked_test.cpp
    src/test/rpc/Book_test.cpp
    src/test/rpc/DepositAuthorized_test.cpp
    src/test/rpc/DeliveredAmount_test.cpp
    src/test/rpc/Feature_test.cpp
    src/test/rpc/Fee_test.cpp
    src/test/rpc/GatewayBalances_test.cpp
    src/test/rpc/GetCounts_test.cpp
    src/test/rpc/JSONRPC_test.cpp
    src/test/rpc/KeyGeneration_test.cpp
    src/test/rpc/LedgerClosed_test.cpp
    src/test/rpc/LedgerData_test.cpp
    src/test/rpc/LedgerRPC_test.cpp
    src/test/rpc/LedgerRequestRPC_test.cpp
    src/test/rpc/ManifestRPC_test.cpp
    src/test/rpc/NodeToShardRPC_test.cpp
    src/test/rpc/NoRippleCheck_test.cpp
    src/test/rpc/NoRipple_test.cpp
    src/test/rpc/OwnerInfo_test.cpp
    src/test/rpc/Peers_test.cpp
    src/test/rpc/ReportingETL_test.cpp
    src/test/rpc/Roles_test.cpp
    src/test/rpc/RPCCall_test.cpp
    src/test/rpc/RPCOverload_test.cpp
    src/test/rpc/RobustTransaction_test.cpp
    src/test/rpc/ServerInfo_test.cpp
    src/test/rpc/ShardArchiveHandler_test.cpp
    src/test/rpc/Status_test.cpp
    src/test/rpc/Submit_test.cpp
    src/test/rpc/Subscribe_test.cpp
    src/test/rpc/Transaction_test.cpp
    src/test/rpc/TransactionEntry_test.cpp
    src/test/rpc/TransactionHistory_test.cpp
    src/test/rpc/Tx_test.cpp
    src/test/rpc/ValidatorInfo_test.cpp
    src/test/rpc/ValidatorRPC_test.cpp
    src/test/rpc/Version_test.cpp
    #[===============================[
       test sources:
         subdir: server
    #]===============================]
    src/test/server/ServerStatus_test.cpp
    src/test/server/Server_test.cpp
    #[===============================[
       test sources:
         subdir: shamap
    #]===============================]
    src/test/shamap/FetchPack_test.cpp
    src/test/shamap/SHAMapSync_test.cpp
    src/test/shamap/SHAMap_test.cpp
    #[===============================[
       test sources:
         subdir: unit_test
    #]===============================]
    src/test/unit_test/multi_runner.cpp)
endif () #tests

target_link_libraries (xrpld
  Xrpl::boost
  Xrpl::opts
  Xrpl::libs
  Xrpl::xrpl_core
  )
exclude_if_included (xrpld)
# define a macro for tests that might need to
# be exluded or run differently in CI environment
if (is_ci)
  target_compile_definitions(xrpld PRIVATE xrpld_RUNNING_IN_CI)
endif ()

if (reporting)
    target_compile_definitions(xrpld PRIVATE xrpld_REPORTING)
endif ()

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.16)
  # any files that don't play well with unity should be added here
  if (tests)
    set_source_files_properties(
      # these two seem to produce conflicts in beast teardown template methods
      src/test/rpc/ValidatorRPC_test.cpp
      src/test/rpc/ShardArchiveHandler_test.cpp
      PROPERTIES SKIP_UNITY_BUILD_INCLUSION TRUE)
  endif () #tests
endif ()
