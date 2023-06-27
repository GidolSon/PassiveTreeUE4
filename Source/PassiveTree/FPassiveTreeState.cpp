// Fill out your copyright notice in the Description page of Project Settings.


#include "FPassiveTreeState.h"
#include <queue>

FPassiveTreeState::FPassiveTreeState()
	: ptrTree(nullptr), totalUsedCost(0)
{
}

FPassiveTreeState::~FPassiveTreeState()
{
}

void FPassiveTreeState::SetTree(FPassiveTree* _ptrTree)
{
	ptrTree = _ptrTree;
}

const FPassiveTree* FPassiveTreeState::GetTree() const
{
	return ptrTree;
}

void FPassiveTreeState::Reset()
{
	// 점유 PNodeID 집합 초기화
	assignedNodeSet.Empty();
	// 투자 비용 초기화
	totalUsedCost = 0;
}

uint32 FPassiveTreeState::GetTotalUsedCost()
{
	return totalUsedCost;
}

void FPassiveTreeState::FindPath(TArray<PNodeID>& outPathReversed, PNodeID targetNodeID)
{
	outPathReversed.Empty();
	if (!ptrTree) return;

	// 잘못된 노드 ID 혹은 기존에 이미 투자한 노드 ID return
	if (targetNodeID < 0 || targetNodeID == FPassiveTree::RootNodeID || assignedNodeSet.Find(targetNodeID) != nullptr)
		return;

	const auto& nodes = ptrTree->GetNodes();
	const auto& links = ptrTree->GetLinks();

	// 트리에 없는 노드 ID return
	if (nodes.Find(targetNodeID) == nullptr)
		return;

	// Dijkstra 알고리즘 최단노드 탐색

	int32 maxInt32 = TNumericLimits<int32>::Max();

	// 각 노드 탐색에 대해 이전에 최단거리로 탐색한 노드
	TMap<PNodeID, PNodeID> nodePathPrevMap;
	// 각 노드 탐색까지 최단거리 비용
	TMap<PNodeID, uint32> nodePathCostMap;
	// 아직 탐색하지 않은 노드
	TSet<PNodeID> unvisitedSet;

	// 탐색정보 초기화
	for (auto& nodeInfo : nodes)
	{
		unvisitedSet.Add(nodeInfo.Key);
		nodePathPrevMap.Add(nodeInfo.Key, -1);

		if (nodeInfo.Key == FPassiveTree::RootNodeID || assignedNodeSet.Find(nodeInfo.Key) != nullptr)
		{
			// 루트노드 및 이미 투자한 노드의 탐색비용은 0
			nodePathCostMap.Add(nodeInfo.Key, 0);
		}
		else
		{
			// 이외에는 MaxInt
			nodePathCostMap.Add(nodeInfo.Key, maxInt32);
		}
	}

	while (unvisitedSet.Num() > 0)
	{
		// 아직 미탐색 노드 중 가장 탐색비용 적은 노드 찾기
		PNodeID curNodeID = -1;
		uint32 minCost = maxInt32;
		for (PNodeID nodeID : unvisitedSet)
		{
			uint32 cost = nodePathCostMap[nodeID];
			if (cost < minCost)
			{
				minCost = cost;
				curNodeID = nodeID;
			}
		}
		if (curNodeID == -1) break;

		// 탐색 노드 처리
		unvisitedSet.Remove(curNodeID);
		const auto& neighbors = links[curNodeID];

		// 현재 탐색 비용
		uint32 curNodePathCost = nodePathCostMap[curNodeID];

		// 각 이웃에 대해
		for (PNodeID neighborNodeID : neighbors)
		{
			const auto& neighborNodeData = nodes[neighborNodeID];

			// 새로운 탐색 비용
			uint32 newCost = (curNodePathCost + neighborNodeData.cost);

			uint32 prevCost = nodePathCostMap[neighborNodeID];
			if (newCost < prevCost)
			{
				// 기존 탐색 비용보다 작으면 탐색 비용 및 PrevNode 갱신
				nodePathPrevMap[neighborNodeID] = curNodeID;
				nodePathCostMap[neighborNodeID] = newCost;
			}
		}
	}

	// 목표 노드로부터 역추적
	PNodeID curNodeID = targetNodeID;
	while (curNodeID != -1)
	{
		outPathReversed.Add(curNodeID);
		curNodeID = nodePathPrevMap[curNodeID];
	}
}

void FPassiveTreeState::AssignPath(const TArray<PNodeID>& path)
{
	if (!ptrTree) return;
	const auto& nodes = ptrTree->GetNodes();

	// 투자 노드 및 투자 비용 추가
	for (PNodeID nodeID : path)
	{
		if (assignedNodeSet.Find(nodeID) == nullptr)
		{
			const FPassiveNodeData& nodeData = nodes[nodeID];
			assignedNodeSet.Add(nodeID);
			totalUsedCost += nodeData.cost;
		}
	}

}