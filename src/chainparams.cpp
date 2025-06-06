// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2022 The Dogecoin Core developers
// Copyright (c) 2023-2024 The Pepecoin Core developers
// Copyright (c) 2024-2025 The Kitehcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"
#include "arith_uint256.h"
#include "arith_uint256.h"
static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1386325540, nBits=0x1e0ffff0, nNonce=99943, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Kitehcoin lucky KITEH 2025";
    const CScript genesisOutputScript = CScript() << ParseHex("049a08bf7e2c78e2a238316a284933c33af5758c6ceda4ad7aeace12e5e9f43a338bf93240445dfbdb0b8182d6c77730901bb5a7b2f993d18c04d91e11b07018a2") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
private:
    Consensus::Params digishieldConsensus;
    Consensus::Params auxpowConsensus;
public:
    CMainParams() {
        strNetworkID = "main";

        consensus.nSubsidyHalvingInterval = 40000;
        consensus.nMajorityEnforceBlockUpgrade = 1500;
        consensus.nMajorityRejectBlockOutdated = 1900;
        consensus.nMajorityWindow = 2000;
        // BIP34 is never enforced in kiteh v2 blocks, so we enforce from v3
        consensus.BIP34Height = 1000;
        consensus.BIP34Hash = uint256S(""); // block 1000
        consensus.BIP65Height = 1000;
        consensus.BIP66Height = 1000;
        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 20;
        consensus.nPowTargetTimespan = 60; // 1 minute
        consensus.nPowTargetSpacing = 60; // 1 minute
        consensus.fDigishieldDifficultyCalculation = true;
        consensus.nCoinbaseMaturity = 100;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowAllowDigishieldMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 9576; // 95% of 10,080
        consensus.nMinerConfirmationWindow = 10080; // 60 * 24 * 7 = 10,080 blocks, or one week
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        // XXX: BIP heights and hashes all need to be updated to kiteh values
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1462060800; // May 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1479168000; // November 15th, 2016.
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 0; // Disabled

        // The best chain should have at least this much work.
	    consensus.nMinimumChainWork = uint256S("00000000000000000000000000000000000000000000000000008f31d5bb43de"); // block 2000
        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x6d1c5bd5f0616a27788e9b8cdeb6df23914b24846a12fa54794d45513e5c6700"); // block 2000

        // AuxPoW parameters
        consensus.nAuxpowChainId = 0x0097; 
        consensus.fStrictChainId = true;
        consensus.fAllowLegacyBlocks = true;
        consensus.nHeightEffective = 0;
        consensus.fSimplifiedRewards = true;

        // Blocks 1000 - 41,999 are Digishield without AuxPoW
        digishieldConsensus = consensus;
        digishieldConsensus.nHeightEffective = 1000;
        digishieldConsensus.fSimplifiedRewards = true;
        digishieldConsensus.fDigishieldDifficultyCalculation = true;
        digishieldConsensus.nPowTargetTimespan = 60; // post-digishield: 1 minute
        digishieldConsensus.nCoinbaseMaturity = 240;

        // Blocks 42,000+ are AuxPoW
        // Some tests from Dogecoin expect non-auxpow blocks. This allows those tests to pass.
        auxpowConsensus = digishieldConsensus;
        auxpowConsensus.nHeightEffective = 30000;
        auxpowConsensus.fAllowLegacyBlocks = false;

        // 10% of Block reward to operations wallet
        nDeveloperFee = 10;

        // This is the block height when the devfee starts working
        nDeveloperFeeStart = 32000;

        // Developer wallet address
        strDeveloperFeeAddress = "K9Ad8NNz8r97tE6z7nT1NuXSsVuGGhQjxn";
        
        // Assemble the binary search tree of consensus parameters
        pConsensusRoot = &digishieldConsensus;
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &auxpowConsensus;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x4B;
        pchMessageStart[1] = 0x49;
        pchMessageStart[2] = 0x54;
        pchMessageStart[3] = 0x45;
        nDefaultPort = 15034;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1748113735, 4294515, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        assert(consensus.hashGenesisBlock == uint256S("0x37bb851f85fcc70d663698ddd5d638b57625579ba6c241aeaa646a746d42810e")); // Hash of the network genesis block mainnet
        assert(genesis.hashMerkleRoot == uint256S("0x24c0b5b96e915f703200c1c5dc6108a1012b923c495c1e89d00667aa150787e9")); // markle hash of the network genesis block mainnet

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.push_back(CDNSSeedData("seed.kitehcoin.org", "seeder.kitehcoin.org"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 45); // Addresses start with K
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 33); 
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1, 45);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0xfa)(0xda)(0xfe).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0xfa)(0xc4)(0x97).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x37bb851f85fcc70d663698ddd5d638b57625579ba6c241aeaa646a746d42810e")) // Hash of the network genesis block mainnet
            ( 1000, uint256S("0x65e437411125c9689a02acb004ce9c37a13fe34852e66b463327bdfc1a503d62"))
            ( 2000, uint256S("0x6d1c5bd5f0616a27788e9b8cdeb6df23914b24846a12fa54794d45513e5c6700"))
        };

        chainTxData = ChainTxData{ };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
