// Fill out your copyright notice in the Description page of Project Settings.


#include "FPassiveTreeBuilder.h"

FPassiveTreeBuilder::CellChecker::CellChecker(int32 cellCountX, int32 cellCountY)
{
	// 지정한 셀 개수로 초기화
	cellCount.X = cellCountX;
	cellCount.Y = cellCountY;

	cellUsed.SetNum(cellCount.Y);
	for (int32 y = 0; y < cellCount.Y; ++y)
	{
		cellUsed[y].SetNum(cellCount.X);
	}
}

PNodeID FPassiveTreeBuilder::CellChecker::TryOccupy(const FPassiveNodeData& nodeData, float nodeHalfSize)
{
	const FVector2D& nodePos = nodeData.pos;

	// 셀 범위 계산
	int32 xMin = FGenericPlatformMath::FloorToInt((nodePos.X - nodeHalfSize) / nodeHalfSize);
	int32 xMax = FGenericPlatformMath::CeilToInt((nodePos.X + nodeHalfSize) / nodeHalfSize);
	int32 yMin = FGenericPlatformMath::FloorToInt((nodePos.Y - nodeHalfSize) / nodeHalfSize);
	int32 yMax = FGenericPlatformMath::CeilToInt((nodePos.Y + nodeHalfSize) / nodeHalfSize);

	// 셀 전체 개수에 따른 전체 범위 넘어가면 -1 리턴
	if (xMin < 0 || xMax > cellCount.X || yMin < 0 || yMax > cellCount.Y)
		return -1;

	// 범위 내 다른 PNodeID 점유중이면 기존 PNodeID 리턴
	for (int32 y = yMin; y < yMax; ++y)
	{
		for (int32 x = xMin; x < xMax; ++x)
		{
			PNodeID prevOccupyNodeID = cellUsed[y][x];
			if (prevOccupyNodeID)
			{
				return prevOccupyNodeID;
			}
		}
	}

	// 새로운 노드로 점유 PNodeID 갱신
	for (int32 y = yMin; y < yMax; ++y)
	{
		for (int32 x = xMin; x < xMax; ++x)
		{
			cellUsed[y][x] = nodeData.id;
		}
	}

	// 새로운 노드 PNodeID 리턴
	return nodeData.id;
}


