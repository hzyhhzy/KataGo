
//-------------------------------------------------------------------------------------
//This file contains various functions for extracting stats and results from the search, choosing a move, etc
//-------------------------------------------------------------------------------------

#include "../search/search.h"

#include <inttypes.h>

#include "../core/fancymath.h"
#include "../program/playutils.h"

using namespace std;
using nlohmann::json;

static const int64_t MIN_VISITS_FOR_LCB = 3;

bool Search::getPlaySelectionValues(
  vector<Loc>& locs, vector<double>& playSelectionValues, double scaleMaxToAtLeast
) const {
  if(rootNode == NULL) {
    locs.clear();
    playSelectionValues.clear();
    return false;
  }
  bool allowDirectPolicyMoves = true;
  return getPlaySelectionValues(*rootNode, locs, playSelectionValues, NULL, scaleMaxToAtLeast, allowDirectPolicyMoves);
}

bool Search::getPlaySelectionValues(
  vector<Loc>& locs, vector<double>& playSelectionValues, vector<double>* retVisitCounts, double scaleMaxToAtLeast
) const {
  if(rootNode == NULL) {
    locs.clear();
    playSelectionValues.clear();
    if(retVisitCounts != NULL)
      retVisitCounts->clear();
    return false;
  }
  bool allowDirectPolicyMoves = true;
  return getPlaySelectionValues(*rootNode, locs, playSelectionValues, retVisitCounts, scaleMaxToAtLeast, allowDirectPolicyMoves);
}

bool Search::getPlaySelectionValues(
  const SearchNode& node,
  vector<Loc>& locs, vector<double>& playSelectionValues, vector<double>* retVisitCounts, double scaleMaxToAtLeast,
  bool allowDirectPolicyMoves
) const {
  double lcbBuf[NNPos::MAX_NN_POLICY_SIZE];
  double radiusBuf[NNPos::MAX_NN_POLICY_SIZE];
  bool result = getPlaySelectionValues(
    node,locs,playSelectionValues,retVisitCounts,scaleMaxToAtLeast,allowDirectPolicyMoves,
    false,false,lcbBuf,radiusBuf
  );
  return result;
}

bool Search::getPlaySelectionValues(
  const SearchNode& node,
  vector<Loc>& locs, vector<double>& playSelectionValues, vector<double>* retVisitCounts, double scaleMaxToAtLeast,
  bool allowDirectPolicyMoves, bool alwaysComputeLcb, bool neverUseLcb,
  //Note: lcbBuf is signed from the player to move's perspective
  double lcbBuf[NNPos::MAX_NN_POLICY_SIZE], double radiusBuf[NNPos::MAX_NN_POLICY_SIZE]
) const {
  locs.clear();
  playSelectionValues.clear();
  if(retVisitCounts != NULL)
    retVisitCounts->clear();

  double totalChildWeight = 0.0;
  double maxChildWeight = 0.0;
  const bool suppressPass = shouldSuppressPass(&node);

  //Store up basic weights
  int childrenCapacity;
  const SearchChildPointer* children = node.getChildren(childrenCapacity);
  for(int i = 0; i<childrenCapacity; i++) {
    const SearchNode* child = children[i].getIfAllocated();
    if(child == NULL)
      break;
    Loc moveLoc = child->prevMoveLoc;

    int64_t childVisits = child->stats.visits.load(std::memory_order_acquire);
    double childWeight = child->stats.weightSum.load(std::memory_order_acquire);

    locs.push_back(moveLoc);
    totalChildWeight += childWeight;
    if(childWeight > maxChildWeight)
      maxChildWeight = childWeight;
    if(suppressPass && moveLoc == Board::PASS_LOC) {
      playSelectionValues.push_back(0.0);
      if(retVisitCounts != NULL)
        (*retVisitCounts).push_back(0.0);
    }
    else {
      playSelectionValues.push_back((double)childWeight);
      if(retVisitCounts != NULL)
        (*retVisitCounts).push_back((double)childVisits);
    }
  }

  int numChildren = playSelectionValues.size();

  //Find the best child by weight
  int mostWeightedIdx = 0;
  double mostWeightedChildWeight = -1e30;
  for(int i = 0; i<numChildren; i++) {
    double value = playSelectionValues[i];
    if(value > mostWeightedChildWeight) {
      mostWeightedChildWeight = value;
      mostWeightedIdx = i;
    }
  }

  //Possibly reduce weight on children that we spend too many visits on in retrospect
  if(&node == rootNode && numChildren > 0) {

    const SearchNode* bestChild = children[mostWeightedIdx].getIfAllocated();
    assert(bestChild != NULL);
    const bool isRoot = true;
    const double policyProbMassVisited = 1.0; //doesn't matter, since fpu value computed from it isn't used here
    double parentUtility;
    double parentWeightPerVisit;
    double parentUtilityStdevFactor;
    double fpuValue = getFpuValueForChildrenAssumeVisited(
      node, rootPla, isRoot, policyProbMassVisited,
      parentUtility, parentWeightPerVisit, parentUtilityStdevFactor
    );

    bool isDuringSearch = false;

    const NNOutput* nnOutput = node.getNNOutput();
    assert(nnOutput != NULL);
    const float* policyProbs = nnOutput->getPolicyProbsMaybeNoised();
    double bestChildExploreSelectionValue = getExploreSelectionValue(
      node,policyProbs,bestChild,totalChildWeight,fpuValue,
      parentUtility,parentWeightPerVisit,parentUtilityStdevFactor,
      isDuringSearch,false,maxChildWeight,NULL
    );

    for(int i = 0; i<numChildren; i++) {
      const SearchNode* child = children[i].getIfAllocated();
      if(suppressPass && child->prevMoveLoc == Board::PASS_LOC) {
        playSelectionValues[i] = 0;
        continue;
      }
      if(i != mostWeightedIdx) {
        double reduced = getReducedPlaySelectionWeight(
          node, policyProbs, child,
          totalChildWeight, parentUtilityStdevFactor, bestChildExploreSelectionValue
        );
        playSelectionValues[i] = (int64_t)ceil(reduced);
      }
    }
  }

  //Now compute play selection values taking into account LCB
  if(!neverUseLcb && (alwaysComputeLcb || (searchParams.useLcbForSelection && numChildren > 0))) {
    double bestLcb = -1e10;
    int bestLcbIndex = -1;
    for(int i = 0; i<numChildren; i++) {
      const SearchNode* child = children[i].getIfAllocated();
      getSelfUtilityLCBAndRadius(node,child,lcbBuf[i],radiusBuf[i]);
      //Check if this node is eligible to be considered for best LCB
      double weight = playSelectionValues[i];
      if(weight >= MIN_VISITS_FOR_LCB && weight >= searchParams.minVisitPropForLCB * mostWeightedChildWeight) {
        if(lcbBuf[i] > bestLcb) {
          bestLcb = lcbBuf[i];
          bestLcbIndex = i;
        }
      }
    }

    if(searchParams.useLcbForSelection && numChildren > 0 && (searchParams.useNonBuggyLcb ? (bestLcbIndex >= 0) : (bestLcbIndex > 0))) {
      //Best LCB move gets a bonus that ensures it is large enough relative to every other child
      double adjustedWeight = playSelectionValues[bestLcbIndex];
      for(int i = 0; i<numChildren; i++) {
        if(i != bestLcbIndex) {
          double excessValue = bestLcb - lcbBuf[i];
          //This move is actually worse lcb than some other move, it's just that the other
          //move failed its checks for having enough minimum weight. So don't actually
          //try to compute how much better this one is than that one, because it's not better.
          if(excessValue < 0)
            continue;

          double radius = radiusBuf[i];
          //How many times wider would the radius have to be before the lcb would be worse?
          //Add adjust the denom so that we cannot possibly gain more than a factor of 5, just as a guard
          double radiusFactor = (radius + excessValue) / (radius + 0.20 * excessValue);

          //That factor, squared, is the number of "weight" more that we should pretend we have, for
          //the purpose of selection, since normally stdev is proportional to 1/weight^2.
          double lbound = radiusFactor * radiusFactor * playSelectionValues[i];
          if(lbound > adjustedWeight)
            adjustedWeight = lbound;
        }
      }
      playSelectionValues[bestLcbIndex] = adjustedWeight;
    }
  }

  const NNOutput* nnOutput = node.getNNOutput();

  //If we have no children, then use the policy net directly. Only for the root, though, if calling this on any subtree
  //then just require that we have children, for implementation simplicity (since it requires that we have a board and a boardhistory too)
  //(and we also use isAllowedRootMove and avoidMoveUntilByLoc)
  if(numChildren == 0) {
    if(nnOutput == NULL || &node != rootNode || !allowDirectPolicyMoves)
      return false;

    bool obeyAllowedRootMove = true;
    while(true) {
      for(int movePos = 0; movePos<policySize; movePos++) {
        Loc moveLoc = NNPos::posToLoc(movePos,rootBoard.x_size,rootBoard.y_size,nnXLen,nnYLen);
        const float* policyProbs = nnOutput->getPolicyProbsMaybeNoised();
        double policyProb = policyProbs[movePos];
        if(!rootHistory.isLegal(rootBoard,moveLoc,rootPla) || policyProb < 0 || (obeyAllowedRootMove && !isAllowedRootMove(moveLoc)))
          continue;
        const std::vector<int>& avoidMoveUntilByLoc = rootPla == P_BLACK ? avoidMoveUntilByLocBlack : avoidMoveUntilByLocWhite;
        if(avoidMoveUntilByLoc.size() > 0) {
          assert(avoidMoveUntilByLoc.size() >= Board::MAX_ARR_SIZE);
          int untilDepth = avoidMoveUntilByLoc[moveLoc];
          if(untilDepth > 0)
            continue;
        }
        locs.push_back(moveLoc);
        playSelectionValues.push_back(policyProb);
        numChildren++;
      }
      //Still no children? Then at this point just ignore isAllowedRootMove.
      if(numChildren == 0 && obeyAllowedRootMove) {
        obeyAllowedRootMove = false;
        continue;
      }
      break;
    }
  }

  //Might happen absurdly rarely if we both have no children and don't properly have an nnOutput
  //but have a hash collision or something so we "found" an nnOutput anyways.
  //Could also happen if we have avoidMoveUntilByLoc pruning all the allowed moves.
  if(numChildren == 0)
    return false;

  double maxValue = 0.0;
  for(int i = 0; i<numChildren; i++) {
    if(playSelectionValues[i] > maxValue)
      maxValue = playSelectionValues[i];
  }

  if(maxValue <= 1e-50)
    return false;

  //Sanity check - if somehow we had more than this, something must have overflowed or gone wrong
  assert(maxValue < 1e40);

  double amountToSubtract = std::min(searchParams.chosenMoveSubtract, maxValue/64.0);
  double amountToPrune = std::min(searchParams.chosenMovePrune, maxValue/64.0);
  double newMaxValue = maxValue - amountToSubtract;
  for(int i = 0; i<numChildren; i++) {
    if(playSelectionValues[i] < amountToPrune)
      playSelectionValues[i] = 0.0;
    else {
      playSelectionValues[i] -= amountToSubtract;
      if(playSelectionValues[i] <= 0.0)
        playSelectionValues[i] = 0.0;
    }
  }

  assert(newMaxValue > 0.0);

  if(newMaxValue < scaleMaxToAtLeast) {
    for(int i = 0; i<numChildren; i++) {
      playSelectionValues[i] *= scaleMaxToAtLeast / newMaxValue;
    }
  }

  return true;
}