private:
    Consensus::Params digishieldConsensus;
    Consensus::Params auxpowConsensus;
    Consensus::Params minDifficultyConsensus;
public:
    CTestNetParams() {
        strNetworkID = "test";

        consensus.nSubsidyHalvingInterval = 50000;
        consensus.nMajorityEnforceBlockUpgrade = 501;
        consensus.nMajorityRejectBlockOutdated = 750;
        consensus.nMajorityWindow = 1000;
        // BIP34 is never enforced in Kitehcoin v2 blocks, so we enforce from v3
        consensus.BIP34Height = 1000;
        consensus.BIP34Hash = uint256S("0x00");
        consensus.BIP65Height = 1000; // 
        consensus.BIP66Height = 1000; // - this is the last block that could be v2, 1900 blocks past the last v2 block
        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 20;
        consensus.nPowTargetTimespan = 60; // 1 minute
        consensus.nPowTargetSpacing = 60; // 1 minute
        consensus.fDigishieldDifficultyCalculation = true;
        consensus.nCoinbaseMaturity = 30;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowAllowDigishieldMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 2880; // 2 days (note this is significantly lower than Bitcoin standard)
        consensus.nMinerConfirmationWindow = 10080; // 60 * 24 * 7 = 10,080 blocks, or one week
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        // XXX: BIP heights and hashes all need to be updated to Kitehcoin values
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1456790400; // March 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1462060800; // May 1st 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 0; // Disabled

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0000000000000000000000000000000000000000000000000000000000100010"); // 0

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x8c72963d5c1338b3faa17be6b141042ea7cf3b20801b63cb9ffbeda498d53b31"); // 0

        // AuxPoW parameters
        consensus.nAuxpowChainId = 0x0097; 
        consensus.fStrictChainId = false;
        consensus.nHeightEffective = 0;
        consensus.fAllowLegacyBlocks = true;
        consensus.fSimplifiedRewards = true;

        // Blocks 1000 - 1249 are Digishield without minimum difficulty on all blocks
        digishieldConsensus = consensus;
        digishieldConsensus.nHeightEffective = 1000;
        digishieldConsensus.nPowTargetTimespan = 60; // post-digishield: 1 minute
        digishieldConsensus.fDigishieldDifficultyCalculation = true;
        digishieldConsensus.fSimplifiedRewards = true;
        digishieldConsensus.fPowAllowMinDifficultyBlocks = false;
        digishieldConsensus.nCoinbaseMaturity = 240;

        // Blocks 1250 - 41,999 are Digishield with minimum difficulty on all blocks
        minDifficultyConsensus = digishieldConsensus;
        minDifficultyConsensus.nHeightEffective = 1250;
        minDifficultyConsensus.fPowAllowDigishieldMinDifficultyBlocks = true;
        minDifficultyConsensus.fPowAllowMinDifficultyBlocks = true;

        // Enable AuxPoW at 42,000
        auxpowConsensus = minDifficultyConsensus;
        auxpowConsensus.nHeightEffective = 42000;
        auxpowConsensus.fPowAllowDigishieldMinDifficultyBlocks = true;
        auxpowConsensus.fAllowLegacyBlocks = false;

        // Assemble the binary search tree of parameters
        pConsensusRoot = &digishieldConsensus;
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &minDifficultyConsensus;
        minDifficultyConsensus.pRight = &auxpowConsensus;

        pchMessageStart[0] = 0xaf; // Or any other value not used by other networks
        pchMessageStart[1] = 0x12;
        pchMessageStart[2] = 0x34;
        pchMessageStart[3] = 0x56;        
        nDefaultPort = 14328;
        nPruneAfterHeight = 10000;

        genesis = CreateGenesisBlock(1748115266, 2379562, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        minDifficultyConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;

        // should i disable this?
        // calculate main genesis block
        // consensus.hashGenesisBlock = uint256S("0x00");
        if (true && (genesis.GetHash() != consensus.hashGenesisBlock)) {
            std::cout << std::string("Calculating main genesis block...\n");
                arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
                uint256 hash;
                genesis.nNonce = 0;
                while (UintToArith256(genesis.GetHash()) > hashTarget)
                {
                    ++genesis.nNonce;
                    if (genesis.nNonce == 0)
                    {
                        ++genesis.nTime;
                    }
                }
                std::cout << "Genesis block found!\n";
                std::cout << "nonce: " << genesis.nNonce << "\n";
                std::cout << "time: " << genesis.nTime << "\n";
                std::cout << "blockhash: " << genesis.GetHash().ToString().c_str() << "\n";
                std::cout << "merklehash: " << genesis.hashMerkleRoot.ToString().c_str() << "\n";
            }

        assert(consensus.hashGenesisBlock == uint256S("0x8c72963d5c1338b3faa17be6b141042ea7cf3b20801b63cb9ffbeda498d53b31"));
        assert(genesis.hashMerkleRoot == uint256S("0x24c0b5b96e915f703200c1c5dc6108a1012b923c495c1e89d00667aa150787e9")); // markle hash of the network genesis block 

        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        // vSeeds.push_back(CDNSSeedData("kitehscoin.io", "seeds-testnet.kitehscoin.io"));
        // vSeeds.push_back(CDNSSeedData("kitehblocks.com", "seeds-testnet.kitehblocks.com"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,113); // 0x71
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196); // 0xc4
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,241); // 0xf1
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xcf).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        // vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x8c72963d5c1338b3faa17be6b141042ea7cf3b20801b63cb9ffbeda498d53b31"))
        };

        chainTxData = ChainTxData{ };
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
private:
    Consensus::Params digishieldConsensus;
    Consensus::Params auxpowConsensus;
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 1;
        consensus.nPowTargetTimespan = 4 * 60 * 60; // pre-digishield: 4 hours
        consensus.nPowTargetSpacing = 1; // regtest: 1 second blocks
        consensus.fDigishieldDifficultyCalculation = false;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 540; // 75% for testchains
        consensus.nMinerConfirmationWindow = 720; // Faster than normal for regtest (2,520 instead of 10,080)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00"); // test 0 block ("0000000000000000000000000000000000000000000000000000000000000002")

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00"); // test 0 block ("0x407124744afb4224c67737340d786a1e6879f4e0431338605702b7f813b6faf9")

        // AuxPow parameters
        consensus.nAuxpowChainId = 0x0097; 
        consensus.fStrictChainId = true;
        consensus.fAllowLegacyBlocks = true;

        // Kitehcoin parameters
        consensus.fSimplifiedRewards = true;
        consensus.nCoinbaseMaturity = 60; // For easier testability in RPC tests

        digishieldConsensus = consensus;
        digishieldConsensus.nHeightEffective = 10;
        digishieldConsensus.nPowTargetTimespan = 1; // regtest: also retarget every second in digishield mode, for conformity
        digishieldConsensus.fDigishieldDifficultyCalculation = true;

        auxpowConsensus = digishieldConsensus;
        auxpowConsensus.fAllowLegacyBlocks = false;
        auxpowConsensus.nHeightEffective = 20;

        // Assemble the binary search tree of parameters
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &auxpowConsensus;
        pConsensusRoot = &digishieldConsensus;

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 14329;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1748116055, 0, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;

        // Mine Genesis(genesis, consensus.powLimit, true);
        // calculate main genesis block
        // consensus.hashGenesisBlock = uint256S("0x00");
        if (true && (genesis.GetHash() != consensus.hashGenesisBlock)) {
            std::cout << std::string("Calculating main genesis block...\n");
                arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
                uint256 hash;
                genesis.nNonce = 0;
                while (UintToArith256(genesis.GetHash()) > hashTarget)
                {
                    ++genesis.nNonce;
                    if (genesis.nNonce == 0)
                    {
                        ++genesis.nTime;
                    }
                }
                std::cout << "Genesis block found!\n";
                std::cout << "nonce: " << genesis.nNonce << "\n";
                std::cout << "time: " << genesis.nTime << "\n";
                std::cout << "blockhash: " << genesis.GetHash().ToString().c_str() << "\n";
                std::cout << "merklehash: " << genesis.hashMerkleRoot.ToString().c_str() << "\n";
            }

        assert(consensus.hashGenesisBlock == uint256S("0xa02aa95bb6afa4993c746c8aac82b8a0dc82106107eb7e6cb72b2d619b9c64f3"));
        assert(genesis.hashMerkleRoot == uint256S("0x24c0b5b96e915f703200c1c5dc6108a1012b923c495c1e89d00667aa150787e9")); // markle hash of the network genesis block

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0xa02aa95bb6afa4993c746c8aac82b8a0dc82106107eb7e6cb72b2d619b9c64f3"))
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);  // 0x6f
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);  // 0xc4
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);  // 0xef
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

const Consensus::Params *Consensus::Params::GetConsensus(uint32_t nTargetHeight) const {
    if (nTargetHeight < this -> nHeightEffective && this -> pLeft != NULL) {
        return this -> pLeft -> GetConsensus(nTargetHeight);
    } else if (nTargetHeight > this -> nHeightEffective && this -> pRight != NULL) {
        const Consensus::Params *pCandidate = this -> pRight -> GetConsensus(nTargetHeight);
        if (pCandidate->nHeightEffective <= nTargetHeight) {
            return pCandidate;
        }
    }

    // No better match below the target height
    return this;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