void FPassiveTreeBuilder::MakeRandomPassiveTree(FPassiveTree& tree)
{
	// 루트 노드 이웃노드 숫자
	uint32 rootNodeNeighborCount = 6;

	// 이웃노드 생성 최소 시도숫자
	uint32 nodeNeighborCountMin = 3;
	// 이웃노드 생성 최대 시도숫자
	uint32 nodeNeighborCountMax = 5;
	// 최대 노드 개수
	uint32 maxNodeCount = 100;
	// 노드 크기
	float nodeSize = 50.f;
	// 노드 절반 크기
	float nodeHalfSize = nodeSize * 0.5f;
	// 생성할 이웃노드간 거리
	float nodeCreateDistance = 100.f;

	tree._clear();

	// 맵 크기
	FVector2D mapSize(1800, 900);

	// 맵 크기와 노드 크기에 따른 셀 개수. 노드 크기 절반당 하나의 셀로 체크
	int32 cellCountX = FGenericPlatformMath::CeilToInt(mapSize.X / nodeHalfSize);
	int32 cellCountY = FGenericPlatformMath::CeilToInt(mapSize.Y / nodeHalfSize);

	// CellChecker
	CellChecker cellChecker(cellCountX, cellCountY);

	// 루트 노드 추가
	FPassiveNodeData newNodeData;
	newNodeData.id = FPassiveTree::RootNodeID;
	newNodeData.pos.X = mapSize.X / 2;
	newNodeData.pos.Y = mapSize.Y / 2;
	newNodeData.stat.type = ePassiveStatType::UNDEFINED;
	newNodeData.stat.value = 0;
	newNodeData.cost = 0;
	tree._addNode(newNodeData);
	cellChecker.TryOccupy(newNodeData, nodeHalfSize);

	// 다음 생성할 PNodeID
	PNodeID nextCreateNodeID = FPassiveTree::RootNodeID + 1;

	// 생성 작업Queue 데이터
	struct TreeBuildQueueData
	{
		PNodeID nodeID;		// 해당 노드 데이터
		PNodeID prevNodeID;	// 이 노드를 만든 당시의 이전 노드 데이터
		TreeBuildQueueData() : nodeID(0), prevNodeID(0) {}
		TreeBuildQueueData(PNodeID id, PNodeID prevID) : nodeID(id), prevNodeID(prevID) {}
	};

	// 생성 작업Queue
	TQueue<TreeBuildQueueData> treeBuildQueue;
	
	FPassiveNodeData& rootNodeData = tree.nodes[FPassiveTree::RootNodeID];


	// 루트 노드의 이웃 추가
	for (uint32 i = 0; i < rootNodeNeighborCount; ++i)
	{
		// 균등 각도 계산
		float angle = (float)i * PI * 2.f / (float)rootNodeNeighborCount;

		newNodeData.id = nextCreateNodeID;
		newNodeData.pos.X = FGenericPlatformMath::FloorToInt(rootNodeData.pos.X + FGenericPlatformMath::Cos(angle) * nodeCreateDistance);
		newNodeData.pos.Y = FGenericPlatformMath::FloorToInt(rootNodeData.pos.Y + FGenericPlatformMath::Sin(angle) * nodeCreateDistance);

		// 노드 점유 체크
		PNodeID occupyNodeID = cellChecker.TryOccupy(newNodeData, nodeHalfSize);

		// 성공시 이웃노드 추가
		if (occupyNodeID == newNodeData.id)
		{
			newNodeData.stat.type = FPassiveStatTypeHelper::getRandomStat();
			newNodeData.stat.value = 1 + (FGenericPlatformMath::Rand() % 5);
			newNodeData.cost = newNodeData.stat.value;
			// 노드 추가
			tree._addNode(newNodeData);
			// 링크 추가
			tree._addLink(FPassiveTree::RootNodeID, newNodeData.id);
			// 작업Queue 추가
			treeBuildQueue.Enqueue(TreeBuildQueueData(newNodeData.id, FPassiveTree::RootNodeID));
			++nextCreateNodeID;
		}
	}

	// Queue 작업 - Queue 노드로부터 이웃 노드 추가
	TreeBuildQueueData queueData;
	while (!treeBuildQueue.IsEmpty() && nextCreateNodeID <= (int32)maxNodeCount)
	{
		treeBuildQueue.Dequeue(queueData);

		// 현재 노드
		FPassiveNodeData& curNodeData = tree.nodes[queueData.nodeID];
		// 이전 노드
		FPassiveNodeData& prevNodeData = tree.nodes[queueData.prevNodeID];

		// 랜덤생성 시도 횟수 결정
		int32 nodeNeighborCount = nodeNeighborCountMin;
		int32 countDelta = nodeNeighborCountMax - nodeNeighborCountMin;
		if (0 < countDelta)
		{
			nodeNeighborCount += FGenericPlatformMath::Rand() % countDelta;
		}

		// 이전 노드로부터의 각도 계산
		float prevAngle = FGenericPlatformMath::Atan2(curNodeData.pos.Y - prevNodeData.pos.Y, curNodeData.pos.X - prevNodeData.pos.X);

		// 이전 노드와 겹치지 않기 위한 최소 최대 각도
		float minAngle = prevAngle + PI * 0.25f;
		float maxAngle = prevAngle + PI * 1.75f;

		// 랜덤생성 시도
		for (int32 i = 0; i < nodeNeighborCount; ++i)
		{
			// 랜덤 각도 계산
			float angle = minAngle + maxAngle * FGenericPlatformMath::FRand();

			newNodeData.id = nextCreateNodeID;
			// 랜덤 각도로 nodeCreateDistance 거리만큼 향한 위치
			newNodeData.pos.X = FGenericPlatformMath::FloorToInt(curNodeData.pos.X + FGenericPlatformMath::Cos(angle) * nodeCreateDistance);
			newNodeData.pos.Y = FGenericPlatformMath::FloorToInt(curNodeData.pos.Y + FGenericPlatformMath::Sin(angle) * nodeCreateDistance);

			// 점유 체크
			PNodeID occupyNodeID = cellChecker.TryOccupy(newNodeData, nodeHalfSize);

			// 새로 점유 성공시
			if (occupyNodeID == newNodeData.id)
			{
				newNodeData.stat.type = FPassiveStatTypeHelper::getRandomStat();
				newNodeData.stat.value = 1 + (FGenericPlatformMath::Rand() % 5);
				newNodeData.cost = newNodeData.stat.value;
				// 노드 추가
				tree._addNode(newNodeData);
				// 링크 추가
				tree._addLink(curNodeData.id, newNodeData.id);
				// 작업Queue 추가
				treeBuildQueue.Enqueue(TreeBuildQueueData(newNodeData.id, curNodeData.id));
				++nextCreateNodeID;

				if (nextCreateNodeID > (int32)maxNodeCount)
					break;
			}
			// 기존 점유한 노드가 있다면
			else if (occupyNodeID != -1 && occupyNodeID != curNodeData.id)
			{
				// 생성 대신 기존 노드와 링크 연결
				tree._addLink(curNodeData.id, occupyNodeID);
			}
		}
	}
}