void Search::maybeRecomputeNormToTApproxTable() {
  if(normToTApproxZ <= 0.0 || normToTApproxZ != searchParams.lcbStdevs || normToTApproxTable.size() <= 0) {
    normToTApproxZ = searchParams.lcbStdevs;
    normToTApproxTable.clear();
    for(int i = 0; i < 512; i++)
      normToTApproxTable.push_back(FancyMath::normToTApprox(normToTApproxZ,(double)(i+MIN_VISITS_FOR_LCB)));
  }
}

double Search::getNormToTApproxForLCB(int64_t numVisits) const {
  int64_t idx = numVisits-MIN_VISITS_FOR_LCB;
  assert(idx >= 0);
  if(idx >= normToTApproxTable.size())
    idx = normToTApproxTable.size()-1;
  return normToTApproxTable[idx];
}

void Search::getSelfUtilityLCBAndRadius(const SearchNode& parent, const SearchNode* child, double& lcbBuf, double& radiusBuf) const {
  double weightSum = child->stats.weightSum.load(std::memory_order_acquire);
  double weightSqSum = child->stats.weightSqSum.load(std::memory_order_acquire);
  double scoreMeanAvg = child->stats.scoreMeanAvg.load(std::memory_order_acquire);
  double scoreMeanSqAvg = child->stats.scoreMeanSqAvg.load(std::memory_order_acquire);
  double utilityAvg = child->stats.utilityAvg.load(std::memory_order_acquire);
  double utilitySqAvg = child->stats.utilitySqAvg.load(std::memory_order_acquire);

  radiusBuf = 2.0 * (searchParams.winLossUtilityFactor + searchParams.staticScoreUtilityFactor + searchParams.dynamicScoreUtilityFactor);
  lcbBuf = -radiusBuf;
  if(weightSum <= 0.0 || weightSqSum <= 0.0)
    return;

  double ess = weightSum * weightSum / weightSqSum;
  int64_t essInt = (int64_t)round(ess);
  if(essInt < MIN_VISITS_FOR_LCB)
    return;

  double utilityNoBonus = utilityAvg;
  double endingScoreBonus = getEndingWhiteScoreBonus(parent,child);
  double utilityDiff = getScoreUtilityDiff(scoreMeanAvg, scoreMeanSqAvg, endingScoreBonus);
  double utilityWithBonus = utilityNoBonus + utilityDiff;
  double selfUtility = parent.nextPla == P_WHITE ? utilityWithBonus : -utilityWithBonus;

  double utilityVariance = std::max(1e-8, utilitySqAvg - utilityNoBonus * utilityNoBonus);
  double estimateStdev = sqrt(utilityVariance / ess);
  double radius = estimateStdev * getNormToTApproxForLCB(essInt);

  lcbBuf = selfUtility - radius;
  radiusBuf = radius;
}

bool Search::getRootValues(ReportedSearchValues& values) const {
  return getNodeValues(rootNode,values);
}

ReportedSearchValues Search::getRootValuesRequireSuccess() const {
  ReportedSearchValues values;
  if(rootNode == NULL)
    throw StringError("Bug? Bot search root was null");
  bool success = getNodeValues(rootNode,values);
  if(!success)
    throw StringError("Bug? Bot search returned no root values");
  return values;
}

bool Search::getRootRawNNValues(ReportedSearchValues& values) const {
  if(rootNode == NULL)
    return false;
  return getNodeRawNNValues(*rootNode,values);
}

ReportedSearchValues Search::getRootRawNNValuesRequireSuccess() const {
  ReportedSearchValues values;
  if(rootNode == NULL)
    throw StringError("Bug? Bot search root was null");
  bool success = getNodeRawNNValues(*rootNode,values);
  if(!success)
    throw StringError("Bug? Bot search returned no root values");
  return values;
}

bool Search::getNodeRawNNValues(const SearchNode& node, ReportedSearchValues& values) const {
  const NNOutput* nnOutput = node.getNNOutput();
  if(nnOutput == NULL)
    return false;

  values.winValue = nnOutput->whiteWinProb;
  values.lossValue = nnOutput->whiteLossProb;
  values.noResultValue = nnOutput->whiteNoResultProb;

  double scoreMean = nnOutput->whiteScoreMean;
  double scoreMeanSq = nnOutput->whiteScoreMeanSq;
  double scoreStdev = getScoreStdev(scoreMean,scoreMeanSq);
  values.staticScoreValue = ScoreValue::expectedWhiteScoreValue(scoreMean,scoreStdev,0.0,2.0,rootBoard);
  values.dynamicScoreValue = ScoreValue::expectedWhiteScoreValue(scoreMean,scoreStdev,recentScoreCenter,searchParams.dynamicScoreCenterScale,rootBoard);
  values.expectedScore = scoreMean;
  values.expectedScoreStdev = scoreStdev;
  values.lead = nnOutput->whiteLead;

  //Sanity check
  assert(values.winValue >= 0.0);
  assert(values.lossValue >= 0.0);
  assert(values.noResultValue >= 0.0);
  assert(values.winValue + values.lossValue + values.noResultValue < 1.001);

  double winLossValue = values.winValue - values.lossValue;
  if(winLossValue > 1.0) winLossValue = 1.0;
  if(winLossValue < -1.0) winLossValue = -1.0;
  values.winLossValue = winLossValue;

  values.weight = computeWeightFromNNOutput(nnOutput);
  values.visits = 1;

  return true;
}


bool Search::getNodeValues(const SearchNode* node, ReportedSearchValues& values) const {
  if(node == NULL)
    return false;
  int64_t visits = node->stats.visits.load(std::memory_order_acquire);
  double weightSum = node->stats.weightSum.load(std::memory_order_acquire);
  double winLossValueAvg = node->stats.winLossValueAvg.load(std::memory_order_acquire);
  double noResultValueAvg = node->stats.noResultValueAvg.load(std::memory_order_acquire);
  double scoreMeanAvg = node->stats.scoreMeanAvg.load(std::memory_order_acquire);
  double scoreMeanSqAvg = node->stats.scoreMeanSqAvg.load(std::memory_order_acquire);
  double leadAvg = node->stats.leadAvg.load(std::memory_order_acquire);
  double utilityAvg = node->stats.utilityAvg.load(std::memory_order_acquire);

  if(weightSum <= 0.0)
    return false;
  assert(visits >= 0);
  if(node == rootNode) {
    //For terminal nodes, we may have no nnoutput and yet we have legitimate visits and terminal evals.
    //But for the root, the root is never treated as a terminal node and always gets an nneval, so if
    //it has visits and weight, it has an nnoutput unless something has gone wrong.
    const NNOutput* nnOutput = node->getNNOutput();
    assert(nnOutput != NULL);
    (void)nnOutput;
  }

  values = ReportedSearchValues(
    *this,
    winLossValueAvg,
    noResultValueAvg,
    scoreMeanAvg,
    scoreMeanSqAvg,
    leadAvg,
    utilityAvg,
    weightSum,
    visits
  );
  return true;
}

const SearchNode* Search::getRootNode() const {
  return rootNode;
}
const SearchNode* Search::getChildForMove(const SearchNode* node, Loc moveLoc) const {
  if(node == NULL)
    return NULL;
  int childrenCapacity;
  const SearchChildPointer* children = node->getChildren(childrenCapacity);
  for(int i = 0; i<childrenCapacity; i++) {
    const SearchNode* child = children[i].getIfAllocated();
    if(child == NULL)
      break;
    if(moveLoc == child->prevMoveLoc)
      return child;
  }
  return NULL;
}

Loc Search::getChosenMoveLoc() {
  if(rootNode == NULL)
    return Board::NULL_LOC;

  vector<Loc> locs;
  vector<double> playSelectionValues;
  bool suc = getPlaySelectionValues(locs,playSelectionValues,0.0);
  if(!suc)
    return Board::NULL_LOC;

  assert(locs.size() == playSelectionValues.size());

  double temperature = interpolateEarly(
    searchParams.chosenMoveTemperatureHalflife, searchParams.chosenMoveTemperatureEarly, searchParams.chosenMoveTemperature
  );

  uint32_t idxChosen = chooseIndexWithTemperature(nonSearchRand, playSelectionValues.data(), playSelectionValues.size(), temperature);
  return locs[idxChosen];
}

