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
  'ripple.NodeStore.Timing'
  'ripple.app.Flow_manual'
  'ripple.app.NoRippleCheckLimits'
  'ripple.app.PayStrandAllPairs'
  'ripple.consensus.ByzantineFailureSim'
  'ripple.consensus.DistributedValidators'
  'ripple.consensus.ScaleFreeSim'
  'ripple.tx.CrossingLimits'
  'ripple.tx.FindOversizeCross'
  'ripple.tx.Offer_manual'
  'ripple.tx.OversizeMeta'
  'ripple.tx.PlumpBook'
)

if [[ ${MANUAL_TESTS} == 'true' ]]; then
  XRPLD_ARGS+=" --unittest=$(join_by , "${manual_tests[@]}")"
else
  XRPLD_ARGS+=" --unittest --quiet --unittest-log"
fi
XRPLD_ARGS+=" --unittest-jobs ${CONCURRENT_TESTS}"

${XRPLD} ${XRPLD_ARGS}
