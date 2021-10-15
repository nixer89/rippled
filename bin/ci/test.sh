#!/usr/bin/env bash

set -o xtrace
set -o errexit

# Set to 'true' to run the known "manual" tests in xrpld.
MANUAL_TESTS=${MANUAL_TESTS:-false}
# The maximum number of concurrent tests.
CONCURRENT_TESTS=${CONCURRENT_TESTS:-$(nproc)}
# The path to xrpld.
XRPLD=${XRPLD:-build/xrpld}
# Additional arguments to xrpld.
XRPLD_ARGS=${XRPLD_ARGS:-}

function join_by { local IFS="$1"; shift; echo "$*"; }

declare -a manual_tests=(
  'beast.chrono.abstract_clock'
  'beast.unit_test.print'
  'xrpl.NodeStore.Timing'
  'xrpl.app.Flow_manual'
  'xrpl.app.NoRippleCheckLimits'
  'xrpl.app.PayStrandAllPairs'
  'xrpl.consensus.ByzantineFailureSim'
  'xrpl.consensus.DistributedValidators'
  'xrpl.consensus.ScaleFreeSim'
  'xrpl.tx.CrossingLimits'
  'xrpl.tx.FindOversizeCross'
  'xrpl.tx.Offer_manual'
  'xrpl.tx.OversizeMeta'
  'xrpl.tx.PlumpBook'
)

if [[ ${MANUAL_TESTS} == 'true' ]]; then
  XRPLD_ARGS+=" --unittest=$(join_by , "${manual_tests[@]}")"
else
  XRPLD_ARGS+=" --unittest --quiet --unittest-log"
fi
XRPLD_ARGS+=" --unittest-jobs ${CONCURRENT_TESTS}"

${XRPLD} ${XRPLD_ARGS}