//Hack to encourage well-behaved dame filling behavior under territory scoring
bool Search::shouldSuppressPass(const SearchNode* n) const {
  if(!searchParams.fillDameBeforePass || n == NULL || n != rootNode)
    return false;
  if(rootHistory.rules.scoringRule != Rules::SCORING_TERRITORY || rootHistory.encorePhase > 0)
    return false;

  const SearchNode& node = *n;
  const NNOutput* nnOutput = node.getNNOutput();
  if(nnOutput == NULL)
    return false;
  if(nnOutput->whiteOwnerMap == NULL)
    return false;
  assert(nnOutput->nnXLen == nnXLen);
  assert(nnOutput->nnYLen == nnYLen);
  const float* whiteOwnerMap = nnOutput->whiteOwnerMap;

  //Find the pass move
  const SearchNode* passNode = NULL;

  int childrenCapacity;
  const SearchChildPointer* children = node.getChildren(childrenCapacity);
  for(int i = 0; i<childrenCapacity; i++) {
    const SearchNode* child = children[i].getIfAllocated();
    if(child == NULL)
      break;
    Loc moveLoc = child->prevMoveLoc;
    if(moveLoc == Board::PASS_LOC) {
      passNode = child;
      break;
    }
  }
  if(passNode == NULL)
    return false;

  double passWeight;
  double passUtility;
  double passScoreMean;
  double passLead;
  {
    int64_t numVisits = passNode->stats.visits.load(std::memory_order_acquire);
    double weightSum = passNode->stats.weightSum.load(std::memory_order_acquire);
    double scoreMeanAvg = passNode->stats.scoreMeanAvg.load(std::memory_order_acquire);
    double leadAvg = passNode->stats.leadAvg.load(std::memory_order_acquire);
    double utilityAvg = passNode->stats.utilityAvg.load(std::memory_order_acquire);

    if(numVisits <= 0 || weightSum <= 1e-10)
      return false;
    passWeight = weightSum;
    passUtility = utilityAvg;
    passScoreMean = scoreMeanAvg;
    passLead = leadAvg;
  }

  const double extreme = 0.95;

  //Suppress pass if we find a move that is not a spot that the opponent almost certainly owns
  //or that is adjacent to a pla owned spot, and is not greatly worse than pass.
  for(int i = 0; i<childrenCapacity; i++) {
    const SearchNode* child = children[i].getIfAllocated();
    if(child == NULL)
      break;
    Loc moveLoc = child->prevMoveLoc;
    if(moveLoc == Board::PASS_LOC)
      continue;
    int pos = NNPos::locToPos(moveLoc,rootBoard.x_size,nnXLen,nnYLen);
    double plaOwnership = rootPla == P_WHITE ? whiteOwnerMap[pos] : -whiteOwnerMap[pos];
    bool oppOwned = plaOwnership < -extreme;
    bool adjToPlaOwned = false;
    for(int j = 0; j<4; j++) {
      Loc adj = moveLoc + rootBoard.adj_offsets[j];
      int adjPos = NNPos::locToPos(adj,rootBoard.x_size,nnXLen,nnYLen);
      double adjPlaOwnership = rootPla == P_WHITE ? whiteOwnerMap[adjPos] : -whiteOwnerMap[adjPos];
      if(adjPlaOwnership > extreme) {
        adjToPlaOwned = true;
        break;
      }
    }
    if(oppOwned && !adjToPlaOwned)
      continue;

    int64_t numVisits = child->stats.visits.load(std::memory_order_acquire);
    double weightSum = child->stats.weightSum.load(std::memory_order_acquire);
    double scoreMeanAvg = child->stats.scoreMeanAvg.load(std::memory_order_acquire);
    double leadAvg = child->stats.leadAvg.load(std::memory_order_acquire);
    double utilityAvg = child->stats.utilityAvg.load(std::memory_order_acquire);

    //Too few visits - reject move
    if((numVisits <= 500 && weightSum <= 2 * sqrt(passWeight)) || weightSum <= 1e-10)
      continue;

    double utility = utilityAvg;
    double scoreMean = scoreMeanAvg;
    double lead = leadAvg;

    if(rootPla == P_WHITE
       && utility > passUtility - 0.1
       && scoreMean > passScoreMean - 0.5
       && lead > passLead - 0.5)
      return true;
    if(rootPla == P_BLACK
       && utility < passUtility + 0.1
       && scoreMean < passScoreMean + 0.5
       && lead < passLead + 0.5)
      return true;
  }
  return false;
}

bool Search::getPolicy(float policyProbs[NNPos::MAX_NN_POLICY_SIZE]) const {
  return getPolicy(rootNode, policyProbs);
}
bool Search::getPolicy(const SearchNode* node, float policyProbs[NNPos::MAX_NN_POLICY_SIZE]) const {
  if(node == NULL)
    return false;
  const NNOutput* nnOutput = node->getNNOutput();
  if(nnOutput == NULL)
    return false;

  std::copy(nnOutput->policyProbs, nnOutput->policyProbs+NNPos::MAX_NN_POLICY_SIZE, policyProbs);
  return true;
}


//Safe to call concurrently with search
double Search::getPolicySurprise() const {
  double surprise = 0.0;
  double searchEntropy = 0.0;
  double policyEntropy = 0.0;
  if(getPolicySurpriseAndEntropy(surprise,searchEntropy,policyEntropy))
    return surprise;
  return 0.0;
}

//Safe to call concurrently with search
bool Search::getPolicySurpriseAndEntropy(double& surpriseRet, double& searchEntropyRet, double& policyEntropyRet) const {
  if(rootNode == NULL)
    return false;
  const NNOutput* nnOutput = rootNode->getNNOutput();
  if(nnOutput == NULL)
    return false;

  vector<Loc> locs;
  vector<double> playSelectionValues;
  bool allowDirectPolicyMoves = true;
  bool alwaysComputeLcb = false;
  double lcbBuf[NNPos::MAX_NN_POLICY_SIZE];
  double radiusBuf[NNPos::MAX_NN_POLICY_SIZE];
  bool suc = getPlaySelectionValues(
    *rootNode,locs,playSelectionValues,NULL,1.0,allowDirectPolicyMoves,alwaysComputeLcb,false,lcbBuf,radiusBuf
  );
  if(!suc)
    return false;

  float policyProbsFromNNBuf[NNPos::MAX_NN_POLICY_SIZE];
  {
    const float* policyProbsFromNN = nnOutput->getPolicyProbsMaybeNoised();
    std::copy(policyProbsFromNN, policyProbsFromNN+NNPos::MAX_NN_POLICY_SIZE, policyProbsFromNNBuf);
  }

  double sumPlaySelectionValues = 0.0;
  for(int i = 0; i<playSelectionValues.size(); i++)
    sumPlaySelectionValues += playSelectionValues[i];

  double surprise = 0.0;
  double searchEntropy = 0.0;
  for(int i = 0; i<playSelectionValues.size(); i++) {
    int pos = getPos(locs[i]);
    double policy = std::max((double)policyProbsFromNNBuf[pos],1e-100);
    double target = playSelectionValues[i] / sumPlaySelectionValues;
    if(target > 1e-100) {
      double logTarget = log(target);
      double logPolicy = log(policy);
      surprise += target * (logTarget - logPolicy);
      searchEntropy += -target * logTarget;
    }
  }

  double policyEntropy = 0.0;
  for(int pos = 0; pos<NNPos::MAX_NN_POLICY_SIZE; pos++) {
    double policy = policyProbsFromNNBuf[pos];
    if(policy > 1e-100) {
      policyEntropy += -policy * log(policy);
    }
  }

  //Just in case, guard against float imprecision
  if(surprise < 0.0)
    surprise = 0.0;
  if(searchEntropy < 0.0)
    searchEntropy = 0.0;
  if(policyEntropy < 0.0)
    policyEntropy = 0.0;

  surpriseRet = surprise;
  searchEntropyRet = searchEntropy;
  policyEntropyRet = policyEntropy;

  return true;
}

void Search::printRootOwnershipMap(ostream& out, Player perspective) const {
  if(rootNode == NULL)
    return;
  const NNOutput* nnOutput = rootNode->getNNOutput();
  if(nnOutput == NULL)
    return;
  if(nnOutput->whiteOwnerMap == NULL)
    return;

  Player perspectiveToUse = (perspective != P_BLACK && perspective != P_WHITE) ? rootPla : perspective;
  double perspectiveFactor = perspectiveToUse == P_BLACK ? -1.0 : 1.0;

  for(int y = 0; y<rootBoard.y_size; y++) {
    for(int x = 0; x<rootBoard.x_size; x++) {
      int pos = NNPos::xyToPos(x,y,nnOutput->nnXLen);
      out << Global::strprintf("%6.1f ", perspectiveFactor * nnOutput->whiteOwnerMap[pos]*100);
    }
    out << endl;
  }
  out << endl;
}

void Search::printRootPolicyMap(ostream& out) const {
  if(rootNode == NULL)
    return;
  const NNOutput* nnOutput = rootNode->getNNOutput();
  if(nnOutput == NULL)
    return;

  const float* policyProbs = nnOutput->getPolicyProbsMaybeNoised();
  for(int y = 0; y<rootBoard.y_size; y++) {
    for(int x = 0; x<rootBoard.x_size; x++) {
      int pos = NNPos::xyToPos(x,y,nnOutput->nnXLen);
      out << Global::strprintf("%6.1f ", policyProbs[pos]*100);
    }
    out << endl;
  }
  out << endl;
}

void Search::printRootEndingScoreValueBonus(ostream& out) const {
  if(rootNode == NULL)
    return;
  const NNOutput* nnOutput = rootNode->getNNOutput();
  if(nnOutput == NULL)
    return;
  if(nnOutput->whiteOwnerMap == NULL)
    return;

  int childrenCapacity;
  const SearchChildPointer* children = rootNode->getChildren(childrenCapacity);
  for(int i = 0; i<childrenCapacity; i++) {
    const SearchNode* child = children[i].getIfAllocated();
    if(child == NULL)
      break;

    int64_t childVisits = child->stats.visits.load(std::memory_order_acquire);
    double scoreMeanAvg = child->stats.scoreMeanAvg.load(std::memory_order_acquire);
    double scoreMeanSqAvg = child->stats.scoreMeanSqAvg.load(std::memory_order_acquire);
    double utilityAvg = child->stats.utilityAvg.load(std::memory_order_acquire);

    double utilityNoBonus = utilityAvg;
    double endingScoreBonus = getEndingWhiteScoreBonus(*rootNode,child);
    double utilityDiff = getScoreUtilityDiff(scoreMeanAvg, scoreMeanSqAvg, endingScoreBonus);
    double utilityWithBonus = utilityNoBonus + utilityDiff;

    out << Location::toString(child->prevMoveLoc,rootBoard) << " " << Global::strprintf(
      "visits %d utilityNoBonus %.2fc utilityWithBonus %.2fc endingScoreBonus %.2f",
      childVisits, utilityNoBonus*100, utilityWithBonus*100, endingScoreBonus
    );
    out << endl;
  }
}

