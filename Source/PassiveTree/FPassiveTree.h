// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPassiveStats.h"

typedef int32 PNodeID;

/**
 * 패시브 노드에 담기는 스탯 데이터
 */
struct FPassiveNodeStatData
{
	ePassiveStatType type;
	int32 value;
};

/**
 * 패시브 노드 데이터
 */
struct FPassiveNodeData
{
	PNodeID id;
	uint32 cost;
	FIntPoint pos;
	FPassiveNodeStatData stat;
};

/**
 * 패시브 트리 노드&엣지 구조
 */
class FPassiveTree
{
public:
	FPassiveTree();
	~FPassiveTree();

	static const PNodeID RootNodeID = 1;

	/* 패시브 트리 노드 */
	const TMap<PNodeID, FPassiveNodeData>& GetNodes() const;
	/* 패시브 트리 엣지 */
	const TMap<PNodeID, TSet<PNodeID>>& GetLinks() const;

protected:
	/* 패시브 트리 노드 */
	TMap<PNodeID, FPassiveNodeData> nodes;
	/* 패시브 트리 엣지 - 하나의 PNodeID와 연결된 다른 PNodeID 집합 저장 */
	TMap<PNodeID, TSet<PNodeID>> links;

	/* 초기화 */
	void _clear();
	/* 노드 추가 */
	void _addNode(const FPassiveNodeData& data);
	/* 두 노드 사이 엣지 추가 */
	void _addLink(const PNodeID id1, const PNodeID id2);
	/* 단방향 엣지 정보 추가 - _addLink에서 수행 */
	void __addLink_SingleDirection(const PNodeID id1, const PNodeID id2);

	friend class FPassiveTreeBuilder;
};
