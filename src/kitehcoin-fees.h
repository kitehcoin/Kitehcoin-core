// Copyright (c) 2021 The Dogecoin Core developers
// Copyright (c) 2023-2024 The Pepecoin Core developers
// Copyright (c) 2024-2025 The Kitehcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_KITEHCOIN_FEES_H
#define BITCOIN_KITEHCOIN_FEES_H

#include "amount.h"
#include "chain.h"
#include "chainparams.h"

#ifdef ENABLE_WALLET

enum FeeRatePreset
{
    MINIMUM,
    LOW,
    MODERATE,
    HIGH,
    VERY_HIGH,
    MAXIMUM
};

/** Estimate fee rate needed to get into the next nBlocks */
CFeeRate GetKitehcoinFeeRate(int priority);
const std::string GetKitehcoinPriorityLabel(int priority);
#endif // ENABLE_WALLET
CAmount GetKitehcoinMinRelayFee(const CTransaction& tx, unsigned int nBytes, bool fAllowFree);
CAmount GetKitehcoinDustFee(const std::vector<CTxOut> &vout, const CAmount dustLimit);

#endif // BITCOIN_KITEHCOIN_FEES_H