void Search::appendPV(vector<Loc>& buf, vector<int64_t>& visitsBuf, vector<Loc>& scratchLocs, vector<double>& scratchValues, const SearchNode* node, int maxDepth) const {
  appendPVForMove(buf,visitsBuf,scratchLocs,scratchValues,node,Board::NULL_LOC,maxDepth);
}

void Search::appendPVForMove(vector<Loc>& buf, vector<int64_t>& visitsBuf, vector<Loc>& scratchLocs, vector<double>& scratchValues, const SearchNode* node, Loc move, int maxDepth) const {
  if(node == NULL)
    return;

  for(int depth = 0; depth < maxDepth; depth++) {
    bool success = getPlaySelectionValues(*node, scratchLocs, scratchValues, NULL, 1.0, false);
    if(!success)
      return;

    double maxSelectionValue = POLICY_ILLEGAL_SELECTION_VALUE;
    int bestChildIdx = -1;
    Loc bestChildMoveLoc = Board::NULL_LOC;

    for(int i = 0; i<scratchValues.size(); i++) {
      Loc moveLoc = scratchLocs[i];
      double selectionValue = scratchValues[i];

      if(depth == 0 && moveLoc == move) {
        maxSelectionValue = selectionValue;
        bestChildIdx = i;
        bestChildMoveLoc = moveLoc;
        break;
      }

      if(selectionValue > maxSelectionValue) {
        maxSelectionValue = selectionValue;
        bestChildIdx = i;
        bestChildMoveLoc = moveLoc;
      }
    }

    if(bestChildIdx < 0 || bestChildMoveLoc == Board::NULL_LOC)
      return;
    if(depth == 0 && move != Board::NULL_LOC && bestChildMoveLoc != move)
      return;

    int childrenCapacity;
    const SearchChildPointer* children = node->getChildren(childrenCapacity);
    assert(bestChildIdx <= childrenCapacity);
    assert(scratchValues.size() <= childrenCapacity);

    const SearchNode* child = children[bestChildIdx].getIfAllocated();
    assert(child != NULL);
    node = child;

    int64_t visits = node->stats.visits.load(std::memory_order_acquire);

    buf.push_back(bestChildMoveLoc);
    visitsBuf.push_back(visits);
  }
}


void Search::printPV(ostream& out, const SearchNode* n, int maxDepth) const {
  vector<Loc> buf;
  vector<int64_t> visitsBuf;
  vector<Loc> scratchLocs;
  vector<double> scratchValues;
  appendPV(buf,visitsBuf,scratchLocs,scratchValues,n,maxDepth);
  printPV(out,buf);
}

void Search::printPV(ostream& out, const vector<Loc>& buf) const {
  bool printedAnything = false;
  for(int i = 0; i<buf.size(); i++) {
    if(printedAnything)
      out << " ";
    if(buf[i] == Board::NULL_LOC)
      continue;
    out << Location::toString(buf[i],rootBoard);
    printedAnything = true;
  }
}

//Child should NOT be locked.
AnalysisData Search::getAnalysisDataOfSingleChild(
  const SearchNode* child, vector<Loc>& scratchLocs, vector<double>& scratchValues,
  Loc move, double policyProb, double fpuValue, double parentUtility, double parentWinLossValue,
  double parentScoreMean, double parentScoreStdev, double parentLead, int maxPVDepth
) const {
  int64_t numVisits = 0;
  double winLossValueAvg = 0.0;
  double noResultValueAvg = 0.0;
  double scoreMeanAvg = 0.0;
  double scoreMeanSqAvg = 0.0;
  double leadAvg = 0.0;
  double utilityAvg = 0.0;
  double utilitySqAvg = 0.0;
  double weightSum = 0.0;
  double weightSqSum = 0.0;

  if(child != NULL) {
    numVisits = child->stats.visits.load(std::memory_order_acquire);
    weightSum = child->stats.weightSum.load(std::memory_order_acquire);
    weightSqSum = child->stats.weightSqSum.load(std::memory_order_acquire);
    winLossValueAvg = child->stats.winLossValueAvg.load(std::memory_order_acquire);
    noResultValueAvg = child->stats.noResultValueAvg.load(std::memory_order_acquire);
    scoreMeanAvg = child->stats.scoreMeanAvg.load(std::memory_order_acquire);
    scoreMeanSqAvg = child->stats.scoreMeanSqAvg.load(std::memory_order_acquire);
    leadAvg = child->stats.leadAvg.load(std::memory_order_acquire);
    utilityAvg = child->stats.utilityAvg.load(std::memory_order_acquire);
    utilitySqAvg = child->stats.utilitySqAvg.load(std::memory_order_acquire);
  }

  AnalysisData data;
  data.move = move;
  data.numVisits = numVisits;
  if(numVisits <= 0 || weightSum <= 1e-30 || weightSqSum <= 1e-60) {
    data.utility = fpuValue;
    data.scoreUtility = getScoreUtility(parentScoreMean,parentScoreMean*parentScoreMean+parentScoreStdev*parentScoreStdev);
    data.resultUtility = fpuValue - data.scoreUtility;
    data.winLossValue = searchParams.winLossUtilityFactor == 1.0 ? parentWinLossValue + (fpuValue - parentUtility) : 0.0;
    // Make sure winloss values due to FPU don't go out of bounds for purposes of reporting to UI
    if(data.winLossValue < -1.0)
      data.winLossValue = -1.0;
    if(data.winLossValue > 1.0)
      data.winLossValue = 1.0;
    data.scoreMean = parentScoreMean;
    data.scoreStdev = parentScoreStdev;
    data.lead = parentLead;
    data.ess = 0.0;
    data.weightSum = 0.0;
    data.weightSqSum = 0.0;
    data.utilitySqAvg = data.utility * data.utility;
    data.scoreMeanSqAvg = parentScoreMean * parentScoreMean + parentScoreStdev * parentScoreStdev;
  }
  else {
    data.utility = utilityAvg;
    data.resultUtility = getResultUtility(winLossValueAvg, noResultValueAvg);
    data.scoreUtility = getScoreUtility(scoreMeanAvg, scoreMeanSqAvg);
    data.winLossValue = winLossValueAvg;
    data.scoreMean = scoreMeanAvg;
    data.scoreStdev = getScoreStdev(scoreMeanAvg,scoreMeanSqAvg);
    data.lead = leadAvg;
    data.ess = weightSum * weightSum / weightSqSum;
    data.weightSum = weightSum;
    data.weightSqSum = weightSqSum;
    data.utilitySqAvg = utilitySqAvg;
    data.scoreMeanSqAvg = scoreMeanSqAvg;
  }

  data.policyPrior = policyProb;
  data.order = 0;

  data.pv.clear();
  data.pv.push_back(move);
  data.pvVisits.clear();
  data.pvVisits.push_back(numVisits);
  appendPV(data.pv, data.pvVisits, scratchLocs, scratchValues, child, maxPVDepth);

  data.node = child;

  return data;
}

void Search::getAnalysisData(
  vector<AnalysisData>& buf,int minMovesToTryToGet, bool includeWeightFactors, int maxPVDepth, bool duplicateForSymmetries
) const {
  buf.clear();
  if(rootNode == NULL)
    return;
  getAnalysisData(*rootNode, buf, minMovesToTryToGet, includeWeightFactors, maxPVDepth, duplicateForSymmetries);
}

void Search::getAnalysisData(
  const SearchNode& node, vector<AnalysisData>& buf, int minMovesToTryToGet, bool includeWeightFactors, int maxPVDepth, bool duplicateForSymmetries
) const {
  buf.clear();
  vector<const SearchNode*> children;
  children.reserve(rootBoard.x_size * rootBoard.y_size + 1);

  int numChildren;
  vector<Loc> scratchLocs;
  vector<double> scratchValues;
  double lcbBuf[NNPos::MAX_NN_POLICY_SIZE];
  double radiusBuf[NNPos::MAX_NN_POLICY_SIZE];
  float policyProbs[NNPos::MAX_NN_POLICY_SIZE];
  {
    int childrenCapacity;
    const SearchChildPointer* childrenArr = node.getChildren(childrenCapacity);
    for(int i = 0; i<childrenCapacity; i++) {
      const SearchNode* child = childrenArr[i].getIfAllocated();
      if(child == NULL)
        break;
      children.push_back(child);
    }
    numChildren = children.size();

    if(numChildren <= 0)
      return;
    assert(numChildren <= NNPos::MAX_NN_POLICY_SIZE);

    bool alwaysComputeLcb = true;
    bool success = getPlaySelectionValues(node, scratchLocs, scratchValues, NULL, 1.0, false, alwaysComputeLcb, false, lcbBuf, radiusBuf);
    if(!success)
      return;

    const NNOutput* nnOutput = node.getNNOutput();
    const float* policyProbsFromNN = nnOutput->getPolicyProbsMaybeNoised();
    for(int i = 0; i<NNPos::MAX_NN_POLICY_SIZE; i++)
      policyProbs[i] = policyProbsFromNN[i];
  }

  //Copy to make sure we keep these values so we can reuse scratch later for PV
  vector<double> playSelectionValues = scratchValues;

  double policyProbMassVisited = 0.0;
  {
    for(int i = 0; i<numChildren; i++) {
      const SearchNode* child = children[i];
      policyProbMassVisited += policyProbs[getPos(child->prevMoveLoc)];
    }
    //Probability mass should not sum to more than 1, giving a generous allowance
    //for floating point error.
    assert(policyProbMassVisited <= 1.0001);
  }

  double parentWinLossValue;
  double parentScoreMean;
  double parentScoreStdev;
  double parentLead;
  {
    double weightSum = node.stats.weightSum.load(std::memory_order_acquire);
    double winLossValueAvg = node.stats.winLossValueAvg.load(std::memory_order_acquire);
    double scoreMeanAvg = node.stats.scoreMeanAvg.load(std::memory_order_acquire);
    double scoreMeanSqAvg = node.stats.scoreMeanSqAvg.load(std::memory_order_acquire);
    double leadAvg = node.stats.leadAvg.load(std::memory_order_acquire);
    assert(weightSum > 0.0);

    parentWinLossValue = winLossValueAvg;
    parentScoreMean = scoreMeanAvg;
    parentScoreStdev = getScoreStdev(parentScoreMean,scoreMeanSqAvg);
    parentLead = leadAvg;
  }

  double parentUtility;
  double parentWeightPerVisit;
  double parentUtilityStdevFactor;
  double fpuValue = getFpuValueForChildrenAssumeVisited(
    node, node.nextPla, true, policyProbMassVisited,
    parentUtility, parentWeightPerVisit, parentUtilityStdevFactor
  );

  vector<MoreNodeStats> statsBuf(numChildren);
  for(int i = 0; i<numChildren; i++) {
    const SearchNode* child = children[i];
    double policyProb = policyProbs[getPos(child->prevMoveLoc)];
    AnalysisData data = getAnalysisDataOfSingleChild(
      child, scratchLocs, scratchValues, child->prevMoveLoc, policyProb, fpuValue, parentUtility, parentWinLossValue,
      parentScoreMean, parentScoreStdev, parentLead, maxPVDepth
    );
    data.playSelectionValue = playSelectionValues[i];
    //Make sure data.lcb is from white's perspective, for consistency with everything else
    //In lcbBuf, it's from self perspective, unlike values at nodes.
    data.lcb = node.nextPla == P_BLACK ? -lcbBuf[i] : lcbBuf[i];
    data.radius = radiusBuf[i];
    buf.push_back(data);

    MoreNodeStats& stats = statsBuf[i];
    stats.stats = NodeStats(child->stats);
    stats.selfUtility = node.nextPla == P_WHITE ? data.utility : -data.utility;
    stats.weightAdjusted = stats.stats.weightSum;
    stats.prevMoveLoc = child->prevMoveLoc;
  }

  //Find all children and compute weighting of the children based on their values
  if(includeWeightFactors) {
    double totalChildWeight = 0.0;
    for(int i = 0; i<numChildren; i++) {
      totalChildWeight += statsBuf[i].weightAdjusted;
    }
    if(searchParams.useNoisePruning) {
      double policyProbsBuf[NNPos::MAX_NN_POLICY_SIZE];
      for(int i = 0; i<numChildren; i++)
        policyProbsBuf[i] = std::max(1e-30, (double)policyProbs[getPos(statsBuf[i].prevMoveLoc)]);
      totalChildWeight = pruneNoiseWeight(statsBuf, numChildren, totalChildWeight, policyProbsBuf);
    }
    double amountToSubtract = 0.0;
    double amountToPrune = 0.0;
    downweightBadChildrenAndNormalizeWeight(
      numChildren, totalChildWeight, totalChildWeight,
      amountToSubtract, amountToPrune, statsBuf
    );
    for(int i = 0; i<numChildren; i++)
      buf[i].weightFactor = statsBuf[i].weightAdjusted;
  }

  //Fill the rest of the moves directly from policy
  if(numChildren < minMovesToTryToGet) {
    //A bit inefficient, but no big deal
    for(int i = 0; i<minMovesToTryToGet - numChildren; i++) {
      int bestPos = -1;
      double bestPolicy = -1.0;
      for(int pos = 0; pos<NNPos::MAX_NN_POLICY_SIZE; pos++) {
        if(policyProbs[pos] < bestPolicy)
          continue;

        bool alreadyUsed = false;
        for(int j = 0; j<buf.size(); j++) {
          if(getPos(buf[j].move) == pos) {
            alreadyUsed = true;
            break;
          }
        }
        if(alreadyUsed)
          continue;

        bestPos = pos;
        bestPolicy = policyProbs[pos];
      }
      if(bestPos < 0 || bestPolicy < 0.0)
        break;

      Loc bestMove = NNPos::posToLoc(bestPos,rootBoard.x_size,rootBoard.y_size,nnXLen,nnYLen);
      AnalysisData data = getAnalysisDataOfSingleChild(
        NULL, scratchLocs, scratchValues, bestMove, bestPolicy, fpuValue, parentUtility, parentWinLossValue,
        parentScoreMean, parentScoreStdev, parentLead, maxPVDepth
      );
      buf.push_back(data);
    }
  }
  std::stable_sort(buf.begin(),buf.end());

  if(duplicateForSymmetries && searchParams.rootSymmetryPruning && rootSymmetries.size() > 1) {
    vector<AnalysisData> newBuf;
    std::set<Loc> isDone;
    for(int i = 0; i<buf.size(); i++) {
      const AnalysisData& data = buf[i];
      for(int symmetry : rootSymmetries) {
        Loc symMove = SymmetryHelpers::getSymLoc(data.move, rootBoard, symmetry);
        if(contains(isDone,symMove))
          continue;
        const std::vector<int>& avoidMoveUntilByLoc = rootPla == P_BLACK ? avoidMoveUntilByLocBlack : avoidMoveUntilByLocWhite;
        if(avoidMoveUntilByLoc.size() > 0 && avoidMoveUntilByLoc[symMove] > 0)
          continue;

        isDone.insert(symMove);
        newBuf.push_back(data);
        //Replace the fields that need to be adjusted for symmetry
        AnalysisData& newData = newBuf.back();
        newData.move = symMove;
        if(symmetry != 0)
          newData.isSymmetryOf = data.move;
        newData.symmetry = symmetry;
        for(int j = 0; j<newData.pv.size(); j++)
          newData.pv[j] = SymmetryHelpers::getSymLoc(newData.pv[j], rootBoard, symmetry);
      }
    }
    buf = std::move(newBuf);
  }

  for(int i = 0; i<buf.size(); i++)
    buf[i].order = i;
}

void Search::printPVForMove(ostream& out, const SearchNode* n, Loc move, int maxDepth) const {
  vector<Loc> buf;
  vector<int64_t> visitsBuf;
  vector<Loc> scratchLocs;
  vector<double> scratchValues;
  appendPVForMove(buf,visitsBuf,scratchLocs,scratchValues,n,move,maxDepth);
  for(int i = 0; i<buf.size(); i++) {
    if(i > 0)
      out << " ";
    out << Location::toString(buf[i],rootBoard);
  }
}

void Search::printTree(ostream& out, const SearchNode* node, PrintTreeOptions options, Player perspective) const {
  if(node == NULL)
    return;
  string prefix;
  AnalysisData data;
  {
    vector<Loc> scratchLocs;
    vector<double> scratchValues;
    //Use dummy values for parent
    double policyProb = NAN;
    double fpuValue = 0;
    double parentUtility = 0;
    double parentWinLossValue = 0;
    double parentScoreMean = 0;
    double parentScoreStdev = 0;
    double parentLead = 0;
    data = getAnalysisDataOfSingleChild(
      node, scratchLocs, scratchValues,
      (node == rootNode ? Board::NULL_LOC : node->prevMoveLoc), policyProb, fpuValue, parentUtility, parentWinLossValue,
      parentScoreMean, parentScoreStdev, parentLead, options.maxPVDepth_
    );
    data.weightFactor = NAN;
  }
  perspective = (perspective != P_BLACK && perspective != P_WHITE) ? node->nextPla : perspective;
  printTreeHelper(out, node, options, prefix, 0, 0, data, perspective);
}

void Search::printTreeHelper(
  ostream& out, const SearchNode* n, const PrintTreeOptions& options,
  string& prefix, int64_t origVisits, int depth, const AnalysisData& data, Player perspective
) const {
  if(n == NULL)
    return;

  const SearchNode& node = *n;

  Player perspectiveToUse = (perspective != P_BLACK && perspective != P_WHITE) ? n->nextPla : perspective;
  double perspectiveFactor = perspectiveToUse == P_BLACK ? -1.0 : 1.0;

  if(depth == 0)
    origVisits = data.numVisits;

  //Output for this node
  {
    out << prefix;
    char buf[128];

    out << ": ";

    if(data.numVisits > 0) {
      sprintf(buf,"T %6.2fc ",(perspectiveFactor * data.utility * 100.0));
      out << buf;
      sprintf(buf,"W %6.2fc ",(perspectiveFactor * data.resultUtility * 100.0));
      out << buf;
      sprintf(buf,"S %6.2fc (%+5.1f L %+5.1f) ",
              perspectiveFactor * data.scoreUtility * 100.0,
              perspectiveFactor * data.scoreMean,
              perspectiveFactor * data.lead
      );
      out << buf;
    }

    // bool hasNNValue = false;
    // double nnResultValue;
    // double nnTotalValue;
    // lock.lock();
    // if(node.nnOutput != nullptr) {
    //   nnResultValue = getResultUtilityFromNN(*node.nnOutput);
    //   nnTotalValue = getUtilityFromNN(*node.nnOutput);
    //   hasNNValue = true;
    // }
    // lock.unlock();

    // if(hasNNValue) {
    //   sprintf(buf,"VW %6.2fc VS %6.2fc ", nnResultValue * 100.0, (nnTotalValue - nnResultValue) * 100.0);
    //   out << buf;
    // }
    // else {
    //   sprintf(buf,"VW ---.--c VS ---.--c ");
    //   out << buf;
    // }

    if(depth > 0 && !isnan(data.lcb)) {
      sprintf(buf,"LCB %7.2fc ", perspectiveFactor * data.lcb * 100.0);
      out << buf;
    }

    if(!isnan(data.policyPrior)) {
      sprintf(buf,"P %5.2f%% ", data.policyPrior * 100.0);
      out << buf;
    }
    if(!isnan(data.weightFactor)) {
      sprintf(buf,"WF %5.1f ", data.weightFactor);
      out << buf;
    }
    if(data.playSelectionValue >= 0 && depth > 0) {
      sprintf(buf,"PSV %7.0f ", data.playSelectionValue);
      out << buf;
    }

    if(options.printSqs_) {
      sprintf(buf,"SMSQ %5.1f USQ %7.5f W %6.2f WSQ %8.2f ", data.scoreMeanSqAvg, data.utilitySqAvg, data.weightSum, data.weightSqSum);
      out << buf;
    }

    if(options.printAvgShorttermError_) {
      std::pair<double,double> wlAndScoreError = getAverageShorttermWLAndScoreError(&node);
      sprintf(buf,"STWL %6.2fc STS %5.1f ", wlAndScoreError.first, wlAndScoreError.second);
      out << buf;
    }

    sprintf(buf,"N %7" PRIu64 "  --  ", data.numVisits);
    out << buf;

    printPV(out, data.pv);
    out << endl;
  }

  if(depth >= options.branch_.size()) {
    if(depth >= options.maxDepth_ + options.branch_.size())
      return;
    if(data.numVisits < options.minVisitsToExpand_)
      return;
    if((double)data.numVisits < origVisits * options.minVisitsPropToExpand_)
      return;
  }
  if(depth == options.branch_.size()) {
    out << "---" << PlayerIO::playerToString(node.nextPla) << "(" << (node.nextPla == perspectiveToUse ? "^" : "v") << ")---" << endl;
  }

  vector<AnalysisData> analysisData;
  bool duplicateForSymmetries = false;
  getAnalysisData(node,analysisData,0,true,options.maxPVDepth_,duplicateForSymmetries);

  int numChildren = analysisData.size();

  //Apply filtering conditions, but include children that don't match the filtering condition
  //but where there are children afterward that do, in case we ever use something more complex
  //than plain visits as a filter criterion. Do this by finding the last child that we want as the threshold.
  int lastIdxWithEnoughVisits = numChildren-1;
  while(true) {
    if(lastIdxWithEnoughVisits <= 0)
      break;

    int64_t childVisits = analysisData[lastIdxWithEnoughVisits].numVisits;
    bool hasEnoughVisits = childVisits >= options.minVisitsToShow_
      && (double)childVisits >= origVisits * options.minVisitsPropToShow_;
    if(hasEnoughVisits)
      break;
    lastIdxWithEnoughVisits--;
  }

  int numChildrenToRecurseOn = numChildren;
  if(options.maxChildrenToShow_ < numChildrenToRecurseOn)
    numChildrenToRecurseOn = options.maxChildrenToShow_;
  if(lastIdxWithEnoughVisits+1 < numChildrenToRecurseOn)
    numChildrenToRecurseOn = lastIdxWithEnoughVisits+1;


  for(int i = 0; i<numChildren; i++) {
    const SearchNode* child = analysisData[i].node;
    Loc moveLoc = child->prevMoveLoc;

    if((depth >= options.branch_.size() && i < numChildrenToRecurseOn) ||
       (depth < options.branch_.size() && moveLoc == options.branch_[depth]))
    {
      size_t oldLen = prefix.length();
      string locStr = Location::toString(moveLoc,rootBoard);
      if(locStr == "pass")
        prefix += "pss";
      else
        prefix += locStr;
      prefix += " ";
      while(prefix.length() < oldLen+4)
        prefix += " ";
      printTreeHelper(
        out,child,options,prefix,origVisits,depth+1,analysisData[i], perspective);
      prefix.erase(oldLen);
    }
  }
}

std::pair<double,double> Search::getAverageShorttermWLAndScoreError(const SearchNode* node) const {
  if(node == NULL)
    node = rootNode;
  if(node == NULL)
    return std::make_pair(0.0,0.0);
  return getAverageShorttermWLAndScoreErrorHelper(node);
}

std::pair<double,double> Search::getAverageShorttermWLAndScoreErrorHelper(const SearchNode* node) const {
  const NNOutput* nnOutput = node->getNNOutput();
  if(nnOutput == NULL) {
    //This will also be correct for terminal nodes, which have no uncertainty.
    //The caller will scale by weightSum, so this all works as intended.
    return std::make_pair(0.0,0.0);
  }

  int childrenCapacity;
  const SearchChildPointer* children = node->getChildren(childrenCapacity);

  int numChildren = 0;
  for(int i = 0; i<childrenCapacity; i++) {
    const SearchNode* child = children[i].getIfAllocated();
    if(child == NULL)
      break;
    numChildren += 1;
  }

  double wlErrorSum = 0.0;
  double scoreErrorSum = 0.0;
  double weightSum = 0.0;
  {
    double thisNodeWeight = computeWeightFromNNOutput(nnOutput);
    wlErrorSum += nnOutput->shorttermWinlossError * thisNodeWeight;
    scoreErrorSum += nnOutput->shorttermScoreError * thisNodeWeight;
    weightSum += thisNodeWeight;
  }

  for(int i = numChildren-1; i>=0; i--) {
    const SearchNode* child = children[i].getIfAllocated();
    assert(child != NULL);
    double childWeight = child->stats.weightSum.load(std::memory_order_acquire);
    std::pair<double,double> result = getAverageShorttermWLAndScoreErrorHelper(child);
    wlErrorSum += result.first * childWeight;
    scoreErrorSum += result.second * childWeight;
    weightSum += childWeight;
  }

  return std::make_pair(wlErrorSum/weightSum, scoreErrorSum/weightSum);
}

bool Search::getSharpScore(const SearchNode* node, double& ret) const {
  if(node == NULL)
    node = rootNode;
  if(node == NULL)
    return false;

  double policyProbsBuf[NNPos::MAX_NN_POLICY_SIZE];
  if(node != rootNode) {
    ret = getSharpScoreHelper(node,policyProbsBuf);
    return true;
  }

  vector<double> playSelectionValues;
  vector<Loc> locs; // not used
  bool allowDirectPolicyMoves = false;
  bool alwaysComputeLcb = false;
  bool neverUseLcb = true;
  bool suc = getPlaySelectionValues(*node,locs,playSelectionValues,NULL,1.0,allowDirectPolicyMoves,alwaysComputeLcb,neverUseLcb,NULL,NULL);
  //If there are no children, or otherwise values could not be computed, then fall back to the normal case
  if(!suc) {
    ReportedSearchValues values;
    if(getNodeValues(node,values)) {
      ret = values.expectedScore;
      return true;
    }
    return false;
  }

  int childrenCapacity;
  const SearchChildPointer* children = node->getChildren(childrenCapacity);

  double scoreMeanSum = 0.0;
  double scoreWeightSum = 0.0;
  double childWeightSum = 0.0;
  for(int i = 0; i<childrenCapacity; i++) {
    const SearchNode* child = children[i].getIfAllocated();
    if(child == NULL)
      break;
    NodeStats stats = NodeStats(child->stats);
    if(stats.visits <= 0 || stats.weightSum <= 0.0)
      continue;
    double weight = playSelectionValues[i];
    double sharpWeight = weight * weight * weight;
    scoreMeanSum += sharpWeight * getSharpScoreHelper(child, policyProbsBuf);
    scoreWeightSum += sharpWeight;
    childWeightSum += weight;
  }

  //Also add in the direct evaluation of this node.
  {
    const NNOutput* nnOutput = node->getNNOutput();
    //If somehow the nnOutput is still null here, skip
    if(nnOutput == NULL)
      return false;
    double scoreMean = (double)nnOutput->whiteScoreMean;
    double thisNodeWeight = computeWeightFromNNOutput(nnOutput);
    double desiredScoreWeight = (scoreWeightSum < 1e-50 || childWeightSum < 1e-50) ? thisNodeWeight : thisNodeWeight * (scoreWeightSum / childWeightSum);
    scoreMeanSum += scoreMean * desiredScoreWeight;
    scoreWeightSum += desiredScoreWeight;
  }
  ret = scoreMeanSum / scoreWeightSum;
  return true;
}

double Search::getSharpScoreHelper(const SearchNode* node, double policyProbsBuf[NNPos::MAX_NN_POLICY_SIZE]) const {
  if(node == NULL)
    return 0.0;
  const NNOutput* nnOutput = node->getNNOutput();
  if(nnOutput == NULL) {
    NodeStats stats = NodeStats(node->stats);
    return stats.scoreMeanAvg;
  }

  int childrenCapacity;
  const SearchChildPointer* children = node->getChildren(childrenCapacity);

  vector<MoreNodeStats> statsBuf;
  for(int i = 0; i<childrenCapacity; i++) {
    const SearchNode* child = children[i].getIfAllocated();
    if(child == NULL)
      break;
    MoreNodeStats stats;
    stats.stats = NodeStats(child->stats);
    stats.selfUtility = node->nextPla == P_WHITE ? stats.stats.utilityAvg : -stats.stats.utilityAvg;
    stats.weightAdjusted = stats.stats.weightSum;
    stats.prevMoveLoc = child->prevMoveLoc;
    statsBuf.push_back(stats);
  }
  int numChildren = (int)statsBuf.size();

  //Find all children and compute weighting of the children based on their values
  {
    double totalChildWeight = 0.0;
    for(int i = 0; i<numChildren; i++) {
      totalChildWeight += statsBuf[i].weightAdjusted;
    }
    const float* policyProbs = nnOutput->getPolicyProbsMaybeNoised();
    if(searchParams.useNoisePruning) {
      for(int i = 0; i<numChildren; i++)
        policyProbsBuf[i] = std::max(1e-30, (double)policyProbs[getPos(statsBuf[i].prevMoveLoc)]);
      totalChildWeight = pruneNoiseWeight(statsBuf, numChildren, totalChildWeight, policyProbsBuf);
    }
    double amountToSubtract = 0.0;
    double amountToPrune = 0.0;
    downweightBadChildrenAndNormalizeWeight(
      numChildren, totalChildWeight, totalChildWeight,
      amountToSubtract, amountToPrune, statsBuf
    );
  }

  double scoreMeanSum = 0.0;
  double scoreWeightSum = 0.0;
  double childWeightSum = 0.0;
  for(int i = 0; i<numChildren; i++) {
    if(statsBuf[i].stats.visits <= 0 || statsBuf[i].stats.weightSum <= 0.0)
      continue;
    double weight = statsBuf[i].weightAdjusted;
    double sharpWeight = weight * weight * weight;
    scoreMeanSum += sharpWeight * getSharpScoreHelper(children[i].getIfAllocated(),policyProbsBuf);
    scoreWeightSum += sharpWeight;
    childWeightSum += weight;
  }

  //Also add in the direct evaluation of this node.
  {
    double scoreMean = (double)nnOutput->whiteScoreMean;
    double thisNodeWeight = computeWeightFromNNOutput(nnOutput);
    double desiredScoreWeight = (scoreWeightSum < 1e-50 || childWeightSum < 1e-50) ? thisNodeWeight : thisNodeWeight * (scoreWeightSum / childWeightSum);
    scoreMeanSum += scoreMean * desiredScoreWeight;
    scoreWeightSum += desiredScoreWeight;
  }
  return scoreMeanSum / scoreWeightSum;
}

vector<double> Search::getAverageTreeOwnership(double minWeight, const SearchNode* node) const {
  if(node == NULL)
    node = rootNode;
  if(!alwaysIncludeOwnerMap)
    throw StringError("Called Search::getAverageTreeOwnership when alwaysIncludeOwnerMap is false");
  vector<double> vec(nnXLen*nnYLen,0.0);
  auto accumulate = [&vec,this](float* ownership, double selfWeight){
    for (int pos = 0; pos < nnXLen*nnYLen; pos++)
      vec[pos] += selfWeight * ownership[pos];
  };
  traverseTreeWithOwnershipAndSelfWeight(minWeight,1.0,node,accumulate);
  return vec;
}

tuple<vector<double>,vector<double>> Search::getAverageAndStandardDeviationTreeOwnership(double minWeight, const SearchNode* node) const {
  if(node == NULL)
    node = rootNode;
  vector<double> average(nnXLen*nnYLen,0.0);
  vector<double> stdev(nnXLen*nnYLen,0.0);
  auto accumulate = [&average,&stdev,this](float* ownership, double selfWeight) {
    for (int pos = 0; pos < nnXLen*nnYLen; pos++) {
      const double value = ownership[pos];
      average[pos] += selfWeight * value;
      stdev[pos] += selfWeight * value * value;
    }
  };
  traverseTreeWithOwnershipAndSelfWeight(minWeight,1.0,node,accumulate);
  for(int pos = 0; pos<nnXLen*nnYLen; pos++) {
    const double avg = average[pos];
    stdev[pos] = sqrt(max(stdev[pos] - avg * avg, 0.0));
  }
  return std::make_tuple(average, stdev);
}

template<typename Func>
double Search::traverseTreeWithOwnershipAndSelfWeight(
  double minWeight,
  double desiredWeight,
  const SearchNode* node,
  Func& accumulate
) const {
  if(node == NULL)
    return 0;

  const NNOutput* nnOutput = node->getNNOutput();
  if(nnOutput == NULL)
    return 0;

  int childrenCapacity;
  const SearchChildPointer* children = node->getChildren(childrenCapacity);

  double actualWeightFromChildren;
  double thisNodeWeight = computeWeightFromNNOutput(nnOutput);
  if(childrenCapacity <= 8) {
    double childWeightBuf[8];
    actualWeightFromChildren = traverseTreeWithOwnershipAndSelfWeightHelper(
      minWeight, desiredWeight, thisNodeWeight, children, childWeightBuf, childrenCapacity, accumulate
    );
  }
  else {
    vector<double> childWeightBuf(childrenCapacity);
    actualWeightFromChildren = traverseTreeWithOwnershipAndSelfWeightHelper(
      minWeight, desiredWeight, thisNodeWeight, children, &childWeightBuf[0], childrenCapacity, accumulate
    );
  }

  double selfWeight = desiredWeight - actualWeightFromChildren;
  float* ownerMap = nnOutput->whiteOwnerMap;
  assert(ownerMap != NULL);
  accumulate(ownerMap, selfWeight);
  return desiredWeight;
}

template<typename Func>
double Search::traverseTreeWithOwnershipAndSelfWeightHelper(
  double minWeight,
  double desiredWeight,
  double thisNodeWeight,
  const SearchChildPointer* children,
  double* childWeightBuf,
  int childrenCapacity,
  Func& accumulate
) const {
  int numChildren = 0;
  for(int i = 0; i<childrenCapacity; i++) {
    const SearchNode* child = children[i].getIfAllocated();
    if(child == NULL)
      break;
    double childWeight = child->stats.weightSum.load(std::memory_order_acquire);
    childWeightBuf[i] = childWeight;
    numChildren += 1;
  }

  double relativeChildrenWeightSum = 0.0;
  double usedChildrenWeightSum = 0;
  for(int i = 0; i<numChildren; i++) {
    double childWeight = childWeightBuf[i];
    if(childWeight < minWeight)
      continue;
    relativeChildrenWeightSum += (double)childWeight * childWeight;
    usedChildrenWeightSum += childWeight;
  }

  double desiredWeightFromChildren = desiredWeight * usedChildrenWeightSum / (usedChildrenWeightSum + thisNodeWeight);

  //Recurse
  double actualWeightFromChildren = 0.0;
  for(int i = 0; i<numChildren; i++) {
    double childWeight = childWeightBuf[i];
    if(childWeight < minWeight)
      continue;
    const SearchNode* child = children[i].getIfAllocated();
    assert(child != NULL);
    double desiredWeightFromChild = (double)childWeight * childWeight / relativeChildrenWeightSum * desiredWeightFromChildren;
    actualWeightFromChildren += traverseTreeWithOwnershipAndSelfWeight(minWeight,desiredWeightFromChild,child,accumulate);
  }

  return actualWeightFromChildren;
}

static double roundStatic(double x, double inverseScale) {
  return round(x * inverseScale) / inverseScale;
}
static double roundDynamic(double x, int precision) {
  double absx = abs(x);
  if(absx <= 1e-60)
    return x;
  int orderOfMagnitude = (int)floor(log10(absx));
  int roundingMagnitude = orderOfMagnitude - precision;
  if(roundingMagnitude >= 0)
    return round(x);
  double inverseScale = pow(10.0,-roundingMagnitude);
  return roundStatic(x, inverseScale);
}


json Search::getJsonOwnershipMap(
  const Player pla, const Player perspective, const Board& board, const SearchNode* node, double ownershipMinWeight, int symmetry
) const {
  vector<double> ownership = getAverageTreeOwnership(ownershipMinWeight, node);
  vector<double> ownershipToOutput(board.y_size * board.x_size, 0.0);

  for(int y = 0; y < board.y_size; y++) {
    for(int x = 0; x < board.x_size; x++) {
      int pos = NNPos::xyToPos(x, y, nnXLen);
      Loc symLoc = SymmetryHelpers::getSymLoc(x, y, board, symmetry);
      int symPos = Location::getY(symLoc, board.x_size) * board.x_size + Location::getX(symLoc, board.x_size);
      assert(symPos >= 0 && symPos < board.y_size * board.x_size);

      double o;
      if(perspective == P_BLACK || (perspective != P_BLACK && perspective != P_WHITE && pla == P_BLACK))
        o = -ownership[pos];
      else
        o = ownership[pos];
      // Round to 10^-6 to limit the size of output.
      // No guarantees that the serializer actually outputs something of this length rather than longer due to float wonkiness, but it should usually be true.
      o = roundStatic(o, 1000000.0);
      ownershipToOutput[symPos] = o;
    }
  }
  return json(ownershipToOutput);
}

std::pair<json,json> Search::getJsonOwnershipAndStdevMap(
  const Player pla, const Player perspective, const Board& board, const SearchNode* node, double ownershipMinWeight, int symmetry
) const {
  const tuple<vector<double>,vector<double>> ownershipAverageAndStdev = getAverageAndStandardDeviationTreeOwnership(ownershipMinWeight, node);
  const vector<double>& ownership = std::get<0>(ownershipAverageAndStdev);
  const vector<double>& ownershipStdev = std::get<1>(ownershipAverageAndStdev);
  vector<double> ownershipToOutput(board.y_size * board.x_size, 0.0);
  vector<double> ownershipStdevToOutput(board.y_size * board.x_size, 0.0);

  for(int y = 0; y < board.y_size; y++) {
    for(int x = 0; x < board.x_size; x++) {
      int pos = NNPos::xyToPos(x, y, nnXLen);
      Loc symLoc = SymmetryHelpers::getSymLoc(x, y, board, symmetry);
      int symPos = Location::getY(symLoc, board.x_size) * board.x_size + Location::getX(symLoc, board.x_size);
      assert(symPos >= 0 && symPos < board.y_size * board.x_size);

      double o;
      if(perspective == P_BLACK || (perspective != P_BLACK && perspective != P_WHITE && pla == P_BLACK))
        o = -ownership[pos];
      else
        o = ownership[pos];
      // Round to 10^-6 to limit the size of output.
      // No guarantees that the serializer actually outputs something of this length rather than longer due to float wonkiness, but it should usually be true.
      o = roundStatic(o, 1000000.0);
      ownershipToOutput[symPos] = o;
      ownershipStdevToOutput[symPos] = roundStatic(ownershipStdev[pos], 1000000.0);
    }
  }
  return std::make_pair(json(ownershipToOutput), json(ownershipStdevToOutput));
}

bool Search::getAnalysisJson(
  const Player perspective,
  int analysisPVLen,
  double ownershipMinWeight,
  bool preventEncore,
  bool includePolicy,
  bool includeOwnership,
  bool includeOwnershipStdev,
  bool includeMovesOwnership,
  bool includeMovesOwnershipStdev,
  bool includePVVisits,
  json& ret
) const {
  vector<AnalysisData> buf;
  static constexpr int minMoves = 0;
  static constexpr int OUTPUT_PRECISION = 8;

  const Board& board = rootBoard;
  const BoardHistory& hist = rootHistory;
  bool duplicateForSymmetries = true;
  getAnalysisData(buf, minMoves, false, analysisPVLen, duplicateForSymmetries);

  // Stats for all the individual moves
  json moveInfos = json::array();
  for(int i = 0; i < buf.size(); i++) {
    const AnalysisData& data = buf[i];
    double winrate = 0.5 * (1.0 + data.winLossValue);
    double utility = data.utility;
    double lcb = PlayUtils::getHackedLCBForWinrate(this, data, rootPla);
    double utilityLcb = data.lcb;
    double scoreMean = data.scoreMean;
    double lead = data.lead;
    if(perspective == P_BLACK || (perspective != P_BLACK && perspective != P_WHITE && rootPla == P_BLACK)) {
      winrate = 1.0 - winrate;
      lcb = 1.0 - lcb;
      utility = -utility;
      scoreMean = -scoreMean;
      lead = -lead;
      utilityLcb = -utilityLcb;
    }

    json moveInfo;
    moveInfo["move"] = Location::toString(data.move, board);
    moveInfo["visits"] = data.numVisits;
    moveInfo["utility"] = roundDynamic(utility,OUTPUT_PRECISION);
    moveInfo["winrate"] = roundDynamic(winrate,OUTPUT_PRECISION);
    moveInfo["scoreMean"] = roundDynamic(lead,OUTPUT_PRECISION);
    moveInfo["scoreSelfplay"] = roundDynamic(scoreMean,OUTPUT_PRECISION);
    moveInfo["scoreLead"] = roundDynamic(lead,OUTPUT_PRECISION);
    moveInfo["scoreStdev"] = roundDynamic(data.scoreStdev,OUTPUT_PRECISION);
    moveInfo["prior"] = roundDynamic(data.policyPrior,OUTPUT_PRECISION);
    moveInfo["lcb"] = roundDynamic(lcb,OUTPUT_PRECISION);
    moveInfo["utilityLcb"] = roundDynamic(utilityLcb,OUTPUT_PRECISION);
    moveInfo["order"] = data.order;
    if(data.isSymmetryOf != Board::NULL_LOC)
      moveInfo["isSymmetryOf"] = Location::toString(data.isSymmetryOf, board);

    json pv = json::array();
    int pvLen =
      (preventEncore && data.pvContainsPass()) ? data.getPVLenUpToPhaseEnd(board, hist, rootPla) : (int)data.pv.size();
    for(int j = 0; j < pvLen; j++)
      pv.push_back(Location::toString(data.pv[j], board));
    moveInfo["pv"] = pv;

    if(includePVVisits) {
      assert(data.pvVisits.size() >= pvLen);
      json pvVisits = json::array();
      for(int j = 0; j < pvLen; j++)
        pvVisits.push_back(data.pvVisits[j]);
      moveInfo["pvVisits"] = pvVisits;
    }

    if(includeMovesOwnership && includeMovesOwnershipStdev) {
      std::pair<json,json> ownershipAndStdev = getJsonOwnershipAndStdevMap(rootPla, perspective, board, data.node, ownershipMinWeight, data.symmetry);
      moveInfo["ownership"] = ownershipAndStdev.first;
      moveInfo["ownershipStdev"] = ownershipAndStdev.second;
    }
    else if(includeMovesOwnershipStdev) {
      std::pair<json,json> ownershipAndStdev = getJsonOwnershipAndStdevMap(rootPla, perspective, board, data.node, ownershipMinWeight, data.symmetry);
      moveInfo["ownershipStdev"] = ownershipAndStdev.second;
    }
    else if(includeMovesOwnership) {
      moveInfo["ownership"] = getJsonOwnershipMap(rootPla, perspective, board, data.node, ownershipMinWeight, data.symmetry);
    }

    moveInfos.push_back(moveInfo);
  }
  ret["moveInfos"] = moveInfos;

  // Stats for root position
  {
    ReportedSearchValues rootVals;
    bool suc = getPrunedRootValues(rootVals);
    if(!suc)
      return false;

    double winrate = 0.5 * (1.0 + rootVals.winLossValue);
    double scoreMean = rootVals.expectedScore;
    double lead = rootVals.lead;
    double utility = rootVals.utility;

    if(perspective == P_BLACK || (perspective != P_BLACK && perspective != P_WHITE && rootPla == P_BLACK)) {
      winrate = 1.0 - winrate;
      scoreMean = -scoreMean;
      lead = -lead;
      utility = -utility;
    }

    json rootInfo;
    rootInfo["visits"] = rootVals.visits;
    rootInfo["winrate"] = roundDynamic(winrate,OUTPUT_PRECISION);
    rootInfo["scoreSelfplay"] = roundDynamic(scoreMean,OUTPUT_PRECISION);
    rootInfo["scoreLead"] = roundDynamic(lead,OUTPUT_PRECISION);
    rootInfo["scoreStdev"] = roundDynamic(rootVals.expectedScoreStdev,OUTPUT_PRECISION);
    rootInfo["utility"] = roundDynamic(utility,OUTPUT_PRECISION);

    Hash128 thisHash;
    Hash128 symHash;
    for(int symmetry = 0; symmetry < SymmetryHelpers::NUM_SYMMETRIES; symmetry++) {
      Board symBoard = SymmetryHelpers::getSymBoard(board,symmetry);
      Hash128 hash = symBoard.getSitHashWithSimpleKo(rootPla);
      if(symmetry == 0) {
        thisHash = hash;
        symHash = hash;
      }
      else {
        if(hash < symHash)
          symHash = hash;
      }
    }
    rootInfo["thisHash"] = Global::uint64ToHexString(thisHash.hash1) + Global::uint64ToHexString(thisHash.hash0);
    rootInfo["symHash"] = Global::uint64ToHexString(symHash.hash1) + Global::uint64ToHexString(symHash.hash0);
    rootInfo["currentPlayer"] = PlayerIO::playerToStringShort(rootPla);

    ret["rootInfo"] = rootInfo;
  }

  // Raw policy prior
  if(includePolicy) {
    float policyProbs[NNPos::MAX_NN_POLICY_SIZE];
    bool suc = getPolicy(policyProbs);
    if(!suc)
      return false;
    json policy = json::array();
    for(int y = 0; y < board.y_size; y++) {
      for(int x = 0; x < board.x_size; x++) {
        int pos = NNPos::xyToPos(x, y, nnXLen);
        policy.push_back(roundDynamic(policyProbs[pos],OUTPUT_PRECISION));
      }
    }

    int passPos = NNPos::locToPos(Board::PASS_LOC, board.x_size, nnXLen, nnYLen);
    policy.push_back(roundDynamic(policyProbs[passPos],OUTPUT_PRECISION));
    ret["policy"] = policy;
  }

  // Average tree ownership
  if(includeOwnership && includeOwnershipStdev) {
    int symmetry = 0;
    std::pair<json,json> ownershipAndStdev = getJsonOwnershipAndStdevMap(rootPla, perspective, board, rootNode, ownershipMinWeight, symmetry);
    ret["ownership"] = ownershipAndStdev.first;
    ret["ownershipStdev"] = ownershipAndStdev.second;
  }
  else if(includeOwnershipStdev) {
    int symmetry = 0;
    std::pair<json,json> ownershipAndStdev = getJsonOwnershipAndStdevMap(rootPla, perspective, board, rootNode, ownershipMinWeight, symmetry);
    ret["ownershipStdev"] = ownershipAndStdev.second;
  }
  else if(includeOwnership) {
    int symmetry = 0;
    ret["ownership"] = getJsonOwnershipMap(rootPla, perspective, board, rootNode, ownershipMinWeight, symmetry);
  }

  return true;
}

//Compute all the stats of the node based on its children, pruning weights such that they are as expected
//based on policy and utility. This is used to give accurate rootInfo even with a lot of wide root noise
bool Search::getPrunedRootValues(ReportedSearchValues& values) const {
  return getPrunedNodeValues(rootNode,values);
}

bool Search::getPrunedNodeValues(const SearchNode* nodePtr, ReportedSearchValues& values) const {
  if(nodePtr == NULL)
    return false;
  const SearchNode& node = *nodePtr;
  int childrenCapacity;
  const SearchChildPointer* children = node.getChildren(childrenCapacity);

  vector<double> playSelectionValues;
  vector<Loc> locs; // not used
  bool allowDirectPolicyMoves = false;
  bool alwaysComputeLcb = false;
  bool neverUseLcb = true;
  bool suc = getPlaySelectionValues(node,locs,playSelectionValues,NULL,1.0,allowDirectPolicyMoves,alwaysComputeLcb,neverUseLcb,NULL,NULL);
  //If there are no children, or otherwise values could not be computed,
  //then fall back to the normal case and just listen to the values on the node rather than trying
  //to recompute things.
  if(!suc) {
    return getNodeValues(nodePtr,values);
  }

  double winLossValueSum = 0.0;
  double noResultValueSum = 0.0;
  double scoreMeanSum = 0.0;
  double scoreMeanSqSum = 0.0;
  double leadSum = 0.0;
  double utilitySum = 0.0;
  double utilitySqSum = 0.0;
  double weightSum = 0.0;
  double weightSqSum = 0.0;
  for(int i = 0; i<childrenCapacity; i++) {
    const SearchNode* child = children[i].getIfAllocated();
    if(child == NULL)
      break;
    NodeStats stats = NodeStats(child->stats);

    if(stats.visits <= 0 || stats.weightSum <= 0.0)
      continue;
    double weight = playSelectionValues[i];
    winLossValueSum += weight * stats.winLossValueAvg;
    noResultValueSum += weight * stats.noResultValueAvg;
    scoreMeanSum += weight * stats.scoreMeanAvg;
    scoreMeanSqSum += weight * stats.scoreMeanSqAvg;
    leadSum += weight * stats.leadAvg;
    utilitySum += weight * stats.utilityAvg;
    utilitySqSum += weight * stats.utilitySqAvg;
    weightSqSum += weight * weight; // TODO not quite right
    weightSum += weight;
  }

  //Also add in the direct evaluation of this node.
  {
    const NNOutput* nnOutput = node.getNNOutput();
    //If somehow the nnOutput is still null here, skip
    if(nnOutput == NULL)
      return false;
    double winProb = (double)nnOutput->whiteWinProb;
    double lossProb = (double)nnOutput->whiteLossProb;
    double noResultProb = (double)nnOutput->whiteNoResultProb;
    double scoreMean = (double)nnOutput->whiteScoreMean;
    double scoreMeanSq = (double)nnOutput->whiteScoreMeanSq;
    double lead = (double)nnOutput->whiteLead;
    double utility =
      getResultUtility(winProb-lossProb, noResultProb)
      + getScoreUtility(scoreMean, scoreMeanSq);

    double weight = 1.0; // TODO also not quite right
    winLossValueSum += (winProb - lossProb) * weight;
    noResultValueSum += noResultProb * weight;
    scoreMeanSum += scoreMean * weight;
    scoreMeanSqSum += scoreMeanSq * weight;
    leadSum += lead * weight;
    utilitySum += utility * weight;
    utilitySqSum += utility * utility * weight;
    weightSqSum += weight * weight;
    weightSum += weight;
  }
  values = ReportedSearchValues(
    *this,
    winLossValueSum / weightSum,
    noResultValueSum / weightSum,
    scoreMeanSum / weightSum,
    scoreMeanSqSum / weightSum,
    leadSum / weightSum,
    utilitySum / weightSum,
    node.stats.weightSum.load(std::memory_order_acquire),
    node.stats.visits.load(std::memory_order_acquire)
  );
  return true;
}
